#include "DirectDefine.h"
#include "D3D11GraphicBase.h"
#include "GraphicState.h"
#include "GraphicView.h"
#include "GraphicResource.h"
#include "DepthStencil.h"
#include "ShaderManagerBase.h"
#include "FactoryManagerBase.h"
#include "ResourceManager.h"
#include "RenderPassBase.h"
#include "RenderManager.h"

#include "ShaderBase.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ComputeShader.h"

#include "MathDefine.h"
#include "EngineData.h"
#include "RenderData.h"

#include "VertexDefine.h"
#include "ShadowPass.h"
#include "DeferredPass.h"
#include "LightPass.h"
#include "EnvironmentPass.h"
#include "SSAOPass.h"
#include "AlphaPass.h"
#include "OITPass.h"
#include "FXAAPass.h"
#include "BloomPass.h"
#include "ToneMapPass.h"
#include "FogPass.h"
#include "PickingPass.h"
#include "CullingPass.h"
#include "DebugPass.h"

#include "RenderDataConverter.h"

#include <algorithm>
#include "./Profiler/Profiler.h"

RenderManager::RenderManager(ID3D11Graphic* graphic, IFactoryManager* factory, IGraphicResourceManager* resource, IShaderManager* shader)
{
	// Rendering Initialize..
	RenderPassBase::Initialize(graphic->GetContext(), factory, resource, shader);

	m_SwapChain = graphic->GetSwapChain();

	// Render Data Converter ����..
	m_Converter = new RenderDataConverter();

	// Render Pass ����..
	m_Deferred		= new DeferredPass();
	m_Light			= new LightPass();
	m_Environment	= new EnvironmentPass();
	m_Shadow		= new ShadowPass();
	m_SSAO			= new SSAOPass();
	m_Alpha			= new AlphaPass();
	m_OIT			= new OITPass();
	m_FXAA			= new FXAAPass();
	m_Bloom			= new BloomPass();
	m_ToneMap		= new ToneMapPass();
	m_Fog			= new FogPass();
	m_Culling		= new CullingPass();
	m_Picking		= new PickingPass();
	m_Debug			= new DebugPass();

	// ������ ���� Render Pass List Up..
	m_RenderPassList.push_back(m_Deferred);
	m_RenderPassList.push_back(m_Light);
	m_RenderPassList.push_back(m_Environment);
	m_RenderPassList.push_back(m_Shadow);
	m_RenderPassList.push_back(m_SSAO);
	m_RenderPassList.push_back(m_Alpha);
	m_RenderPassList.push_back(m_OIT);
	m_RenderPassList.push_back(m_FXAA);
	m_RenderPassList.push_back(m_Bloom);
	m_RenderPassList.push_back(m_ToneMap);
	m_RenderPassList.push_back(m_Fog);
	m_RenderPassList.push_back(m_Culling);
	m_RenderPassList.push_back(m_Picking);
	m_RenderPassList.push_back(m_Debug);
}

RenderManager::~RenderManager()
{

}

void RenderManager::Create(int width, int height)
{
	// Render Pass Resource Create..
	for (RenderPassBase* renderPass : m_RenderPassList)
	{
		renderPass->Create(width, height);
	}
}

void RenderManager::Start(int width, int height)
{
	// Render Pass Resource Set..
	for (RenderPassBase* renderPass : m_RenderPassList)
	{
		renderPass->Start(width, height);
	}
}

void RenderManager::OnResize(int width, int height)
{
	// Resource Resize Data ����..
	for (RenderPassBase* renderPass : m_RenderPassList)
	{
		renderPass->SetResize(width, height);
	}

	// Resource Resize ����..
	RenderPassBase::g_Resource->OnResize(width, height);

	// Resize Resource ����ȭ..
	for (RenderPassBase* renderPass : m_RenderPassList)
	{
		renderPass->OnResize(width, height);
	}
}

