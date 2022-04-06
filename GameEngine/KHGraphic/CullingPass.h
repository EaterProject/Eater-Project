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
	void Release() override;

public:
	void RenderOccluders();
	void OcclusionCullingQuery();
	void DrawStateUpdate();

	bool FrustumCulling(const RenderData* meshData);

public:
	void PushCullingMesh(RenderData* meshData);
	void DeleteCullingMesh(RenderData* meshData);

private:
	void MipMapResourceRelease();
	void MipMapResourceCreate(int width, int height);
	void MipMapCommandListReserve(int width, int height, const D3D11_TEXTURE2D_DESC* hizDesc);
	void CullingBufferCreate();

private:
	std::vector<RenderData*> CullingRenderMeshList;

	Cull::Frustum m_Frustum;
	RenderData* m_RenderData;
	BoundingSphere m_Sphere;
	Vector4 m_ColliderData;

	VertexShader* m_Screen_VS;
	PixelShader* m_HizMipMap_PS;

	VertexShader* m_Depth_VS;
	PixelShader* m_Depth_PS;

	ComputeShader* m_HizCull_CS;

	DrawBuffer* m_Screen_DB;

	ID3D11RasterizerState* m_NoCull_RS;

	D3D11_VIEWPORT* m_Screen_VP;




	ID3D11CommandList* m_MipMap_CommandList;

	UINT m_MaxMipLevel;

	ID3D11Texture2D* m_Hiz_Buffer;
	ID3D11ShaderResourceView* m_Hiz_SRV;
	std::vector<ID3D11RenderTargetView*> m_MipMap_RTV;
	std::vector<ID3D11ShaderResourceView*> m_MipMap_SRV;
	std::vector<Vector4> m_ColliderList;
	std::vector<float> m_ResultList;

	ID3D11Buffer* m_Collider_Buffer;
	ID3D11Buffer* m_Culling_Buffer;
	ID3D11Buffer* m_ResultCopy_Buffer;

	ID3D11ShaderResourceView* m_Collider_SRV;
	ID3D11UnorderedAccessView* m_Culling_UAV;

	ID3D11DepthStencilView* m_HizDepth_DSV;

	UINT m_RenderCount = 1160;

	float m_Width;
	float m_Height;
};

