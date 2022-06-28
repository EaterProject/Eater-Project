#pragma once

class Light_Pass : public RenderPassBase
{
public:
	Light_Pass();
	~Light_Pass();

public:
	void Start(int width, int height) override;

	void OnResize(int width, int height) override;
	void Release() override;

	void ApplyOption() override;

	void RenderUpdate();

private:
	DrawBuffer* m_Screen_DB;

	VertexShader* m_Light_VS;
	PixelShader* m_Light_PS; 
	
	RenderTexture* m_OutPut_RT;

	RenderTexture* m_Albedo_RT;
	RenderTexture* m_Normal_RT;
	RenderTexture* m_Position_RT;
	RenderTexture* m_Emissive_RT;

private:
	ID3D11RenderTargetView* m_OutPut_RTV;

	D3D11_VIEWPORT* m_Screen_VP;
};