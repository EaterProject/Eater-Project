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
#include "RenderDataConverterBase.h"
#include "RenderManager.h"

#include "ShaderBase.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ComputeShader.h"

#include "MathDefine.h"
#include "EngineData.h"
#include "RenderData.h"

#include "VertexDefine.h"
#include "Shadow_Pass.h"
#include "Deferred_Pass.h"
#include "Light_Pass.h"
#include "Sky_Pass.h"
#include "UI_Pass.h"
#include "SSAO_Pass.h"
#include "Alpha_Pass.h"
#include "OIT_Pass.h"
#include "FXAA_Pass.h"
#include "Bloom_Pass.h"
#include "Combine_Pass.h"
#include "Fog_Pass.h"
#include "Picking_Pass.h"
#include "Culling_Pass.h"
#include "OutLine_Pass.h"
#include "Debug_Pass.h"

#include <algorithm>

#include "./Profiler/Profiler.h"


#define CREATE_PASS(ClassName, MemberName) \
MemberName = new ClassName;	\
m_RenderPassList.push_back(MemberName);	\
PushFunction(MemberName);	\

RenderManager::RenderManager(ID3D11Graphic* graphic, IFactoryManager* factory, IGraphicResourceManager* resource, IShaderManager* shader, IRenderDataConverter* converter, RenderOption* renderOption)
{
	// Rendering Initialize..
	RenderPassBase::Initialize(graphic->GetDevice(), graphic->GetContext(), factory, resource, shader, renderOption);

	m_SwapChain = graphic->GetSwapChain();

	// Render Data Converter ����..
	m_Converter = converter;

	// Render Pass ����..
	CREATE_PASS(Deferred_Pass,	m_Deferred);
	CREATE_PASS(Light_Pass,		m_Light);
	CREATE_PASS(Sky_Pass,		m_Sky);
	CREATE_PASS(UI_Pass,		m_UI);
	CREATE_PASS(Shadow_Pass,	m_Shadow);
	CREATE_PASS(SSAO_Pass,		m_SSAO);
	CREATE_PASS(Alpha_Pass,		m_Alpha);
	CREATE_PASS(OIT_Pass,		m_OIT);
	CREATE_PASS(FXAA_Pass,		m_FXAA);
	CREATE_PASS(Bloom_Pass,		m_Bloom);
	CREATE_PASS(Fog_Pass,		m_Fog);
	CREATE_PASS(Culling_Pass,	m_Culling);
	CREATE_PASS(Picking_Pass,	m_Picking);
	CREATE_PASS(OutLine_Pass,	m_OutLine);
	CREATE_PASS(Combine_Pass,	m_Combine);
	CREATE_PASS(Debug_Pass,		m_Debug);
}

RenderManager::~RenderManager()
{
	
}

template<typename T>
void RenderManager::PushFunction(T* pass)
{
	///using ClassType = std::remove_reference<decltype(*ref)>::type;

	// OnResize Override �Լ� ���..
	if (typeid(&RenderPassBase::OnResize).hash_code() != typeid(&T::OnResize).hash_code())
	{
		OnResizeFunction += std::bind_front(&T::OnResize, pass);
	}
	// InstanceResize Override �Լ� ���..
	if (typeid(&RenderPassBase::InstanceResize).hash_code() != typeid(&T::InstanceResize).hash_code())
	{
		InstanceResizeFunction += std::bind_front(&T::InstanceResize, pass);
	}
	// SetResize Override �Լ� ���..
	if (typeid(&RenderPassBase::SetResize).hash_code() != typeid(&T::SetResize).hash_code())
	{
		SetResizeFunction += std::bind_front(&T::SetResize, pass);
	}
	// ApplyOption Override �Լ� ���..
	if (typeid(&RenderPassBase::ApplyOption).hash_code() != typeid(&T::ApplyOption).hash_code())
	{
		ApplyOptionFunction += std::bind_front(&T::ApplyOption, pass);
	}
	// PreUpdate Override �Լ� ���..
	if (typeid(&RenderPassBase::PreUpdate).hash_code() != typeid(&T::PreUpdate).hash_code())
	{
		PreUpdateFunction += std::bind_front(&T::PreUpdate, pass);
	}
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

	// ���� Render Setting..
	RenderSetting();
}

