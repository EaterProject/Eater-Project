#pragma once
#include "RenderManagerBase.h"
#include "Utility.h"

class RenderPassBase;
class Shadow_Pass;
class Deferred_Pass;
class Light_Pass;
class Sky_Pass;
class UI_Pass;
class SSAO_Pass;
class Alpha_Pass;
class OIT_Pass;
class FXAA_Pass;
class Bloom_Pass;
class Fog_Pass;
class Culling_Pass;
class Picking_Pass;
class OutLine_Pass;
class Combine_Pass;
class Debug_Pass;

class RenderData;
class InstanceLayer;

class RenderManager : public IRenderManager
{
public:
	RenderManager(ID3D11Graphic* graphic, IFactoryManager* factory, IGraphicResourceManager* resource, IShaderManager* shader, IRenderDataConverter* converter, RenderOption* renderOption);
	~RenderManager();

public:
	void Create(int width, int height) override;
	void Start(int width, int height) override;
	void OnResize(int width, int height) override;

	void Release() override;

	void RenderSetting() override;
	void RenderSetting(RenderOption* renderOption) override;
	void SetGlobalData(GlobalData* globalData) override;

public:
	void SetSkyCube(TextureBuffer* resource) override;
	void SetSkyLight(SkyLightBuffer* resource) override;

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
	void PreUpdate();

	void InstanceResize();

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
	template<typename T>
	void PushFunction(T* pass);
	void ConvertPushInstance();									// 현재 프레임 진행중 추가된 Instance 변환..
	void ConvertChangeInstance();								// 현재 프레임 진행중 변경된 Instance 변환..

	void PushOpacityRenderData(RenderData* renderData);			// Opacity Mesh Render Data 삽입..
	void PushTransparencyRenderData(RenderData* renderData);	// Transparency Mesh Render Data 삽입..
	void PushUIRenderData(RenderData* renderData);				// UI Mesh Render Data 삽입..
	void PushUnRenderData(RenderData* renderData);				// Un Render Data 삽입..

	void ChangeOpacityRenderData(MeshData* meshData);			// Opacity Mesh Render Data 변환..
	void ChangeTransparencyRenderData(MeshData* meshData);		// Transparency Mesh Render Data 변환..
	void ChangeUIRenderData(MeshData* meshData);				// UI Mesh Render Data 변환..
	void ChangeUnRenderData(MeshData* meshData);				// Un Render Data 변환..

	void DeleteOpacityRenderData(MeshData* meshData);			// Opacity Mesh Render Data 제거..
	void DeleteTransparencyRenderData(MeshData* meshData);		// Transparency Mesh Render Data 제거..
	void DeleteUIRenderData(MeshData* meshData);				// UI Mesh Render Data 제거..
	void DeleteUnRenderData(MeshData* meshData);				// Un Render Data 제거..

	void CheckInstanceLayer(std::vector<InstanceLayer*>& layerList);						// 비어있는 Insatnce Layer 검사 및 제거..
	void FindInstanceLayer(std::vector<InstanceLayer*>& layerList, InstanceLayer* layer);	// 해당 Instance Layer 검색 및 추가..

	bool SortLayer(InstanceLayer* layer1, InstanceLayer* layer2);
	bool SortDistance(RenderData* obj1, RenderData* obj2);

private:
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_SwapChain;

private:
	std::queue<MeshData*> m_PushInstanceList;
	std::queue<MeshData*> m_ChangeInstanceList;

	std::queue<RenderData*> m_RenderQueue;

	std::vector<InstanceLayer*> m_OpacityMeshList;
	std::vector<InstanceLayer*> m_TransparencyMeshList;
	std::vector<RenderData*> m_UIRenderMeshList;
	std::vector<RenderData*> m_UnRenderMeshList;

	std::vector<RenderPassBase*> m_RenderPassList;

	Eater::Delegate<int, int> OnResizeFunction;
	Eater::Delegate<size_t&, size_t&> InstanceResizeFunction;
	Eater::Delegate<int, int> SetResizeFunction;
	Eater::Delegate<> ApplyOptionFunction;
	Eater::Delegate<> PreUpdateFunction;

	InstanceLayer* m_InstanceLayer;
	RenderData* m_RenderData;

	IRenderDataConverter* m_Converter;

	RenderOption* m_RenderOption;
	RenderOption m_NowRenderOption;

	Deferred_Pass*		m_Deferred;
	Light_Pass*			m_Light;
	Sky_Pass*			m_Sky;
	UI_Pass*			m_UI;
	Shadow_Pass*		m_Shadow;
	SSAO_Pass*			m_SSAO;
	Alpha_Pass*			m_Alpha;
	OIT_Pass*			m_OIT;
	FXAA_Pass*			m_FXAA;
	Bloom_Pass*			m_Bloom;
	Fog_Pass*			m_Fog;
	Culling_Pass*		m_Culling;
	Picking_Pass*		m_Picking;
	OutLine_Pass*		m_OutLine;
	Combine_Pass*		m_Combine;
	Debug_Pass*			m_Debug;
};
