#pragma once
class LightPass : public RenderPassBase
{
public:
	LightPass();
	~LightPass();

public:
	void Create(int width, int height) override;
	void Start(int width, int height) override;

	void OnResize(int width, int height) override;
	void Release() override;

	void SetOption(RenderOption* renderOption) override;
	void SetIBLEnvironmentMapResource(EnvironmentBuffer* resource);

	void Reset();

	void RenderUpdate();

private:
	DrawBuffer* m_Screen_DB;

	VertexShader* m_Light_VS;
	PixelShader* m_Light_PS; 
	
	ID3D11RenderTargetView* m_OutPut_RTV;

	RenderTexture* m_OutPut_RT;

	RenderTexture* m_Albedo_RT;
	RenderTexture* m_Normal_RT;
	RenderTexture* m_Position_RT;
	RenderTexture* m_Emissive_RT;

	D3D11_VIEWPORT* m_Screen_VP;
};

