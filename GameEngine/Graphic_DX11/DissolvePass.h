#pragma once
class Dissolve_Pass : public RenderPassBase
{
public:
	Dissolve_Pass();
	~Dissolve_Pass();

public:
	void Start(int width, int height) override;

	void Release() override;

	void PreUpdate() override;

private:
	void SetShaderList();
	void SetShaderResourceView();
	void SetShaderConstantBuffer();
	void Test();

private:
	std::vector<std::string> TextureName;
	int index = 0;

	float m_DissolvePlayTime = 1.0f;
	float m_DissolveTimer = 0.0f;
	float m_DissolveSpeed = 0.25f;
	float m_DissolveThickness = 0.01f;

	float m_DissolveInnerFactor = 90.0f;
	float m_DissolveOuterFactor = 20.0f;
};