void RenderManager::Release()
{
	for (RenderPassBase* renderPass : m_RenderPassList)
	{
		RELEASE_COM(renderPass);
	}

	m_RenderPassList.clear();
}

void RenderManager::RenderSetting(RenderOption* renderOption)
{
	// RenderOption ����..
	m_RenderOption = renderOption;

	// ���� Render Option ����..
	m_NowRenderOption.DebugOption = m_RenderOption->DebugOption;
	m_NowRenderOption.RenderingOption = m_RenderOption->RenderingOption;
	m_NowRenderOption.PostProcessOption = m_RenderOption->PostProcessOption;

	// ���� Render Setting..
	for (RenderPassBase* renderPass : m_RenderPassList)
	{
		renderPass->SetOption(m_RenderOption);
	}
}

void RenderManager::RenderSetting()
{
	// ������ Render Option�� �������..
	if (*m_RenderOption == m_NowRenderOption) return;

	// ���� Render Option ����..
	m_NowRenderOption.DebugOption = m_RenderOption->DebugOption;
	m_NowRenderOption.RenderingOption = m_RenderOption->RenderingOption;
	m_NowRenderOption.PostProcessOption = m_RenderOption->PostProcessOption;

	// ���� Render Setting..
	for (RenderPassBase* renderPass : m_RenderPassList)
	{
		renderPass->SetOption(m_RenderOption);
	}
}

void RenderManager::SetGlobalData(GlobalData* globalData)
{
	RenderPassBase::g_GlobalData = globalData;
}

void RenderManager::SetEnvironmentMap(bool enable)
{
	m_Light->SetIBLEnvironmentMapResource(enable);
	m_Environment->SetEnvironmentMapResource(enable);
}

void RenderManager::PushInstance(MeshData* instance)
{
	// ���� ����ִ� �����̹Ƿ� ���� ������ ���� Mesh Data ����..
	m_PushInstanceList.push(instance);
}

void RenderManager::PushMesh(MeshBuffer* mesh)
{
	m_Converter->PushMesh(mesh);
}

void RenderManager::PushMaterial(MaterialBuffer* material)
{
	m_Converter->PushMaterial(material);
}

void RenderManager::PushChangeInstance(MeshData* instance)
{
	// �ش� Mesh�� Mesh Buffer�� �ٲ�ų�, Material Buffer�� �ٲ��� ����ȭ..
	m_ChangeInstanceList.push(instance);
}

void RenderManager::PushChangeMesh(MeshBuffer* mesh)
{
	// ���� �ٲ� Mesh Buffer ����ȭ�� ���� ����..
	m_Converter->PushChangeMesh(mesh);
}

void RenderManager::PushChangeMaterial(MaterialBuffer* material)
{
	// ���� �ٲ� Material Buffer ����ȭ�� ���� ����..
	m_Converter->PushChangeMaterial(material);
}

void RenderManager::DeleteInstance(MeshData* instance)
{
	// Object Type�� ���� Render Mesh Data ����..
	switch (instance->Object_Data->ObjType)
	{
	case OBJECT_TYPE::BASE:
	case OBJECT_TYPE::SKINNING:
	case OBJECT_TYPE::TERRAIN:
		DeleteMeshRenderData(instance);
		break;
	case OBJECT_TYPE::PARTICLE_SYSTEM:
		DeleteParticleRenderData(instance);
		break;
	case OBJECT_TYPE::PARTICLE:
		break;
	default:
		DeleteUnRenderData(instance);
		break;
	}
}

void RenderManager::DeleteMesh(MeshBuffer* mesh)
{
	m_Converter->DeleteMesh(mesh->BufferIndex);
}

void RenderManager::DeleteMaterial(MaterialBuffer* material)
{
	m_Converter->DeleteMaterial(material->BufferIndex);
}

