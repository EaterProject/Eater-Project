#include "DirectDefine.h"
#include "RenderPassBase.h"
#include "ShaderBase.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "GraphicState.h"
#include "GraphicView.h"
#include "GraphicResource.h"
#include "DepthStencil.h"
#include "RenderTarget.h"
#include "DrawBuffer.h"
#include "MathDefine.h"
#include "EngineData.h"
#include "RenderData.h"
#include "VertexDefine.h"
#include "Alpha_Pass.h"

#include "ResourceFactoryBase.h"
#include "ResourceManagerBase.h"
#include "ShaderManagerBase.h"
#include "ConstantBufferDefine.h"
#include "DepthStencilViewDefine.h"
#include "DepthStencilStateDefine.h"
#include "BlendStateDefine.h"
#include "ShaderResourceViewDefine.h"
#include "RenderTargetDefine.h"
#include "RasterizerStateDefine.h"
#include "ShaderResourceViewDefine.h"
#include "InstanceBufferDefine.h"

#define ALBEDO_MAP		0x00000001
#define NORMAL_MAP		0x00000010
#define EMISSIVE_MAP    0x00000100
#define ORM_MAP         0x00001000
#define LIM_LIGHT       0x00010000
#define DISSOLVE        0x00100000

Alpha_Pass::Alpha_Pass()
{

}

Alpha_Pass::~Alpha_Pass()
{

}

void Alpha_Pass::Start(int width, int height)
{
	// Shader 설정..
	m_ParticleInst_VS = g_Shader->GetShader("Particle_Instance_VS");
	m_Particle_PS = g_Shader->GetShader("OIT_Particle_PS_Option1");

	m_Mesh_VS = g_Shader->GetShader("StaticMesh_VS");
	m_MeshInst_VS = g_Shader->GetShader("StaticMesh_Instance_VS");

	m_Skin_VS = g_Shader->GetShader("SkinMesh_VS");
	m_SkinInst_VS = g_Shader->GetShader("SkinMesh_Instance_VS");

	m_Mesh_PS = g_Shader->GetShader("OIT_Mesh_PS_Option7");

	m_Mesh_IB = g_Resource->GetInstanceBuffer<IB_Mesh>();
	m_Particle_IB = g_Resource->GetInstanceBuffer<IB_Particle>();

	m_ParticleInstance.resize(500);
}

void Alpha_Pass::InstanceResize(size_t& renderMaxCount, size_t& unRenderMaxCount)
{
	m_MeshInstance.resize(renderMaxCount);
}

void Alpha_Pass::Release()
{

}

void Alpha_Pass::ApplyOption()
{
	UINT alphaOption = g_RenderOption->RenderingOption & (RENDER_SHADOW | RENDER_IBL | RENDER_FOG);

	if (alphaOption & RENDER_FOG)
	{
		m_Particle_PS = g_Shader->GetShader("OIT_Particle_PS_Option1");
	}
	else
	{
		m_Particle_PS = g_Shader->GetShader("OIT_Particle_PS_Option0");
	}

	switch (alphaOption)
	{
	case RENDER_SHADOW:
		m_Mesh_PS = g_Shader->GetShader("OIT_Mesh_PS_Option1");
		break;
	case RENDER_IBL:
		m_Mesh_PS = g_Shader->GetShader("OIT_Mesh_PS_Option2");
		break;
	case RENDER_FOG:
		m_Mesh_PS = g_Shader->GetShader("OIT_Mesh_PS_Option3");
		break;
	case RENDER_SHADOW | RENDER_IBL:
		m_Mesh_PS = g_Shader->GetShader("OIT_Mesh_PS_Option4");
		break;
	case RENDER_SHADOW | RENDER_FOG:
		m_Mesh_PS = g_Shader->GetShader("OIT_Mesh_PS_Option5");
		break;
	case RENDER_IBL | RENDER_FOG:
		m_Mesh_PS = g_Shader->GetShader("OIT_Mesh_PS_Option6");
		break;
	case RENDER_SHADOW | RENDER_IBL | RENDER_FOG:
		m_Mesh_PS = g_Shader->GetShader("OIT_Mesh_PS_Option7");
		break;
	default:
		m_Mesh_PS = g_Shader->GetShader("OIT_Mesh_PS_Option0");
		break;
	}
}

