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

	// Render Data Converter 생성..
	m_Converter = new RenderDataConverter();

	// Render Pass 생성..
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

	// 설정을 위한 Render Pass List Up..
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
	// Resource Resize Data 설정..
	for (RenderPassBase* renderPass : m_RenderPassList)
	{
		renderPass->SetResize(width, height);
	}

	// Resource Resize 실행..
	RenderPassBase::g_Resource->OnResize(width, height);

	// Resize Resource 동기화..
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
	// RenderOption 저장..
	m_RenderOption = renderOption;

	// 현재 Render Option 저장..
	m_NowRenderOption.DebugOption = m_RenderOption->DebugOption;
	m_NowRenderOption.RenderingOption = m_RenderOption->RenderingOption;
	m_NowRenderOption.PostProcessOption = m_RenderOption->PostProcessOption;

	// 최초 Render Setting..
	for (RenderPassBase* renderPass : m_RenderPassList)
	{
		renderPass->SetOption(m_RenderOption);
	}
}

void RenderManager::RenderSetting()
{
	// 변동된 Render Option이 없을경우..
	if (*m_RenderOption == m_NowRenderOption) return;

	// 현재 Render Option 저장..
	m_NowRenderOption.DebugOption = m_RenderOption->DebugOption;
	m_NowRenderOption.RenderingOption = m_RenderOption->RenderingOption;
	m_NowRenderOption.PostProcessOption = m_RenderOption->PostProcessOption;

	// 최초 Render Setting..
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
	// 현재 비어있는 상태이므로 추후 설정을 위해 Mesh Data 삽입..
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
	// 해당 Mesh의 Mesh Buffer가 바뀌거나, Material Buffer가 바뀔경우 동기화..
	m_ChangeInstanceList.push(instance);
}

void RenderManager::PushChangeMesh(MeshBuffer* mesh)
{
	// 현재 바뀐 Mesh Buffer 동기화를 위해 삽입..
	m_Converter->PushChangeMesh(mesh);
}

void RenderManager::PushChangeMaterial(MaterialBuffer* material)
{
	// 현재 바뀐 Material Buffer 동기화를 위해 삽입..
	m_Converter->PushChangeMaterial(material);
}

void RenderManager::DeleteInstance(MeshData* instance)
{
	// Object Type에 따른 Render Mesh Data 제거..
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
	// Render Resource 동기화 작업..
	m_Converter->ResourceUpdate();

	// 현재 프레임 동안 추가된 Render Data Convert 작업..
	ConvertPushInstance();

	// 현재 프레임 동안 변경된 Render Data Convert 작업..
	ConvertChangeInstance();
}

void RenderManager::SelectRenderData()
{
	//PROFILE_TIMER_START(PROFILE_OUTPUT::CONSOLE, 60, "Culling");

	// Camera View Frustum Culling..
	int renderCount = 0;
	for (InstanceLayer* layer : m_RenderMeshList)
	{
		// 해당 Layer Distance Sorting..
		//std::sort(layer->m_MeshList.begin(), layer->m_MeshList.end(), [this](RenderData* rd1, RenderData* rd2) { return this->SortDistance(rd1, rd2); });

		// 해당 Layer Render List 초기화..
		memset(&layer->m_RenderList[0], 0, layer->m_RenderList.size());

		for (RenderData* renderData : layer->m_MeshList)
		{
			if (m_Culling->FrustumCulling(renderData))
			{
				layer->m_RenderList[renderCount++] = renderData;
			}
		}

		// Layer 변경시 Index 초기화..
		layer->m_RenderCount = renderCount;
		renderCount = 0;
	}

	//PROFILE_TIMER_END("Culling");
}

void RenderManager::Render()
{
	// Rendering Option Setting..
	RenderSetting();

	// Rendering Resource 동기화 작업..
	ConvertRenderData();

	// Render Data 선별 작업..
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
	
	// 현재 클릭한 Pixel ID 검출..
	UINT pickID = m_Picking->FindPick(x, y);

	GPU_END_EVENT_DEBUG_NAME();

	// 해당 Render Data 검색..
	RenderData* renderData = m_Converter->GetRenderData(pickID);
	
	// 검색된 Render Data가 없는것은 선택된 Object가 없다는 것..
	if (renderData == nullptr) return nullptr;

	// 해당 Render Data의 원본 GameObject를 반환..
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

	// 최종 출력..
	m_SwapChain->Present(0, 0);
}

