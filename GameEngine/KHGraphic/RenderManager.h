#pragma once
#include "RenderManagerBase.h"

class RenderPassBase;
class ShadowPass;
class DeferredPass;
class LightPass;
class EnvironmentPass;
class SSAOPass;
class AlphaPass;
class OITPass;
class FXAAPass;
class BloomPass;
class ToneMapPass;
class FogPass;
class CullingPass;
class PickingPass;
class DebugPass;

class RenderData;
class InstanceLayer;
class RenderDataConverter;

class RenderManager : public IRenderManager
{
public:
	RenderManager(ID3D11Graphic* graphic, IFactoryManager* factory, IGraphicResourceManager* resource, IShaderManager* shader);
	~RenderManager();

public:
	void Create(int width, int height) override;
	void Start(int width, int height) override;
	void OnResize(int width, int height) override;
	void Release() override;

	void RenderSetting(RenderOption* renderOption) override;
	void SetGlobalData(GlobalData* globalData) override;

public:
	void SetEnvironmentMap(EnvironmentBuffer* resource) override;

public:
	void PushInstance(MeshData* instance) override;
	void PushMesh(MeshBuffer* mesh) override;
	void PushMaterial(MaterialBuffer* material) override;
	void PushAnimation(AnimationBuffer* animation) override;
	
	void PushChangeInstance(MeshData* instance) override;
	void PushChangeMesh(MeshBuffer* mesh) override;
	void PushChangeMaterial(MaterialBuffer* material) override;
	void PushChangeAnimation(AnimationBuffer* animation) override;

	void DeleteInstance(MeshData* instance) override;
	void DeleteMesh(MeshBuffer* mesh) override;
	void DeleteMaterial(MaterialBuffer* material) override;
	void DeleteAnimation(AnimationBuffer* animation) override;

	void Render() override;
	void* PickingRender(int x, int y) override;

private:
	void InstanceResize();

	void RenderSetting();				// 현재 프레임에 설정된 Render Option 적용..

	void ConvertRenderData();			// 현재 프레임에 추가 및 변경된 Render Data 변환..

	void SelectRenderData();			// 현재 프레임에 Rendering 될 Mesh 선별 작업..

	void ShadowRender();
	void DeferredRender();
	void SSAORender();
	void LightRender();
	void EnvironmentRender();
	void AlphaRender();
	void PostProcessingRender();
	void UIRender();
	void DebugRender();
	void EndRender();

private:
	void ConvertPushInstance();								// 현재 프레임 진행중 추가된 Instance 변환..
	void ConvertChangeInstance();							// 현재 프레임 진행중 변경된 Instance 변환..

	void PushMeshRenderData(RenderData* renderData);		// Static Mesh Render List 삽입..
	void PushParticleRenderData(RenderData* renderData);	// Transparency Mesh Render List 삽입..
	void PushUnRenderData(RenderData* renderData);			// Un Render List 삽입..

	void ChangeMeshRenderData(MeshData* meshData);			// Static Mesh Render Data 변환..
	void ChangeParticleRenderData(MeshData* meshData);		// Transparency Mesh Render Data 변환..
	void ChangeUnRenderData(MeshData* meshData);			// Un Render Data 변환..

	void DeleteMeshRenderData(MeshData* meshData);			// Static Mesh Render Data 제거..
	void DeleteParticleRenderData(MeshData* meshData);		// Transparency Mesh Render Data 제거..
	void DeleteUnRenderData(MeshData* meshData);			// Un Render Data 제거..

	void CheckInstanceLayer(std::vector<InstanceLayer*>& layerList);						// 비어있는 Insatnce Layer 검사 및 제거..
	void FindInstanceLayer(std::vector<InstanceLayer*>& layerList, InstanceLayer* layer);	// 해당 Instance Layer 검색 및 추가..

	bool SortDistance(RenderData* obj1, RenderData* obj2);

private:
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_SwapChain;

private:
	std::queue<MeshData*> m_PushInstanceList;
	std::queue<MeshData*> m_ChangeInstanceList;

	std::queue<RenderData*> m_RenderQueue;

	std::vector<InstanceLayer*> m_RenderMeshList;
	std::vector<InstanceLayer*> m_ParticleMeshList;
	std::vector<RenderData*> m_UnRenderMeshList;

	std::vector<RenderPassBase*> m_RenderPassList;

	InstanceLayer* m_InstanceLayer;
	RenderData* m_RenderData;

	RenderDataConverter* m_Converter;

	RenderOption* m_RenderOption;
	RenderOption m_NowRenderOption;

	DeferredPass*		m_Deferred;
	LightPass*			m_Light;
	EnvironmentPass*	m_Environment;
	ShadowPass*			m_Shadow;
	SSAOPass*			m_SSAO;
	AlphaPass*			m_Alpha;
	OITPass*			m_OIT;
	FXAAPass*			m_FXAA;
	BloomPass*			m_Bloom;
	ToneMapPass*		m_ToneMap;
	FogPass*			m_Fog;
	CullingPass*		m_Culling;
	PickingPass*		m_Picking;
	DebugPass*			m_Debug;
};