void RenderManager::ConvertRenderData()
{
	// Render Resource ����ȭ �۾�..
	m_Converter->ResourceUpdate();

	// ���� ������ ���� �߰��� Render Data Convert �۾�..
	ConvertPushInstance();

	// ���� ������ ���� ����� Render Data Convert �۾�..
	ConvertChangeInstance();
}

void RenderManager::SelectRenderData()
{
	//PROFILE_TIMER_START(PROFILE_OUTPUT::CONSOLE, 60, "Culling");

	// Camera View Frustum Culling..
	int renderCount = 0;
	for (InstanceLayer* layer : m_RenderMeshList)
	{
		// �ش� Layer Distance Sorting..
		//std::sort(layer->m_MeshList.begin(), layer->m_MeshList.end(), [this](RenderData* rd1, RenderData* rd2) { return this->SortDistance(rd1, rd2); });

		// �ش� Layer Render List �ʱ�ȭ..
		memset(&layer->m_RenderList[0], 0, layer->m_RenderList.size());

		for (RenderData* renderData : layer->m_MeshList)
		{
			if (m_Culling->FrustumCulling(renderData))
			{
				layer->m_RenderList[renderCount++] = renderData;
			}
		}

		// Layer ����� Index �ʱ�ȭ..
		layer->m_RenderCount = renderCount;
		renderCount = 0;
	}

	//PROFILE_TIMER_END("Culling");
}

void RenderManager::Render()
{
	// Rendering Option Setting..
	RenderSetting();

	// Rendering Resource ����ȭ �۾�..
	ConvertRenderData();

	// Render Data ���� �۾�..
	SelectRenderData();

	// Shadow Render..
	GPU_BEGIN_EVENT_DEBUG_NAME("Shadow Pass");
	ShadowRender();
	GPU_END_EVENT_DEBUG_NAME();

	// Deferred Render..
	GPU_BEGIN_EVENT_DEBUG_NAME("Deferred Pass");
	DeferredRender();
	GPU_END_EVENT_DEBUG_NAME();

	// SSAO Render..
	GPU_BEGIN_EVENT_DEBUG_NAME("SSAO Pass");
	SSAORender();
	GPU_END_EVENT_DEBUG_NAME();

	// Light Render..
	GPU_BEGIN_EVENT_DEBUG_NAME("Light Pass");
	LightRender();
	GPU_END_EVENT_DEBUG_NAME();

	// Environment Render..
	GPU_BEGIN_EVENT_DEBUG_NAME("Environment Pass");
	EnvironmentRender();
	GPU_END_EVENT_DEBUG_NAME();

	// Alpha Render..
	GPU_BEGIN_EVENT_DEBUG_NAME("Alpha Pass");
	AlphaRender();
	GPU_END_EVENT_DEBUG_NAME();

	// PostProcess Render..
	GPU_BEGIN_EVENT_DEBUG_NAME("PostProcessing Pass");
	PostProcessingRender();
	GPU_END_EVENT_DEBUG_NAME();

	// Debug Render..
	GPU_BEGIN_EVENT_DEBUG_NAME("Debug Pass");
	DebugRender();
	GPU_END_EVENT_DEBUG_NAME();

	// End Render..
	GPU_BEGIN_EVENT_DEBUG_NAME("Present");
	EndRender();
	GPU_END_EVENT_DEBUG_NAME();
}

