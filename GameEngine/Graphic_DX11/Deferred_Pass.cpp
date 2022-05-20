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
#include "Deferred_Pass.h"

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
#define LIM_LIGHT       0x00010000

Deferred_Pass::Deferred_Pass()
{

}

Deferred_Pass::~Deferred_Pass()
{

}

void Deferred_Pass::Create(int width, int height)
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

void Deferred_Pass::Start(int width, int height)
{
	// Shader 설정..
	m_Mesh_VS = g_Shader->GetShader("StaticMesh_VS");
	m_MeshInst_VS = g_Shader->GetShader("StaticMesh_Instance_VS");
	m_Skin_VS = g_Shader->GetShader("SkinMesh_VS");
	m_SkinInst_VS = g_Shader->GetShader("SkinMesh_Instance_VS");
	m_Terrain_VS = g_Shader->GetShader("TerrainMesh_VS");

	m_Deferred_PS = g_Shader->GetShader("Deferred_PBR_PS");
	m_Terrain_PS = g_Shader->GetShader("Terrain_PBR_PS");

	// Insatance Buffer 설정..
	m_Mesh_IB = g_Resource->GetInstanceBuffer<IB_Mesh>();
	m_SkinMesh_IB = g_Resource->GetInstanceBuffer<IB_SkinMesh>();

	// DepthStencilView 설정..
	m_Defalt_DSV = g_Resource->GetDepthStencilView<DS_Defalt>()->Get();

	// Graphic State 설정..
	m_Defalt_DSS = g_Resource->GetDepthStencilState<DSS_Defalt>()->Get();
	m_Solid_RS = g_Resource->GetRasterizerState<RS_Solid>()->Get();
	m_AlphaBlend_BS = g_Resource->GetBlendState<BS_AlphaBlend>()->Get();

	// ViewPort 설정..
	m_Screen_VP = g_Resource->GetViewPort<VP_FullScreen>()->Get();

	// RenderTarget 설정..
	m_Albedo_RT = g_Resource->GetRenderTexture<RT_Deffered_Albedo>();
	m_Emissive_RT = g_Resource->GetRenderTexture<RT_Deffered_Emissive>();
	m_Normal_RT = g_Resource->GetRenderTexture<RT_Deffered_Normal>();
	m_Position_RT = g_Resource->GetRenderTexture<RT_Deffered_Position>();
	m_NormalDepth_RT = g_Resource->GetRenderTexture<RT_Deffered_NormalDepth>();

	// RenderTargetView 설정..
	m_RTV_List.resize(5);
	m_RTV_List[0] = m_Albedo_RT->GetRTV()->Get();
	m_RTV_List[1] = m_Emissive_RT->GetRTV()->Get();
	m_RTV_List[2] = m_Normal_RT->GetRTV()->Get();
	m_RTV_List[3] = m_Position_RT->GetRTV()->Get();
	m_RTV_List[4] = m_NormalDepth_RT->GetRTV()->Get();

	// Deferred Shader List Up..
	SetShaderList();

	// Deferred Shader Resource View Set Up..
	SetShaderResourceView();
}

void Deferred_Pass::OnResize(int width, int height)
{
	// DepthStencilView 재설정..
	m_Defalt_DSV = g_Resource->GetDepthStencilView<DS_Defalt>()->Get();

	// RenderTargetView List 재설정..
	m_RTV_List[0] = m_Albedo_RT->GetRTV()->Get();
	m_RTV_List[1] = m_Emissive_RT->GetRTV()->Get();
	m_RTV_List[2] = m_Normal_RT->GetRTV()->Get();
	m_RTV_List[3] = m_Position_RT->GetRTV()->Get();
	m_RTV_List[4] = m_NormalDepth_RT->GetRTV()->Get();

	// Deferred Shader Resource View Set Up..
	SetShaderResourceView();
}

