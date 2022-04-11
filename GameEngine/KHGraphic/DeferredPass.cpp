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
#include "DeferredPass.h"

#include "MathDefine.h"
#include "FactoryManagerBase.h"
#include "ResourceManagerBase.h"
#include "ShaderManagerBase.h"
#include "ConstantBufferDefine.h"
#include "ShaderResourceViewDefine.h"
#include "ViewPortDefine.h"
#include "RenderTargetDefine.h"
#include "DepthStencilStateDefine.h"
#include "DepthStencilViewDefine.h"
#include "RasterizerStateDefine.h"
#include "BlendStateDefine.h"
#include "DrawBufferDefine.h"
#include "InstanceBufferDefine.h"
#include "MathHelper.h"

#define ALBEDO_MAP		0x00000001
#define NORMAL_MAP		0x00000010
#define EMISSIVE_MAP    0x00000100
#define ORM_MAP         0x00001000

DeferredPass::DeferredPass()
{

}

DeferredPass::~DeferredPass()
{

}

void DeferredPass::Create(int width, int height)
{
	/// Texture 2D
	// Albedo 전용 Texture 2D 
	D3D11_TEXTURE2D_DESC texDescDiffuse;
	ZeroMemory(&texDescDiffuse, sizeof(texDescDiffuse));
	texDescDiffuse.Width = width;
	texDescDiffuse.Height = height;
	texDescDiffuse.MipLevels = 1;
	texDescDiffuse.ArraySize = 1;
	texDescDiffuse.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDescDiffuse.SampleDesc.Count = 1;
	texDescDiffuse.SampleDesc.Quality = 0;
	texDescDiffuse.Usage = D3D11_USAGE_DEFAULT;
	texDescDiffuse.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDescDiffuse.CPUAccessFlags = 0;
	texDescDiffuse.MiscFlags = 0;

	// Pixel Data 전용 Texture 2D
	D3D11_TEXTURE2D_DESC texDescPosNormal;
	ZeroMemory(&texDescPosNormal, sizeof(texDescPosNormal));
	texDescPosNormal.Width = width;
	texDescPosNormal.Height = height;
	texDescPosNormal.MipLevels = 1;
	texDescPosNormal.ArraySize = 1;
	texDescPosNormal.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	texDescPosNormal.SampleDesc.Count = 1;
	texDescPosNormal.SampleDesc.Quality = 0;
	texDescPosNormal.Usage = D3D11_USAGE_DEFAULT;
	texDescPosNormal.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDescPosNormal.CPUAccessFlags = 0;
	texDescPosNormal.MiscFlags = 0;

	/// RenderTargetView 2D
	D3D11_RENDER_TARGET_VIEW_DESC rtvDescDiffuse;
	ZeroMemory(&rtvDescDiffuse, sizeof(rtvDescDiffuse));
	rtvDescDiffuse.Format = texDescDiffuse.Format;
	rtvDescDiffuse.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDescDiffuse.Texture2D.MipSlice = 0;

	D3D11_RENDER_TARGET_VIEW_DESC rtvDescPosNormal;
	ZeroMemory(&rtvDescPosNormal, sizeof(rtvDescPosNormal));
	rtvDescPosNormal.Format = texDescPosNormal.Format;
	rtvDescPosNormal.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDescPosNormal.Texture2D.MipSlice = 0;

	/// ShaderResourceView 2D
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDescDiffuse;
	ZeroMemory(&srvDescDiffuse, sizeof(srvDescDiffuse));
	srvDescDiffuse.Format = texDescDiffuse.Format;
	srvDescDiffuse.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDescDiffuse.Texture2D.MostDetailedMip = 0;
	srvDescDiffuse.Texture2D.MipLevels = 1;

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDescPosNormal;
	ZeroMemory(&srvDescPosNormal, sizeof(srvDescPosNormal));
	srvDescPosNormal.Format = texDescPosNormal.Format;
	srvDescPosNormal.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDescPosNormal.Texture2D.MostDetailedMip = 0;
	srvDescPosNormal.Texture2D.MipLevels = 1;

	// RenderTarget 생성..
	g_Factory->CreateRenderTexture<RT_Deffered_Albedo>(&texDescDiffuse, nullptr, &rtvDescDiffuse, &srvDescDiffuse, nullptr);
	g_Factory->CreateRenderTexture<RT_Deffered_Emissive>(&texDescPosNormal, nullptr, &rtvDescPosNormal, &srvDescPosNormal, nullptr);
	g_Factory->CreateRenderTexture<RT_Deffered_Normal>(&texDescPosNormal, nullptr, &rtvDescPosNormal, &srvDescPosNormal, nullptr);
	g_Factory->CreateRenderTexture<RT_Deffered_Position>(&texDescPosNormal, nullptr, &rtvDescPosNormal, &srvDescPosNormal, nullptr);
	g_Factory->CreateRenderTexture<RT_Deffered_NormalDepth>(&texDescPosNormal, nullptr, &rtvDescPosNormal, &srvDescPosNormal, nullptr);
}

