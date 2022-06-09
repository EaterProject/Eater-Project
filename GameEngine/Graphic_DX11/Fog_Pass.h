#pragma once
class Fog_Pass : public RenderPassBase
{
public:
	Fog_Pass();
	~Fog_Pass();

public:
	void Create(int width, int height) override;
	void Start(int width, int height) override;

	void OnResize(int width, int height) override;
	void Release() override;

	void ApplyOption() override;
	void PreUpdate() override;

private:
	void SetShaderList();
	void SetShaderResourceView();
	void SetShaderConstantBuffer();

private:
	float m_FogTimer;

private:
	DrawBuffer* m_Screen_DB;
	
	VertexShader* m_Screen_VS;
	PixelShader* m_Fog_PS;

	RenderTexture* m_OutPut_RT;

private:
	ID3D11RenderTargetView* m_OutPut_RTV;

	D3D11_VIEWPORT* m_Screen_VP;
};

