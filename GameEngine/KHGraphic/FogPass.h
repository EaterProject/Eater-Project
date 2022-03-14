#pragma once
class FogPass : public RenderPassBase
{
public:
	FogPass();
	~FogPass();

public:
	void Create(int width, int height) override;
	void Start(int width, int height) override;

	void OnResize(int width, int height) override;
	void Release() override;

	void SetOption(RenderOption* renderOption) override;

	void RenderUpdate();

private:
	DrawBuffer* m_Screen_DB;
	
	VertexShader* m_Screen_VS;
	PixelShader* m_Fog_PS;

	RenderTexture* m_OutPut_RT;

	ID3D11RenderTargetView* m_OutPut_RTV;

	D3D11_VIEWPORT* m_Screen_VP;
};

