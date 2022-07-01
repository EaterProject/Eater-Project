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
#include "Blur_Pass.h"
#include "Bloom_Pass.h"
#include "Combine_Pass.h"
#include "Fog_Pass.h"
#include "Picking_Pass.h"
#include "Culling_Pass.h"
#include "OutLine_Pass.h"
#include "Debug_Pass.h"

#include "ConstantBufferManager.h"

#include <algorithm>

#include "./Profiler/Profiler.h"


#define CREATE_PASS(ClassName, MemberName) \
MemberName = new ClassName;	\
m_RenderPassList.push_back(MemberName);	\
PushFunction(MemberName);	\

RenderManager::RenderManager(ID3D11Graphic* graphic, IFactoryManager* factory, IGraphicResourceManager* resource, IShaderManager* shader, IRenderDataConverter* converter, RenderOption* renderOption)
	:m_BlockQueueMax(100)
{
	// Rendering Initialize..
	RenderPassBase::Initialize(graphic->GetDevice(), graphic->GetContext(), factory, resource, shader, renderOption);

	m_SwapChain = graphic->GetSwapChain();

	// Render Data Converter ����..
	m_Converter = converter;
	m_Converter->Initialize(this);

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
	CREATE_PASS(Blur_Pass,		m_Blur);
	CREATE_PASS(Bloom_Pass,		m_Bloom);
	CREATE_PASS(Fog_Pass,		m_Fog);
	CREATE_PASS(Culling_Pass,	m_Culling);
	CREATE_PASS(Picking_Pass,	m_Picking);
	//CREATE_PASS(OutLine_Pass,	m_OutLine);
	CREATE_PASS(Combine_Pass,	m_Combine);
	CREATE_PASS(Debug_Pass,		m_Debug);

	m_OpacityBlockList.resize(m_BlockQueueMax, nullptr);
	m_TransparencyBlockList.resize(m_BlockQueueMax, nullptr);
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
		OnResizeFunction += Eater::Bind(&T::OnResize, pass);
	}
	// InstanceResize Override �Լ� ���..
	if (typeid(&RenderPassBase::InstanceResize).hash_code() != typeid(&T::InstanceResize).hash_code())
	{
		InstanceResizeFunction += Eater::Bind(&T::InstanceResize, pass);
	}
	// SetResize Override �Լ� ���..
	if (typeid(&RenderPassBase::SetResize).hash_code() != typeid(&T::SetResize).hash_code())
	{
		SetResizeFunction += Eater::Bind(&T::SetResize, pass);
	}
	// ApplyOption Override �Լ� ���..
	if (typeid(&RenderPassBase::ApplyOption).hash_code() != typeid(&T::ApplyOption).hash_code())
	{
		ApplyOptionFunction += Eater::Bind(&T::ApplyOption, pass);
	}
	// PreUpdate Override �Լ� ���..
	if (typeid(&RenderPassBase::PreUpdate).hash_code() != typeid(&T::PreUpdate).hash_code())
	{
		PreUpdateFunction += Eater::Bind(&T::PreUpdate, pass);
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

void RenderManager::RelocationLayer(MaterialRenderBuffer* material)
{
	std::queue<InstanceLayer*> layer_list;
	std::queue<int> index_list;

	const MaterialProperty* material_property = material->m_MaterialProperty;

	if (material_property->Alpha)
	{
		// Opacity Layer List ���ο��� �ش� Layer �˻�..
		for (int index = 0; index < m_OpacityMeshList.size(); index++)
		{
			if (m_OpacityMeshList[index]->m_Instance->m_Material == material)
			{
				layer_list.push(m_OpacityMeshList[index]);
				index_list.push(index);
			}
		}

		// �ش� Material ���� Layer ���� ����..
		while (!layer_list.empty())
		{
			InstanceLayer* change_layer = layer_list.front();
			int delete_index = index_list.front();

			// �ش� Layer ����..
			m_OpacityMeshList.erase(std::next(m_OpacityMeshList.begin(), delete_index));

			// Opacity -> Transparency �����̹Ƿ� Transparency�� �ƴ� ���� ������Ѵ�..
			assert(material_property->Alpha != false);

			// Layer ����..
			m_TransparencyMeshList.push_back(change_layer);

			layer_list.pop();
			index_list.pop();
		}

		// Layer ����..
		std::sort(m_TransparencyMeshList.begin(), m_TransparencyMeshList.end(), [](InstanceLayer* layer1, InstanceLayer* layer2) {return layer1->m_Instance->m_Type < layer2->m_Instance->m_Type; });
	}
	else
	{
		// Transparency Layer List ���ο��� �ش� Layer �˻�..
		for (int index = 0; index < m_TransparencyMeshList.size(); index++)
		{
			if (m_TransparencyMeshList[index]->m_Instance->m_Material == material)
			{
				layer_list.push(m_TransparencyMeshList[index]);
				index_list.push(index);
			}
		}

		// �ش� Material ���� Layer ���� ����..
		while (!layer_list.empty())
		{
			InstanceLayer* change_layer = layer_list.front();
			int delete_index = index_list.front();

			// �ش� Layer ����..
			m_TransparencyMeshList.erase(std::next(m_TransparencyMeshList.begin(), delete_index));

			// Transparency -> Opacity �����̹Ƿ� Opacity�� �ƴ� ���� ������Ѵ�..
			assert(material_property->Alpha != true);

			// Layer ����..
			m_OpacityMeshList.push_back(change_layer);

			layer_list.pop();
			index_list.pop();
		}

		// Layer ����..
		std::sort(m_OpacityMeshList.begin(), m_OpacityMeshList.end(), [](InstanceLayer* layer1, InstanceLayer* layer2) {return layer1->m_Instance->m_Type < layer2->m_Instance->m_Type; });
	}
}

void RenderManager::PreUpdate()
{
	// Global Constant Buffer Update..
	ConstantBufferManager::Update();

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
	m_GraphicRenderOption = *RenderPassBase::g_RenderOption;
}

void RenderManager::SetGlobalData(GlobalData* globalData)
{
	RenderPassBase::SetGlobalData(globalData);
}

void RenderManager::SetFullScreenBlur(bool enable, UINT blur_count)
{
	m_GraphicRenderOption.FullScreenBlurOption = enable;
	m_GraphicRenderOption.FullScreenBlurCount = blur_count;
}

void RenderManager::SetSkyCube(TextureBuffer* resource)
{
	m_Sky->SetSkyCubeResource(resource);
}

void RenderManager::SetSkyLight(SkyLightBuffer* resource, UINT index)
{
	m_Sky->SetSkyLightResource(resource, index);
}

void RenderManager::SetColorGradingBaseTexture(TextureBuffer* lut_resource)
{
	m_Combine->SetColorGradingBaseTexture(lut_resource);
}

void RenderManager::SetColorGradingBlendTexture(TextureBuffer* lut_resource)
{
	m_Combine->SetColorGradingBlendTexture(lut_resource);
}

void RenderManager::SetColorGradingBlendFactor(float blend_factor)
{
	m_Combine->SetColorGradingBlendFactor(blend_factor);
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

	RenderData* renderData = (RenderData*)instance->Render_Data;
	
	// Object Type�� ���� Render Mesh Data ����..
	switch (instance->Object_Data->ObjType)
	{
	case OBJECT_TYPE::BASE:
	case OBJECT_TYPE::SKINNING:
	case OBJECT_TYPE::TERRAIN:
	case OBJECT_TYPE::PARTICLE_SYSTEM:
		DeleteMeshRenderData(renderData);
		break;
	case OBJECT_TYPE::PARTICLE:
		break;
	case OBJECT_TYPE::UI:
		DeleteUIRenderData(renderData);
		break;
	default:
		DeleteUnRenderData(renderData);
		break;
	}

	if (renderData == RenderPassBase::g_Picking)
	{
		RenderPassBase::g_Picking = nullptr;
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

void RenderManager::PushMaterialBlockInstance(MeshData* instance)
{
	m_BlockInstanceList.push(instance);
}

void RenderManager::PopMaterialBlockInstance(MeshData* instance)
{
	MaterialPropertyBlock* material_block = instance->Object_Data->Material_Block;
	
	RenderData* render_data = m_Converter->GetRenderData(instance->Object_Data->ObjectIndex);

	if (render_data == nullptr) return;

	// ��ϵ� Material Block�� �ƴѰ��..
	if (material_block->Queue_Index == -1) return;

	// �ش� Block Alpha ���¿� ���� ����..
	if (material_block->Alpha)
	{
		// ���Խ� �����ص״� �ش� ��ġ�� ����ش�..
		m_TransparencyBlockList[material_block->Queue_Index] = nullptr;

		// Render Data Block ��Ȱ��ȭ..
		render_data->m_Block = false;

		// ���� ���� �ε����� ���� ū ���¶��..
		if (material_block->Queue_Index == m_TransparencyBlockSize - 1)
		{
			// �������� ���鼭 �ִ� ������ �缳��..
			for (int i = material_block->Queue_Index - 1; i >= 0; i--)
			{
				if (m_TransparencyBlockList[i] != nullptr)
				{
					m_TransparencyBlockSize = i + 1;
					break;
				}
			}
		}

		// �ش� �ε��� �ʱ�ȭ..
		material_block->Queue_Index = -1;
	}
	else
	{
		// ���Խ� �����ص״� �ش� ��ġ�� ����ش�..
		m_OpacityBlockList[material_block->Queue_Index] = nullptr;

		// Render Data Block ��Ȱ��ȭ..
		render_data->m_Block = false;

		// ���� ���� �ε����� ���� ū ���¶��..
		if (material_block->Queue_Index == m_OpacityBlockSize - 1)
		{
			// �������� ���鼭 �ִ� ������ �缳��..
			for (int i = material_block->Queue_Index - 1; i >= 0; i--)
			{
				if (m_OpacityBlockList[i] != nullptr)
				{
					m_OpacityBlockSize = i + 1;
					break;
				}
			}
		}

		// �ش� �ε��� �ʱ�ȭ..
		material_block->Queue_Index = -1;
	}
}

void RenderManager::ConvertRenderData()
{
	// Render Resource ����ȭ �۾�..
	m_Converter->ResourceUpdate();

	// ���� ������ ���� �߰��� Render Data Convert �۾�..
	ConvertPushInstance();

	// ���� ������ ���� ����� Render Data Convert �۾�..
	ConvertChangeInstance();

	// ���� ������ ���� �߰��� Material Block Render Data ����..
	PushMaterialBlockInstance();
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

		m_Picking->RenderUpdate(m_InstanceLayer->m_Instance, m_InstanceLayer->m_InstanceList);
	}

	// Transparency Object Picking Draw..
	for (int i = 0; i < m_TransparencyMeshList.size(); i++)
	{
		m_InstanceLayer = m_TransparencyMeshList[i];

		m_Picking->RenderUpdate(m_InstanceLayer->m_Instance, m_InstanceLayer->m_InstanceList);
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
	if (m_GraphicRenderOption.RenderingOption & RENDER_OPTION::RENDER_SHADOW)
	{
		m_Shadow->BeginRender();

		for (int i = 0; i < m_OpacityMeshList.size(); i++)
		{
			m_InstanceLayer = m_OpacityMeshList[i];

			m_Shadow->RenderUpdate(m_InstanceLayer->m_Instance, m_InstanceLayer->m_InstanceList);
		}
	}
}

void RenderManager::DeferredRender()
{
	m_Deferred->BeginRender();

	for (int i = 0; i < m_OpacityMeshList.size(); i++)
	{
		m_InstanceLayer = m_OpacityMeshList[i];

		m_Deferred->RenderUpdate(m_InstanceLayer->m_Instance, m_InstanceLayer->m_InstanceList);
	}

	for (int i = 0; i < m_OpacityBlockSize; i++)
	{
		m_RenderData = m_OpacityBlockList[i];

		if (m_RenderData == nullptr || m_RenderData->m_Draw == false) continue;

		m_Deferred->BlockRenderUpdate(m_RenderData);
	}
}

void RenderManager::SSAORender()
{
	if (m_GraphicRenderOption.RenderingOption & RENDER_OPTION::RENDER_SSAO)
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

		m_Alpha->RenderUpdate(m_InstanceLayer->m_Instance, m_InstanceLayer->m_InstanceList);
	}

	for (int i = 0; i < m_TransparencyBlockSize; i++)
	{
		m_RenderData = m_TransparencyBlockList[i];

		if (m_RenderData == nullptr || m_RenderData->m_Draw == false) continue;
		
		m_Alpha->BlockRenderUpdate(m_RenderData);
	}

	GPU_BEGIN_EVENT_DEBUG_NAME("OIT Pass");
	m_OIT->RenderUpdate();
	GPU_END_EVENT_DEBUG_NAME();
}

void RenderManager::PostProcessingRender()
{
	GPU_BEGIN_EVENT_DEBUG_NAME("Bloom Pass");
	if (m_GraphicRenderOption.PostProcessOption & POSTPROCESS_OPTION::POSTPROCESS_BLOOM)
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
	if (m_GraphicRenderOption.PostProcessOption & POSTPROCESS_OPTION::POSTPROCESS_FXAA)
	{
		m_FXAA->RenderUpdate();
	}
	GPU_END_EVENT_DEBUG_NAME();

	GPU_BEGIN_EVENT_DEBUG_NAME("Screen Blur Pass");
	if (m_GraphicRenderOption.FullScreenBlurOption)
	{
		m_Blur->ScreenBlur(m_GraphicRenderOption.FullScreenBlurCount);
	}
	GPU_END_EVENT_DEBUG_NAME();
}

void RenderManager::UIRender()
{
	for (int i = 0; i < m_UIRenderMeshList.size(); i++)
	{
		m_UILayer = m_UIRenderMeshList[i];

		m_UI->RenderUpdate(m_UILayer->m_InstanceList);
	}
}

void RenderManager::DebugRender()
{
	if (m_GraphicRenderOption.DebugOption & DEBUG_OPTION::DEBUG_MODE)
	{
		GPU_MARKER_DEBUG_NAME("Object Debug");
		m_Debug->BeginRender();

		// Render Mesh Debugging..
		for (int i = 0; i < m_OpacityMeshList.size(); i++)
		{
			m_InstanceLayer = m_OpacityMeshList[i];

			for (int j = 0; j < m_InstanceLayer->m_InstanceList.size(); j++)
			{
				m_RenderData = m_InstanceLayer->m_InstanceList[j];

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

			for (int j = 0; j < m_InstanceLayer->m_InstanceList.size(); j++)
			{
				m_RenderData = m_InstanceLayer->m_InstanceList[j];

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

		if (m_GraphicRenderOption.DebugOption & DEBUG_RENDERTARGET)
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
		case OBJECT_TYPE::PARTICLE_SYSTEM:
			PushMeshRenderData(convertRenderData);
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
		InstanceRenderBuffer* instance = m_Converter->GetInstance(convertRenderData->m_InstanceLayerIndex);

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
		case OBJECT_TYPE::PARTICLE_SYSTEM:
			ChangeMeshRenderData(originMeshData);
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
	CheckEmptyLayer(m_OpacityMeshList);
	CheckEmptyLayer(m_TransparencyMeshList);
	CheckEmptyLayer(m_UIRenderMeshList);
}

void RenderManager::PushMaterialBlockInstance()
{
	// Queue�� ����ִٸ� ó������ �ʴ´�..
	if (m_BlockInstanceList.empty()) return;

	// ���� ������ ������ �׾Ƶ� ����� Render Data List ���� �۾�..
	while (!m_BlockInstanceList.empty())
	{
		// �ش� ���� Mesh Data ����..
		MeshData* originMeshData = m_BlockInstanceList.front();

		// �ش� Render Data ����..
		RenderData* convertRenderData = (RenderData*)originMeshData->Render_Data;

		assert(convertRenderData != nullptr);

		// �ش� Material Block ����..
		MaterialPropertyBlock* material_block = originMeshData->Object_Data->Material_Block;

		// �ش� Material Block�� �̹� ��ϵ� ���..
		if (material_block->Queue_Index != -1) return;

		// �ش� Block Alpha ���¿� ���� ����..
		if (material_block->Alpha)
		{
			for (int index = 0; index < m_BlockQueueMax; index++)
			{
				// �� ������ �־��ش�..
				if (m_TransparencyBlockList[index] == nullptr)
				{
					m_TransparencyBlockList[index] = convertRenderData;

					// Render Data Block Ȱ��ȭ..
					convertRenderData->m_Block = true;

					// ���� ����Ʈ ���ο����� �ε��� ����..
					material_block->Queue_Index = index;

					// ���� ���� ū �ε����� ��� �ִ� ����� �缳��..
					if (index >= m_TransparencyBlockSize)
					{
						m_TransparencyBlockSize = index + 1;
					}
					break;
				}
			}
		}
		else
		{
			for (int index = 0; index < m_BlockQueueMax; index++)
			{
				// �� ������ �־��ش�..
				if (m_OpacityBlockList[index] == nullptr)
				{
					m_OpacityBlockList[index] = convertRenderData;

					// Render Data Block Ȱ��ȭ..
					convertRenderData->m_Block = true;

					// ���� ����Ʈ ���ο����� �ε��� ����..
					material_block->Queue_Index = index;

					// ���� ���� ū �ε����� ��� �ִ� ����� �缳��..
					if (index >= m_OpacityBlockSize)
					{
						m_OpacityBlockSize = index + 1;
					}
					break;
				}
			}
		}

		m_BlockInstanceList.pop();
	}
}

void RenderManager::PushMeshRenderData(RenderData* renderData)
{
	// �׸��� ���� ������ ��� Layer�� �������� �ʴ´�..
	if (renderData->m_InstanceLayerIndex == -1) return;

	// �ش� Layer �˻�..
	InstanceLayer* instanceLayer = m_Converter->GetInstanceLayer(renderData->m_InstanceLayerIndex);

	// �ش� Layer�� Render Data ����..
	instanceLayer->PushRenderData(renderData);

	switch (renderData->m_ObjectData->ObjType)
	{
	case OBJECT_TYPE::BASE:
	case OBJECT_TYPE::SKINNING:
	case OBJECT_TYPE::TERRAIN:
	{			
		// Culling ���� List ����..
		m_Culling->PushCullingMesh(renderData);

		// �ش� Layer�� ��ϵǾ� �ִ��� Ȯ��..
		if (instanceLayer->m_Instance->m_Material->m_MaterialProperty->Alpha)
		{
			FindLayer(m_TransparencyMeshList, instanceLayer);
		}
		else
		{

			FindLayer(m_OpacityMeshList, instanceLayer);
		}
	}
		break;
	case OBJECT_TYPE::PARTICLE_SYSTEM:
	{
		FindLayer(m_TransparencyMeshList, instanceLayer);
	}
		break;
	default:
		break;
	}
}

void RenderManager::PushUIRenderData(RenderData* renderData)
{
	// �ش� Layer �˻�..
	UILayer* layer = m_Converter->GetUILayer(renderData->m_UI->m_BufferLayer);

	// �ش� Layer�� UI Data ����..
	layer->PushRenderData(renderData);

	// �ش� Layer�� ��ϵǾ� �ִ��� Ȯ��..
	FindLayer(m_UIRenderMeshList, layer);
}

void RenderManager::PushUnRenderData(RenderData* renderData)
{
	m_UnRenderMeshList.push_back(renderData);
}

void RenderManager::ChangeMeshRenderData(MeshData* meshData)
{
	// Render Data ��ȯ..
	RenderData* renderData = (RenderData*)meshData->Render_Data;

	// ���� �� Layer Index ����..
	UINT prev_LayerIndex = renderData->m_InstanceLayerIndex;

	// ���� ���� �� Layer �˻�..
	InstanceLayer* instanceLayer = m_Converter->GetInstanceLayer(prev_LayerIndex);

	// �ش� Layer�� ���� ���� ������Ѵ�..
	assert(instanceLayer != nullptr);

	// Render Data �缳��..
	m_Converter->ConvertInstanceData(meshData, renderData);

	// Layer�� ������ ���� ��� ó������ �ʴ´�..
	if (prev_LayerIndex == renderData->m_InstanceLayerIndex) return;

	// �ش� Render Data List���� ����..
	instanceLayer->PopRenderData(renderData);

	// ���� Layer�� ����ִ� ���¶�� Layer ����..
	if (instanceLayer->m_InstanceList.empty())
	{
		m_Converter->DeleteInstanceLayer(renderData->m_InstanceLayerIndex);
	}

	// �缳���� Layer �˻�..
	instanceLayer = m_Converter->GetInstanceLayer(renderData->m_InstanceLayerIndex);

	// ���� Layer�� Instance �߰�..
	instanceLayer->PushRenderData(renderData);

	// �ش� Layer�� ��ϵǾ� �ִ��� Ȯ��..
	switch (renderData->m_ObjectData->ObjType)
	{
	case OBJECT_TYPE::BASE:
	case OBJECT_TYPE::SKINNING:
	case OBJECT_TYPE::TERRAIN:
	{
		// �ش� Layer�� ��ϵǾ� �ִ��� Ȯ��..
		if (instanceLayer->m_Instance->m_Material->m_MaterialProperty->Alpha)
		{
			FindLayer(m_TransparencyMeshList, instanceLayer);
		}
		else
		{
			FindLayer(m_OpacityMeshList, instanceLayer);
		}
	}
		break;
	case OBJECT_TYPE::PARTICLE_SYSTEM:
	{
		FindLayer(m_TransparencyMeshList, instanceLayer);
	}
		break;
	default:
		break;
	}
}

void RenderManager::ChangeUIRenderData(MeshData* meshData)
{
	// Render Data ��ȯ..
	RenderData* renderData = (RenderData*)meshData->Render_Data;

	// UI Buffer ����..
	UIRenderBuffer* ui_Buffer = renderData->m_UI;

	// ���� �� Layer Index ����..
	UINT prev_LayerIndex = ui_Buffer->m_BufferLayer;

	// ���� ���� �� Layer �˻�..
	UILayer* layer = m_Converter->GetUILayer(prev_LayerIndex);

	// �ش� Layer�� ���� ���� ������Ѵ�..
	assert(layer != nullptr);

	// Render Data �缳��..
	m_Converter->ConvertUIData(meshData, renderData);

	// Layer�� ������ ���� ��� ó������ �ʴ´�..
	if (prev_LayerIndex == ui_Buffer->m_BufferLayer) return;

	// �ش� Render Data List���� ����..
	layer->PopRenderData(renderData);

	// ���� Layer�� ����ִ� ���¶�� Layer ����..
	if (layer->m_InstanceList.empty())
	{
		m_Converter->DeleteUILayer(ui_Buffer->m_BufferLayer);
	}

	// �缳���� Layer �˻�..
	layer = m_Converter->GetUILayer(ui_Buffer->m_BufferLayer);

	// ���� Layer�� Instance �߰�..
	layer->PushRenderData(renderData);

	// �ش� Layer�� ��ϵǾ� �ִ��� Ȯ��..
	FindLayer(m_UIRenderMeshList, layer);
}

void RenderManager::ChangeUnRenderData(MeshData* meshData)
{

}

void RenderManager::DeleteMeshRenderData(RenderData* renderData)
{
	// Render Data�� List �������� Index..
	UINT layerIndex = renderData->m_InstanceLayerIndex;
	UINT renderDataIndex = renderData->m_ObjectData->ObjectIndex;

	// �ش� Layer �˻�..
	InstanceLayer* layer = m_Converter->GetInstanceLayer(layerIndex);

	// �ش� Instance Layer���� ����...
	layer->PopRenderData(renderData);

	// �ش� Render Data ����..
	m_Converter->DeleteRenderData(renderDataIndex + 1);

	// �ش� Culling Render Data ����..
	m_Culling->PopCullingMesh(renderData);

	// Render Buffer..
	InstanceRenderBuffer* renderBuffer = layer->m_Instance;

	// Layer ��� üũ..
	switch (renderBuffer->m_Type)
	{
	case OBJECT_TYPE::BASE:
	case OBJECT_TYPE::SKINNING:
	case OBJECT_TYPE::TERRAIN:
	{
		if (renderBuffer->m_Material->m_MaterialProperty->Alpha)
		{
			CheckEmptyLayer(m_TransparencyMeshList);
		}
		else
		{
			CheckEmptyLayer(m_OpacityMeshList);
		}
	}
		break;
	case OBJECT_TYPE::PARTICLE_SYSTEM:
		CheckEmptyLayer(m_TransparencyMeshList);
		break;
	default:
		break;
	}

	// ���� Layer�� ����ִ� ���¶�� Layer ����..
	if (layer->m_InstanceList.empty())
	{
		m_Converter->DeleteInstanceLayer(layerIndex);
	}
}

void RenderManager::DeleteUIRenderData(RenderData* renderData)
{
	// Render Data�� List �������� Index..
	UINT layerIndex = renderData->m_UI->m_BufferLayer;
	UINT bufferIndex = renderData->m_UI->m_BufferIndex;
	UINT renderDataIndex = renderData->m_ObjectData->ObjectIndex;

	// �ش� Layer �˻�..
	UILayer* layer = m_Converter->GetUILayer(layerIndex);

	// �ش� UI Render Buffer ����..
	m_Converter->DeleteUI(bufferIndex);

	// �ش� Layer���� ����..
	layer->PopRenderData(renderData);

	// �ش� Render Data ����..
	m_Converter->DeleteRenderData(renderDataIndex + 1);

	// Layer ��� üũ..
	CheckEmptyLayer(m_UIRenderMeshList);

	// ���� Layer�� ����ִ� ���¶�� Layer ����..
	if (layer->m_InstanceList.empty())
	{
		m_Converter->DeleteUILayer(layerIndex);
	}
}

void RenderManager::DeleteUnRenderData(RenderData* renderData)
{
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

	UINT renderDataIndex = renderData->m_ObjectData->ObjectIndex;

	// �ش� Render Data ����..
	m_Converter->DeleteRenderData(renderDataIndex + 1);

	// �ش� Instance List���� ����...
	m_UnRenderMeshList.erase(std::next(m_UnRenderMeshList.begin(), index));
}

template<typename Layer>
void RenderManager::CheckEmptyLayer(std::vector<Layer*>& layerList)
{
	for (int index = 0; index < layerList.size(); index++)
	{
		// �ش� Layer List Index �˻�..
		if (layerList[index]->m_InstanceList.empty())
		{
			// �ش� Layer�� ����ִٸ� Layer ����..
			layerList.erase(std::next(layerList.begin(), index));
			break;
		}
	}
}

template<>
void RenderManager::FindLayer(std::vector<InstanceLayer*>& layerList, InstanceLayer* layer)
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

	// Layer ����..
	std::sort(layerList.begin(), layerList.end(), [](InstanceLayer* layer1, InstanceLayer* layer2) {return layer1->m_Instance->m_Type < layer2->m_Instance->m_Type; });
}

template<>
void RenderManager::FindLayer(std::vector<UILayer*>& layerList, UILayer* layer)
{
	for (UILayer* uiLayer : layerList)
	{
		// �ش� Layer�� �̹� ��ϵǾ� �ִٸ� �߰����� �ʴ´�..
		if (uiLayer == layer)
		{
			return;
		}
	}

	// ���� Layer�� �˻����� �ʾҴٸ� Layer List�� ����..
	layerList.push_back(layer);

	// Layer ����..
	std::sort(layerList.begin(), layerList.end(), [](UILayer* layer1, UILayer* layer2) {return layer1->m_LayerIndex < layer2->m_LayerIndex; });
}

template<typename Layer>
void RenderManager::DeleteLayer(std::vector<Layer*>& layerList, Layer* layer)
{
	for (int index = 0; index < layerList.size(); index++)
	{
		// �ش� Layer List Index �˻�..
		if (layerList[index] == layer)
		{
			// �ش� Layer�� ����ִٸ� Layer ����..
			layerList.erase(std::next(layerList.begin(), index));
			break;
		}
	}
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