void Deferred_Pass::InstanceResize(size_t& renderMaxCount, size_t& unRenderMaxCount)
{
	m_MeshInstance.resize(renderMaxCount);
	m_SkinMeshInstance.resize(renderMaxCount);
}

void Deferred_Pass::Release()
{

}

void Deferred_Pass::ApplyOption(UINT renderOption)
{

}

void Deferred_Pass::BeginRender()
{
	g_Context->OMSetRenderTargets(5, &m_RTV_List[0], m_Defalt_DSV);

	// RenderTarget 초기화.. 
	g_Context->ClearRenderTargetView(m_RTV_List[0], reinterpret_cast<const float*>(&DXColors::NonBlack));
	g_Context->ClearRenderTargetView(m_RTV_List[1], reinterpret_cast<const float*>(&DXColors::NonBlack));
	g_Context->ClearRenderTargetView(m_RTV_List[2], reinterpret_cast<const float*>(&DXColors::NonBlack));
	g_Context->ClearRenderTargetView(m_RTV_List[3], reinterpret_cast<const float*>(&DXColors::NonBlack));
	g_Context->ClearRenderTargetView(m_RTV_List[4], reinterpret_cast<const float*>(&DXColors::NonBlack));

	g_Context->ClearDepthStencilView(m_Defalt_DSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	
	g_Context->OMSetDepthStencilState(m_Defalt_DSS, 0);
	g_Context->OMSetBlendState(0, 0, 0xffffffff);
	
	g_Context->RSSetViewports(1, m_Screen_VP);
	g_Context->RSSetState(m_Solid_RS);
}

void Deferred_Pass::RenderUpdate(const InstanceRenderBuffer* instance, const std::vector<RenderData*>& meshlist)
{
	m_RenderCount = (int)meshlist.size();

	if (m_RenderCount == 1)
	{
		RenderUpdate(instance, meshlist[0]);
		return;
	}

	const CameraData* cam = g_GlobalData->MainCamera_Data;
	const MeshRenderBuffer* mesh = instance->m_Mesh;
	const MaterialRenderBuffer* mat = instance->m_Material;
	const MaterialProperty* matSub = mat->m_MaterialProperty;

	const Matrix& view = cam->CamView;
	const Matrix& proj = cam->CamProj;

	switch (instance->m_Type)
	{
	case OBJECT_TYPE::BASE:
	{
		int loopCount = 0;
		int renderCount = 0;
		int nowCount = 0;
		int checkCount = 0;

		ObjectData* object = nullptr;
		AnimationData* animation = nullptr;

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

				if (object->IsMaterialBlock)
				{
					RenderUpdate(instance, m_RenderData);
					continue;
				}

				// 해당 Instance Data 삽입..
				m_MeshData.World = m_RenderData->m_ObjectData->World;
				m_MeshData.InvWorld = m_RenderData->m_ObjectData->InvWorld;

				m_MeshInstance[m_InstanceCount++] = m_MeshData;
			}

			if (m_InstanceCount == 0) return;

			// Instance Buffer Update..
			UpdateBuffer(m_Mesh_IB->InstanceBuf->Get(), &m_MeshInstance[0], (size_t)m_Mesh_IB->Stride * (size_t)m_InstanceCount);

			// Vertex Shader Update..
			CB_InstanceStaticMesh objectBuf;
			objectBuf.gView = view;
			objectBuf.gProj = proj;

			m_MeshInst_VS->ConstantBufferUpdate(&objectBuf);

			m_MeshInst_VS->Update();

			// Pixel Shader Update..
			CB_Material materialBuf;
			materialBuf.gAddColor = matSub->AddColor;
			materialBuf.gRoughnessFactor = matSub->RoughnessFactor;
			materialBuf.gMetallicFactor = matSub->MetallicFactor;
			materialBuf.gEmissiveColor = matSub->EmissiveColor * matSub->EmissiveFactor;
			materialBuf.gLimLightColor = matSub->LimLightColor * matSub->LimLightFactor;
			materialBuf.gLimLightWidth = matSub->LimLightWidth;

			CB_Camera cameraBuf;
			cameraBuf.gEyePos = cam->CamPos;

			if (mat->m_Albedo)
			{
				materialBuf.gOption |= ALBEDO_MAP;
				m_Deferred_PS->SetShaderResourceView<gDiffuseMap>(mat->m_Albedo);
			}
			if (mat->m_Normal)
			{
				materialBuf.gOption |= NORMAL_MAP;
				m_Deferred_PS->SetShaderResourceView<gNormalMap>(mat->m_Normal);
			}
			if (mat->m_Emissive)
			{
				materialBuf.gOption |= EMISSIVE_MAP;
				m_Deferred_PS->SetShaderResourceView<gEmissiveMap>(mat->m_Emissive);
			}
			if (mat->m_ORM)
			{
				materialBuf.gOption |= ORM_MAP;
				m_Deferred_PS->SetShaderResourceView<gORMMap>(mat->m_ORM);
			}
			if (matSub->LimLightFactor > 0.0f)
			{
				materialBuf.gOption |= LIM_LIGHT;
			}

			m_Deferred_PS->ConstantBufferUpdate(&materialBuf);
			m_Deferred_PS->ConstantBufferUpdate(&cameraBuf);

			m_Deferred_PS->Update();

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

			// Mesh Instance Data Clear..
			m_InstanceCount = 0;
		}
	}
	break;
	case OBJECT_TYPE::SKINNING:
	{
		int loopCount = 0;
		int renderCount = 0;
		int nowCount = 0;
		int checkCount = 0;

		ObjectData* object = nullptr;
		AnimationData* animation = nullptr;

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

				if (object->IsMaterialBlock)
				{
					RenderUpdate(instance, m_RenderData);
					continue;
				}

				animation = m_RenderData->m_AnimationData;

				// 해당 Instance Data 삽입..
				m_SkinMeshData.World = object->World;
				m_SkinMeshData.InvWorld = object->InvWorld;
				m_SkinMeshData.PrevAnimationIndex = animation->PrevAnimationIndex + animation->PrevFrameIndex;
				m_SkinMeshData.NextAnimationIndex = animation->NextAnimationIndex + animation->NextFrameIndex;
				m_SkinMeshData.FrameTime = animation->FrameTime;

				m_SkinMeshInstance[m_InstanceCount++] = m_SkinMeshData;
			}

			if (m_InstanceCount == 0) return;

			// Instance Buffer Update..
			UpdateBuffer(m_SkinMesh_IB->InstanceBuf->Get(), &m_SkinMeshInstance[0], (size_t)m_SkinMesh_IB->Stride * (size_t)m_InstanceCount);

			// Vertex Shader Update..
			CB_InstanceSkinMesh objectBuf;
			objectBuf.gView = view;
			objectBuf.gProj = proj;

			m_SkinInst_VS->ConstantBufferUpdate(&objectBuf);
			m_SkinInst_VS->SetShaderResourceView<gAnimationBuffer>(instance->m_Animation->m_AnimationBuf);

			m_SkinInst_VS->Update();

			// Pixel Shader Update..
			CB_Material materialBuf;
			materialBuf.gAddColor = matSub->AddColor;
			materialBuf.gRoughnessFactor = matSub->RoughnessFactor;
			materialBuf.gMetallicFactor = matSub->MetallicFactor;
			materialBuf.gEmissiveColor = matSub->EmissiveColor * matSub->EmissiveFactor;
			materialBuf.gLimLightColor = matSub->LimLightColor * matSub->LimLightFactor;
			materialBuf.gLimLightWidth = matSub->LimLightWidth;

			CB_Camera cameraBuf;
			cameraBuf.gEyePos = cam->CamPos;

			if (mat->m_Albedo)
			{
				materialBuf.gOption |= ALBEDO_MAP;
				m_Deferred_PS->SetShaderResourceView<gDiffuseMap>(mat->m_Albedo);
			}
			if (mat->m_Normal)
			{
				materialBuf.gOption |= NORMAL_MAP;
				m_Deferred_PS->SetShaderResourceView<gNormalMap>(mat->m_Normal);
			}
			if (mat->m_Emissive)
			{
				materialBuf.gOption |= EMISSIVE_MAP;
				m_Deferred_PS->SetShaderResourceView<gEmissiveMap>(mat->m_Emissive);
			}
			if (mat->m_ORM)
			{
				materialBuf.gOption |= ORM_MAP;
				m_Deferred_PS->SetShaderResourceView<gORMMap>(mat->m_ORM);
			}
			if (matSub->LimLightFactor > 0.0f)
			{
				materialBuf.gOption |= LIM_LIGHT;
			}

			m_Deferred_PS->ConstantBufferUpdate(&materialBuf);
			m_Deferred_PS->ConstantBufferUpdate(&cameraBuf);

			m_Deferred_PS->Update();

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

			// Mesh Instance Data Clear..
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

void Deferred_Pass::RenderUpdate(const InstanceRenderBuffer* instance, const RenderData* meshData)
{
	if (meshData->m_Draw == false) return;

	const CameraData* cam = g_GlobalData->MainCamera_Data;
	const ObjectData* obj = meshData->m_ObjectData;
	const MeshRenderBuffer* mesh = instance->m_Mesh;
	const MaterialRenderBuffer* mat = instance->m_Material;
	const MaterialProperty* matSub;

	if (obj->IsMaterialBlock)
	{
		matSub = obj->Material_Block;
	}
	else
	{
		matSub = mat->m_MaterialProperty;
	}

	const Matrix& world = obj->World;
	const Matrix& invWorld = obj->InvWorld;
	const Matrix& view = cam->CamView;
	const Matrix& proj = cam->CamProj;

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

		m_Mesh_VS->ConstantBufferUpdate(&objectBuf);

		m_Mesh_VS->Update();

		// Pixel Shader Update..
		CB_Material materialBuf;
		materialBuf.gAddColor = matSub->AddColor;
		materialBuf.gRoughnessFactor = matSub->RoughnessFactor;
		materialBuf.gMetallicFactor = matSub->MetallicFactor;
		materialBuf.gEmissiveColor = matSub->EmissiveColor * matSub->EmissiveFactor;
		materialBuf.gLimLightColor = matSub->LimLightColor * matSub->LimLightFactor;
		materialBuf.gLimLightWidth = matSub->LimLightWidth;

		CB_Camera cameraBuf;
		cameraBuf.gEyePos = cam->CamPos;

		if (mat->m_Albedo)
		{
			materialBuf.gOption |= ALBEDO_MAP;
			m_Deferred_PS->SetShaderResourceView<gDiffuseMap>(mat->m_Albedo);
		}
		if (mat->m_Normal)
		{
			materialBuf.gOption |= NORMAL_MAP;
			m_Deferred_PS->SetShaderResourceView<gNormalMap>(mat->m_Normal);
		}
		if (mat->m_Emissive)
		{
			materialBuf.gOption |= EMISSIVE_MAP;
			m_Deferred_PS->SetShaderResourceView<gEmissiveMap>(mat->m_Emissive);
		}
		if (mat->m_ORM)
		{
			materialBuf.gOption |= ORM_MAP;
			m_Deferred_PS->SetShaderResourceView<gORMMap>(mat->m_ORM);
		}
		if (matSub->LimLightFactor > 0.0f)
		{
			materialBuf.gOption |= LIM_LIGHT;
		}

		m_Deferred_PS->ConstantBufferUpdate(&materialBuf);
		m_Deferred_PS->ConstantBufferUpdate(&cameraBuf);

		m_Deferred_PS->Update();

		// Draw..
		g_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		g_Context->IASetVertexBuffers(0, 1, &mesh->m_VertexBuf, &mesh->m_Stride, &mesh->m_Offset);
		g_Context->IASetIndexBuffer(mesh->m_IndexBuf, DXGI_FORMAT_R32_UINT, 0);

		g_Context->DrawIndexed(mesh->m_IndexCount, 0, 0);
	}
	break;
	case OBJECT_TYPE::TERRAIN:
	{
		TerrainRenderBuffer* terrain = meshData->m_Terrain;

		const Matrix& TexTM = *terrain->m_Tex;

		// Vertex Shader Update..
		CB_StaticMesh objectBuf;
		objectBuf.gWorld = world;
		objectBuf.gInvWorld = invWorld;
		objectBuf.gView = view;
		objectBuf.gProj = proj;
		objectBuf.gTexScale.x = TexTM._11;
		objectBuf.gTexScale.y = TexTM._22;
		objectBuf.gTexPos.x = TexTM._41;
		objectBuf.gTexPos.y = TexTM._42;
		m_Terrain_VS->ConstantBufferUpdate(&objectBuf);

		m_Terrain_VS->Update();

		// Pixel Shader Update..
		MaterialRenderBuffer* layer1 = terrain->m_MaterialList[0];
		MaterialRenderBuffer* layer2 = terrain->m_MaterialList[1];
		MaterialRenderBuffer* layer3 = terrain->m_MaterialList[2];
		MaterialRenderBuffer* layer4 = terrain->m_MaterialList[3];
		m_Terrain_PS->SetShaderResourceView<gDiffuseLayer1>(layer1->m_Albedo);
		m_Terrain_PS->SetShaderResourceView<gNormalLayer1>(layer1->m_Normal);
		m_Terrain_PS->SetShaderResourceView<gORMLayer1>(layer1->m_ORM);
		m_Terrain_PS->SetShaderResourceView<gDiffuseLayer2>(layer2->m_Albedo);
		m_Terrain_PS->SetShaderResourceView<gNormalLayer2>(layer2->m_Normal);
		m_Terrain_PS->SetShaderResourceView<gORMLayer2>(layer2->m_ORM);
		m_Terrain_PS->SetShaderResourceView<gDiffuseLayer3>(layer3->m_Albedo);
		m_Terrain_PS->SetShaderResourceView<gNormalLayer3>(layer3->m_Normal);
		m_Terrain_PS->SetShaderResourceView<gORMLayer3>(layer3->m_ORM);
		m_Terrain_PS->SetShaderResourceView<gDiffuseLayer4>(layer4->m_Albedo);
		m_Terrain_PS->SetShaderResourceView<gNormalLayer4>(layer4->m_Normal);
		m_Terrain_PS->SetShaderResourceView<gORMLayer4>(layer4->m_ORM);

		m_Terrain_PS->Update();

		// Draw..
		g_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		g_Context->IASetVertexBuffers(0, 1, &mesh->m_VertexBuf, &mesh->m_Stride, &mesh->m_Offset);
		g_Context->IASetIndexBuffer(mesh->m_IndexBuf, DXGI_FORMAT_R32_UINT, 0);
		
		g_Context->DrawIndexed(mesh->m_IndexCount, 0, 0);
	}
	break;
	case OBJECT_TYPE::SKINNING:
	{
		AnimationData* animation = meshData->m_AnimationData;
		
		// Vertex Shader Update..
		CB_SkinMesh objectBuf;
		objectBuf.gWorld = world;
		objectBuf.gInvWorld = invWorld;
		objectBuf.gView = view;
		objectBuf.gProj = proj;
		objectBuf.gPrevAnimationIndex = animation->PrevAnimationIndex + animation->PrevFrameIndex;
		objectBuf.gNextAnimationIndex = animation->NextAnimationIndex + animation->NextFrameIndex;
		objectBuf.gFrameTime = animation->FrameTime;

		m_Skin_VS->ConstantBufferUpdate(&objectBuf);
		m_Skin_VS->SetShaderResourceView<gAnimationBuffer>(instance->m_Animation->m_AnimationBuf);

		m_Skin_VS->Update();

		// Pixel Shader Update..
		CB_Material materialBuf;
		materialBuf.gAddColor = matSub->AddColor;
		materialBuf.gRoughnessFactor = matSub->RoughnessFactor;
		materialBuf.gMetallicFactor = matSub->MetallicFactor;
		materialBuf.gEmissiveColor = matSub->EmissiveColor * matSub->EmissiveFactor;
		materialBuf.gLimLightColor = matSub->LimLightColor * matSub->LimLightFactor;
		materialBuf.gLimLightWidth = matSub->LimLightWidth;

		CB_Camera cameraBuf;
		cameraBuf.gEyePos = cam->CamPos;

		if (mat->m_Albedo)
		{
			materialBuf.gOption |= ALBEDO_MAP;
			m_Deferred_PS->SetShaderResourceView<gDiffuseMap>(mat->m_Albedo);
		}
		if (mat->m_Normal)
		{
			materialBuf.gOption |= NORMAL_MAP;
			m_Deferred_PS->SetShaderResourceView<gNormalMap>(mat->m_Normal);
		}
		if (mat->m_Emissive)
		{
			materialBuf.gOption |= EMISSIVE_MAP;
			m_Deferred_PS->SetShaderResourceView<gEmissiveMap>(mat->m_Emissive);
		}
		if (mat->m_ORM)
		{
			materialBuf.gOption |= ORM_MAP;
			m_Deferred_PS->SetShaderResourceView<gORMMap>(mat->m_ORM);
		}
		if (matSub->LimLightFactor > 0.0f)
		{
			materialBuf.gOption |= LIM_LIGHT;
		}

		m_Deferred_PS->ConstantBufferUpdate(&materialBuf);
		m_Deferred_PS->ConstantBufferUpdate(&cameraBuf);

		m_Deferred_PS->Update();

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

void Deferred_Pass::SetShaderList()
{
	PushShader("Light_PBR_PS_Option0");
	PushShader("Light_PBR_PS_Option1");
	PushShader("Light_PBR_PS_Option2");
	PushShader("Light_PBR_PS_Option3");
	PushShader("Light_PBR_PS_Option4");
	PushShader("Light_PBR_PS_Option5");
	PushShader("Light_PBR_PS_Option6");
	PushShader("Light_PBR_PS_Option7");

	PushShader("Light_IBL_PS_Option0");
	PushShader("Light_IBL_PS_Option1");
	PushShader("Light_IBL_PS_Option2");
	PushShader("Light_IBL_PS_Option3");
	PushShader("Light_IBL_PS_Option4");
	PushShader("Light_IBL_PS_Option5");
	PushShader("Light_IBL_PS_Option6");
	PushShader("Light_IBL_PS_Option7");

	PushShader("SSAO_PS");
	PushShader("SSAOBlur_PS");
}

void Deferred_Pass::SetShaderResourceView()
{
	ID3D11ShaderResourceView* albedoMap			= m_Albedo_RT->GetSRV()->Get();
	ID3D11ShaderResourceView* emissiveMap		= m_Emissive_RT->GetSRV()->Get();
	ID3D11ShaderResourceView* normalMap			= m_Normal_RT->GetSRV()->Get();
	ID3D11ShaderResourceView* positionMap		= m_Position_RT->GetSRV()->Get();
	ID3D11ShaderResourceView* normalDepthMap	= m_NormalDepth_RT->GetSRV()->Get();

	for (ShaderBase* shader : m_OptionShaderList)
	{
		shader->SetShaderResourceView<gAlbedoRT>(albedoMap);
		shader->SetShaderResourceView<gEmissiveRT>(emissiveMap);
		shader->SetShaderResourceView<gNormalRT>(normalMap);
		shader->SetShaderResourceView<gPositionRT>(positionMap);
		shader->SetShaderResourceView<gNormalDepthRT>(normalDepthMap);
	}
}
