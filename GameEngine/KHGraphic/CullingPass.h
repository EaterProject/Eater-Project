#pragma once
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
	bool FrustumCulling(const RenderData* meshData);
	void ResetCount();

private:
	void MipMap_CommandList_Reserve(int width, int height);

private:
	ID3D11CommandList* m_MipMapCommandList;

	ID3D11DepthStencilView* m_DefaltDSV;
	ID3D11DepthStencilState* m_DefaltDSS;
	ID3D11RasterizerState* m_SolidRS;

	int m_CullingCount = 0;
	int m_RenderCount = 0;

	D3D11_VIEWPORT* m_ScreenVP;
};