void RenderManager::OnResize(int width, int height)
{
	// Resource Resize Data ����..
	SetResizeFunction(width, height);

	// Resource Resize ����..
	RenderPassBase::g_Resource->OnResize(width, height);

	// Resize Resource ����ȭ..
	OnResizeFunction(width, height);
}

void RenderManager::Release()
{
	for (RenderPassBase* renderPass : m_RenderPassList)
	{
		RELEASE_COM(renderPass);
	}

	m_RenderPassList.clear();
}

void RenderManager::PreUpdate()
{
	// PreUpdate �Լ� ����Ʈ ����..
	PreUpdateFunction();
}

void RenderManager::InstanceResize()
{
	size_t&& renderMaxCount = m_Converter->FindMaxInstanceCount();
	size_t&& unRenderMaxCount = m_UnRenderMeshList.size();

	// Instance Resize �Լ� ����Ʈ ����..
	InstanceResizeFunction(renderMaxCount, unRenderMaxCount);
}

void RenderManager::RenderSetting(RenderOption* renderOption)
{
	// RenderOption ����..
	RenderPassBase::g_RenderOption = renderOption;

	// ���� Render Setting..
	RenderSetting();
}

void RenderManager::RenderSetting()
{
	// Apply Option �Լ� ����Ʈ ����..
	ApplyOptionFunction();

	// ���� Render Option ����..
	m_NowRenderOption = *RenderPassBase::g_RenderOption;
}

void RenderManager::SetGlobalData(GlobalData* globalData)
{
	RenderPassBase::g_GlobalData = globalData;
}

void RenderManager::SetSkyCube(TextureBuffer* resource)
{
	m_Sky->SetSkyCubeResource(resource);
}

void RenderManager::SetSkyLight(SkyLightBuffer* resource)
{
	m_Sky->SetSkyLightResource(resource);
}

void RenderManager::PushInstance(MeshData* instance)
{
	// ���� ����ִ� �����̹Ƿ� ���� ������ ���� Mesh Data ����..
	m_PushInstanceList.push(instance);
}

void RenderManager::PushMesh(MeshBuffer* mesh)
{
	// ���� �߰��� Mesh Buffer ����ȭ�� ���� ����..
	m_Converter->PushMesh(mesh);
}

void RenderManager::PushMaterial(MaterialBuffer* material)
{
	// ���� �߰��� Material Buffer ����ȭ�� ���� ����..
	m_Converter->PushMaterial(material);
}

