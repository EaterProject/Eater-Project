#pragma once
class FXAAPass : public RenderPassBase
{
public:
	FXAAPass();
	~FXAAPass();

public:
	void Create(int width, int height) override;
	void Start(int width, int height) override;
	void OnResize(int width, int height) override;
	void Release() override;

	void SetOption(RenderOption* renderOption) override;

	void RenderUpdate();

private:
	VertexShader* m_Screen_VS;
	PixelShader* m_FXAA_PS;

	DrawBuffer* m_Screen_DB;

	RenderTexture* m_Origin_RT;

private:
	ID3D11RenderTargetView* m_OutPut_RTV;
};

