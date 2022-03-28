#include "DirectDefine.h"
#include "RenderPassBase.h"
#include "ShaderBase.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "GraphicState.h"
#include "GraphicView.h"
#include "GraphicResource.h"
#include "DrawBuffer.h"
#include "DepthStencil.h"
#include "RenderTarget.h"
#include "EngineData.h"
#include "RenderData.h"
#include "VertexDefine.h"
#include "PickingPass.h"

#include "MathDefine.h"
#include "FactoryManagerBase.h"
#include "ResourceManagerBase.h"
#include "ShaderManagerBase.h"
#include "ConstantBufferDefine.h"
#include "ShaderResourceViewDefine.h"
#include "ViewPortDefine.h"
#include "RenderTargetDefine.h"
#include "DepthStencilViewDefine.h"
#include "DepthStencilStateDefine.h"
#include "RasterizerStateDefine.h"
#include "BlendStateDefine.h"
#include "DrawBufferDefine.h"
#include "InstanceBufferDefine.h"

PickingPass::PickingPass()
{

}

PickingPass::~PickingPass()
{

}

void PickingPass::Create(int width, int height)
{
	// Picking 전용 Texture 2D
	// 해당 Texture는 데이터 입력은 Shader에서 하되, 해당 값을 CPU에서 읽을수 있어야 하므로
	// Usage Option을 D3D11_USAGE_STAGING으로 설정..
	D3D11_TEXTURE2D_DESC texCopyDesc;
	ZeroMemory(&texCopyDesc, sizeof(texCopyDesc));
	texCopyDesc.Width = 1;
	texCopyDesc.Height = 1;
	texCopyDesc.MipLevels = 1;
	texCopyDesc.ArraySize = 1;
	texCopyDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texCopyDesc.SampleDesc.Count = 1;
	texCopyDesc.SampleDesc.Quality = 0;
	texCopyDesc.Usage = D3D11_USAGE_STAGING;
	texCopyDesc.BindFlags = 0;
	texCopyDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
	texCopyDesc.MiscFlags = 0;

	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(texDesc));
	texDesc.Width = width;
	texDesc.Height = height;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	/// RenderTargetView 2D
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	ZeroMemory(&rtvDesc, sizeof(rtvDesc));
	rtvDesc.Format = texDesc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;

	// RenderTarget 생성..
	g_Factory->CreateRenderTexture<RT_ID_Copy>(&texCopyDesc, nullptr, nullptr, nullptr, nullptr);
	g_Factory->CreateRenderTexture<RT_ID>(&texDesc, nullptr, &rtvDesc, nullptr, nullptr);
}

void PickingPass::Start(int width, int height)
{
	// Shader 설정..
	m_Mesh_VS = g_Shader->GetShader("ID_StaticMesh_VS");
	m_Mesh_Inst_VS = g_Shader->GetShader("ID_StaticMesh_Instance_VS");
	m_Skin_VS = g_Shader->GetShader("ID_SkinMesh_VS");
	m_Skin_Inst_VS = g_Shader->GetShader("ID_SkinMesh_Instance_VS");
	m_Mesh_ID_PS = g_Shader->GetShader("ID_Mesh_PS");

	m_MeshID_IB = g_Resource->GetInstanceBuffer<IB_MeshID>();
	m_Box_DB = g_Resource->GetDrawBuffer<DB_Box>();

	// Render Target 설정..
	m_ID_RT = g_Resource->GetRenderTexture<RT_ID>();
	m_ID_RTV = m_ID_RT->GetRTV()->Get();
	m_ID_Tex2D = m_ID_RT->GetTex2D()->Get();
	m_ID_Copy_RT = g_Resource->GetRenderTexture<RT_ID_Copy>();
	m_ID_CopyTex2D = m_ID_Copy_RT->GetTex2D()->Get();

	// DepthStencilView 설정..
	m_Defalt_DSV = g_Resource->GetDepthStencilView<DS_Defalt>()->Get();

	// Grpahic State 설정..
	m_Defalt_DSS = g_Resource->GetDepthStencilState<DSS_Defalt>()->Get();

	// ViewPort 설정..
	m_Screen_VP = g_Resource->GetViewPort<VP_FullScreen>()->Get();
}

void PickingPass::OnResize(int width, int height)
{
	m_ID_RTV = m_ID_RT->GetRTV()->Get();
	m_ID_Tex2D = m_ID_RT->GetTex2D()->Get();
}

void PickingPass::Release()
{

}

