#pragma once
class ToneMapPass : public RenderPassBase
{
public:
	ToneMapPass();
	~ToneMapPass();

public:
	void Create(int width, int height) override;
	void Start(int width, int height) override;
	void OnResize(int width, int height) override;
	void Release() override;
	
	void SetOption(RenderOption* renderOption) override;

	void RenderUpdate();
	void SetConstantBuffer();

private:
	VertexShader* m_Screen_VS;
	PixelShader* m_ToneMapping_PS;

	DrawBuffer* m_Screen_DB;

	RenderTexture* m_Origin_RT;
	RenderTexture* m_Bloom_RT;
	RenderTexture* m_OutPut_RT;

	ID3D11RenderTargetView* m_OutPut_RTV;

	D3D11_VIEWPORT* m_Screen_VP;
};

