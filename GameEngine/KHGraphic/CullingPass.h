#pragma once
#include "CullingData.h"

class CullingPass : public RenderPassBase
{
public:
	CullingPass();
	~CullingPass();

public:
	void Create(int width, int height) override;
	void Start(int width, int height) override;

	void OnResize(int width, int height) override;
	void InstanceResize(size_t& renderMaxCount, size_t& unRenderMaxCount) override;
		
	void Release() override;

public:
	void RenderOccluders();
	void OcclusionCullingQuery();
	void DrawStateUpdate();

	void FrustumCulling();

public:
	void PushCullingMesh(RenderData* meshData);
	void DeleteCullingMesh(RenderData* meshData);
	void CullingBufferCreate();

private:
	void MipMapResourceRelease();
	void MipMapResourceCreate();
	void MipMapCommandListReserve(const D3D11_TEXTURE2D_DESC* hizDesc);

private:
	VertexShader* m_Screen_VS;
	VertexShader* m_Depth_VS;

	PixelShader* m_HizMipMap_PS;
	PixelShader* m_Depth_PS;

	ComputeShader* m_HizCull_CS;

	DrawBuffer* m_Screen_DB;

	ID3D11RasterizerState* m_NoCull_RS;

	D3D11_VIEWPORT* m_Hiz_VP;

private:
	ID3D11CommandList* m_MipMap_CommandList;

	ID3D11Texture2D* m_Hiz_Buffer;
	ID3D11ShaderResourceView* m_Hiz_SRV;
	std::vector<ID3D11RenderTargetView*> m_MipMap_RTV;
	std::vector<ID3D11ShaderResourceView*> m_MipMap_SRV;

	ID3D11Buffer* m_Collider_Buffer;
	ID3D11Buffer* m_Culling_Buffer;
	ID3D11Buffer* m_ResultCopy_Buffer;

	ID3D11ShaderResourceView* m_Collider_SRV;
	ID3D11UnorderedAccessView* m_Culling_UAV;

	ID3D11DepthStencilView* m_HizDepth_DSV;

private:
	std::vector<RenderData*> CullingRenderMeshList;
	
	RenderData* m_RenderData;
	Cull::Frustum m_Frustum;
	BoundingSphere m_Sphere;
	Vector4 m_ColliderData;

	std::vector<Vector4> m_ColliderList;
	std::vector<float> m_ResultList;

	UINT m_MaxMipLevel = 0;
	UINT m_RenderCount = 0;
	UINT m_RenderMaxCount = 0;

	float m_Width;
	float m_Height;
};

