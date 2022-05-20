#pragma once
class UI_Pass : public RenderPassBase
{
public:
	UI_Pass();
	~UI_Pass();
	
public:
	void Create(int width, int height) override;
	void Start(int width, int height) override;

	void OnResize(int width, int height) override;
	void Release() override;

	void ApplyOption() override;

public:

};