void* RenderManager::PickingRender(int x, int y)
{
	GPU_BEGIN_EVENT_DEBUG_NAME("Picking Pass");
	m_Picking->BeginRender();
	
	// Static Object Picking Draw..
	for (int i = 0; i < m_RenderMeshList.size(); i++)
	{
		m_InstanceLayer = m_RenderMeshList[i];

		m_Picking->RenderUpdate(m_InstanceLayer->m_Instance, m_InstanceLayer->m_MeshList);
	}

	// Transparency Object Picking Draw..
	for (int i = 0; i < m_ParticleMeshList.size(); i++)
	{
		m_InstanceLayer = m_ParticleMeshList[i];

		m_Picking->RenderUpdate(m_InstanceLayer->m_Instance, m_InstanceLayer->m_MeshList);
	}

	// UnRender Object Picking Draw..
	m_Picking->NoneMeshRenderUpdate(m_UnRenderMeshList);
	
	// ���� Ŭ���� Pixel ID ����..
	UINT pickID = m_Picking->FindPick(x, y);

	GPU_END_EVENT_DEBUG_NAME();

	// �ش� Render Data �˻�..
	RenderData* renderData = m_Converter->GetRenderData(pickID);
	
	// �˻��� Render Data�� ���°��� ���õ� Object�� ���ٴ� ��..
	if (renderData == nullptr) return nullptr;

	// �ش� Render Data�� ���� GameObject�� ��ȯ..
	return renderData->m_ObjectData->Object;
}

void RenderManager::ShadowRender()
{
	if (m_NowRenderOption.RenderingOption & RENDER_SHADOW)
	{
		m_Shadow->BeginRender();

		for (int i = 0; i < m_RenderMeshList.size(); i++)
		{
			m_InstanceLayer = m_RenderMeshList[i];

			m_Shadow->RenderUpdate(m_InstanceLayer->m_Instance, m_InstanceLayer->m_RenderList, m_InstanceLayer->m_RenderCount);
			//m_Shadow->RenderUpdate(m_InstanceLayer->m_Instance, m_InstanceLayer->m_MeshList, m_InstanceLayer->m_MeshList.size());
		}
	}
}

void RenderManager::DeferredRender()
{
	m_Deferred->BeginRender();

	for (int i = 0; i < m_RenderMeshList.size(); i++)
	{
		m_InstanceLayer = m_RenderMeshList[i];

		m_Deferred->RenderUpdate(m_InstanceLayer->m_Instance, m_InstanceLayer->m_RenderList, m_InstanceLayer->m_RenderCount);
		//m_Deferred->RenderUpdate(m_InstanceLayer->m_Instance, m_InstanceLayer->m_MeshList, m_InstanceLayer->m_MeshList.size());
	}
}

void RenderManager::SSAORender()
{
	if (m_NowRenderOption.RenderingOption & RENDER_SSAO)
	{
		m_SSAO->RenderUpdate();
		m_SSAO->BlurRender(4);
	}
}

void RenderManager::LightRender()
{
	// Deferred Light Render..
	m_Light->RenderUpdate();
}

void RenderManager::EnvironmentRender()
{
	// Environment Map Render..
	m_Environment->RenderUpdate();
}

void RenderManager::AlphaRender()
{
	m_OIT->BeginRender();

	for (int i = 0; i < m_ParticleMeshList.size(); i++)
	{
		m_InstanceLayer = m_ParticleMeshList[i];

		m_Alpha->RenderUpdate(m_InstanceLayer->m_Instance, m_InstanceLayer->m_MeshList);
	}

	GPU_BEGIN_EVENT_DEBUG_NAME("OIT Pass");
	m_OIT->RenderUpdate();
	GPU_END_EVENT_DEBUG_NAME();
}

void RenderManager::PostProcessingRender()
{
	GPU_BEGIN_EVENT_DEBUG_NAME("Fog Pass");

	if (m_NowRenderOption.PostProcessOption & RENDER_FOG)
	{
		m_Fog->RenderUpdate();
	}

	GPU_END_EVENT_DEBUG_NAME();

	GPU_BEGIN_EVENT_DEBUG_NAME("Bloom Pass");

	if (m_NowRenderOption.PostProcessOption & RENDER_BLOOM)
	{
		m_Bloom->RenderUpdate();
	}

	GPU_END_EVENT_DEBUG_NAME();

	GPU_BEGIN_EVENT_DEBUG_NAME("ToneMap + Bloom Blend Pass");

	m_ToneMap->RenderUpdate();

	GPU_END_EVENT_DEBUG_NAME();

	GPU_BEGIN_EVENT_DEBUG_NAME("FXAA Pass");

	if (m_NowRenderOption.PostProcessOption & RENDER_FXAA)
	{
		m_FXAA->RenderUpdate();
	}

	GPU_END_EVENT_DEBUG_NAME();
}

