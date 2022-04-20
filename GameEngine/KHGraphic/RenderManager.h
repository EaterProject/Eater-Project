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

	void RenderSetting();				// ���� �����ӿ� ������ Render Option ����..

	void ConvertRenderData();			// ���� �����ӿ� �߰� �� ����� Render Data ��ȯ..

	void SelectRenderData();			// ���� �����ӿ� Rendering �� Mesh ���� �۾�..

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
	void ConvertPushInstance();								// ���� ������ ������ �߰��� Instance ��ȯ..
	void ConvertChangeInstance();							// ���� ������ ������ ����� Instance ��ȯ..

	void PushMeshRenderData(RenderData* renderData);		// Static Mesh Render List ����..
	void PushParticleRenderData(RenderData* renderData);	// Transparency Mesh Render List ����..
	void PushUnRenderData(RenderData* renderData);			// Un Render List ����..

	void ChangeMeshRenderData(MeshData* meshData);			// Static Mesh Render Data ��ȯ..
	void ChangeParticleRenderData(MeshData* meshData);		// Transparency Mesh Render Data ��ȯ..
	void ChangeUnRenderData(MeshData* meshData);			// Un Render Data ��ȯ..

	void DeleteMeshRenderData(MeshData* meshData);			// Static Mesh Render Data ����..
	void DeleteParticleRenderData(MeshData* meshData);		// Transparency Mesh Render Data ����..
	void DeleteUnRenderData(MeshData* meshData);			// Un Render Data ����..

	void CheckInstanceLayer(std::vector<InstanceLayer*>& layerList);						// ����ִ� Insatnce Layer �˻� �� ����..
	void FindInstanceLayer(std::vector<InstanceLayer*>& layerList, InstanceLayer* layer);	// �ش� Instance Layer �˻� �� �߰�..

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
