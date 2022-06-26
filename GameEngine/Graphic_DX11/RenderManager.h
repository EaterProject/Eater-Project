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
class Blur_Pass;
class Bloom_Pass;
class Fog_Pass;
class Culling_Pass;
class Picking_Pass;
class OutLine_Pass;
class Combine_Pass;
class Debug_Pass;

class RenderData;
class InstanceLayer;
class UILayer;

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
	void SetSkyLight(SkyLightBuffer* resource, UINT index) override;
	void SetColorGradingBaseTexture(TextureBuffer* lut_resource) override;
	void SetColorGradingBlendTexture(TextureBuffer* lut_resource) override;
	void SetColorGradingFactor(float factor) override;

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

	void PushMaterialBlockInstance(MeshData* instance) override;
	void PopMaterialBlockInstance(MeshData* instance) override;

	void Render() override;
	void* PickingRender(int x, int y) override;

private:
	void RelocationLayer(MaterialRenderBuffer* material) override;

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
	void PushMaterialBlockInstance();							// 현재 프레임 진행중 추가된 Material Block Instance 삽입..
	
	void PushMeshRenderData(RenderData* renderData);			// Mesh Render Data 삽입..
	void PushUIRenderData(RenderData* renderData);				// UI Mesh Render Data 삽입..
	void PushUnRenderData(RenderData* renderData);				// Un Render Data 삽입..

	void ChangeMeshRenderData(MeshData* meshData);				// Mesh Render Data 변환..
	void ChangeUIRenderData(MeshData* meshData);				// UI Mesh Render Data 변환..
	void ChangeUnRenderData(MeshData* meshData);				// Un Render Data 변환..

	void DeleteMeshRenderData(RenderData* renderData);			// Mesh Render Data 제거..
	void DeleteUIRenderData(RenderData* renderData);			// UI Mesh Render Data 제거..
	void DeleteUnRenderData(RenderData* renderData);			// Un Render Data 제거..

	template<typename Layer>
	void CheckEmptyLayer(std::vector<Layer*>& layerList);				// 비어있는 Layer 검사 및 제거..
	
	template<typename Layer>
	void FindLayer(std::vector<Layer*>& layerList, Layer* layer);		// 해당 Layer 검색 및 추가..

	template<typename Layer>
	void DeleteLayer(std::vector<Layer*>& layerList, Layer* layer);		// 해당 Layer 검색 및 삭제..

	bool SortDistance(RenderData* obj1, RenderData* obj2);

private:
	std::queue<MeshData*> m_PushInstanceList;
	std::queue<MeshData*> m_ChangeInstanceList;
	std::queue<MeshData*> m_BlockInstanceList;

	int m_BlockQueueMax = 0;
	int m_OpacityBlockSize = 0;
	int m_TransparencyBlockSize = 0;

	std::vector<RenderData*> m_OpacityBlockList;
	std::vector<RenderData*> m_TransparencyBlockList;

	std::vector<InstanceLayer*> m_OpacityMeshList;
	std::vector<InstanceLayer*> m_TransparencyMeshList;
	std::vector<UILayer*> m_UIRenderMeshList;
	std::vector<RenderData*> m_UnRenderMeshList;

	std::vector<RenderPassBase*> m_RenderPassList;

	Eater::Delegate<int, int> SetResizeFunction;
	Eater::Delegate<int, int> OnResizeFunction;
	Eater::Delegate<size_t&, size_t&> InstanceResizeFunction;
	Eater::Delegate<> ApplyOptionFunction;
	Eater::Delegate<> PreUpdateFunction;

	InstanceLayer* m_InstanceLayer;
	UILayer* m_UILayer;
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
	Blur_Pass*			m_Blur;
	Bloom_Pass*			m_Bloom;
	Fog_Pass*			m_Fog;
	Culling_Pass*		m_Culling;
	Picking_Pass*		m_Picking;
	OutLine_Pass*		m_OutLine;
	Combine_Pass*		m_Combine;
	Debug_Pass*			m_Debug;

private:
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_SwapChain;
};

template<typename Layer>
inline void RenderManager::FindLayer(std::vector<Layer*>& layerList, Layer* layer) {}

template<>
inline void RenderManager::FindLayer(std::vector<InstanceLayer*>& layerList, InstanceLayer* layer);

template<>
inline void RenderManager::FindLayer(std::vector<UILayer*>& layerList, UILayer* layer);
