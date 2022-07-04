#pragma once
class UI_Pass : public RenderPassBase
{
public:
	UI_Pass();
	~UI_Pass();
	
public:
	void Start(int width, int height) override;

	void OnResize(int width, int height) override;
	void Release() override;

	void BeginRender();
	void RenderUpdate(std::vector<RenderData*>& meshlist);
	void EndRender();

private:
	VertexShader* m_UI_VS;
	PixelShader* m_UI_PS;
	PixelShader* m_UI_Fill_PS;
	PixelShader* m_UI_Fill_Reverse_PS;

	DrawBuffer* m_Screen_DB;

	RenderTexture* m_Origin_RT;

private:
	ID3D11BlendState* m_AlphaBlend_BS;

	ID3D11RenderTargetView* m_OutPut_RTV;

private:
	RenderData* m_RenderData;

	UINT m_RenderCount;
};

