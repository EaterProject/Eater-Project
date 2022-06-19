#pragma once

class SSAO_Pass : public RenderPassBase
{
public:
	SSAO_Pass();
	~SSAO_Pass();

public:
	void Create(int width, int height) override;
	void Start(int width, int height) override;
	void OnResize(int width, int height) override;
	void Release() override;
	
	void ApplyOption() override;

public:
	void RenderUpdate();
	void BlurRender_PS();
	void BlurRender_CS();

private:
	void SetOffsetVectors();
	void SetRandomVectorTexture();
	void SetFrustumFarCorners(int width, int height);

private:
	void SetShaderList();
	void SetShaderResourceView();

private:
	VertexShader* m_Ssao_VS;
	PixelShader* m_Ssao_PS;
	VertexShader* m_Blur_VS;
	PixelShader* m_Blur_PS;
	ComputeShader* m_BlurHorizon_CS;
	ComputeShader* m_BlurVertical_CS;

	DrawBuffer* m_Ssao_DB;

	RenderTexture* m_Ssao_RT;
	RenderTexture* m_SsaoBlur_RT;

	UINT m_BlurCount;

	UINT m_Width;
	UINT m_Height;

	UINT m_NumGroupsX;
	UINT m_NumGroupsY;

private:
	ID3D11RenderTargetView* m_Ssao_RTV;
	ID3D11RenderTargetView* m_SsaoBlur_RTV;

	ID3D11ShaderResourceView* m_Ssao_SRV;
	ID3D11ShaderResourceView* m_SsaoBlur_SRV;

	ID3D11UnorderedAccessView* m_Ssao_UAV;
	ID3D11UnorderedAccessView* m_SsaoBlur_UAV;

	ID3D11ShaderResourceView* m_NullSRV = nullptr;
	ID3D11UnorderedAccessView* m_NullUAV = nullptr;

	ID3D11RasterizerState* m_Solid_RS;

	D3D11_VIEWPORT* m_HalfScreen_VP;
};