void RenderManager::ConvertPushInstance()
{
	// 현재 프레임 진행중 쌓아둔 추가된 Render Data List 삽입 작업..
	while (!m_PushInstanceList.empty())
	{
		// 해당 원본 Mesh Data 추출..
		MeshData* originMeshData = m_PushInstanceList.front();

		// 새로운 Render Data 생성..
		RenderData* convertRenderData = new RenderData();

		// Mesh Data -> Render Data 변환..
		m_Converter->ConvertMeshData(originMeshData, convertRenderData);

		// Object Type에 따른 리스트 삽입..
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

		// 변환한 Mesh Data Pop..
		m_PushInstanceList.pop();
	}

	// 모든 변환이 끝난후 List 재설정..
	CheckMaxSizeLayer(m_RenderMeshList);
	CheckMaxSizeLayer(m_ParticleMeshList);
}

void RenderManager::ConvertChangeInstance()
{
	// 현재 프레임 진행중 쌓아둔 변경된 Render Data List 삽입 작업..
	while (!m_ChangeInstanceList.empty())
	{
		// 해당 원본 Mesh Data 추출..
		MeshData* originMeshData = m_ChangeInstanceList.front();

		// 해당 Render Data 추출..
		RenderData* convertRenderData = (RenderData*)originMeshData->Render_Data;

		// 해당 Instance Buffer 추출..
		InstanceRenderBuffer* instance = m_Converter->GetInstance(convertRenderData->m_InstanceIndex);

		if (instance)
		{
			// 혹시라도 변하지 않은 경우 변환하지 않음..
			if (instance->m_Mesh->m_BufferIndex == originMeshData->Mesh_Buffer->BufferIndex &&
				instance->m_Material->m_BufferIndex == originMeshData->Material_Buffer->BufferIndex)
			{
				m_ChangeInstanceList.pop();
				continue;
			}
		}

		// Object Type에 따른 List 삽입 제거 작업..
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

		// 변환한 Mesh Data Pop..
		m_ChangeInstanceList.pop();
	}

	// 모든 변환이 끝난후 List 재설정..
	CheckInstanceLayer(m_RenderMeshList);
	CheckInstanceLayer(m_ParticleMeshList);
}

void RenderManager::PushMeshRenderData(RenderData* renderData)
{
	// 해당 Layer 검색..
	InstanceLayer* instanceLayer = m_Converter->GetLayer(renderData->m_InstanceLayerIndex);

	// List 내의 Layer 유무 확인..
	for (InstanceLayer* layer : m_RenderMeshList)
	{
		// 이미 현재 Layer가 List에 등록 됬다면 등록하지 않는다..
		if (layer->m_LayerIndex == instanceLayer->m_LayerIndex)
		{
			return;
		}
	}

	// 등록되지 않은 Layer 삽입..
	m_RenderMeshList.push_back(instanceLayer);
}

void RenderManager::PushParticleRenderData(RenderData* renderData)
{
	// 해당 Layer 검색..
	InstanceLayer* instanceLayer = m_Converter->GetLayer(renderData->m_InstanceLayerIndex);

	// List 내의 Layer 유무 확인..
	for (InstanceLayer* layer : m_ParticleMeshList)
	{
		// 이미 현재 Layer가 List에 등록 됬다면 등록하지 않는다..
		if (layer->m_LayerIndex == instanceLayer->m_LayerIndex)
		{
			return;
		}
	}

	// 등록되지 않은 Layer 삽입..
	m_ParticleMeshList.push_back(instanceLayer);
}

void RenderManager::PushUnRenderData(RenderData* renderData)
{
	m_UnRenderMeshList.push_back(renderData);
}

void RenderManager::ChangeMeshRenderData(MeshData* meshData)
{
	// Render Data 변환..
	RenderData* convertRenderData = (RenderData*)meshData->Render_Data;

	// 현재 변경 전 Layer 검색..
	InstanceLayer* layer = m_Converter->GetLayer(convertRenderData->m_InstanceLayerIndex);

	// 해당 Layer가 존재한다면..
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

		// 해당 Render Data List에서 제거..
		layer->DeleteRenderData(index);
	}

	// Render Data 재설정..
	m_Converter->ConvertRenderData(meshData, convertRenderData);

	// 재설정된 Layer 검색..
	layer = m_Converter->GetLayer(convertRenderData->m_InstanceLayerIndex);

	// 해당 Layer가 등록되어 있는지 확인..
	FindInstanceLayer(m_RenderMeshList, layer);
}

void RenderManager::ChangeParticleRenderData(MeshData* meshData)
{
	// Render Data 변환..
	RenderData* convertRenderData = (RenderData*)meshData->Render_Data;

	// 현재 변경 전 Layer 검색..
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

	// 해당 Render Data List에서 제거..
	layer->DeleteRenderData(index);

	// Render Data 재설정..
	m_Converter->ConvertRenderData(meshData, convertRenderData);

	// 해당 Layer가 등록되어 있는지 확인..
	FindInstanceLayer(m_ParticleMeshList, layer);
}