void RenderManager::UIRender()
{

}

void RenderManager::DebugRender()
{
	if (m_NowRenderOption.RenderingOption & RENDER_DEBUG)
	{
		GPU_MARKER_DEBUG_NAME("Object Debug");
		m_Debug->BeginRender();

		// Render Mesh Debugging..
		for (int i = 0; i < m_RenderMeshList.size(); i++)
		{
			m_InstanceLayer = m_RenderMeshList[i];

			for (int j = 0; j < m_InstanceLayer->m_MeshList.size(); j++)
			{
				m_RenderData = m_InstanceLayer->m_MeshList[j];

				if (m_RenderData == nullptr) continue;

				switch (m_RenderData->m_ObjectData->ObjType)
				{
				case OBJECT_TYPE::DEFALT:
				case OBJECT_TYPE::BASE:
				case OBJECT_TYPE::SKINNING:
				case OBJECT_TYPE::BONE:
				case OBJECT_TYPE::LIGHT:
				case OBJECT_TYPE::PARTICLE_SYSTEM:
					m_Debug->RenderUpdate(m_RenderData);
					break;
				default:
					break;
				}
			}
		}

		// Particle Mesh Debugging..
		for (int i = 0; i < m_ParticleMeshList.size(); i++)
		{
			m_InstanceLayer = m_ParticleMeshList[i];

			for (int j = 0; j < m_InstanceLayer->m_MeshList.size(); j++)
			{
				m_RenderData = m_InstanceLayer->m_MeshList[j];

				if (m_RenderData == nullptr) continue;

				switch (m_RenderData->m_ObjectData->ObjType)
				{
				case OBJECT_TYPE::PARTICLE_SYSTEM:
					m_Debug->RenderUpdate(m_RenderData);
					break;
				default:
					break;
				}
			}
		}

		// UnRender Mesh Debugging..
		for (int i = 0; i < m_UnRenderMeshList.size(); i++)
		{
			m_RenderData = m_UnRenderMeshList[i];

			if (m_RenderData == nullptr) continue;

			switch (m_RenderData->m_ObjectData->ObjType)
			{
			case OBJECT_TYPE::DEFALT:
			case OBJECT_TYPE::CAMERA:
			case OBJECT_TYPE::BONE:
			case OBJECT_TYPE::LIGHT:
				m_Debug->RenderUpdate(m_RenderData);
				break;
			default:
				break;
			}
		}

		GPU_MARKER_DEBUG_NAME("Global Debug");
		m_Debug->GlobalRender();

		if (m_NowRenderOption.DebugOption == DEBUG_ENGINE)
		{
			GPU_MARKER_DEBUG_NAME("MRT Debug");
			m_Debug->MRTRender();
		}

		m_Debug->CountReset();
	}
}

void RenderManager::EndRender()
{
	// Graphic State Reset..
	RenderPassBase::GraphicReset();

	// ���� ���..
	m_SwapChain->Present(0, 0);
}

void RenderManager::ConvertPushInstance()
{
	// ���� ������ ������ �׾Ƶ� �߰��� Render Data List ���� �۾�..
	while (!m_PushInstanceList.empty())
	{
		// �ش� ���� Mesh Data ����..
		MeshData* originMeshData = m_PushInstanceList.front();

		// ���ο� Render Data ����..
		RenderData* convertRenderData = new RenderData();

		// Mesh Data -> Render Data ��ȯ..
		m_Converter->ConvertMeshData(originMeshData, convertRenderData);

		// Object Type�� ���� ����Ʈ ����..
		switch (convertRenderData->m_ObjectData->ObjType)
		{
		case OBJECT_TYPE::BASE:
		case OBJECT_TYPE::SKINNING:
		case OBJECT_TYPE::TERRAIN:
			PushMeshRenderData(convertRenderData);
			break;
		case OBJECT_TYPE::PARTICLE_SYSTEM:
			PushParticleRenderData(convertRenderData);
			break;
		default:
			PushUnRenderData(convertRenderData);
			break;
		}

		// ��ȯ�� Mesh Data Pop..
		m_PushInstanceList.pop();
	}

	// ��� ��ȯ�� ������ List �缳��..
	CheckMaxSizeLayer(m_RenderMeshList);
	CheckMaxSizeLayer(m_ParticleMeshList);
}

