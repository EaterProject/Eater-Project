#pragma once
class OITPass : public RenderPassBase
{
public:
	OITPass();
	~OITPass();

public:
	void Create(int width, int height) override;
	void Start(int width, int height) override;

	void OnResize(int width, int height) override;
	void Release() override;

	void SetResize(int width, int height) override;
	void SetOption(UINT renderOption) override;

	void BeginRender();
	void RenderUpdate();

private:
	UINT m_Multiple;

	VertexShader* m_OITRenderVS;
	PixelShader* m_OITRenderPS;
	PixelShader* m_OITParticlePS;

	DrawBuffer* m_Screen_DB;

	RenderTexture* m_OutPut_RT;

	RenderBuffer* m_PieceLinkRB;
	RenderBuffer* m_FirstOffsetRB;

	ID3D11DepthStencilView* m_DefaltDSV;
	ID3D11DepthStencilState* m_NoStencilDSS;
	ID3D11RasterizerState* m_NoCullRS;

	ID3D11RenderTargetView* m_OutPut_RTV;

	ID3D11UnorderedAccessView* m_PieceLinkUAV;
	ID3D11UnorderedAccessView* m_FirstOffsetUAV;

	UINT m_MagicValue;
	std::vector<ID3D11UnorderedAccessView*> m_UAVList;
};

