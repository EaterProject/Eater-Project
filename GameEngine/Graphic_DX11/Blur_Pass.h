#pragma once
class Blur_Pass : public RenderPassBase
{
public:
	Blur_Pass();
	~Blur_Pass();

public:
	void Create(int width, int height) override;
	void Start(int width, int height) override;

	void OnResize(int width, int height) override;

	void Release() override;

	void RenderUpdate(RenderTarget* input, RenderTarget* output);

private:
	ComputeShader* m_BlurHorizon_CS;
	ComputeShader* m_BlurVertical_CS;

private:
	ID3D11RasterizerState* m_Solid_RS;


private:
	UINT m_Width;
	UINT m_Height;

	UINT m_NumGroupsX;
	UINT m_NumGroupsY;
};

