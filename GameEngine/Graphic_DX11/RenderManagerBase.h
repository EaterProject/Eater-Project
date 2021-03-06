#pragma once
#include "EngineData.h"

///
/// 2021/11/12 18:02
/// SeoKyuHwang
///
/// RenderManger Interface Class
///
/// - 모든 RenderPass를 관리하는 Class
/// - 외부 Game Engine에서 요청한 RenderPass를 관리하는 Class
/// - 내부적으로 Render방식에 따른 정렬은 해준다.
/// 

class MaterialRenderBuffer;

interface IRenderManager
{
public:
	friend class RenderDataConverter;

public:
	virtual void Create(int width, int height) abstract;
	virtual void Start(int width, int height) abstract;
	virtual void Release() abstract;

	// Resize Resource..
	virtual void OnResize(int width, int hegiht) abstract;

public:
	// Render Option Setting..
	virtual void RenderSetting() abstract;
	virtual void RenderSetting(RenderOption* renderOption) abstract;

	// Global Data Setting..
	virtual void SetGlobalData(GlobalData* globalData) abstract;

public:
	virtual void SetFullScreenBlur(bool enable, UINT blur_count) abstract;

public:
	// Environment Map Setting..
	virtual void SetSkyCube(TextureBuffer* resource) abstract;
	virtual void SetSkyLight(SkyLightBuffer* resource, UINT index) abstract;
	virtual void SetColorGradingBaseTexture(TextureBuffer* lut_resource) abstract;
	virtual void SetColorGradingBlendTexture(TextureBuffer* lut_resource) abstract;
	virtual void SetColorGradingBlendFactor(float blend_factor) abstract;

public:
	// Render Data Push..
	virtual void PushInstance(MeshData* instance) abstract;
	virtual void PushMesh(MeshBuffer* mesh) abstract;
	virtual void PushMaterial(MaterialBuffer* material) abstract;
	virtual void PushAnimation(AnimationBuffer* animation) abstract;

	// Render Data Change..
	virtual void PushChangeInstance(MeshData* instance) abstract;
	virtual void PushChangeMesh(MeshBuffer* mesh) abstract;
	virtual void PushChangeMaterial(MaterialBuffer* material) abstract;
	virtual void PushChangeAnimation(AnimationBuffer* animation) abstract;

	// Render Data Delete..
	virtual void DeleteInstance(MeshData* instance) abstract;
	virtual void DeleteMesh(MeshBuffer* mesh) abstract;
	virtual void DeleteMaterial(MaterialBuffer* material) abstract;
	virtual void DeleteAnimation(AnimationBuffer* animation) abstract;

	// Block Render Data Push..
	virtual void PushMaterialBlockInstance(MeshData* instance) abstract;

	// Block Render Data Pop..
	virtual void PopMaterialBlockInstance(MeshData* instance) abstract;

public:
	// Render Update..
	virtual void Render() abstract;

	virtual void* PickingRender(int x, int y) abstract;

private:
	virtual void RelocationLayer(MaterialRenderBuffer* material) abstract;
};