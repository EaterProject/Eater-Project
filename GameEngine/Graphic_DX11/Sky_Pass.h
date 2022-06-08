#pragma once
class Sky_Pass : public RenderPassBase
{
public:
	Sky_Pass();
	~Sky_Pass();

public:
	void Create(int width, int height) override;
	void Start(int width, int height) override;

	void OnResize(int width, int height) override;
	void Release() override;

	void ApplyOption() override;

public:
	void SetSkyCubeResource(TextureBuffer* resource);
	void SetSkyLightResource(SkyLightBuffer* resource, UINT index);

	void RenderUpdate();

private:
	void SetShaderList();

private:
	DrawBuffer* m_Box_DB;

	VertexShader* m_SkyBox_VS;
	PixelShader* m_SkyBox_PS;

	RenderTexture* m_OutPut_RT;
	
private:
	ID3D11RasterizerState* m_CubeMap_RS;
	ID3D11DepthStencilState* m_CubeMap_DSS;

	ID3D11RenderTargetView* m_OutPut_RTV;
	ID3D11DepthStencilView* m_Defalt_DSV;

private:
	Matrix m_EnvironmentWorld;
};

