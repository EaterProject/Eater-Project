#pragma once
class OutLinePass : public RenderPassBase
{
public:
	OutLinePass();
	~OutLinePass();

public:
	void Create(int width, int height) override;
	void Start(int width, int height) override;

	void OnResize(int width, int height) override;

	void Release() override;

	void RenderUpdate();

private:
	void BeginMask();
	void BeginOutLine();

private:
	VertexShader* m_MeshOrigin_VS;
	VertexShader* m_SkinMeshOrigin_VS;
	VertexShader* m_MeshOutLine_VS;
	VertexShader* m_SkinMeshOutLine_VS;
	PixelShader* m_OutLine_PS;

	RenderTexture* m_Origin_RT;
	RenderTexture* m_OutLine_RT;

private:
	ID3D11DepthStencilView* m_OutLine_DSV;

	ID3D11RenderTargetView* m_Origin_RTV;
	ID3D11RenderTargetView* m_OutLine_RTV;

	ID3D11DepthStencilState* m_Mask_DSS;
	ID3D11DepthStencilState* m_OutLine_DSS;
	ID3D11RasterizerState* m_Solid_RS;

	D3D11_VIEWPORT* m_Screen_VP;
};

