#pragma once
class PickingPass : public RenderPassBase
{
public:
	PickingPass();
	~PickingPass();

public:
	void Create(int width, int height) override;
	void Start(int width, int height) override;

	void OnResize(int width, int height) override;
	void Release() override;

private:
	Vector4 HashIDColor(int hash);
};

