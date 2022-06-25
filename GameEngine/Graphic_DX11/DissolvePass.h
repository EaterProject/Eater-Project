#pragma once
class DissolvePass : public RenderPassBase
{
public:
	DissolvePass();
	~DissolvePass();

public:
	void Create(int width, int height) override;
	void Start(int width, int height) override;

	void Release() override;

	void PreUpdate() override;

private:
	void SetShaderList();
	void SetShaderResourceView();
	void SetShaderConstantBuffer();

private:
	float m_DissolveTimer;
};

