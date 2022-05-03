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
#include "DrawBufferDefine.h"

ShadowPass::ShadowPass()
	:m_Shadow_DSV(nullptr)
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
	texDesc.Width = (UINT)width * 4;
	texDesc.Height = (UINT)height * 4;
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
	m_MeshShadow_VS = g_Shader->GetShader("Shadow_StaticMesh_VS");
	m_MeshInstShadow_VS = g_Shader->GetShader("Shadow_StaticMesh_Instance_VS");
	m_SkinShadow_VS = g_Shader->GetShader("Shadow_SkinMesh_VS");
	m_SkinInstShadow_VS = g_Shader->GetShader("Shadow_SkinMesh_Instance_VS");

	// Insatance Buffer 설정..
	m_Mesh_IB = g_Resource->GetInstanceBuffer<IB_MeshDepth>();
	m_SkinMesh_IB = g_Resource->GetInstanceBuffer<IB_SkinMeshDepth>();

	m_Shadow_DS = g_Resource->GetDepthStencil<DS_Shadow>();
	m_Shadow_DS->SetRatio(4.0f, 4.0f);

	m_Shadow_VP = g_Resource->GetViewPort<VP_Shadow>()->Get();
	m_Depth_RS = g_Resource->GetRasterizerState<RS_Depth>()->Get();

	// Shadow DepthStencilView 설정..
	m_Shadow_DSV = m_Shadow_DS->GetDSV()->Get();
}

void ShadowPass::OnResize(int width, int height)
{
	// Shadow DepthStencilView 재설정..
	m_Shadow_DSV = m_Shadow_DS->GetDSV()->Get();
}

void ShadowPass::InstanceResize(size_t& renderMaxCount, size_t& unRenderMaxCount)
{
	m_MeshInstance.resize(renderMaxCount);
	m_SkinMeshInstance.resize(renderMaxCount);
}

void ShadowPass::Release()
{

}

