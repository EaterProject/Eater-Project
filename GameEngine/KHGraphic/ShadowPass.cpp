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
#include "VertexDefine.h"
#include "EngineData.h"
#include "RenderData.h"
#include "ShadowPass.h"

#include "MathDefine.h"
#include "FactoryManagerBase.h"
#include "ResourceManagerBase.h"
#include "ShaderManagerBase.h"
#include "ConstantBufferDefine.h"
#include "ShaderResourceViewDefine.h"
#include "DepthStencilViewDefine.h"
#include "DepthStencilStateDefine.h"
#include "ViewPortDefine.h"
#include "RenderTargetDefine.h"
#include "RasterizerStateDefine.h"
#include "InstanceBufferDefine.h"

ShadowPass::ShadowPass()
	:m_ShadowDSV(nullptr)
{
}

ShadowPass::~ShadowPass()
{

}

void ShadowPass::Create(int width, int height)
{
	// ViewPort 설정..
	g_Factory->CreateViewPort<VP_Shadow>(0.0f, 0.0f, 4.0f, 4.0f, (float)width, (float)height);

	// DepthStencilView 설정..
	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(texDesc));
	texDesc.Width = width * 4.0f;
	texDesc.Height = height * 4.0f;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(dsvDesc));
	dsvDesc.Flags = 0;
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;

	// Shadow DepthStencilView 생성..
	g_Factory->CreateDepthStencil<DS_Shadow>(&texDesc, nullptr, &dsvDesc, &srvDesc);
}

void ShadowPass::Start(int width, int height)
{
	// Shader 설정..
	m_MeshShadowVS = g_Shader->GetShader("Shadow_StaticMesh_VS");
	m_MeshInstShadowVS = g_Shader->GetShader("Shadow_StaticMesh_Instance_VS");
	m_SkinShadowVS = g_Shader->GetShader("Shadow_SkinMesh_VS");
	m_SkinInstShadowVS = g_Shader->GetShader("Shadow_SkinMesh_Instance_VS");
	
	// Insatance Buffer 설정..
	m_Mesh_IB = g_Resource->GetInstanceBuffer<IB_MeshDepth>();

	m_ShadowDS = g_Resource->GetDepthStencil<DS_Shadow>();
	m_ShadowDS->SetRatio(4.0f, 4.0f);

	m_ShadowVP = g_Resource->GetViewPort<VP_Shadow>()->Get();
	m_DepthRS = g_Resource->GetRasterizerState<RS_Depth>()->Get();

	// Shadow DepthStencilView 설정..
	m_ShadowDSV = m_ShadowDS->GetDSV()->Get();
}

void ShadowPass::OnResize(int width, int height)
{
	// Shadow DepthStencilView 재설정..
	m_ShadowDSV = m_ShadowDS->GetDSV()->Get();
}

void ShadowPass::Release()
{

}

