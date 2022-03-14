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
#include "DebugPass.h"

#include <algorithm>

RenderManager::RenderManager(ID3D11Graphic* graphic, IFactoryManager* factory, IGraphicResourceManager* resource, IShaderManager* shader)
{
	// Rendering Initialize..
	RenderPassBase::Initialize(graphic->GetContext(), factory, resource, shader);

	m_SwapChain = graphic->GetSwapChain();

	m_Deferred = new DeferredPass();
	m_Light = new LightPass();
	m_Environment = new EnvironmentPass();
	m_Shadow = new ShadowPass();
	m_SSAO = new SSAOPass();
	m_Alpha = new AlphaPass();
	m_OIT = new OITPass();
	m_FXAA = new FXAAPass();
	m_Bloom = new BloomPass();
	m_ToneMap = new ToneMapPass();
	m_Fog = new FogPass();
	m_Debug = new DebugPass();

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
	m_NowRenderOption.DebugOption		= m_RenderOption->DebugOption;
	m_NowRenderOption.RenderingOption	= m_RenderOption->RenderingOption;
	m_NowRenderOption.PostProcessOption	= m_RenderOption->PostProcessOption;

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
	m_NowRenderOption.DebugOption		= m_RenderOption->DebugOption;
	m_NowRenderOption.RenderingOption	= m_RenderOption->RenderingOption;
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

void RenderManager::AddMeshData(MeshData* meshData)
{
	// 현재 비어있는 상태이므로 추후 설정을 위해 Mesh Data 삽입..
	m_UnConvertMeshList.push(meshData);
}

void RenderManager::DeleteMeshData(MeshData* meshData)
{
	// Object Type에 따른 Render Mesh Data 제거..
	switch (meshData->ObjType)
	{
	case OBJECT_TYPE::BASE:
	case OBJECT_TYPE::SKINNING:
	case OBJECT_TYPE::TERRAIN:
		DeleteMeshRenderData(meshData);
		break;
	case OBJECT_TYPE::PARTICLE_SYSTEM:
		DeleteParticleRenderData(meshData);
		break;
	case OBJECT_TYPE::PARTICLE:
		break;
	default:
		DeleteUnRenderData(meshData);
		break;
	}
}

void RenderManager::ConvertMeshData()
{
	while (!m_UnConvertMeshList.empty())
	{
		MeshData* originMeshData = m_UnConvertMeshList.front();
		RenderData* newMeshData = new RenderData();

		// Renderer 전용 Data로 변환..
		newMeshData->ConvertData(originMeshData);

		// Object Type에 따른 리스트 삽입..
		switch (newMeshData->m_ObjectType)
		{
			case OBJECT_TYPE::BASE:
			case OBJECT_TYPE::SKINNING:
			case OBJECT_TYPE::TERRAIN:
				ConvertMeshRenderData(originMeshData, newMeshData);
				break;
			case OBJECT_TYPE::PARTICLE_SYSTEM:
				ConvertParticleRenderData(originMeshData, newMeshData);
				break;
			default:
				ConvertUnRenderData(originMeshData, newMeshData);
				break;
		}

		// 변환한 Mesh Data Pop..
		m_UnConvertMeshList.pop();
	}
}

void RenderManager::Render()
{
	// Rendering Option Setting..
	RenderSetting();

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

void RenderManager::ShadowRender()
{
	m_Shadow->BeginRender();

	for (int i = 0; i < m_RenderMeshList.size(); i++)
	{
		m_Shadow->RenderUpdate(m_RenderMeshList[i]);
	}
}

void RenderManager::DeferredRender()
{
	m_Deferred->BeginRender();

	for (int i = 0; i < m_RenderMeshList.size(); i++)
	{
		m_Deferred->RenderUpdate(m_RenderMeshList[i]);

		//for (int j = 0; j < m_RenderMeshList[i].size(); j++)
		//{
		//	m_Deferred->RenderUpdate(m_RenderMeshList[i][j]);
		//}
	}
}

void RenderManager::SSAORender()
{
	m_SSAO->RenderUpdate();
	m_SSAO->BlurRender(4);
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
		m_RenderData = m_ParticleMeshList[i];

		if (m_RenderData == nullptr) continue;

		m_Alpha->RenderUpdate(m_RenderData);
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
			m_MeshList = m_RenderMeshList[i];

			for (int j = 0; j < m_MeshList.size(); j++)
			{
				m_RenderData = m_MeshList[j];

				if (m_RenderData == nullptr) continue;

				switch (m_RenderData->m_ObjectType)
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
			m_RenderData = m_ParticleMeshList[i];

			if (m_RenderData == nullptr) continue;

			switch (m_RenderData->m_ObjectType)
			{
			case OBJECT_TYPE::PARTICLE_SYSTEM:
				m_Debug->RenderUpdate(m_RenderData);
				break;
			default:
				break;
			}
		}

		// UnRender Mesh Debugging..
		for (int i = 0; i < m_UnRenderMeshList.size(); i++)
		{
			m_RenderData = m_UnRenderMeshList[i];

			if (m_RenderData == nullptr) continue;

			switch (m_RenderData->m_ObjectType)
			{
			case OBJECT_TYPE::DEFALT:
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
	}
}

void RenderManager::EndRender()
{
	// Graphic State Reset..
	RenderPassBase::GraphicReset();

	// 최종 출력..
	m_SwapChain->Present(0, 0);
}

void RenderManager::ConvertMeshRenderData(MeshData* meshData, RenderData* renderData)
{
	bool addList = true;
	UINT meshIndex = renderData->m_MeshIndex;

	for (MeshIndexData& index : m_MeshIndexList)
	{
		if (meshIndex == index.m_MeshIndex)
		{
			// 해당 Mesh List Index 삽입..
			meshData->RenderListIndex = index.m_ListIndex;
			meshData->RenderMeshIndex = (UINT)m_RenderMeshList[index.m_ListIndex].size();

			// 해당 Instance List에 삽입..
			m_RenderMeshList[index.m_ListIndex].push_back(renderData);
			addList = false;
			break;
		}
	}

	if (addList)
	{
		// Index List 삽입..
		MeshIndexData indexData;
		indexData.m_MeshIndex = meshIndex;
		indexData.m_ListIndex = (UINT)m_RenderMeshList.size();
		m_MeshIndexList.push_back(indexData);

		// 해당 Mesh List Index 삽입..
		meshData->RenderListIndex = (UINT)m_RenderMeshList.size();
		meshData->RenderMeshIndex = 0;

		// 해당 Instance List에 삽입..
		std::vector<RenderData*> newMeshListData;
		newMeshListData.push_back(renderData);

		m_RenderMeshList.push_back(newMeshListData);
	}
}

void RenderManager::ConvertParticleRenderData(MeshData* meshData, RenderData* renderData)
{
	meshData->RenderMeshIndex = (UINT)m_ParticleMeshList.size();

	m_ParticleMeshList.push_back(renderData);
}

void RenderManager::ConvertUnRenderData(MeshData* meshData, RenderData* renderData)
{
	meshData->RenderMeshIndex = (UINT)m_UnRenderMeshList.size();
	
	m_UnRenderMeshList.push_back(renderData);
}

void RenderManager::DeleteMeshRenderData(MeshData* meshData)
{
	UINT listIndex = (UINT)meshData->RenderListIndex;
	UINT meshIndex = (UINT)meshData->RenderMeshIndex;

	// 해당 Mesh List 내의 Render Data 제거..
	SAFE_DELETE(m_RenderMeshList[listIndex][meshIndex]);
	m_RenderMeshList[listIndex].erase(std::next(m_RenderMeshList[listIndex].begin(), meshIndex));

	MeshData* originData = nullptr;
	for (int i = meshIndex; i < m_RenderMeshList[listIndex].size(); i++)
	{
		originData = (MeshData*)m_RenderMeshList[listIndex][i]->m_OriginData;
		originData->RenderMeshIndex--;
	}

	if (m_RenderMeshList[listIndex].empty())
	{
		// 해당 Mesh List가 비어있다면 해당 Mesh List 제거..
		m_RenderMeshList.erase(std::next(m_RenderMeshList.begin(), listIndex));

		// 해당 Mesh에 해당하는 Index Data 제거..
		m_MeshIndexList.erase(std::next(m_MeshIndexList.begin(), listIndex));

		for (int i = listIndex; i < m_RenderMeshList.size(); i++)
		{
			// Index List의 List Index 변경..
			m_MeshIndexList[i].m_ListIndex--;

			for (int j = 0; j < m_RenderMeshList[i].size(); j++)
			{
				originData = (MeshData*)m_RenderMeshList[i][j]->m_OriginData;
				originData->RenderListIndex--;
			}
		}
	}
}

void RenderManager::DeleteParticleRenderData(MeshData* meshData)
{
	UINT listIndex = (UINT)meshData->RenderListIndex;
	UINT meshIndex = (UINT)meshData->RenderMeshIndex;

	// 해당 Mesh List 내의 Render Data 제거..
	SAFE_DELETE(m_ParticleMeshList[meshIndex]);
	m_ParticleMeshList.erase(std::next(m_ParticleMeshList.begin(), meshIndex));

	MeshData* originData = nullptr;
	for (int i = meshIndex; i < m_ParticleMeshList.size(); i++)
	{
		originData = (MeshData*)m_ParticleMeshList[i]->m_OriginData;
		originData->RenderMeshIndex--;
	}
}

void RenderManager::DeleteUnRenderData(MeshData* meshData)
{
	UINT listIndex = (UINT)meshData->RenderListIndex;
	UINT meshIndex = (UINT)meshData->RenderMeshIndex;

	// 해당 Mesh List 내의 Render Data 제거..
	SAFE_DELETE(m_UnRenderMeshList[meshIndex]);
	m_UnRenderMeshList.erase(std::next(m_UnRenderMeshList.begin(), meshIndex));

	MeshData* originData = nullptr;
	for (int i = meshIndex; i < m_UnRenderMeshList.size(); i++)
	{
		originData = (MeshData*)m_UnRenderMeshList[i]->m_OriginData;
		originData->RenderMeshIndex--;
	}
}