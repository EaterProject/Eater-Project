#pragma once

class SSAOPass : public RenderPassBase
{
public:
	SSAOPass();
	~SSAOPass();

public:
	void Create(int width, int height) override;
	void Start(int width, int height) override;
	void OnResize(int width, int height) override;
	void Release() override;
	
	void ApplyOption() override;

public:
	void RenderUpdate();
	void BlurRender(int blurCount);
	void BlurRender();

private:
	void SetOffsetVectors();
	void SetRandomVectorTexture();
	void SetFrustumFarCorners(int width, int height);

private:
	VertexShader* m_Ssao_VS;
	PixelShader* m_Ssao_PS;
	VertexShader* m_Blur_VS;
	PixelShader* m_Blur_PS;

	DrawBuffer* m_Ssao_DB;

	RenderTexture* m_Ssao_RT;
	RenderTexture* m_SsaoBlur_RT;

private:
	ID3D11RenderTargetView* m_Ssao_RTV;
	ID3D11RenderTargetView* m_SsaoBlur_RTV;

	ID3D11ShaderResourceView* m_Ssao_SRV;
	ID3D11ShaderResourceView* m_SsaoBlur_SRV;

	ID3D11RasterizerState* m_Solid_RS;

	D3D11_VIEWPORT* m_HalfScreen_VP;
};