void ShadowPass::SetOption(RenderOption* renderOption)
{
	g_Context->ClearDepthStencilView(m_ShadowDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void ShadowPass::BeginRender()
{
	g_Context->OMSetBlendState(0, 0, 0xffffffff);
	g_Context->RSSetViewports(1, m_ShadowVP);

	// 그리기만 할 것이므로 null Render Target 설정..
	// 깊이 버퍼, null Rendering 대상을 설정하면 색상 쓰기가 비활성화 된다..
	g_Context->OMSetRenderTargets(0, nullptr, m_ShadowDSV);
	g_Context->ClearDepthStencilView(m_ShadowDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
	g_Context->RSSetState(m_DepthRS);
}

void ShadowPass::RenderUpdate(const InstanceRenderBuffer* instance, const std::vector<RenderData*>& meshlist, const UINT& renderCount)
{
	if (renderCount == 1)
	{
		RenderUpdate(instance, meshlist[0]);
		return;
	}

	Matrix viewproj = g_GlobalData->DirectionLights[0]->LightViewProj;

	MeshRenderBuffer* mesh = instance->m_Mesh;

	for (int i = 0; i < renderCount; i++)
	{
		if (meshlist[i] == nullptr) continue;

		// 해당 Instance Data 삽입..
		m_MeshData.World = *meshlist[i]->m_ObjectData->World;

		m_MeshInstance.push_back(m_MeshData);
		m_InstanceCount++;
	}

	if (m_MeshInstance.empty()) return;

	// Mapping SubResource Data..
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	// GPU Access Lock Buffer Data..
	g_Context->Map(m_Mesh_IB->InstanceBuf->Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	m_InstanceStride = (size_t)m_Mesh_IB->Stride * (size_t)m_InstanceCount;

	// Copy Resource Data..
	memcpy(mappedResource.pData, &m_MeshInstance[0], m_InstanceStride);

	// GPU Access UnLock Buffer Data..
	g_Context->Unmap(m_Mesh_IB->InstanceBuf->Get(), 0);

	switch (instance->m_Type)
	{
	case OBJECT_TYPE::BASE:
	case OBJECT_TYPE::TERRAIN:
	{
		CB_InstanceDepthStaticMesh shadowBuf;
		shadowBuf.gViewProj = viewproj;

		m_MeshInstShadowVS->ConstantBufferCopy(&shadowBuf);

		m_MeshInstShadowVS->Update();

		ID3D11Buffer* vertexBuffers[2] = { mesh->m_VertexBuf, m_Mesh_IB->InstanceBuf->Get() };
		UINT strides[2] = { mesh->m_Stride, m_Mesh_IB->Stride };
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
		/// 임시 코드
		for (int i = 0; i < meshlist.size(); i++)
		{
			RenderUpdate(instance, meshlist[i]);
		}

		//CB_InstanceDepthSkinMesh shadowBuf;
		//shadowBuf.gViewProj = viewproj;
		//
		//for (int i = 0; i < mesh->m_BoneOffsetTM->size(); i++)
		//{
		//	shadowBuf.gBoneTransforms[i] = (*mesh->m_BoneOffsetTM)[i];
		//}
		//
		//m_SkinInstShadowVS->ConstantBufferCopy(&shadowBuf);
		//
		//m_SkinInstShadowVS->Update();
		//
		//ID3D11Buffer* vertexBuffers[2] = { mesh->m_VertexBuf, m_Mesh_IB->InstanceBuf->Get() };
		//UINT strides[2] = { mesh->m_Stride, m_Mesh_IB->Stride };
		//UINT offsets[2] = { 0,0 };
		//
		//// Draw..
		//g_Context->IASetVertexBuffers(0, 2, vertexBuffers, strides, offsets);
		//g_Context->IASetIndexBuffer(mesh->m_IndexBuf, DXGI_FORMAT_R32_UINT, 0);
		//g_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//
		//g_Context->DrawIndexedInstanced(mesh->m_IndexCount, m_InstanceCount, 0, 0, 0);
	}
	break;
	default:
		break;
	}


	// Mesh Instance Data Clear..
	m_MeshInstance.clear();
	m_InstanceCount = 0;
}

void ShadowPass::RenderUpdate(const InstanceRenderBuffer* instance, const RenderData* meshData)
{
	ObjectData* obj = meshData->m_ObjectData;
	MeshRenderBuffer* mesh = instance->m_Mesh;

	Matrix world = *obj->World;
	Matrix viewproj = g_GlobalData->DirectionLights[0]->LightViewProj;

	if (mesh == nullptr) return;

	switch (instance->m_Type)
	{
	case OBJECT_TYPE::BASE:
	case OBJECT_TYPE::TERRAIN:
	{
		CB_DepthStaticMesh shadowBuf;
		shadowBuf.gWorldViewProj = world * viewproj;

		m_MeshShadowVS->ConstantBufferCopy(&shadowBuf);

		m_MeshShadowVS->Update();
	}
	break;
	case OBJECT_TYPE::SKINNING:
	{
		CB_DepthSkinMesh shadowBuf;
		shadowBuf.gWorldViewProj = world * viewproj;

		for (int i = 0; i < obj->BoneOffsetTM.size(); i++)
		{
			shadowBuf.gBoneTransforms[i] = (obj->BoneOffsetTM)[i];
		}

		m_SkinShadowVS->ConstantBufferCopy(&shadowBuf);

		m_SkinShadowVS->Update();
	}
	break;
	default:
		break;
	}

	// Draw..
	g_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	g_Context->IASetVertexBuffers(0, 1, &mesh->m_VertexBuf, &mesh->m_Stride, &mesh->m_Offset);
	g_Context->IASetIndexBuffer(mesh->m_IndexBuf, DXGI_FORMAT_R32_UINT, 0);

	g_Context->DrawIndexed(mesh->m_IndexCount, 0, 0);
}
