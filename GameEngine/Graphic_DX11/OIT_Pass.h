#pragma once
class OIT_Pass : public RenderPassBase
{
public:
	OIT_Pass();
	~OIT_Pass();

public:
	void Create(int width, int height) override;
	void Start(int width, int height) override;

	void OnResize(int width, int height) override;
	void Release() override;

	void SetResize(int width, int height) override;

	void BeginRender();
	void RenderUpdate();

private:
	void SetShaderList();
	void SetShaderConstantBuffer(UINT width, UINT height);

private:
	VertexShader* m_OITRender_VS;
	PixelShader* m_OITRender_PS;

	PixelShader* m_OITMesh_PS;
	PixelShader* m_OITParticle_PS;

	DrawBuffer* m_Screen_DB;

	RenderBuffer* m_PieceLink_RB;
	RenderBuffer* m_FirstOffset_RB;

	RenderTexture* m_OutPut_RT;

private:
	ID3D11DepthStencilView* m_Defalt_DSV;
	ID3D11DepthStencilState* m_NoStencil_DSS;
	ID3D11RasterizerState* m_NoCull_RS;

	ID3D11RenderTargetView* m_OutPut_RTV;

	ID3D11UnorderedAccessView* m_PieceLink_UAV;
	ID3D11UnorderedAccessView* m_FirstOffset_UAV;

	ID3D11UnorderedAccessView* m_UAV_List[2];

	ID3D11ShaderResourceView* m_NullSRV = nullptr;
	ID3D11UnorderedAccessView* m_NullUAV = nullptr;

private:
	UINT m_Multiple;
	UINT m_MagicValue;
	UINT m_InitCounts[2];
};

