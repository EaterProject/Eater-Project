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
	
	void SetOption(RenderOption* renderOption) override;

public:
	void RenderUpdate();
	void BlurRender(int blurCount);
	void BlurRender();

private:
	void SetOffsetVectors();
	void SetRandomVectorTexture();
	void SetFrustumFarCorners(int width, int height);

private:
	DrawBuffer* m_SsaoDB;

	VertexShader* m_SsaoVS;
	PixelShader* m_SsaoPS;
	VertexShader* m_BlurVS;
	PixelShader* m_BlurPS;

	RenderTexture* m_SsaoRT;
	RenderTexture* m_SsaoBlurRT;

	ID3D11RenderTargetView* m_SsaoRTV;
	ID3D11RenderTargetView* m_SsaoBlurRTV;

	ID3D11ShaderResourceView* m_SsaoSRV;
	ID3D11ShaderResourceView* m_SsaoBlurSRV;

	D3D11_VIEWPORT* m_HalfScreenVP;
};