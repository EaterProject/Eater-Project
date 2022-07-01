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

	// Render Data Converter 생성..
	m_Converter = converter;
	m_Converter->Initialize(this);

	// Render Pass 생성..
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

	// OnResize Override 함수 등록..
	if (typeid(&RenderPassBase::OnResize).hash_code() != typeid(&T::OnResize).hash_code())
	{
		OnResizeFunction += Eater::Bind(&T::OnResize, pass);
	}
	// InstanceResize Override 함수 등록..
	if (typeid(&RenderPassBase::InstanceResize).hash_code() != typeid(&T::InstanceResize).hash_code())
	{
		InstanceResizeFunction += Eater::Bind(&T::InstanceResize, pass);
	}
	// SetResize Override 함수 등록..
	if (typeid(&RenderPassBase::SetResize).hash_code() != typeid(&T::SetResize).hash_code())
	{
		SetResizeFunction += Eater::Bind(&T::SetResize, pass);
	}
	// ApplyOption Override 함수 등록..
	if (typeid(&RenderPassBase::ApplyOption).hash_code() != typeid(&T::ApplyOption).hash_code())
	{
		ApplyOptionFunction += Eater::Bind(&T::ApplyOption, pass);
	}
	// PreUpdate Override 함수 등록..
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

	// 최초 Render Setting..
	RenderSetting();
}

void RenderManager::OnResize(int width, int height)
{
	// Resource Resize Data 설정..
	SetResizeFunction(width, height);

	// Resource Resize 실행..
	RenderPassBase::g_Resource->OnResize(width, height);

	// Resize Resource 동기화..
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
		// Opacity Layer List 내부에서 해당 Layer 검색..
		for (int index = 0; index < m_OpacityMeshList.size(); index++)
		{
			if (m_OpacityMeshList[index]->m_Instance->m_Material == material)
			{
				layer_list.push(m_OpacityMeshList[index]);
				index_list.push(index);
			}
		}

		// 해당 Material 관련 Layer 전부 제거..
		while (!layer_list.empty())
		{
			InstanceLayer* change_layer = layer_list.front();
			int delete_index = index_list.front();

			// 해당 Layer 제거..
			m_OpacityMeshList.erase(std::next(m_OpacityMeshList.begin(), delete_index));

			// Opacity -> Transparency 변경이므로 Transparency가 아닐 경우는 없어야한다..
			assert(material_property->Alpha != false);

			// Layer 삽입..
			m_TransparencyMeshList.push_back(change_layer);

			layer_list.pop();
			index_list.pop();
		}

		// Layer 정렬..
		std::sort(m_TransparencyMeshList.begin(), m_TransparencyMeshList.end(), [](InstanceLayer* layer1, InstanceLayer* layer2) {return layer1->m_Instance->m_Type < layer2->m_Instance->m_Type; });
	}
	else
	{
		// Transparency Layer List 내부에서 해당 Layer 검색..
		for (int index = 0; index < m_TransparencyMeshList.size(); index++)
		{
			if (m_TransparencyMeshList[index]->m_Instance->m_Material == material)
			{
				layer_list.push(m_TransparencyMeshList[index]);
				index_list.push(index);
			}
		}

		// 해당 Material 관련 Layer 전부 제거..
		while (!layer_list.empty())
		{
			InstanceLayer* change_layer = layer_list.front();
			int delete_index = index_list.front();

			// 해당 Layer 제거..
			m_TransparencyMeshList.erase(std::next(m_TransparencyMeshList.begin(), delete_index));

			// Transparency -> Opacity 변경이므로 Opacity가 아닐 경우는 없어야한다..
			assert(material_property->Alpha != true);

			// Layer 삽입..
			m_OpacityMeshList.push_back(change_layer);

			layer_list.pop();
			index_list.pop();
		}

		// Layer 정렬..
		std::sort(m_OpacityMeshList.begin(), m_OpacityMeshList.end(), [](InstanceLayer* layer1, InstanceLayer* layer2) {return layer1->m_Instance->m_Type < layer2->m_Instance->m_Type; });
	}
}

void RenderManager::PreUpdate()
{
	// Global Constant Buffer Update..
	ConstantBufferManager::Update();

	// PreUpdate 함수 리스트 실행..
	PreUpdateFunction();
}