void ShadowPass::ApplyOption()
{
	g_Context->ClearDepthStencilView(m_Shadow_DSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void ShadowPass::BeginRender()
{
	g_Context->OMSetBlendState(0, 0, 0xffffffff);
	g_Context->RSSetViewports(1, m_Shadow_VP);

	// 그리기만 할 것이므로 null Render Target 설정..
	// 깊이 버퍼, null Rendering 대상을 설정하면 색상 쓰기가 비활성화 된다..
	g_Context->OMSetRenderTargets(0, nullptr, m_Shadow_DSV);
	g_Context->ClearDepthStencilView(m_Shadow_DSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
	g_Context->RSSetState(m_Depth_RS);
}

void ShadowPass::RenderUpdate(const InstanceRenderBuffer* instance, const std::vector<RenderData*>& meshlist)
{
	m_RenderCount = (int)meshlist.size();

	if (m_RenderCount == 1)
	{
		RenderUpdate(instance, meshlist[0]);
		return;
	}

	Matrix viewproj = g_GlobalData->DirectionLightList[0]->LightViewProj;

	MeshRenderBuffer* mesh = instance->m_Mesh;

	switch (instance->m_Type)
	{
	case OBJECT_TYPE::BASE:
	{
		int loopCount = 0;
		int renderCount = 0;
		int nowCount = 0;
		int checkCount = 0;

		while (true)
		{
			checkCount = m_RenderCount - renderCount;
			nowCount = (checkCount > 500) ? 500 : checkCount;

			if (nowCount < 1) break;

			// Instance Update..
			for (int i = 0; i < nowCount; i++)
			{
				if (meshlist[i]->m_Draw == false) continue;

				// 해당 Instance Data 삽입..
				m_MeshData.World = meshlist[i + renderCount]->m_ObjectData->World;

				m_MeshInstance[m_InstanceCount++] = m_MeshData;
			}

			if (m_InstanceCount == 0) return;

			// Instance Buffer Update..
			UpdateBuffer(m_Mesh_IB->InstanceBuf->Get(), &m_MeshInstance[0], (size_t)m_Mesh_IB->Stride * (size_t)m_InstanceCount);

			// Vertex Shader Update..
			CB_InstanceDepthStaticMesh shadowBuf;
			shadowBuf.gViewProj = viewproj;

			m_MeshInstShadow_VS->ConstantBufferUpdate(&shadowBuf);

			m_MeshInstShadow_VS->Update();

			ID3D11Buffer* vertexBuffers[2] = { mesh->m_VertexBuf, m_Mesh_IB->InstanceBuf->Get() };
			UINT strides[2] = { mesh->m_Stride, m_Mesh_IB->Stride };
			UINT offsets[2] = { 0,0 };

			// Draw..
			g_Context->IASetVertexBuffers(0, 2, vertexBuffers, strides, offsets);
			g_Context->IASetIndexBuffer(mesh->m_IndexBuf, DXGI_FORMAT_R32_UINT, 0);
			g_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			g_Context->DrawIndexedInstanced(mesh->m_IndexCount, m_InstanceCount, 0, 0, 0);

			loopCount++;
			renderCount = loopCount * 500;
			m_InstanceCount = 0;
		}
	}
	break;
	case OBJECT_TYPE::SKINNING:
	{
		ObjectData* object = nullptr;
		AnimationData* animation = nullptr;

		int loopCount = 0;
		int renderCount = 0;
		int nowCount = 0;
		int checkCount = 0;

		while (true)
		{
			checkCount = m_RenderCount - renderCount;
			nowCount = (checkCount > 500) ? 500 : checkCount;

			if (nowCount < 1) break;

			// Instance Update..
			for (int i = 0; i < nowCount; i++)
			{
				m_RenderData = meshlist[i + renderCount];

				if (m_RenderData->m_Draw == false) continue;

				object = m_RenderData->m_ObjectData;
				animation = m_RenderData->m_AnimationData;

				// 해당 Instance Data 삽입..
				m_SkinMeshData.World = object->World;
				m_SkinMeshData.PrevAnimationIndex = animation->PrevAnimationIndex + animation->PrevFrameIndex;
				m_SkinMeshData.NextAnimationIndex = animation->NextAnimationIndex + animation->NextFrameIndex;
				m_SkinMeshData.FrameTime = animation->FrameTime;

				m_SkinMeshInstance[m_InstanceCount++] = m_SkinMeshData;
			}

			if (m_InstanceCount == 0) return;

			// Instance Buffer Update..
			UpdateBuffer(m_SkinMesh_IB->InstanceBuf->Get(), &m_SkinMeshInstance[0], (size_t)m_SkinMesh_IB->Stride * (size_t)m_InstanceCount);

			// Vertex Shader Update..
			CB_InstanceDepthSkinMesh shadowBuf;
			shadowBuf.gViewProj = viewproj;

			m_SkinInstShadow_VS->ConstantBufferUpdate(&shadowBuf);
			m_SkinInstShadow_VS->SetShaderResourceView<gAnimationBuffer>(instance->m_Animation->m_AnimationBuf);

			m_SkinInstShadow_VS->Update();

			ID3D11Buffer* vertexBuffers[2] = { mesh->m_VertexBuf, m_SkinMesh_IB->InstanceBuf->Get() };
			UINT strides[2] = { mesh->m_Stride, m_SkinMesh_IB->Stride };
			UINT offsets[2] = { 0,0 };

			// Draw..
			g_Context->IASetVertexBuffers(0, 2, vertexBuffers, strides, offsets);
			g_Context->IASetIndexBuffer(mesh->m_IndexBuf, DXGI_FORMAT_R32_UINT, 0);
			g_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			g_Context->DrawIndexedInstanced(mesh->m_IndexCount, m_InstanceCount, 0, 0, 0);

			loopCount++;
			renderCount = loopCount * 500;
			m_InstanceCount = 0;
		}
	}
	break;
	default:
		break;
	}

	// Mesh Instance Data Clear..
	m_InstanceCount = 0;
}

void ShadowPass::RenderUpdate(const InstanceRenderBuffer* instance, const RenderData* meshData)
{
	if (meshData->m_Draw == false) return;

	ObjectData* obj = meshData->m_ObjectData;
	MeshRenderBuffer* mesh = instance->m_Mesh;

	const Matrix& world = obj->World;
	const Matrix& viewproj = g_GlobalData->DirectionLightList[0]->LightViewProj;

	switch (instance->m_Type)
	{
	case OBJECT_TYPE::BASE:
	case OBJECT_TYPE::TERRAIN:
	{
		// Vertex Shader Update..
		CB_DepthStaticMesh shadowBuf;
		shadowBuf.gWorldViewProj = world * viewproj;

		m_MeshShadow_VS->ConstantBufferUpdate(&shadowBuf);

		m_MeshShadow_VS->Update();
	}
	break;
	case OBJECT_TYPE::SKINNING:
	{
		AnimationData* animation = meshData->m_AnimationData;

		// Vertex Shader Update..
		CB_DepthSkinMesh shadowBuf;
		shadowBuf.gWorldViewProj = world * viewproj;
		shadowBuf.gPrevAnimationIndex = animation->PrevAnimationIndex + animation->PrevFrameIndex;
		shadowBuf.gNextAnimationIndex = animation->NextAnimationIndex + animation->NextFrameIndex;
		shadowBuf.gFrameTime = animation->FrameTime;

		m_SkinShadow_VS->ConstantBufferUpdate(&shadowBuf);
		m_SkinShadow_VS->SetShaderResourceView<gAnimationBuffer>(instance->m_Animation->m_AnimationBuf);

		m_SkinShadow_VS->Update();
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