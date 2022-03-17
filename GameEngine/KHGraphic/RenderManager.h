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
class DebugPass;

class RenderData;
class MeshIndexData;

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

	void PushInstance(MeshData* meshData) override;
	void PushMaterial(MaterialBuffer* material) override;
	void PushMesh(MeshBuffer* mesh) override;

	void AddChangeMeshData(MeshData* meshData) override;
	void AddChangeMaterialData(MaterialBuffer* materialData) override;
	void DeleteMeshData(MeshData* mesh) override;

	void Render() override;

private:
	void RenderSetting();
	void ConvertMeshData();
	void ChangeMeshData();

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
	void ConvertMeshRenderData(MeshData* meshData, RenderData* renderData);
	void ConvertParticleRenderData(MeshData* meshData, RenderData* renderData);
	void ConvertUnRenderData(MeshData* meshData, RenderData* renderData);

	void ChangeMeshRenderData(MeshData* meshData);
	void ChangeParticleRenderData(MeshData* meshData);
	void ChangeUnRenderData(MeshData* meshData);

	void DeleteMeshRenderData(MeshData* meshData);
	void DeleteParticleRenderData(MeshData* meshData);
	void DeleteUnRenderData(MeshData* meshData);

private:
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_SwapChain;

private:
	std::queue<MeshData*> m_UnConvertMeshList;
	std::queue<MeshData*> m_ChangeMeshList;
	std::queue<MaterialBuffer*> m_ChangeMaterialList;

	std::vector<MeshIndexData> m_MeshIndexList;

	std::vector<std::vector<RenderData*>> m_RenderMeshList;
	std::vector<RenderData*> m_ParticleMeshList;
	std::vector<RenderData*> m_UnRenderMeshList;

	std::vector<RenderPassBase*> m_RenderPassList;

	std::vector<RenderData*> m_MeshList;
	RenderData* m_RenderData;

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
	DebugPass*			m_Debug;
};