void RenderManager::PushAnimation(AnimationBuffer* animation)
{
	// ���� �߰��� Animation Buffer ����ȭ�� ���� ����..
	m_Converter->PushAnimation(animation);
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

void RenderManager::PushChangeAnimation(AnimationBuffer* animation)
{
	// ���� �ٲ� Animation Buffer ����ȭ�� ���� ����..
	m_Converter->PushChangeAnimation(animation);
}

void RenderManager::DeleteInstance(MeshData* instance)
{
	if (instance == nullptr) return;

	// Object Type�� ���� Render Mesh Data ����..
	switch (instance->Object_Data->ObjType)
	{
	case OBJECT_TYPE::BASE:
	case OBJECT_TYPE::SKINNING:
	case OBJECT_TYPE::TERRAIN:
		DeleteOpacityRenderData(instance);
		break;
	case OBJECT_TYPE::PARTICLE_SYSTEM:
		DeleteTransparencyRenderData(instance);
		break;
	case OBJECT_TYPE::PARTICLE:
		break;
	case OBJECT_TYPE::UI:
		DeleteUIRenderData(instance);
		break;
	default:
		DeleteUnRenderData(instance);
		break;
	}
}

void RenderManager::DeleteMesh(MeshBuffer* mesh)
{
	if (mesh == nullptr) return;

	m_Converter->DeleteMesh(mesh->BufferIndex);
}

void RenderManager::DeleteMaterial(MaterialBuffer* material)
{
	if (material == nullptr) return;

	m_Converter->DeleteMaterial(material->BufferIndex);
}

void RenderManager::DeleteAnimation(AnimationBuffer* animation)
{
	if (animation == nullptr) return;

	m_Converter->DeleteAnimation(animation->BufferIndex);
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
	GPU_BEGIN_EVENT_DEBUG_NAME("Culling Pass");
	/// GPGPU Hierachical Z-Map Occlusion Culling..
	m_Culling->RenderOccluders();

	m_Culling->OcclusionCullingQuery();

	m_Culling->DrawStateUpdate();

	/// CPU Camera View Frustum Culling..
	//m_Culling->FrustumCulling();
	GPU_END_EVENT_DEBUG_NAME();
}

void RenderManager::Render()
{
	// Rendering Resource ����ȭ �۾�..
	ConvertRenderData();

	// Render Data ���� �۾�..
	SelectRenderData();

	// Render Pass ���� Update..
	PreUpdate();

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

	// RectTransform Render..
	GPU_BEGIN_EVENT_DEBUG_NAME("UI Pass");
	UIRender();
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
	for (int i = 0; i < m_OpacityMeshList.size(); i++)
	{
		m_InstanceLayer = m_OpacityMeshList[i];

		m_Picking->RenderUpdate(m_InstanceLayer->m_Instance, m_InstanceLayer->m_MeshList);
	}

	// Transparency Object Picking Draw..
	for (int i = 0; i < m_TransparencyMeshList.size(); i++)
	{
		m_InstanceLayer = m_TransparencyMeshList[i];

		m_Picking->RenderUpdate(m_InstanceLayer->m_Instance, m_InstanceLayer->m_MeshList);
	}

	// UnRender Object Picking Draw..
	m_Picking->NoneMeshRenderUpdate(m_UnRenderMeshList);
	
	// ���� Ŭ���� Pixel ID ����..
	int pickID = (int)m_Picking->FindPick(x, y);

	GPU_END_EVENT_DEBUG_NAME();

	// �ش� Render Data �˻�..
	RenderData* renderData = m_Converter->GetRenderData(pickID);

	/// Test
	RenderPassBase::g_Picking = renderData;

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

		for (int i = 0; i < m_OpacityMeshList.size(); i++)
		{
			m_InstanceLayer = m_OpacityMeshList[i];

			m_Shadow->RenderUpdate(m_InstanceLayer->m_Instance, m_InstanceLayer->m_MeshList);
		}
	}
}

void RenderManager::DeferredRender()
{
	m_Deferred->BeginRender();

	for (int i = 0; i < m_OpacityMeshList.size(); i++)
	{
		m_InstanceLayer = m_OpacityMeshList[i];

		m_Deferred->RenderUpdate(m_InstanceLayer->m_Instance, m_InstanceLayer->m_MeshList);
	}
}

