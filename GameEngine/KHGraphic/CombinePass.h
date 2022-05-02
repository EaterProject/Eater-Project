#pragma once
struct CB_DrawFinal;

class CombinePass : public RenderPassBase
{
public:
	CombinePass();
	~CombinePass();

public:
	void Create(int width, int height) override;
	void Start(int width, int height) override;
	void OnResize(int width, int height) override;
	void Release() override;
	
	void ApplyOption() override;

	void RenderUpdate();

private:
	VertexShader* m_Screen_VS;
	PixelShader* m_Combine_PS;

	DrawBuffer* m_Screen_DB;

	RenderTexture* m_Origin_RT;
	RenderTexture* m_Bloom_RT;
	RenderTexture* m_OutLine_RT;
	RenderTexture* m_OutPut_RT;

private:
	ID3D11RenderTargetView* m_OutPut_RTV;

	D3D11_VIEWPORT* m_Screen_VP;
};