void PickingPass::BeginRender()
{
	g_Context->OMSetRenderTargets(1, &m_ID_RTV, m_Defalt_DSV);

	g_Context->ClearRenderTargetView(m_ID_RTV, reinterpret_cast<const float*>(&DXColors::NonID));
	g_Context->ClearDepthStencilView(m_Defalt_DSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	g_Context->OMSetDepthStencilState(m_Defalt_DSS, 0);
	g_Context->RSSetViewports(1, m_Screen_VP);
}

void PickingPass::RenderUpdate(const InstanceRenderBuffer* instance, const RenderData* meshData)
{
	if (meshData == nullptr) return;
	
	ObjectData* obj = meshData->m_ObjectData;
	MeshRenderBuffer* mesh = meshData->m_Mesh;

	Matrix world = *obj->World;
	Matrix viewproj = g_GlobalData->CamViewProj;
	Vector4 hashColor = obj->HashColor;

	switch (obj->ObjType)
	{
	case OBJECT_TYPE::BASE:
	case OBJECT_TYPE::TERRAIN:
	{
		// Vertex Shader Update..
		CB_StaticMesh_ID objectBuf;
		objectBuf.gWorldViewProj = world * viewproj;
		objectBuf.gHashColor = hashColor;

		m_Mesh_VS->ConstantBufferCopy(&objectBuf);
		m_Mesh_VS->Update();

		// Pixel Shader Update..
		m_Mesh_ID_PS->Update();

		// Draw..
		g_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		g_Context->IASetVertexBuffers(0, 1, &mesh->m_VertexBuf, &mesh->m_Stride, &mesh->m_Offset);
		g_Context->IASetIndexBuffer(mesh->m_IndexBuf, DXGI_FORMAT_R32_UINT, 0);

		g_Context->DrawIndexed(mesh->m_IndexCount, 0, 0);
	}
	break;
	case OBJECT_TYPE::SKINNING:
	{
		// Vertex Shader Update..
		CB_SkinMesh_ID objectBuf;
		objectBuf.gWorldViewProj = world * viewproj;
		objectBuf.gHashColor = hashColor;

		for (int i = 0; i < obj->BoneOffsetTM.size(); i++)
		{
			objectBuf.gBoneTransforms[i] = (obj->BoneOffsetTM)[i];
		}

		m_Skin_VS->ConstantBufferCopy(&objectBuf);

		m_Skin_VS->Update();

		// Pixel Shader Update..
		m_Mesh_ID_PS->Update();

		// Draw..
		g_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		g_Context->IASetVertexBuffers(0, 1, &mesh->m_VertexBuf, &mesh->m_Stride, &mesh->m_Offset);
		g_Context->IASetIndexBuffer(mesh->m_IndexBuf, DXGI_FORMAT_R32_UINT, 0);

		g_Context->DrawIndexed(mesh->m_IndexCount, 0, 0);
	}
	break;
	default:
		break;
	}
}

void PickingPass::RenderUpdate(const InstanceRenderBuffer* instance, const std::vector<RenderData*>& meshlist)
{
	if (meshlist.size() == 1)
	{
		RenderUpdate(instance, meshlist[0]);
		return;
	}

	Matrix viewproj = g_GlobalData->CamViewProj;

	ObjectData* obj = nullptr;
	MeshRenderBuffer* mesh = instance->m_Mesh;

	for (int i = 0; i < meshlist.size(); i++)
	{
		if (meshlist[i] == nullptr) continue;

		// 해당 Instance Data 삽입..
		obj = meshlist[i]->m_ObjectData;
		m_MeshData.World = *obj->World;
		m_MeshData.HashColor = obj->HashColor;

		m_MeshInstance.push_back(m_MeshData);
		m_InstanceCount++;
	}

	if (m_MeshInstance.empty()) return;

	// Mapping SubResource Data..
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	// GPU Access Lock Buffer Data..
	g_Context->Map(m_MeshID_IB->InstanceBuf->Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	m_InstanceStride = (size_t)m_MeshID_IB->Stride * (size_t)m_InstanceCount;

	// Copy Resource Data..
	memcpy(mappedResource.pData, &m_MeshInstance[0], m_InstanceStride);

	// GPU Access UnLock Buffer Data..
	g_Context->Unmap(m_MeshID_IB->InstanceBuf->Get(), 0);

	switch (instance->m_Type)
	{
	case OBJECT_TYPE::BASE:
	{
		// Vertex Shader Update..
		CB_Instance_StaticMesh_ID objectBuf;
		objectBuf.gViewProj = viewproj;

		m_Mesh_Inst_VS->ConstantBufferCopy(&objectBuf);

		m_Mesh_Inst_VS->Update();

		// Pixel Shader Update..
		m_Mesh_ID_PS->Update();

		ID3D11Buffer* vertexBuffers[2] = { mesh->m_VertexBuf, m_MeshID_IB->InstanceBuf->Get() };
		UINT strides[2] = { mesh->m_Stride, m_MeshID_IB->Stride };
		UINT offsets[2] = { 0,0 };

		// Draw..
		g_Context->IASetVertexBuffers(0, 2, vertexBuffers, strides, offsets);
		g_Context->IASetIndexBuffer(mesh->m_IndexBuf, DXGI_FORMAT_R32_UINT, 0);
		g_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		g_Context->DrawIndexedInstanced(mesh->m_IndexCount, m_InstanceCount, 0, 0, 0);
	}
	break;
	case OBJECT_TYPE::SKINNING:
	{

	}
	break;
	default:
		break;
	}

	// Mesh Instance Data Clear..
	m_MeshInstance.clear();
	m_InstanceCount = 0;
}

void PickingPass::NoneMeshRenderUpdate(const std::vector<RenderData*>& meshlist)
{
	Matrix viewproj = g_GlobalData->CamViewProj;

	ObjectData* obj = nullptr;

	for (int i = 0; i < meshlist.size(); i++)
	{
		if (meshlist[i] == nullptr) continue;

		// 해당 Instance Data 삽입..
		obj = meshlist[i]->m_ObjectData;
		m_MeshData.World = *obj->World;
		m_MeshData.HashColor = obj->HashColor;

		m_MeshInstance.push_back(m_MeshData);
		m_InstanceCount++;
	}

	if (m_MeshInstance.empty()) return;

	// Mapping SubResource Data..
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	// GPU Access Lock Buffer Data..
	g_Context->Map(m_MeshID_IB->InstanceBuf->Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	m_InstanceStride = (size_t)m_MeshID_IB->Stride * (size_t)m_InstanceCount;

	// Copy Resource Data..
	memcpy(mappedResource.pData, &m_MeshInstance[0], m_InstanceStride);

	// GPU Access UnLock Buffer Data..
	g_Context->Unmap(m_MeshID_IB->InstanceBuf->Get(), 0);

	// Vertex Shader Update..
	CB_Instance_StaticMesh_ID objectBuf;
	objectBuf.gViewProj = viewproj;

	m_Mesh_Inst_VS->ConstantBufferCopy(&objectBuf);

	m_Mesh_Inst_VS->Update();

	// Pixel Shader Update..
	m_Mesh_ID_PS->Update();

	ID3D11Buffer* vertexBuffers[2] = { m_Box_DB->VertexBuf->Get(), m_MeshID_IB->InstanceBuf->Get() };
	UINT strides[2] = { m_Box_DB->Stride, m_MeshID_IB->Stride };
	UINT offsets[2] = { 0,0 };

	// Draw..
	g_Context->IASetVertexBuffers(0, 2, vertexBuffers, strides, offsets);
	g_Context->IASetIndexBuffer(m_Box_DB->IndexBuf->Get(), DXGI_FORMAT_R32_UINT, 0);
	g_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	g_Context->DrawIndexedInstanced(m_Box_DB->IndexCount, m_InstanceCount, 0, 0, 0);

	// Mesh Instance Data Clear..
	m_MeshInstance.clear();
	m_InstanceCount = 0;
}

UINT PickingPass::FindPick(int x, int y)
{
	D3D11_BOX box;
	box.left = x,
	box.right = x + 1;
	box.top = y,
	box.bottom = y + 1;
	box.front = 0;
	box.back = 1;

	g_Context->OMSetRenderTargets(0, nullptr, nullptr);

	g_Context->CopySubresourceRegion(m_ID_CopyTex2D, 0, 0, 0, 0, m_ID_Tex2D, 0, &box);

	// Mapping SubResource Data..
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	// GPU Access Lock Texture Data..
	g_Context->Map(m_ID_CopyTex2D, 0, D3D11_MAP_READ, 0, &mappedResource);

	UINT* pickID = (UINT*)mappedResource.pData;

	// GPU Access UnLock Texture Data..
	g_Context->Unmap(m_ID_CopyTex2D, 0);

	return *pickID;
}
