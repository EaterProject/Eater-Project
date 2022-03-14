#pragma once

struct SamplingData
{
	// Down Sampling View Port Size
	float SamplingWidth;	
	float SamplingHeight;

	// Render Target View Port Size
	float OriginWidth;
	float OriginHeight;

	// Down Sampling View Port Offset
	float HalfOffsetWidth;
	float HalfOffsetHeight;

	// Render Target One Texel Size
	float TexelWidth;
	float TexelHeight;
};

class BloomPass : public RenderPassBase
{
public:
	BloomPass();
	~BloomPass();

public:
	void Create(int width, int height) override;
	void Start(int width, int height) override;
	void OnResize(int width, int height) override;
	void Release() override;

	void SetOption(UINT renderOption) override;
	void RenderUpdate();

	void BrightRender();
	void DownSampling(ID3D11RenderTargetView* rtv, ID3D11ShaderResourceView* originSRV, const SamplingData& downScreen);
	void UpSampling(ID3D11RenderTargetView* rtv, ID3D11ShaderResourceView* originSRV, ID3D11ShaderResourceView* downSRV, const SamplingData& downScreen);
	void GaussianBlur(ID3D11RenderTargetView* rtv[2], ID3D11ShaderResourceView* srv[2], const SamplingData& downScreen);

private:
	void SetConstantBuffer();
	void SetSamplingViewPort(const SamplingData& downScreen);
	void SetBlurViewPort(const SamplingData& downScreen);

private:
	VertexShader* m_Screen_VS;
	PixelShader* m_BloomBright_PS;
	PixelShader* m_BloomDownSampling_PS;
	PixelShader* m_BloomUpSampling_PS;
	PixelShader* m_BloomBlur_PS;

	DrawBuffer* m_Screen_DB;

	RenderTexture* m_Origin_RT;
	RenderTexture* m_Brightx4_RT1;
	RenderTexture* m_Brightx4_RT2;
	RenderTexture* m_Downx24_RT1;
	RenderTexture* m_Downx24_RT2;
	RenderTexture* m_Downx144_RT1;
	RenderTexture* m_Downx144_RT2;

	ID3D11ShaderResourceView* m_Brightx4_SRV1;
	ID3D11ShaderResourceView* m_Brightx4_SRV2;
	ID3D11ShaderResourceView* m_Downx24_SRV1;
	ID3D11ShaderResourceView* m_Downx24_SRV2;
	ID3D11ShaderResourceView* m_Downx144_SRV1;
	ID3D11ShaderResourceView* m_Downx144_SRV2;

	ID3D11RenderTargetView* m_Brightx4_RTV1;
	ID3D11RenderTargetView* m_Brightx4_RTV2;
	ID3D11RenderTargetView* m_Downx24_RTV1;
	ID3D11RenderTargetView* m_Downx24_RTV2;
	ID3D11RenderTargetView* m_Downx144_RTV1;
	ID3D11RenderTargetView* m_Downx144_RTV2;

	ID3D11UnorderedAccessView* m_Brightx4_UAV1;
	ID3D11UnorderedAccessView* m_Brightx4_UAV2;
	ID3D11UnorderedAccessView* m_Downx24_UAV1;
	ID3D11UnorderedAccessView* m_Downx24_UAV2;
	ID3D11UnorderedAccessView* m_Downx144_UAV1;
	ID3D11UnorderedAccessView* m_Downx144_UAV2;

	D3D11_VIEWPORT* m_DownSample_VP;

	SamplingData m_DownSampleData[3];

	float m_Down4x4;
	float m_Down24x24;
	float m_Down144x144;

	UINT m_ThreadGroupCountX;
	UINT m_ThreadGroupCountY;
};