void DeferredPass::Start(int width, int height)
{
	// Shader 설정..
	m_MeshVS = g_Shader->GetShader("StaticMesh_VS");
	m_MeshInstVS = g_Shader->GetShader("StaticMesh_Instance_VS");
	m_SkinVS = g_Shader->GetShader("SkinMesh_VS");
	m_TerrainVS = g_Shader->GetShader("TerrainMesh_VS");

	m_DeferredPS = g_Shader->GetShader("Deferred_PBR_PS");
	m_TerrainPS = g_Shader->GetShader("Terrain_PBR_PS");

	// Insatance Buffer 설정..
	m_Mesh_IB = g_Resource->GetInstanceBuffer<IB_Mesh>();

	// DepthStencilView 설정..
	m_DefaltDSV = g_Resource->GetDepthStencilView<DS_Defalt>()->Get();

	// Graphic State 설정..
	m_DefaltDSS = g_Resource->GetDepthStencilState<DSS_Defalt>()->Get();
	m_SolidRS = g_Resource->GetRasterizerState<RS_Solid>()->Get();
	m_AlphaBlendBS = g_Resource->GetBlendState<BS_AlphaBlend>()->Get();

	// ViewPort 설정..
	m_ScreenVP = g_Resource->GetViewPort<VP_FullScreen>()->Get();

	// RenderTarget 설정..
	m_AlbedoRT = g_Resource->GetRenderTexture<RT_Deffered_Albedo>();
	m_EmissiveRT = g_Resource->GetRenderTexture<RT_Deffered_Emissive>();
	m_NormalRT = g_Resource->GetRenderTexture<RT_Deffered_Normal>();
	m_PositionRT = g_Resource->GetRenderTexture<RT_Deffered_Position>();
	m_NormalDepthRT = g_Resource->GetRenderTexture<RT_Deffered_NormalDepth>();

	// RenderTargetView 설정..
	m_RTVList.resize(5);
	m_RTVList[0] = m_AlbedoRT->GetRTV()->Get();
	m_RTVList[1] = m_EmissiveRT->GetRTV()->Get();
	m_RTVList[2] = m_NormalRT->GetRTV()->Get();
	m_RTVList[3] = m_PositionRT->GetRTV()->Get();
	m_RTVList[4] = m_NormalDepthRT->GetRTV()->Get();
}

void DeferredPass::OnResize(int width, int height)
{
	// DepthStencilView 재설정..
	m_DefaltDSV = g_Resource->GetDepthStencilView<DS_Defalt>()->Get();

	// RenderTargetView List 재설정..
	m_RTVList[0] = m_AlbedoRT->GetRTV()->Get();
	m_RTVList[1] = m_EmissiveRT->GetRTV()->Get();
	m_RTVList[2] = m_NormalRT->GetRTV()->Get();
	m_RTVList[3] = m_PositionRT->GetRTV()->Get();
	m_RTVList[4] = m_NormalDepthRT->GetRTV()->Get();
}

void DeferredPass::InstanceResize(size_t& renderMaxCount, size_t& unRenderMaxCount)
{
	m_MeshInstance.resize(renderMaxCount);
}

void DeferredPass::Release()
{

}

void DeferredPass::SetOption(UINT renderOption)
{

}

