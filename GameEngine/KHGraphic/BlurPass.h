#pragma once
class BlurPass : public RenderPassBase
{
public:
	BlurPass();
	~BlurPass();

public:
	void Create(int width, int height) override;
	void Start(int width, int height) override;

	void OnResize(int width, int height) override;

	void Release() override;

	void RenderUpdate();

private:
	ComputeShader* m_BlurHorizon_CS;
	ComputeShader* m_BlurVertical_CS;

};