void RenderManager::ConvertChangeInstance()
{
	// ���� ������ ������ �׾Ƶ� ����� Render Data List ���� �۾�..
	while (!m_ChangeInstanceList.empty())
	{
		// �ش� ���� Mesh Data ����..
		MeshData* originMeshData = m_ChangeInstanceList.front();

		// �ش� Render Data ����..
		RenderData* convertRenderData = (RenderData*)originMeshData->Render_Data;

		// �ش� Instance Buffer ����..
		InstanceRenderBuffer* instance = m_Converter->GetInstance(convertRenderData->m_InstanceIndex);

		if (instance)
		{
			// Ȥ�ö� ������ ���� ��� ��ȯ���� ����..
			if (instance->m_Mesh->m_BufferIndex == originMeshData->Mesh_Buffer->BufferIndex &&
				instance->m_Material->m_BufferIndex == originMeshData->Material_Buffer->BufferIndex)
			{
				m_ChangeInstanceList.pop();
				continue;
			}
		}

		// Object Type�� ���� List ���� ���� �۾�..
		switch (originMeshData->Object_Data->ObjType)
		{
		case OBJECT_TYPE::BASE:
		case OBJECT_TYPE::SKINNING:
		case OBJECT_TYPE::TERRAIN:
			ChangeMeshRenderData(originMeshData);
			break;
		case OBJECT_TYPE::PARTICLE_SYSTEM:
			ChangeParticleRenderData(originMeshData);
			break;
		default:
			ChangeUnRenderData(originMeshData);
			break;
		}

		// ��ȯ�� Mesh Data Pop..
		m_ChangeInstanceList.pop();
	}

	// ��� ��ȯ�� ������ List �缳��..
	CheckInstanceLayer(m_RenderMeshList);
	CheckInstanceLayer(m_ParticleMeshList);
}

void RenderManager::PushMeshRenderData(RenderData* renderData)
{
	// �ش� Layer �˻�..
	InstanceLayer* instanceLayer = m_Converter->GetLayer(renderData->m_InstanceLayerIndex);

	// List ���� Layer ���� Ȯ��..
	for (InstanceLayer* layer : m_RenderMeshList)
	{
		// �̹� ���� Layer�� List�� ��� ��ٸ� ������� �ʴ´�..
		if (layer->m_LayerIndex == instanceLayer->m_LayerIndex)
		{
			return;
		}
	}

	// ��ϵ��� ���� Layer ����..
	m_RenderMeshList.push_back(instanceLayer);
}

void RenderManager::PushParticleRenderData(RenderData* renderData)
{
	// �ش� Layer �˻�..
	InstanceLayer* instanceLayer = m_Converter->GetLayer(renderData->m_InstanceLayerIndex);

	// List ���� Layer ���� Ȯ��..
	for (InstanceLayer* layer : m_ParticleMeshList)
	{
		// �̹� ���� Layer�� List�� ��� ��ٸ� ������� �ʴ´�..
		if (layer->m_LayerIndex == instanceLayer->m_LayerIndex)
		{
			return;
		}
	}

	// ��ϵ��� ���� Layer ����..
	m_ParticleMeshList.push_back(instanceLayer);
}

void RenderManager::PushUnRenderData(RenderData* renderData)
{
	m_UnRenderMeshList.push_back(renderData);
}