void RenderManager::ChangeUnRenderData(MeshData* meshData)
{

}

void RenderManager::DeleteMeshRenderData(MeshData* meshData)
{
	// Render Data 변환..
	RenderData* renderData = (RenderData*)meshData->Render_Data;

	// 해당 Layer 검색..
	InstanceLayer* instanceLayer = m_Converter->GetLayer(renderData->m_InstanceLayerIndex);

	// Render Data의 List 내에서의 Index..
	int index = -1;

	for (int i = 0; i < instanceLayer->m_MeshList.size(); i++)
	{
		// 해당 Render Data List Index 검색..
		if (instanceLayer->m_MeshList[i] == renderData)
		{
			index = i;
			break;
		}
	}

	// Index가 검색이 안되면 안된다..
	assert(index != -1);

	UINT renderDataIndex = instanceLayer->m_MeshList[index]->m_ObjectData->ObjectIndex;

	// 해당 Render Data 제거..
	m_Converter->DeleteRenderData(renderDataIndex + 1);

	// 해당 Instance List에서 제거...
	instanceLayer->DeleteRenderData(index);

	// Instance Layer 빈곳 체크..
	CheckInstanceLayer(m_RenderMeshList);
}

void RenderManager::DeleteParticleRenderData(MeshData* meshData)
{
	// Render Data 변환..
	RenderData* renderData = (RenderData*)meshData->Render_Data;

	// 해당 Layer 검색..
	InstanceLayer* instanceLayer = m_Converter->GetLayer(renderData->m_InstanceLayerIndex);

	// Render Data의 List 내에서의 Index..
	int index = -1;

	for (int i = 0; i < instanceLayer->m_MeshList.size(); i++)
	{
		// 해당 Render Data List Index 검색..
		if (instanceLayer->m_MeshList[i] == renderData)
		{
			index = i;
			break;
		}
	}

	// Index가 검색이 안되면 안된다..
	assert(index != -1);

	UINT renderDataIndex = instanceLayer->m_MeshList[index]->m_ObjectData->ObjectIndex;

	// 해당 Render Data 제거..
	m_Converter->DeleteRenderData(renderDataIndex + 1);

	// 해당 Instance List에서 제거...
	instanceLayer->DeleteRenderData(index);

	// Instance Layer 빈곳 체크..
	CheckInstanceLayer(m_ParticleMeshList);
}

void RenderManager::DeleteUnRenderData(MeshData* meshData)
{
	// Render Data 변환..
	RenderData* renderData = (RenderData*)meshData->Render_Data;

	// Render Data의 List 내에서의 Index..
	int index = -1;

	for (int i = 0; i < m_UnRenderMeshList.size(); i++)
	{
		// 해당 Render Data List Index 검색..
		if (m_UnRenderMeshList[i] == renderData)
		{
			index = i;
			break;
		}
	}

	UINT renderDataIndex = m_UnRenderMeshList[index]->m_ObjectData->ObjectIndex;

	// 해당 Render Data 제거..
	m_Converter->DeleteRenderData(renderDataIndex + 1);

	// 해당 Instance List에서 제거...
	m_UnRenderMeshList.erase(std::next(m_UnRenderMeshList.begin(), index));
}

void RenderManager::CheckInstanceLayer(std::vector<InstanceLayer*>& layerList)
{
	// Layer List 내에서의 Layer Index..
	int index = -1;

	for (int i = 0; i < layerList.size(); i++)
	{
		// 해당 Layer List Index 검색..
		if (layerList[i]->m_MeshList.empty())
		{
			// 해당 Layer가 비어있다면 Layer 삭제..
			index = i;
			break;
		}
	}

	// 비어있는 Layer가 없다면 처리하지 않음..
	if (index == -1) return;

	// 해당 Layer 리스트에서 제거..
	layerList.erase(std::next(layerList.begin(), index));
}

void RenderManager::CheckMaxSizeLayer(std::vector<InstanceLayer*>& layerList)
{
	for (InstanceLayer* instanceLayer : layerList)
	{
		// Mesh List Size 기준으로 Render List Size 설정..
		instanceLayer->m_RenderList.resize(instanceLayer->m_MeshList.size());
	}
}

void RenderManager::FindInstanceLayer(std::vector<InstanceLayer*>& layerList, InstanceLayer* layer)
{
	for (InstanceLayer* instanceLayer : layerList)
	{
		// 해당 Layer가 이미 등록되어 있다면 추가하지 않는다..
		if (instanceLayer == layer)
		{
			return;
		}
	}

	// 만약 Layer가 검색되지 않았다면 Layer List에 삽입..
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
