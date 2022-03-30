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

private:
	ID3D11DepthStencilView* m_DefaltDSV;
	ID3D11DepthStencilState* m_DefaltDSS;
	ID3D11RasterizerState* m_SolidRS;

	D3D11_VIEWPORT* m_ScreenVP;

	XNA::Frustum m_Frustum;
};

