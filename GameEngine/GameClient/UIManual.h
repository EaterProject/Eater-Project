#pragma once
#include <Component.h>

class Image;
class Button;

class UIManual : public Component
{
public:
	UIManual();
	~UIManual();

private:
	enum MANUAL
	{
		KEY,
		COMBO
	};

public:
	void Awake() override;

public:
	void Set_ManualUI_Active(bool active);

private:
	void SetNextIn();
	void SetNextOut();
	void SetNextClick();

	void SetReturnIn();
	void SetReturnOut();
	void SetReturnClick();

private:
	Image* ManualImage;
	Button* ReturnButton[2];

};