void RenderManager::InstanceResize()
{
	size_t&& renderMaxCount = m_Converter->FindMaxInstanceCount();
	size_t&& unRenderMaxCount = m_UnRenderMeshList.size();

	// Instance Resize 함수 리스트 실행..
	InstanceResizeFunction(renderMaxCount, unRenderMaxCount);
}

void RenderManager::RenderSetting(RenderOption* renderOption)
{
	// RenderOption 저장..
	RenderPassBase::g_RenderOption = renderOption;

	// 최초 Render Setting..
	RenderSetting();
}

void RenderManager::RenderSetting()
{
	// Apply Option 함수 리스트 실행..
	ApplyOptionFunction();

	// 현재 Render Option 저장..
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
	// 현재 비어있는 상태이므로 추후 설정을 위해 Mesh Data 삽입..
	m_PushInstanceList.push(instance);
}

void RenderManager::PushMesh(MeshBuffer* mesh)
{
	// 현재 추가된 Mesh Buffer 동기화를 위해 삽입..
	m_Converter->PushMesh(mesh);
}

void RenderManager::PushMaterial(MaterialBuffer* material)
{
	// 현재 추가된 Material Buffer 동기화를 위해 삽입..
	m_Converter->PushMaterial(material);
}

void RenderManager::PushAnimation(AnimationBuffer* animation)
{
	// 현재 추가된 Animation Buffer 동기화를 위해 삽입..
	m_Converter->PushAnimation(animation);
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

void RenderManager::PushChangeAnimation(AnimationBuffer* animation)
{
	// 현재 바뀐 Animation Buffer 동기화를 위해 삽입..
	m_Converter->PushChangeAnimation(animation);
}

void RenderManager::DeleteInstance(MeshData* instance)
{
	if (instance == nullptr) return;

	RenderData* renderData = (RenderData*)instance->Render_Data;
	
	// Object Type에 따른 Render Mesh Data 제거..
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

	// 등록된 Material Block이 아닌경우..
	if (material_block->Queue_Index == -1) return;

	// 해당 Block Alpha 상태에 따른 삭제..
	if (material_block->Alpha)
	{
		// 삽입시 저장해뒀던 해당 위치를 비워준다..
		m_TransparencyBlockList[material_block->Queue_Index] = nullptr;

		// Render Data Block 비활성화..
		render_data->m_Block = false;

		// 만약 현재 인덱스가 가장 큰 상태라면..
		if (material_block->Queue_Index == m_TransparencyBlockSize - 1)
		{
			// 역순으로 돌면서 최대 사이즈 재설정..
			for (int i = material_block->Queue_Index - 1; i >= 0; i--)
			{
				if (m_TransparencyBlockList[i] != nullptr)
				{
					m_TransparencyBlockSize = i + 1;
					break;
				}
			}
		}

		// 해당 인덱스 초기화..
		material_block->Queue_Index = -1;
	}
	else
	{
		// 삽입시 저장해뒀던 해당 위치를 비워준다..
		m_OpacityBlockList[material_block->Queue_Index] = nullptr;

		// Render Data Block 비활성화..
		render_data->m_Block = false;

		// 만약 현재 인덱스가 가장 큰 상태라면..
		if (material_block->Queue_Index == m_OpacityBlockSize - 1)
		{
			// 역순으로 돌면서 최대 사이즈 재설정..
			for (int i = material_block->Queue_Index - 1; i >= 0; i--)
			{
				if (m_OpacityBlockList[i] != nullptr)
				{
					m_OpacityBlockSize = i + 1;
					break;
				}
			}
		}

		// 해당 인덱스 초기화..
		material_block->Queue_Index = -1;
	}
}

void RenderManager::ConvertRenderData()
{
	// Render Resource 동기화 작업..
	m_Converter->ResourceUpdate();

	// 현재 프레임 동안 추가된 Render Data Convert 작업..
	ConvertPushInstance();

	// 현재 프레임 동안 변경된 Render Data Convert 작업..
	ConvertChangeInstance();

	// 현재 프레임 동안 추가된 Material Block Render Data 삽입..
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
	// Rendering Resource 동기화 작업..
	ConvertRenderData();

	// Render Data 선별 작업..
	SelectRenderData();

	// Render Pass 사전 Update..
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
	
	// 현재 클릭한 Pixel ID 검출..
	int pickID = (int)m_Picking->FindPick(x, y);

	GPU_END_EVENT_DEBUG_NAME();

	// 해당 Render Data 검색..
	RenderData* renderData = m_Converter->GetRenderData(pickID);

	/// Test
	RenderPassBase::g_Picking = renderData;

	// 검색된 Render Data가 없는것은 선택된 Object가 없다는 것..
	if (renderData == nullptr) return nullptr;

	// 해당 Render Data의 원본 GameObject를 반환..
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

	// 최종 출력..
	m_SwapChain->Present(0, 0);
}

void RenderManager::ConvertPushInstance()
{
	// Queue가 비어있다면 처리하지 않는다..
	if (m_PushInstanceList.empty()) return;

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

		// 변환한 Mesh Data Pop..
		m_PushInstanceList.pop();
	}

	// Instance Resize..
	InstanceResize();
}

