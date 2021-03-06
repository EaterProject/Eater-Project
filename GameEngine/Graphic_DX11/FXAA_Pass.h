#pragma once
class FXAA_Pass : public RenderPassBase
{
public:
	FXAA_Pass();
	~FXAA_Pass();

public:
	void Start(int width, int height) override;
	void OnResize(int width, int height) override;
	void Release() override;

	void RenderUpdate();

private:
	VertexShader* m_Screen_VS;
	PixelShader* m_FXAA_PS;

	DrawBuffer* m_Screen_DB;

	RenderTexture* m_Output_RT;
	RenderTexture* m_Origin_RT;

private:
	ID3D11RenderTargetView* m_OutPut_RTV;
};

