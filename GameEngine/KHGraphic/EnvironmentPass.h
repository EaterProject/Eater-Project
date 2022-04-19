#pragma once
class EnvironmentPass : public RenderPassBase
{
public:
	EnvironmentPass();
	~EnvironmentPass();

public:
	void Create(int width, int height) override;
	void Start(int width, int height) override;

	void OnResize(int width, int height) override;
	void Release() override;

	void SetOption(RenderOption* renderOption) override;
	void SetEnvironmentMapResource(EnvironmentBuffer* resource);

	void RenderUpdate();

private:
	DrawBuffer* m_BoxDB;

	VertexShader* m_SkyBoxVS;
	PixelShader* m_SkyBoxPS;

	RenderTexture* m_OutPut_RT;
	RenderTexture* m_Position_RT;
	
	ID3D11RasterizerState* m_CubeMapRS;
	ID3D11DepthStencilState* m_CubeMapDSS;

	ID3D11RenderTargetView* m_OutPut_RTV;
	ID3D11RenderTargetView* m_Position_RTV;
	ID3D11DepthStencilView* m_DefaltDSV;
};

