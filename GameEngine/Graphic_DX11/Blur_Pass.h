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

	void RenderUpdate(RenderTexture* input, RenderTexture* output, UINT count);
	void RenderUpdate(RenderTexture* input, UINT count);

private:
	ComputeShader* m_BlurHorizon_CS;
	ComputeShader* m_BlurVertical_CS;

	RenderTexture* m_Blur_RT;

	ID3D11ShaderResourceView* m_NullSRV = nullptr;
	ID3D11UnorderedAccessView* m_NullUAV = nullptr;

private:
	UINT m_Width;
	UINT m_Height;

	UINT m_NumGroupsX;
	UINT m_NumGroupsY;
};