void Alpha_Pass::RenderUpdate(const InstanceRenderBuffer* instance, const RenderData* meshData)
{
	const CameraData* cam = g_GlobalData->MainCamera_Data;
	const ObjectData* obj = meshData->m_ObjectData;
	const MeshRenderBuffer* mesh = instance->m_Mesh;
	const MaterialRenderBuffer* mat = instance->m_Material;
	MaterialProperty* matSub = mat->m_MaterialProperty;

	const Matrix& world = obj->World;
	const Matrix& invWorld = obj->InvWorld;
	const Matrix& view = cam->CamView;
	const Matrix& proj = cam->CamProj;
	const Matrix& viewproj = cam->CamViewProj;

	// Constant Buffer Setting..
	CB_Material materialBuf;
	materialBuf.gAddColor = matSub->AddColor;
	materialBuf.gRoughnessFactor = matSub->RoughnessFactor;
	materialBuf.gMetallicFactor = matSub->MetallicFactor;
	materialBuf.gEmissiveColor = matSub->EmissiveColor * matSub->EmissiveFactor;
	materialBuf.gLimLightColor = matSub->LimLightColor * matSub->LimLightFactor;
	materialBuf.gLimLightWidth = matSub->LimLightWidth;
	materialBuf.gSkyLightIndex = matSub->SkyLightIndex;

	// Instance Draw..
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

		// Pixel Shader Update..
		if (mat->m_Albedo)
		{
			materialBuf.gOption |= ALBEDO_MAP;
			m_Mesh_PS->SetShaderResourceView<gDiffuseMap>(mat->m_Albedo);
		}
		if (mat->m_Normal)
		{
			materialBuf.gOption |= NORMAL_MAP;
			m_Mesh_PS->SetShaderResourceView<gNormalMap>(mat->m_Normal);
		}
		if (mat->m_Emissive)
		{
			materialBuf.gOption |= EMISSIVE_MAP;
			m_Mesh_PS->SetShaderResourceView<gEmissiveMap>(mat->m_Emissive);
		}
		if (mat->m_ORM)
		{
			materialBuf.gOption |= ORM_MAP;
			m_Mesh_PS->SetShaderResourceView<gORMMap>(mat->m_ORM);
		}
		if (matSub->LimLightFactor > 0.0f)
		{
			materialBuf.gOption |= LIM_LIGHT;
		}
		if (matSub->Dissolve)
		{
			materialBuf.gOption |= DISSOLVE;

			matSub->DissolveTimer += g_GlobalData->Time / matSub->DissolvePlayTime;

			if (matSub->DissolveTimer > 1.0f)
			{
				matSub->DissolveTimer = 0.0f;
				matSub->Dissolve = false;
			}
			else
			{
				CB_DissolveOption dissolveOptionBuf;
				dissolveOptionBuf.gEdgeColor = matSub->DissolveEdgecolor;
				dissolveOptionBuf.gOuterFactor = matSub->DissolveOuterFactor;
				dissolveOptionBuf.gInnerFactor = matSub->DissolveInnerFactor;
				dissolveOptionBuf.gThickness = 1.0f / matSub->DissolveThickness;

				switch (matSub->DissolveOption)
				{
				case DISSOLVE_OPTION::DISSOLVE_FADEOUT:
					dissolveOptionBuf.gOuterEdge = matSub->DissolveTimer;
					dissolveOptionBuf.gInnerEdge = dissolveOptionBuf.gOuterEdge + matSub->DissolveThickness;
					break;
				case DISSOLVE_OPTION::DISSOLVE_FADEIN:
					dissolveOptionBuf.gOuterEdge = 1.0f - matSub->DissolveTimer;
					dissolveOptionBuf.gInnerEdge = dissolveOptionBuf.gOuterEdge + matSub->DissolveThickness;
					break;
				default:
					break;
				}

				if (matSub->DissolveTexture)
				{
					m_Mesh_PS->SetShaderResourceView<gNoiseTexture>((ID3D11ShaderResourceView*)matSub->DissolveTexture->pTextureBuf);
				}

				m_Mesh_PS->ConstantBufferUpdate(&dissolveOptionBuf);
			}
		}

		m_Mesh_VS->ConstantBufferUpdate(&objectBuf);

		m_Mesh_VS->Update();

		m_Mesh_PS->ConstantBufferUpdate(&materialBuf);		
		m_Mesh_PS->ConstantBufferUpdate(&ConstantBufferManager::LIGHT_BUFFER);
		m_Mesh_PS->ConstantBufferUpdate(&ConstantBufferManager::LIGHT_SUB_BUFFER);

		m_Mesh_PS->Update();

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
		m_Skin_VS->SetShaderResourceView<gAnimationBuffer>(meshData->m_Animation->m_AnimationBuf);

		m_Skin_VS->Update();

		// Pixel Shader Update..
		if (mat->m_Albedo)
		{
			materialBuf.gOption |= ALBEDO_MAP;
			m_Mesh_PS->SetShaderResourceView<gDiffuseMap>(mat->m_Albedo);
		}
		if (mat->m_Normal)
		{
			materialBuf.gOption |= NORMAL_MAP;
			m_Mesh_PS->SetShaderResourceView<gNormalMap>(mat->m_Normal);
		}
		if (mat->m_Emissive)
		{
			materialBuf.gOption |= EMISSIVE_MAP;
			m_Mesh_PS->SetShaderResourceView<gEmissiveMap>(mat->m_Emissive);
		}
		if (mat->m_ORM)
		{
			materialBuf.gOption |= ORM_MAP;
			m_Mesh_PS->SetShaderResourceView<gORMMap>(mat->m_ORM);
		}
		if (matSub->LimLightFactor > 0.0f)
		{
			materialBuf.gOption |= LIM_LIGHT;
		}
		if (matSub->Dissolve)
		{
			materialBuf.gOption |= DISSOLVE;
		}

		m_Mesh_PS->ConstantBufferUpdate(&materialBuf);
		m_Mesh_PS->ConstantBufferUpdate(&ConstantBufferManager::LIGHT_BUFFER);
		m_Mesh_PS->ConstantBufferUpdate(&ConstantBufferManager::LIGHT_SUB_BUFFER);

		m_Mesh_PS->Update();

		// Draw..
		g_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		g_Context->IASetVertexBuffers(0, 1, &mesh->m_VertexBuf, &mesh->m_Stride, &mesh->m_Offset);
		g_Context->IASetIndexBuffer(mesh->m_IndexBuf, DXGI_FORMAT_R32_UINT, 0);

		g_Context->DrawIndexed(mesh->m_IndexCount, 0, 0);
	}
	break;
	case OBJECT_TYPE::PARTICLE_SYSTEM:
	{
		ParticleData* particleSystem = meshData->m_ParticleData;
		OneParticle* particle = nullptr;

		const Matrix& invView = g_GlobalData->MainCamera_Data->CamInvView;

		Matrix converseTM = Matrix::Identity;

		switch (particleSystem->RenderType)
		{
		case PARTICLE_RENDER_OPTION::BILLBOARD:
			converseTM = invView;
			break;
		case PARTICLE_RENDER_OPTION::VERTICAL_BILLBOARD:
			converseTM = invView;
			converseTM._21 = 0; converseTM._22 = 1; converseTM._23 = 0;
			break;
		case PARTICLE_RENDER_OPTION::HORIZONTAL_BILLBOARD:
			converseTM = Matrix::CreateRotationX(3.1415926535f * 0.5f);
			break;
		case PARTICLE_RENDER_OPTION::MESH:
			break;
		default:
			break;
		}

		for (int i = 0; i < particleSystem->Particle_Count; i++)
		{
			particle = particleSystem->m_Particles[i];

			if (particle->Playing == false) continue;

			// 파티클시스템 로컬기준 월드 적용..
			Matrix&& result = *particle->Local * converseTM;

			result._41 = world._41 + particle->Pos.x;
			result._42 = world._42 + particle->Pos.y;
			result._43 = world._43 + particle->Pos.z;

			// Vertex Input Data 삽입..
			m_ParticleData.World = result;
			m_ParticleData.TexScale = particle->TexScale;
			m_ParticleData.TexPos = particle->TexPos;
			m_ParticleData.Color = particle->Color;

			m_ParticleInstance[m_InstanceCount++] = m_ParticleData;
		}

		if (m_InstanceCount == 0) return;

		// Instance Buffer Update..
		UpdateBuffer(m_Particle_IB->InstanceBuf->Get(), &m_ParticleInstance[0], (size_t)m_Particle_IB->Stride * (size_t)m_InstanceCount);

		// Veretex Shader Update..
		CB_InstanceParticleMesh particleBuf;
		particleBuf.gViewProj = viewproj;

		m_ParticleInst_VS->ConstantBufferUpdate(&particleBuf);

		m_ParticleInst_VS->Update();

		// Pixel Shader Update..
		if (mat->m_Albedo)
		{
			m_Particle_PS->SetShaderResourceView<gDiffuseMap>(mat->m_Albedo);
		}

		CB_ParticleOption particleOptionBuf;
		particleOptionBuf.gStrength = meshData->m_ParticleData->Particle_Strength;

		m_Particle_PS->ConstantBufferUpdate(&particleOptionBuf);

		m_Particle_PS->Update();

		ID3D11Buffer* vertexBuffers[2] = { mesh->m_VertexBuf, m_Particle_IB->InstanceBuf->Get() };
		UINT strides[2] = { mesh->m_Stride, m_Particle_IB->Stride };
		UINT offsets[2] = { 0,0 };

		g_Context->IASetVertexBuffers(0, 2, vertexBuffers, strides, offsets);
		g_Context->IASetIndexBuffer(mesh->m_IndexBuf, DXGI_FORMAT_R32_UINT, 0);
		g_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// Draw..
		g_Context->DrawIndexedInstanced(mesh->m_IndexCount, m_InstanceCount, 0, 0, 0);
	}
	break;
	default:
		break;
	}

	// Alpha Mesh Instance Data Clear..
	m_InstanceCount = 0;
}