void DeferredPass::BeginRender()
{
	g_Context->OMSetRenderTargets(5, &m_RTVList[0], m_DefaltDSV);

	// RenderTarget 초기화.. 
	g_Context->ClearRenderTargetView(m_RTVList[0], reinterpret_cast<const float*>(&DXColors::NonBlack));
	g_Context->ClearRenderTargetView(m_RTVList[1], reinterpret_cast<const float*>(&DXColors::NonBlack));
	g_Context->ClearRenderTargetView(m_RTVList[2], reinterpret_cast<const float*>(&DXColors::NonBlack));
	g_Context->ClearRenderTargetView(m_RTVList[3], reinterpret_cast<const float*>(&DXColors::NonBlack));
	g_Context->ClearRenderTargetView(m_RTVList[4], reinterpret_cast<const float*>(&DXColors::NonBlack));

	g_Context->ClearDepthStencilView(m_DefaltDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	
	g_Context->OMSetDepthStencilState(m_DefaltDSS, 0);
	g_Context->OMSetBlendState(0, 0, 0xffffffff);
	
	g_Context->RSSetViewports(1, m_ScreenVP);
	g_Context->RSSetState(m_SolidRS);
}

void DeferredPass::RenderUpdate(const InstanceRenderBuffer* instance, const std::vector<RenderData*>& meshlist)
{
	int RenderCount = (int)meshlist.size();

	if (RenderCount == 1)
	{
		RenderUpdate(instance, meshlist[0]);
		return;
	}

	CameraData* cam = g_GlobalData->MainCamera_Data;
	MeshRenderBuffer* mesh = instance->m_Mesh;
	MaterialRenderBuffer* mat = instance->m_Material;
	MaterialSubData* matSub = mat->m_MaterialSubData;

	Matrix& view = cam->CamView;
	Matrix& proj = cam->CamProj;
	
	for (int i = 0; i < RenderCount; i++)
	{
		if (meshlist[i]->m_Draw == false) continue;

		// 해당 Instance Data 삽입..
		m_MeshData.World = *meshlist[i]->m_ObjectData->World;
		m_MeshData.InvWorld = MathHelper::InverseTranspose(m_MeshData.World);

		m_MeshInstance[m_InstanceCount++] = m_MeshData;
	}

	if (m_InstanceCount == 0) return;

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
	{
		//if (mesh->m_Alpha) break;

		// Vertex Shader Update..
		CB_InstanceStaticMesh objectBuf;
		objectBuf.gView = view;
		objectBuf.gProj = proj;

		m_MeshInstVS->ConstantBufferUpdate(&objectBuf);

		m_MeshInstVS->Update();

		// Pixel Shader Update..
		CB_Material materialBuf;
		materialBuf.gAddColor = matSub->AddColor;
		materialBuf.gEmissiveFactor = matSub->EmissiveFactor;
		materialBuf.gRoughnessFactor = matSub->RoughnessFactor;
		materialBuf.gMetallicFactor = matSub->MetallicFactor;

		if (mat->m_Albedo)
		{
			materialBuf.gTexID |= ALBEDO_MAP;
			m_DeferredPS->SetShaderResourceView<gDiffuseMap>(mat->m_Albedo);
		}
		if (mat->m_Normal)
		{
			materialBuf.gTexID |= NORMAL_MAP;
			m_DeferredPS->SetShaderResourceView<gNormalMap>(mat->m_Normal);
		}
		if (mat->m_Emissive)
		{
			materialBuf.gTexID |= EMISSIVE_MAP;
			m_DeferredPS->SetShaderResourceView<gEmissiveMap>(mat->m_Emissive);
		}
		if (mat->m_ORM)
		{
			materialBuf.gTexID |= ORM_MAP;
			m_DeferredPS->SetShaderResourceView<gORMMap>(mat->m_ORM);
		}

		m_DeferredPS->ConstantBufferUpdate(&materialBuf);

		m_DeferredPS->Update();

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
		for (int i = 0; i < RenderCount; i++)
		{
			RenderUpdate(instance, meshlist[i]);
		}
	}
	break;
	default:
		break;
	}

	// Mesh Instance Data Clear..
	m_InstanceCount = 0;
}