void RenderManager::ConvertChangeInstance()
{
	// Queue가 비어있다면 처리하지 않는다..
	if (m_ChangeInstanceList.empty()) return;

	// 현재 프레임 진행중 쌓아둔 변경된 Render Data List 삽입 작업..
	while (!m_ChangeInstanceList.empty())
	{
		// 해당 원본 Mesh Data 추출..
		MeshData* originMeshData = m_ChangeInstanceList.front();

		// 해당 Render Data 추출..
		RenderData* convertRenderData = (RenderData*)originMeshData->Render_Data;

		// 해당 Instance Buffer 추출..
		InstanceRenderBuffer* instance = m_Converter->GetInstance(convertRenderData->m_InstanceLayerIndex);

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

		// 변환한 Mesh Data Pop..
		m_ChangeInstanceList.pop();
	}

	// 모든 변환이 끝난후 List 재설정..
	CheckEmptyLayer(m_OpacityMeshList);
	CheckEmptyLayer(m_TransparencyMeshList);
	CheckEmptyLayer(m_UIRenderMeshList);
}

void RenderManager::PushMaterialBlockInstance()
{
	// Queue가 비어있다면 처리하지 않는다..
	if (m_BlockInstanceList.empty()) return;

	// 현재 프레임 진행중 쌓아둔 변경된 Render Data List 삽입 작업..
	while (!m_BlockInstanceList.empty())
	{
		// 해당 원본 Mesh Data 추출..
		MeshData* originMeshData = m_BlockInstanceList.front();

		// 해당 Render Data 추출..
		RenderData* convertRenderData = (RenderData*)originMeshData->Render_Data;

		assert(convertRenderData != nullptr);

		// 해당 Material Block 추출..
		MaterialPropertyBlock* material_block = originMeshData->Object_Data->Material_Block;

		// 해당 Material Block이 이미 등록된 경우..
		if (material_block->Queue_Index != -1) return;

		// 해당 Block Alpha 상태에 따른 삽입..
		if (material_block->Alpha)
		{
			for (int index = 0; index < m_BlockQueueMax; index++)
			{
				// 빈 공간에 넣어준다..
				if (m_TransparencyBlockList[index] == nullptr)
				{
					m_TransparencyBlockList[index] = convertRenderData;

					// Render Data Block 활성화..
					convertRenderData->m_Block = true;

					// 현재 리스트 내부에서의 인덱스 저장..
					material_block->Queue_Index = index;

					// 현재 가장 큰 인덱스일 경우 최대 사이즈를 재설정..
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
				// 빈 공간에 넣어준다..
				if (m_OpacityBlockList[index] == nullptr)
				{
					m_OpacityBlockList[index] = convertRenderData;

					// Render Data Block 활성화..
					convertRenderData->m_Block = true;

					// 현재 리스트 내부에서의 인덱스 저장..
					material_block->Queue_Index = index;

					// 현재 가장 큰 인덱스일 경우 최대 사이즈를 재설정..
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
	// 그릴수 없는 상태인 경우 Layer에 삽입하지 않는다..
	if (renderData->m_InstanceLayerIndex == -1) return;

	// 해당 Layer 검색..
	InstanceLayer* instanceLayer = m_Converter->GetInstanceLayer(renderData->m_InstanceLayerIndex);

	// 해당 Layer에 Render Data 삽입..
	instanceLayer->PushRenderData(renderData);

	switch (renderData->m_ObjectData->ObjType)
	{
	case OBJECT_TYPE::BASE:
	case OBJECT_TYPE::SKINNING:
	case OBJECT_TYPE::TERRAIN:
	{			
		// Culling 전용 List 삽입..
		m_Culling->PushCullingMesh(renderData);

		// 해당 Layer가 등록되어 있는지 확인..
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
	// 해당 Layer 검색..
	UILayer* layer = m_Converter->GetUILayer(renderData->m_UI->m_BufferLayer);

	// 해당 Layer에 UI Data 삽입..
	layer->PushRenderData(renderData);

	// 해당 Layer가 등록되어 있는지 확인..
	FindLayer(m_UIRenderMeshList, layer);
}

void RenderManager::PushUnRenderData(RenderData* renderData)
{
	m_UnRenderMeshList.push_back(renderData);
}

void RenderManager::ChangeMeshRenderData(MeshData* meshData)
{
	// Render Data 변환..
	RenderData* renderData = (RenderData*)meshData->Render_Data;

	// 변경 전 Layer Index 저장..
	UINT prev_LayerIndex = renderData->m_InstanceLayerIndex;

	// 현재 변경 전 Layer 검색..
	InstanceLayer* instanceLayer = m_Converter->GetInstanceLayer(prev_LayerIndex);

	// 해당 Layer가 없을 경우는 없어야한다..
	assert(instanceLayer != nullptr);

	// Render Data 재설정..
	m_Converter->ConvertInstanceData(meshData, renderData);

	// Layer가 변동이 없을 경우 처리하지 않는다..
	if (prev_LayerIndex == renderData->m_InstanceLayerIndex) return;

	// 해당 Render Data List에서 제거..
	instanceLayer->PopRenderData(renderData);

	// 현재 Layer가 비어있는 상태라면 Layer 제거..
	if (instanceLayer->m_InstanceList.empty())
	{
		m_Converter->DeleteInstanceLayer(renderData->m_InstanceLayerIndex);
	}

	// 재설정된 Layer 검색..
	instanceLayer = m_Converter->GetInstanceLayer(renderData->m_InstanceLayerIndex);

	// 현재 Layer에 Instance 추가..
	instanceLayer->PushRenderData(renderData);

	// 해당 Layer가 등록되어 있는지 확인..
	switch (renderData->m_ObjectData->ObjType)
	{
	case OBJECT_TYPE::BASE:
	case OBJECT_TYPE::SKINNING:
	case OBJECT_TYPE::TERRAIN:
	{
		// 해당 Layer가 등록되어 있는지 확인..
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
	// Render Data 변환..
	RenderData* renderData = (RenderData*)meshData->Render_Data;

	// UI Buffer 추출..
	UIRenderBuffer* ui_Buffer = renderData->m_UI;

	// 변경 전 Layer Index 저장..
	UINT prev_LayerIndex = ui_Buffer->m_BufferLayer;

	// 현재 변경 전 Layer 검색..
	UILayer* layer = m_Converter->GetUILayer(prev_LayerIndex);

	// 해당 Layer가 없을 경우는 없어야한다..
	assert(layer != nullptr);

	// Render Data 재설정..
	m_Converter->ConvertUIData(meshData, renderData);

	// Layer가 변동이 없을 경우 처리하지 않는다..
	if (prev_LayerIndex == ui_Buffer->m_BufferLayer) return;

	// 해당 Render Data List에서 제거..
	layer->PopRenderData(renderData);

	// 현재 Layer가 비어있는 상태라면 Layer 제거..
	if (layer->m_InstanceList.empty())
	{
		m_Converter->DeleteUILayer(ui_Buffer->m_BufferLayer);
	}

	// 재설정된 Layer 검색..
	layer = m_Converter->GetUILayer(ui_Buffer->m_BufferLayer);

	// 현재 Layer에 Instance 추가..
	layer->PushRenderData(renderData);

	// 해당 Layer가 등록되어 있는지 확인..
	FindLayer(m_UIRenderMeshList, layer);
}

void RenderManager::ChangeUnRenderData(MeshData* meshData)
{

}

void RenderManager::DeleteMeshRenderData(RenderData* renderData)
{
	// Render Data의 List 내에서의 Index..
	UINT layerIndex = renderData->m_InstanceLayerIndex;
	UINT renderDataIndex = renderData->m_ObjectData->ObjectIndex;

	// 해당 Layer 검색..
	InstanceLayer* layer = m_Converter->GetInstanceLayer(layerIndex);

	// 해당 Instance Layer에서 제거...
	layer->PopRenderData(renderData);

	// 해당 Render Data 제거..
	m_Converter->DeleteRenderData(renderDataIndex + 1);

	// 해당 Culling Render Data 제거..
	m_Culling->PopCullingMesh(renderData);

	// Render Buffer..
	InstanceRenderBuffer* renderBuffer = layer->m_Instance;

	// Layer 빈곳 체크..
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

	// 현재 Layer가 비어있는 상태라면 Layer 제거..
	if (layer->m_InstanceList.empty())
	{
		m_Converter->DeleteInstanceLayer(layerIndex);
	}
}

void RenderManager::DeleteUIRenderData(RenderData* renderData)
{
	// Render Data의 List 내에서의 Index..
	UINT layerIndex = renderData->m_UI->m_BufferLayer;
	UINT bufferIndex = renderData->m_UI->m_BufferIndex;
	UINT renderDataIndex = renderData->m_ObjectData->ObjectIndex;

	// 해당 Layer 검색..
	UILayer* layer = m_Converter->GetUILayer(layerIndex);

	// 해당 UI Render Buffer 제거..
	m_Converter->DeleteUI(bufferIndex);

	// 해당 Layer에서 제거..
	layer->PopRenderData(renderData);

	// 해당 Render Data 제거..
	m_Converter->DeleteRenderData(renderDataIndex + 1);

	// Layer 빈곳 체크..
	CheckEmptyLayer(m_UIRenderMeshList);

	// 현재 Layer가 비어있는 상태라면 Layer 제거..
	if (layer->m_InstanceList.empty())
	{
		m_Converter->DeleteUILayer(layerIndex);
	}
}

void RenderManager::DeleteUnRenderData(RenderData* renderData)
{
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

	// Index가 검색이 안되면 안된다..
	assert(index != -1);

	UINT renderDataIndex = renderData->m_ObjectData->ObjectIndex;

	// 해당 Render Data 제거..
	m_Converter->DeleteRenderData(renderDataIndex + 1);

	// 해당 Instance List에서 제거...
	m_UnRenderMeshList.erase(std::next(m_UnRenderMeshList.begin(), index));
}

template<typename Layer>
void RenderManager::CheckEmptyLayer(std::vector<Layer*>& layerList)
{
	for (int index = 0; index < layerList.size(); index++)
	{
		// 해당 Layer List Index 검색..
		if (layerList[index]->m_InstanceList.empty())
		{
			// 해당 Layer가 비어있다면 Layer 삭제..
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
		// 해당 Layer가 이미 등록되어 있다면 추가하지 않는다..
		if (instanceLayer == layer)
		{
			return;
		}
	}

	// 만약 Layer가 검색되지 않았다면 Layer List에 삽입..
	layerList.push_back(layer);

	// Layer 정렬..
	std::sort(layerList.begin(), layerList.end(), [](InstanceLayer* layer1, InstanceLayer* layer2) {return layer1->m_Instance->m_Type < layer2->m_Instance->m_Type; });
}

template<>
void RenderManager::FindLayer(std::vector<UILayer*>& layerList, UILayer* layer)
{
	for (UILayer* uiLayer : layerList)
	{
		// 해당 Layer가 이미 등록되어 있다면 추가하지 않는다..
		if (uiLayer == layer)
		{
			return;
		}
	}

	// 만약 Layer가 검색되지 않았다면 Layer List에 삽입..
	layerList.push_back(layer);

	// Layer 정렬..
	std::sort(layerList.begin(), layerList.end(), [](UILayer* layer1, UILayer* layer2) {return layer1->m_LayerIndex < layer2->m_LayerIndex; });
}

template<typename Layer>
void RenderManager::DeleteLayer(std::vector<Layer*>& layerList, Layer* layer)
{
	for (int index = 0; index < layerList.size(); index++)
	{
		// 해당 Layer List Index 검색..
		if (layerList[index] == layer)
		{
			// 해당 Layer가 비어있다면 Layer 삭제..
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