void RenderManager::ChangeMeshRenderData(MeshData* meshData)
{
	// Render Data ��ȯ..
	RenderData* convertRenderData = (RenderData*)meshData->Render_Data;

	// ���� ���� �� Layer �˻�..
	InstanceLayer* layer = m_Converter->GetLayer(convertRenderData->m_InstanceLayerIndex);

	// �ش� Layer�� �����Ѵٸ�..
	if (layer)
	{
		int index = -1;

		for (int i = 0; i < layer->m_MeshList.size(); i++)
		{
			if (layer->m_MeshList[i] == convertRenderData)
			{
				index = i;
				break;
			}
		}

		assert(index != -1);

		// �ش� Render Data List���� ����..
		layer->DeleteRenderData(index);
	}

	// Render Data �缳��..
	m_Converter->ConvertRenderData(meshData, convertRenderData);

	// �缳���� Layer �˻�..
	layer = m_Converter->GetLayer(convertRenderData->m_InstanceLayerIndex);

	// �ش� Layer�� ��ϵǾ� �ִ��� Ȯ��..
	FindInstanceLayer(m_RenderMeshList, layer);
}

void RenderManager::ChangeParticleRenderData(MeshData* meshData)
{
	// Render Data ��ȯ..
	RenderData* convertRenderData = (RenderData*)meshData->Render_Data;

	// ���� ���� �� Layer �˻�..
	InstanceLayer* layer = m_Converter->GetLayer(convertRenderData->m_InstanceLayerIndex);

	int index = -1;

	for (int i = 0; i < layer->m_MeshList.size(); i++)
	{
		if (layer->m_MeshList[i] == convertRenderData)
		{
			index = i;
			break;
		}
	}

	assert(index != -1);

	// �ش� Render Data List���� ����..
	layer->DeleteRenderData(index);

	// Render Data �缳��..
	m_Converter->ConvertRenderData(meshData, convertRenderData);

	// �ش� Layer�� ��ϵǾ� �ִ��� Ȯ��..
	FindInstanceLayer(m_ParticleMeshList, layer);
}

void RenderManager::ChangeUnRenderData(MeshData* meshData)
{

}

void RenderManager::DeleteMeshRenderData(MeshData* meshData)
{
	// Render Data ��ȯ..
	RenderData* renderData = (RenderData*)meshData->Render_Data;

	// �ش� Layer �˻�..
	InstanceLayer* instanceLayer = m_Converter->GetLayer(renderData->m_InstanceLayerIndex);

	// Render Data�� List �������� Index..
	int index = -1;

	for (int i = 0; i < instanceLayer->m_MeshList.size(); i++)
	{
		// �ش� Render Data List Index �˻�..
		if (instanceLayer->m_MeshList[i] == renderData)
		{
			index = i;
			break;
		}
	}

	// Index�� �˻��� �ȵǸ� �ȵȴ�..
	assert(index != -1);

	UINT renderDataIndex = instanceLayer->m_MeshList[index]->m_ObjectData->ObjectIndex;

	// �ش� Render Data ����..
	m_Converter->DeleteRenderData(renderDataIndex + 1);

	// �ش� Instance List���� ����...
	instanceLayer->DeleteRenderData(index);

	// Instance Layer ��� üũ..
	CheckInstanceLayer(m_RenderMeshList);
}

