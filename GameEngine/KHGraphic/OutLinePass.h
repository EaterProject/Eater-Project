#pragma once
class OutLinePass : public RenderPassBase
{
public:
	OutLinePass();
	~OutLinePass();

public:
	void Create(int width, int height) override;
	void Start(int width, int height) override;

	void OnResize(int width, int height) override;

	void Release() override;

	void RenderUpdate();
};