void Alpha_Pass::RenderUpdate(const InstanceRenderBuffer* instance, const std::vector<RenderData*>& meshlist)
{
	m_RenderCount = (int)meshlist.size();

	if (m_RenderCount == 1)
	{
		m_RenderData = meshlist[0];

		// 현재 패스에서 그리지 않아야 할 경우..
		if (m_RenderData->m_Draw == false || m_RenderData->m_Block == true) return;

		RenderUpdate(instance, m_RenderData);
		return;
	}

	const CameraData* cam = g_GlobalData->MainCamera_Data;
	const MeshRenderBuffer* mesh = instance->m_Mesh;
	const MaterialRenderBuffer* mat = instance->m_Material;
	MaterialProperty* matSub = mat->m_MaterialProperty;

	const Matrix& view = cam->CamView;
	const Matrix& proj = cam->CamProj;
	const Matrix& viewproj = cam->CamViewProj;
	const Matrix& viewprojtex = cam->CamViewProjTex;

	// Pixel Shader Update..
	CB_Material materialBuf;
	materialBuf.gAddColor = matSub->AddColor;
	materialBuf.gRoughnessFactor = matSub->RoughnessFactor;
	materialBuf.gMetallicFactor = matSub->MetallicFactor;
	materialBuf.gEmissiveColor = matSub->EmissiveColor * matSub->EmissiveFactor;
	materialBuf.gLimLightColor = matSub->LimLightColor * matSub->LimLightFactor;
	materialBuf.gLimLightWidth = matSub->LimLightWidth;
	materialBuf.gSkyLightIndex = matSub->SkyLightIndex;

	if (mat->m_Albedo)
	{
		materialBuf.gOption |= ALBEDO_MAP;
		m_Mesh_PS->SetShaderResourceView<gDiffuseMap>(mat->m_Albedo);
	}
	if (mat->m_Normal)
	{
		materialBuf.gOption |= NORMAL_MAP;
		m_Mesh_PS->SetShaderResourceView<gNormalMap>(mat->m_Normal);
	}
	if (mat->m_Emissive)
	{
		materialBuf.gOption |= EMISSIVE_MAP;
		m_Mesh_PS->SetShaderResourceView<gEmissiveMap>(mat->m_Emissive);
	}
	if (mat->m_ORM)
	{
		materialBuf.gOption |= ORM_MAP;
		m_Mesh_PS->SetShaderResourceView<gORMMap>(mat->m_ORM);
	}
	if (matSub->LimLightFactor > 0.0f)
	{
		materialBuf.gOption |= LIM_LIGHT;
	}
	if (matSub->Dissolve)
	{
		materialBuf.gOption |= DISSOLVE;

		matSub->DissolveTimer += g_GlobalData->Time / matSub->DissolvePlayTime;

		if (matSub->DissolveTimer > 1.0f)
		{
			matSub->DissolveTimer = 0.0f;
			matSub->Dissolve = false;
		}
		else
		{
			CB_DissolveOption dissolveOptionBuf;
			dissolveOptionBuf.gEdgeColor = matSub->DissolveEdgecolor;
			dissolveOptionBuf.gOuterFactor = matSub->DissolveOuterFactor;
			dissolveOptionBuf.gInnerFactor = matSub->DissolveInnerFactor;
			dissolveOptionBuf.gThickness = 1.0f / matSub->DissolveThickness;

			switch (matSub->DissolveOption)
			{
			case DISSOLVE_OPTION::DISSOLVE_FADEOUT:
				dissolveOptionBuf.gOuterEdge = matSub->DissolveTimer;
				dissolveOptionBuf.gInnerEdge = dissolveOptionBuf.gOuterEdge + matSub->DissolveThickness;
				break;
			case DISSOLVE_OPTION::DISSOLVE_FADEIN:
				dissolveOptionBuf.gOuterEdge = 1.0f - matSub->DissolveTimer;
				dissolveOptionBuf.gInnerEdge = dissolveOptionBuf.gOuterEdge + matSub->DissolveThickness;
				break;
			default:
				break;
			}

			if (matSub->DissolveTexture)
			{
				m_Mesh_PS->SetShaderResourceView<gNoiseTexture>((ID3D11ShaderResourceView*)matSub->DissolveTexture->pTextureBuf);
			}

			m_Mesh_PS->ConstantBufferUpdate(&dissolveOptionBuf);
		}
	}

	m_Mesh_PS->ConstantBufferUpdate(&materialBuf);
	m_Mesh_PS->ConstantBufferUpdate(&ConstantBufferManager::LIGHT_BUFFER);
	m_Mesh_PS->ConstantBufferUpdate(&ConstantBufferManager::LIGHT_SUB_BUFFER);

	m_Mesh_PS->Update();

	switch (instance->m_Type)
	{
	case OBJECT_TYPE::BASE:
	{
		// Vertex Shader Constant Buffer Update..
		CB_InstanceStaticMesh objectBuf;
		objectBuf.gView = view;
		objectBuf.gProj = proj;

		// Vertex Shader Update..
		m_MeshInst_VS->ConstantBufferUpdate(&objectBuf);

		m_MeshInst_VS->Update();

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

				// 현재 패스에서 그리지 않아야 할 경우..
				if (m_RenderData->m_Draw == false || m_RenderData->m_Block == true) continue;

				// 해당 Instance Data 삽입..
				m_MeshData.World = m_RenderData->m_ObjectData->World;
				m_MeshData.InvWorld = m_RenderData->m_ObjectData->InvWorld;

				m_MeshInstance[m_InstanceCount++] = m_MeshData;
			}

			if (m_InstanceCount == 0) return;


			// Instance Buffer Update..
			UpdateBuffer(m_Mesh_IB->InstanceBuf->Get(), &m_MeshInstance[0], (size_t)m_Mesh_IB->Stride * (size_t)m_InstanceCount);

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

	}
	break;
	default:
		break;
	}

	// Alpha Mesh Instance Data Clear..
	m_InstanceCount = 0;
}

