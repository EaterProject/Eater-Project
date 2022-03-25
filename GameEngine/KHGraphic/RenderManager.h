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
	void SetEnvironmentMap(bool enable) override;

public:
	void PushInstance(MeshData* instance) override;
	void PushMesh(MeshBuffer* mesh) override;
	void PushMaterial(MaterialBuffer* material) override;

	void PushChangeInstance(MeshData* instance) override;
	void PushChangeMesh(MeshBuffer* mesh) override;
	void PushChangeMaterial(MaterialBuffer* material) override;

	void DeleteInstance(MeshData* instance) override;
	void DeleteMesh(MeshBuffer* mesh) override;
	void DeleteMaterial(MaterialBuffer* material) override;

	void Render() override;
	void PickingRender(int x, int y) override;

private:
	void RenderSetting();

	void ConvertRenderData();

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
	void ConvertPushInstance();
	void ConvertChangeInstance();

	void PushMeshRenderData(RenderData* renderData);
	void PushParticleRenderData(RenderData* renderData);
	void PushUnRenderData(RenderData* renderData);

	void ChangeMeshRenderData(MeshData* meshData);
	void ChangeParticleRenderData(MeshData* meshData);
	void ChangeUnRenderData(MeshData* meshData);

	void DeleteMeshRenderData(MeshData* meshData);
	void DeleteParticleRenderData(MeshData* meshData);
	void DeleteUnRenderData(MeshData* meshData);

	void CheckInstanceLayer(std::vector<InstanceLayer*>& layerList);
	void FindInstanceLayer(std::vector<InstanceLayer*>& layerList, InstanceLayer* layer);

private:
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_SwapChain;

private:
	std::queue<MeshData*> m_PushInstanceList;
	std::queue<MeshData*> m_ChangeInstanceList;

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
	PickingPass*		m_Picking;
	DebugPass*			m_Debug;
};