void DeferredPass::RenderUpdate(const InstanceRenderBuffer* instance, const RenderData* meshData)
{
	if (meshData->m_Draw == false) return;

	CameraData* cam = g_GlobalData->MainCamera_Data;
	ObjectData* obj = meshData->m_ObjectData;
	MeshRenderBuffer* mesh = instance->m_Mesh;
	MaterialRenderBuffer* mat = instance->m_Material;
	MaterialSubData* matSub = mat->m_MaterialSubData;

	Matrix& world = *obj->World;
	Matrix&& invWorld = MathHelper::InverseTranspose(world);
	Matrix& view = cam->CamView;
	Matrix& proj = cam->CamProj;

	switch (instance->m_Type)
	{
	case OBJECT_TYPE::BASE:
	{
		// Vertex Shader Update..
		CB_StaticMesh objectBuf;
		objectBuf.gWorld = world;
		objectBuf.gInvWorld = invWorld;
		objectBuf.gView = view;
		objectBuf.gProj = proj;

		m_MeshVS->ConstantBufferUpdate(&objectBuf);

		m_MeshVS->Update();

		// Pixel Shader Update..
		CB_Material materialBuf;
		materialBuf.gAddColor = matSub->AddColor;
		materialBuf.gEmissiveFactor = matSub->EmissiveFactor;
		materialBuf.gRoughnessFactor = matSub->RoughnessFactor;
		materialBuf.gMetallicFactor = matSub->MetallicFactor;

		if (mat->m_Albedo)
		{
			materialBuf.gTexID |= ALBEDO_MAP;
			m_DeferredPS->SetShaderResourceView<gDiffuseMap>(mat->m_Albedo);
		}
		if (mat->m_Normal)
		{
			materialBuf.gTexID |= NORMAL_MAP;
			m_DeferredPS->SetShaderResourceView<gNormalMap>(mat->m_Normal);
		}
		if (mat->m_Emissive)
		{
			materialBuf.gTexID |= EMISSIVE_MAP;
			m_DeferredPS->SetShaderResourceView<gEmissiveMap>(mat->m_Emissive);
		}
		if (mat->m_ORM)
		{
			materialBuf.gTexID |= ORM_MAP;
			m_DeferredPS->SetShaderResourceView<gORMMap>(mat->m_ORM);
		}

		m_DeferredPS->ConstantBufferUpdate(&materialBuf);

		m_DeferredPS->Update();

		// Draw..
		g_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		g_Context->IASetVertexBuffers(0, 1, &mesh->m_VertexBuf, &mesh->m_Stride, &mesh->m_Offset);
		g_Context->IASetIndexBuffer(mesh->m_IndexBuf, DXGI_FORMAT_R32_UINT, 0);

		g_Context->DrawIndexed(mesh->m_IndexCount, 0, 0);
	}
	break;
	case OBJECT_TYPE::TERRAIN:
	{
		TerrainRenderBuffer* terrain = meshData->m_TerrainBuffer;

		// Vertex Shader Update..
		CB_StaticMesh objectBuf;
		objectBuf.gWorld = world;
		objectBuf.gInvWorld = invWorld;
		objectBuf.gView = view;
		objectBuf.gProj = proj;
		objectBuf.gTexTransform = *terrain->m_Tex;
		m_TerrainVS->ConstantBufferUpdate(&objectBuf);

		m_TerrainVS->Update();

		// Pixel Shader Update..
		MaterialRenderBuffer* layer1 = terrain->m_MaterialList[0];
		MaterialRenderBuffer* layer2 = terrain->m_MaterialList[1];
		MaterialRenderBuffer* layer3 = terrain->m_MaterialList[2];
		MaterialRenderBuffer* layer4 = terrain->m_MaterialList[3];
		m_TerrainPS->SetShaderResourceView<gDiffuseLayer1>(layer1->m_Albedo);
		m_TerrainPS->SetShaderResourceView<gNormalLayer1>(layer1->m_Normal);
		m_TerrainPS->SetShaderResourceView<gORMLayer1>(layer1->m_ORM);
		m_TerrainPS->SetShaderResourceView<gDiffuseLayer2>(layer2->m_Albedo);
		m_TerrainPS->SetShaderResourceView<gNormalLayer2>(layer2->m_Normal);
		m_TerrainPS->SetShaderResourceView<gORMLayer2>(layer2->m_ORM);
		m_TerrainPS->SetShaderResourceView<gDiffuseLayer3>(layer3->m_Albedo);
		m_TerrainPS->SetShaderResourceView<gNormalLayer3>(layer3->m_Normal);
		m_TerrainPS->SetShaderResourceView<gORMLayer3>(layer3->m_ORM);
		m_TerrainPS->SetShaderResourceView<gDiffuseLayer4>(layer4->m_Albedo);
		m_TerrainPS->SetShaderResourceView<gNormalLayer4>(layer4->m_Normal);
		m_TerrainPS->SetShaderResourceView<gORMLayer4>(layer4->m_ORM);

		m_TerrainPS->Update();

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
		CB_SkinMesh objectBuf;
		objectBuf.gWorld = XMMatrixIdentity();
		objectBuf.gInvWorld = invWorld;
		objectBuf.gView = view;
		objectBuf.gProj = proj;

		for (int i = 0; i < obj->BoneOffsetTM.size(); i++)
		{
			objectBuf.gBoneTransforms[i] = (obj->BoneOffsetTM)[i];
		}

		m_SkinVS->ConstantBufferUpdate(&objectBuf);

		m_SkinVS->Update();

		// Pixel Shader Update..
		CB_Material materialBuf;
		materialBuf.gAddColor = matSub->AddColor;
		materialBuf.gEmissiveFactor = matSub->EmissiveFactor;
		materialBuf.gRoughnessFactor = matSub->RoughnessFactor;
		materialBuf.gMetallicFactor = matSub->MetallicFactor;

		if (mat->m_Albedo)
		{
			materialBuf.gTexID |= ALBEDO_MAP;
			m_DeferredPS->SetShaderResourceView<gDiffuseMap>(mat->m_Albedo);
		}
		if (mat->m_Normal)
		{
			materialBuf.gTexID |= NORMAL_MAP;
			m_DeferredPS->SetShaderResourceView<gNormalMap>(mat->m_Normal);
		}
		if (mat->m_Emissive)
		{
			materialBuf.gTexID |= EMISSIVE_MAP;
			m_DeferredPS->SetShaderResourceView<gEmissiveMap>(mat->m_Emissive);
		}
		if (mat->m_ORM)
		{
			materialBuf.gTexID |= ORM_MAP;
			m_DeferredPS->SetShaderResourceView<gORMMap>(mat->m_ORM);
		}

		m_DeferredPS->ConstantBufferUpdate(&materialBuf);

		m_DeferredPS->Update();

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