void Alpha_Pass::BlockRenderUpdate(const RenderData* meshData)
{
	const CameraData* cam = g_GlobalData->MainCamera_Data;
	const ObjectData* obj = meshData->m_ObjectData;
	const MeshRenderBuffer* mesh = meshData->m_Mesh;
	const MaterialRenderBuffer* mat = meshData->m_Material;
	MaterialProperty* matSub = obj->Material_Block;

	const Matrix& world = obj->World;
	const Matrix& invWorld = obj->InvWorld;
	const Matrix& view = cam->CamView;
	const Matrix& proj = cam->CamProj;
	const Matrix& viewproj = cam->CamViewProj;

	// Pixel Shader Update..
	CB_Material materialBuf;
	materialBuf.gAddColor = matSub->AddColor;
	materialBuf.gRoughnessFactor = matSub->RoughnessFactor;
	materialBuf.gMetallicFactor = matSub->MetallicFactor;
	materialBuf.gEmissiveColor = matSub->EmissiveColor * matSub->EmissiveFactor;
	materialBuf.gLimLightColor = matSub->LimLightColor * matSub->LimLightFactor;
	materialBuf.gLimLightWidth = matSub->LimLightWidth;
	materialBuf.gSkyLightIndex = matSub->SkyLightIndex;
	materialBuf.gAlphaFactor = matSub->AlphaFactor;

	if (mat->m_Albedo)
	{
		materialBuf.gOption |= ALBEDO_MAP;
		m_Mesh_PS->SetShaderResourceView<gDiffuseMap>(mat->m_Albedo);
	}
	if (mat->m_Normal)
	{
		materialBuf.gOption |= NORMAL_MAP;
		m_Mesh_PS->SetShaderResourceView<gNormalMap>(mat->m_Normal);
	}
	if (mat->m_Emissive)
	{
		materialBuf.gOption |= EMISSIVE_MAP;
		m_Mesh_PS->SetShaderResourceView<gEmissiveMap>(mat->m_Emissive);
	}
	if (mat->m_ORM)
	{
		materialBuf.gOption |= ORM_MAP;
		m_Mesh_PS->SetShaderResourceView<gORMMap>(mat->m_ORM);
	}
	if (matSub->LimLightFactor > 0.0f)
	{
		materialBuf.gOption |= LIM_LIGHT;
	}
	if (matSub->Dissolve)
	{
		materialBuf.gOption |= DISSOLVE;

		matSub->DissolveTimer += g_GlobalData->Time / matSub->DissolvePlayTime;

		if (matSub->DissolveTimer > 1.0f)
		{
			matSub->DissolveTimer = 0.0f;
			matSub->Dissolve = false;
		}
		else
		{
			CB_DissolveOption dissolveOptionBuf;
			dissolveOptionBuf.gEdgeColor = matSub->DissolveEdgecolor;
			dissolveOptionBuf.gOuterFactor = matSub->DissolveOuterFactor;
			dissolveOptionBuf.gInnerFactor = matSub->DissolveInnerFactor;
			dissolveOptionBuf.gThickness = 1.0f / matSub->DissolveThickness;

			switch (matSub->DissolveOption)
			{
			case DISSOLVE_OPTION::DISSOLVE_FADEOUT:
				dissolveOptionBuf.gOuterEdge = matSub->DissolveTimer;
				dissolveOptionBuf.gInnerEdge = dissolveOptionBuf.gOuterEdge + matSub->DissolveThickness;
				break;
			case DISSOLVE_OPTION::DISSOLVE_FADEIN:
				dissolveOptionBuf.gOuterEdge = 1.0f - matSub->DissolveTimer;
				dissolveOptionBuf.gInnerEdge = dissolveOptionBuf.gOuterEdge + matSub->DissolveThickness;
				break;
			default:
				break;
			}

			if (matSub->DissolveTexture)
			{
				m_Mesh_PS->SetShaderResourceView<gNoiseTexture>((ID3D11ShaderResourceView*)matSub->DissolveTexture->pTextureBuf);
			}

			m_Mesh_PS->ConstantBufferUpdate(&dissolveOptionBuf);
		}
	}

	m_Mesh_PS->ConstantBufferUpdate(&materialBuf);
	m_Mesh_PS->ConstantBufferUpdate(&ConstantBufferManager::LIGHT_BUFFER);
	m_Mesh_PS->ConstantBufferUpdate(&ConstantBufferManager::LIGHT_SUB_BUFFER);

	m_Mesh_PS->Update();

	switch (obj->ObjType)
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
		m_Skin_VS->SetShaderResourceView<gAnimationBuffer>(meshData->m_Animation->m_AnimationBuf);

		m_Skin_VS->Update();

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