void RenderManager::DeleteParticleRenderData(MeshData* meshData)
{
	// Render Data ��ȯ..
	RenderData* renderData = (RenderData*)meshData->Render_Data;

	// �ش� Layer �˻�..
	InstanceLayer* instanceLayer = m_Converter->GetLayer(renderData->m_InstanceLayerIndex);

	// Render Data�� List �������� Index..
	int index = -1;

	for (int i = 0; i < instanceLayer->m_MeshList.size(); i++)
	{
		// �ش� Render Data List Index �˻�..
		if (instanceLayer->m_MeshList[i] == renderData)
		{
			index = i;
			break;
		}
	}

	// Index�� �˻��� �ȵǸ� �ȵȴ�..
	assert(index != -1);

	UINT renderDataIndex = instanceLayer->m_MeshList[index]->m_ObjectData->ObjectIndex;

	// �ش� Render Data ����..
	m_Converter->DeleteRenderData(renderDataIndex + 1);

	// �ش� Instance List���� ����...
	instanceLayer->DeleteRenderData(index);

	// Instance Layer ��� üũ..
	CheckInstanceLayer(m_ParticleMeshList);
}

void RenderManager::DeleteUnRenderData(MeshData* meshData)
{
	// Render Data ��ȯ..
	RenderData* renderData = (RenderData*)meshData->Render_Data;

	// Render Data�� List �������� Index..
	int index = -1;

	for (int i = 0; i < m_UnRenderMeshList.size(); i++)
	{
		// �ش� Render Data List Index �˻�..
		if (m_UnRenderMeshList[i] == renderData)
		{
			index = i;
			break;
		}
	}

	UINT renderDataIndex = m_UnRenderMeshList[index]->m_ObjectData->ObjectIndex;

	// �ش� Render Data ����..
	m_Converter->DeleteRenderData(renderDataIndex + 1);

	// �ش� Instance List���� ����...
	m_UnRenderMeshList.erase(std::next(m_UnRenderMeshList.begin(), index));
}

void RenderManager::CheckInstanceLayer(std::vector<InstanceLayer*>& layerList)
{
	// Layer List �������� Layer Index..
	int index = -1;

	for (int i = 0; i < layerList.size(); i++)
	{
		// �ش� Layer List Index �˻�..
		if (layerList[i]->m_MeshList.empty())
		{
			// �ش� Layer�� ����ִٸ� Layer ����..
			index = i;
			break;
		}
	}

	// ����ִ� Layer�� ���ٸ� ó������ ����..
	if (index == -1) return;

	// �ش� Layer ����Ʈ���� ����..
	layerList.erase(std::next(layerList.begin(), index));
}

void RenderManager::CheckMaxSizeLayer(std::vector<InstanceLayer*>& layerList)
{
	for (InstanceLayer* instanceLayer : layerList)
	{
		// Mesh List Size �������� Render List Size ����..
		instanceLayer->m_RenderList.resize(instanceLayer->m_MeshList.size());
	}
}

void RenderManager::FindInstanceLayer(std::vector<InstanceLayer*>& layerList, InstanceLayer* layer)
{
	for (InstanceLayer* instanceLayer : layerList)
	{
		// �ش� Layer�� �̹� ��ϵǾ� �ִٸ� �߰����� �ʴ´�..
		if (instanceLayer == layer)
		{
			return;
		}
	}

	// ���� Layer�� �˻����� �ʾҴٸ� Layer List�� ����..
	layerList.push_back(layer);
}

bool RenderManager::SortDistance(RenderData* obj1, RenderData* obj2)
{
	const Vector3& camPos = RenderPassBase::g_GlobalData->Camera_Data->CamPos;
	const Vector3& objPos1 = { (*obj1->m_ObjectData->World)._41 - camPos.x, (*obj1->m_ObjectData->World)._42 - camPos.y, (*obj1->m_ObjectData->World)._43 - camPos.z };
	const Vector3& objPos2 = { (*obj2->m_ObjectData->World)._41 - camPos.x, (*obj2->m_ObjectData->World)._42 - camPos.y, (*obj2->m_ObjectData->World)._43 - camPos.z };

	const float& distance1 = sqrtf(objPos1.x * objPos1.x + objPos1.y * objPos1.y + objPos1.z * objPos1.z);
	const float& distance2 = sqrtf(objPos2.x * objPos2.x + objPos2.y * objPos2.y + objPos2.z * objPos2.z);

	return (distance1 < distance2) ? true : false;
}
