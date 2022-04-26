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
#include "AlphaPass.h"

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

AlphaPass::AlphaPass()
{

}

AlphaPass::~AlphaPass()
{

}

void AlphaPass::Create(int width, int height)
{


}

void AlphaPass::Start(int width, int height)
{
	// Shader 설정..
	m_ParticleInstVS = g_Shader->GetShader("Particle_Instance_VS");
	m_ParticlePS = g_Shader->GetShader("OIT_Particle_PS");

	m_MeshInst_VS = g_Shader->GetShader("StaticMesh_Instance_VS");
	m_MeshPS = g_Shader->GetShader("Deferred_PBR_PS");

	m_Mesh_IB = g_Resource->GetInstanceBuffer<IB_Mesh>();
	m_Particle_IB = g_Resource->GetInstanceBuffer<IB_Particle>();

	m_ParticleInstance.resize(500);
}

void AlphaPass::OnResize(int width, int height)
{

}

void AlphaPass::Release()
{

}

void AlphaPass::SetOption(RenderOption* renderOption)
{

}

void AlphaPass::BeginRender()
{

}

void AlphaPass::RenderUpdate(const InstanceRenderBuffer* instance, const RenderData* meshData)
{
	const CameraData* cam = g_GlobalData->MainCamera_Data;
	const ObjectData* obj = meshData->m_ObjectData;
	const MeshRenderBuffer* mesh = instance->m_Mesh;
	const MaterialRenderBuffer* mat = instance->m_Material;

	const Matrix& viewproj = cam->CamViewProj;

	switch (instance->m_Type)
	{
	case OBJECT_TYPE::BASE:
	case OBJECT_TYPE::SKINNING:
	{

	}
	break;
	case OBJECT_TYPE::PARTICLE_SYSTEM:
	{
		ParticleUpdate(meshData->m_ParticleData);

		if (m_InstanceCount == 0) return;

		// Instance Buffer Update..
		UpdateBuffer(m_Particle_IB->InstanceBuf->Get(), &m_ParticleInstance[0], (size_t)m_Particle_IB->Stride * (size_t)m_InstanceCount);

		// Veretex Shader Update..
		CB_InstanceParticleMesh particleBuf;
		particleBuf.gViewProj = viewproj;

		m_ParticleInstVS->ConstantBufferUpdate(&particleBuf);

		m_ParticleInstVS->Update();

		// Pixel Shader Update..
		if (mat->m_Albedo)
		{
			m_ParticlePS->SetShaderResourceView<gDiffuseMap>(mat->m_Albedo);
		}

		m_ParticlePS->Update();

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

void AlphaPass::RenderUpdate(const InstanceRenderBuffer* instance, const std::vector<RenderData*>& meshlist)
{
	m_RenderCount = (int)meshlist.size();

	if (m_RenderCount == 1)
	{
		RenderUpdate(instance, meshlist[0]);
	}

	const CameraData* cam = g_GlobalData->MainCamera_Data;
	const MeshRenderBuffer* mesh = instance->m_Mesh;
	const MaterialRenderBuffer* mat = instance->m_Material;

	const Matrix& viewproj = cam->CamViewProj;

	switch (instance->m_Type)
	{
	case OBJECT_TYPE::BASE:
	case OBJECT_TYPE::SKINNING:
	{

	}
		break;
	case OBJECT_TYPE::PARTICLE_SYSTEM:
	{
		for (int i = 0; i < m_RenderCount; i++)
		{
			ParticleUpdate(meshlist[i]->m_ParticleData);
		}

		if (m_InstanceCount == 0) return;

		// Instance Buffer Update..
		UpdateBuffer(m_Particle_IB->InstanceBuf->Get(), &m_ParticleInstance[0], (size_t)m_Particle_IB->Stride * (size_t)m_InstanceCount);

		// Veretex Shader Update..
		CB_InstanceParticleMesh particleBuf;
		particleBuf.gViewProj = viewproj;

		m_ParticleInstVS->ConstantBufferUpdate(&particleBuf);

		m_ParticleInstVS->Update();

		// Pixel Shader Update..
		if (mat->m_Albedo)
		{
			m_ParticlePS->SetShaderResourceView<gDiffuseMap>(mat->m_Albedo);
		}

		m_ParticlePS->Update();

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

void AlphaPass::ParticleUpdate(ParticleData* particleSystem)
{
	OneParticle* particle = nullptr;

	const Matrix& invView = g_GlobalData->MainCamera_Data->CamInvView;
	Matrix converseTM = Matrix::Identity;
	Matrix particleWorld;

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
		converseTM = Matrix::CreateRotationX(3.1415926535f / 2.0f);
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

		// Render Type에 맞는 행렬 변환..
		particleWorld = *particle->World * converseTM;

		particleWorld._41 = particle->World->_41;
		particleWorld._42 = particle->World->_42;
		particleWorld._43 = particle->World->_43;

		// Vertex Input Data 삽입..
		m_ParticleData.World = particleWorld;
		m_ParticleData.TexScale = particle->TexScale;
		m_ParticleData.TexPos = particle->TexPos;
		m_ParticleData.Color = particle->Color;

		m_ParticleInstance[m_InstanceCount++] = m_ParticleData;
	}
}