void RenderManager::SSAORender()
{
	if (m_NowRenderOption.RenderingOption & RENDER_SSAO)
	{
		m_SSAO->RenderUpdate();
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
	m_Sky->RenderUpdate();
}

void RenderManager::AlphaRender()
{
	m_OIT->BeginRender();

	for (int i = 0; i < m_TransparencyMeshList.size(); i++)
	{
		m_InstanceLayer = m_TransparencyMeshList[i];

		m_Alpha->RenderUpdate(m_InstanceLayer->m_Instance, m_InstanceLayer->m_MeshList);
	}

	GPU_BEGIN_EVENT_DEBUG_NAME("OIT Pass");
	m_OIT->RenderUpdate();
	GPU_END_EVENT_DEBUG_NAME();
}

void RenderManager::PostProcessingRender()
{
	GPU_BEGIN_EVENT_DEBUG_NAME("Bloom Pass");
	if (m_NowRenderOption.PostProcessOption & RENDER_BLOOM)
	{
		m_Bloom->RenderUpdate();
	}
	GPU_END_EVENT_DEBUG_NAME();

	GPU_BEGIN_EVENT_DEBUG_NAME("OutLine Pass");
	//m_OutLine->RenderUpdate();
	GPU_END_EVENT_DEBUG_NAME();

	GPU_BEGIN_EVENT_DEBUG_NAME("Combine Pass");
	m_Combine->RenderUpdate();
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
	m_UI->RenderUpdate(m_UIRenderMeshList);
}

void RenderManager::DebugRender()
{
	if (m_NowRenderOption.DebugOption & DEBUG_MODE)
	{
		GPU_MARKER_DEBUG_NAME("Object Debug");
		m_Debug->BeginRender();

		// Render Mesh Debugging..
		for (int i = 0; i < m_OpacityMeshList.size(); i++)
		{
			m_InstanceLayer = m_OpacityMeshList[i];

			for (int j = 0; j < m_InstanceLayer->m_MeshList.size(); j++)
			{
				m_RenderData = m_InstanceLayer->m_MeshList[j];

				if (m_RenderData == nullptr) continue;

				switch (m_RenderData->m_ObjectData->ObjType)
				{
				case OBJECT_TYPE::DEFALT:
				case OBJECT_TYPE::BASE:
				case OBJECT_TYPE::TERRAIN:
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
		for (int i = 0; i < m_TransparencyMeshList.size(); i++)
		{
			m_InstanceLayer = m_TransparencyMeshList[i];

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

		if (m_NowRenderOption.DebugOption & DEBUG_RENDERTARGET)
		{
			GPU_MARKER_DEBUG_NAME("MRT Debug");
			m_Debug->MRTRender();
		}
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
	// Queue�� ����ִٸ� ó������ �ʴ´�..
	if (m_PushInstanceList.empty()) return;

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
			PushOpacityRenderData(convertRenderData);
			break;
		case OBJECT_TYPE::PARTICLE_SYSTEM:
			PushTransparencyRenderData(convertRenderData);
			break;
		case OBJECT_TYPE::UI:
			PushUIRenderData(convertRenderData);
			break;
		default:
			PushUnRenderData(convertRenderData);
			break;
		}

		// ��ȯ�� Mesh Data Pop..
		m_PushInstanceList.pop();
	}

	// Instance Resize..
	InstanceResize();
}

void RenderManager::ConvertChangeInstance()
{
	// Queue�� ����ִٸ� ó������ �ʴ´�..
	if (m_ChangeInstanceList.empty()) return;

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
			ChangeOpacityRenderData(originMeshData);
			break;
		case OBJECT_TYPE::PARTICLE_SYSTEM:
			ChangeTransparencyRenderData(originMeshData);
			break;
		case OBJECT_TYPE::UI:
			ChangeUIRenderData(originMeshData);
			break;
		default:
			ChangeUnRenderData(originMeshData);
			break;
		}

		// ��ȯ�� Mesh Data Pop..
		m_ChangeInstanceList.pop();
	}

	// ��� ��ȯ�� ������ List �缳��..
	CheckInstanceLayer(m_OpacityMeshList);
	CheckInstanceLayer(m_TransparencyMeshList);
}

void RenderManager::PushOpacityRenderData(RenderData* renderData)
{
	// �׸��� ���� ������ ��� Layer�� �������� �ʴ´�..
	if (renderData->m_InstanceLayerIndex == -1) return;

	// �ش� Layer �˻�..
	InstanceLayer* instanceLayer = m_Converter->GetLayer(renderData->m_InstanceLayerIndex);

	// �ش� Layer�� Render Data ����..
	instanceLayer->PushRenderData(renderData);

	// Culling ���� List ����..
	m_Culling->PushCullingMesh(renderData);

	// �ش� Layer�� ��ϵǾ� �ִ��� Ȯ��..
	if (instanceLayer->m_Instance->m_Material->m_MaterialProperty->Alpha)
	{
		FindInstanceLayer(m_TransparencyMeshList, instanceLayer);
	}
	else
	{
		FindInstanceLayer(m_OpacityMeshList, instanceLayer);
	}
}

void RenderManager::PushTransparencyRenderData(RenderData* renderData)
{
	// �ش� Layer �˻�..
	InstanceLayer* instanceLayer = m_Converter->GetLayer(renderData->m_InstanceLayerIndex);

	// �ش� Layer�� Render Data ����..
	instanceLayer->PushRenderData(renderData);

	// Culling ���� List ����..
	///m_Culling->PushCullingMesh(renderData);

	// �ش� Layer�� ��ϵǾ� �ִ��� Ȯ��..
	FindInstanceLayer(m_TransparencyMeshList, instanceLayer);
}

void RenderManager::PushUIRenderData(RenderData* renderData)
{
	m_UIRenderMeshList.push_back(renderData);
}

void RenderManager::PushUnRenderData(RenderData* renderData)
{
	m_UnRenderMeshList.push_back(renderData);
}

void RenderManager::ChangeOpacityRenderData(MeshData* meshData)
{
	// Render Data ��ȯ..
	RenderData* convertRenderData = (RenderData*)meshData->Render_Data;

	// ���� �� Layer Index ����..
	UINT prev_LayerIndex = convertRenderData->m_InstanceLayerIndex;

	// ���� ���� �� Layer �˻�..
	InstanceLayer* layer = m_Converter->GetLayer(prev_LayerIndex);

	// �ش� Layer�� ���� ���� ������Ѵ�..
	assert(layer != nullptr);

	// Render Data �缳��..
	m_Converter->ConvertRenderData(meshData, convertRenderData);

	// Layer�� ������ ���� ��� ó������ �ʴ´�..
	if (prev_LayerIndex == convertRenderData->m_InstanceIndex) return;

	// �ش� Layer�� �����Ѵٸ�..
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

	// �缳���� Layer �˻�..
	layer = m_Converter->GetLayer(convertRenderData->m_InstanceLayerIndex);

	// ���� Layer�� Instance �߰�..
	layer->PushRenderData(convertRenderData);

	// �ش� Layer�� ��ϵǾ� �ִ��� Ȯ��..
	if (layer->m_Instance->m_Material->m_MaterialProperty->Alpha)
	{
		FindInstanceLayer(m_TransparencyMeshList, layer);
	}
	else
	{
		FindInstanceLayer(m_OpacityMeshList, layer);
	}
}

void RenderManager::ChangeTransparencyRenderData(MeshData* meshData)
{
	// Render Data ��ȯ..
	RenderData* convertRenderData = (RenderData*)meshData->Render_Data;

	// ���� �� Layer Index ����..
	UINT prev_LayerIndex = convertRenderData->m_InstanceLayerIndex;

	// ���� ���� �� Layer �˻�..
	InstanceLayer* layer = m_Converter->GetLayer(convertRenderData->m_InstanceLayerIndex);

	// �ش� Layer�� ���� ���� ������Ѵ�..
	assert(layer != nullptr);

	// Render Data �缳��..
	m_Converter->ConvertRenderData(meshData, convertRenderData);

	// Layer�� ������ ���� ��� ó������ �ʴ´�..
	if (prev_LayerIndex == convertRenderData->m_InstanceIndex) return;

	// �ش� Layer�� �����Ѵٸ�..
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

	// �缳���� Layer �˻�..
	layer = m_Converter->GetLayer(convertRenderData->m_InstanceLayerIndex);

	// ���� Layer�� Instance �߰�..
	layer->PushRenderData(convertRenderData);

	// �ش� Layer�� ��ϵǾ� �ִ��� Ȯ��..
	FindInstanceLayer(m_TransparencyMeshList, layer);
}

void RenderManager::ChangeUIRenderData(MeshData* meshData)
{
	// Render Data ��ȯ..
	RenderData* convertRenderData = (RenderData*)meshData->Render_Data;

	// Render Data �缳��..
	convertRenderData->m_UI->m_Albedo = (ID3D11ShaderResourceView*)meshData->UI_Buffer->Albedo->pTextureBuf;
}

void RenderManager::ChangeUnRenderData(MeshData* meshData)
{

}

void RenderManager::DeleteOpacityRenderData(MeshData* meshData)
{
	// Render Data ��ȯ..
	RenderData* renderData = (RenderData*)meshData->Render_Data;

	/// Test
	if (renderData == RenderPassBase::g_Picking)
	{
		RenderPassBase::g_Picking = nullptr;
	}

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

	// �ش� Culling Render Data ����..
	m_Culling->DeleteCullingMesh(renderData);

	// �ش� Instance List���� ����...
	instanceLayer->DeleteRenderData(index);

	// Instance Layer ��� üũ..
	CheckInstanceLayer(m_OpacityMeshList);
}

void RenderManager::DeleteTransparencyRenderData(MeshData* meshData)
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
	CheckInstanceLayer(m_TransparencyMeshList);
}

void RenderManager::DeleteUIRenderData(MeshData* meshData)
{
	// Render Data ��ȯ..
	RenderData* renderData = (RenderData*)meshData->Render_Data;

	// Render Data�� List �������� Index..
	int index = -1;

	for (int i = 0; i < m_UIRenderMeshList.size(); i++)
	{
		// �ش� Render Data List Index �˻�..
		if (m_UIRenderMeshList[i] == renderData)
		{
			index = i;
			break;
		}
	}

	// Index�� �˻��� �ȵǸ� �ȵȴ�..
	assert(index != -1);

	UINT renderDataIndex = m_UIRenderMeshList[index]->m_ObjectData->ObjectIndex;

	// �ش� UI Render Buffer ����..
	m_Converter->DeleteUI(renderData->m_UI->m_BufferIndex);

	// �ش� Render Data ����..
	m_Converter->DeleteRenderData(renderDataIndex + 1);

	// �ش� Instance List���� ����...
	m_UIRenderMeshList.erase(std::next(m_UIRenderMeshList.begin(), index));
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

	// Index�� �˻��� �ȵǸ� �ȵȴ�..
	assert(index != -1);

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

	std::sort(layerList.begin(), layerList.end(), [](InstanceLayer* layer1, InstanceLayer* layer2) {return layer1->m_Instance->m_Type < layer2->m_Instance->m_Type; });
}

bool RenderManager::SortLayer(InstanceLayer* layer1, InstanceLayer* layer2)
{
	return layer1->m_MeshList[0]->m_ObjectData->ObjType < layer2->m_MeshList[0]->m_ObjectData->ObjType;
}

bool RenderManager::SortDistance(RenderData* obj1, RenderData* obj2)
{
	const Vector3& camPos = RenderPassBase::g_GlobalData->MainCamera_Data->CamPos;
	const Vector3& objPos1 = { (obj1->m_ObjectData->World)._41 - camPos.x, (obj1->m_ObjectData->World)._42 - camPos.y, (obj1->m_ObjectData->World)._43 - camPos.z };
	const Vector3& objPos2 = { (obj2->m_ObjectData->World)._41 - camPos.x, (obj2->m_ObjectData->World)._42 - camPos.y, (obj2->m_ObjectData->World)._43 - camPos.z };

	const float& distance1 = sqrtf(objPos1.x * objPos1.x + objPos1.y * objPos1.y + objPos1.z * objPos1.z);
	const float& distance2 = sqrtf(objPos2.x * objPos2.x + objPos2.y * objPos2.y + objPos2.z * objPos2.z);

	return (distance1 < distance2) ? true : false;
}
