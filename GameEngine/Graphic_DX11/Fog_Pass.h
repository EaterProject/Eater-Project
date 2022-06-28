#pragma once
class Fog_Pass : public RenderPassBase
{
public:
	Fog_Pass();
	~Fog_Pass();

public:
	void Start(int width, int height) override;

	void Release() override;

	void ApplyOption() override;
	void PreUpdate() override;

private:
	void SetShaderList();
	void SetShaderResourceView();
	void SetShaderConstantBuffer();

private:
	float m_FogTimer;
};

