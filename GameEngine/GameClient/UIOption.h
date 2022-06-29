#pragma once
#include <Component.h>
class Button;
class Image;
class ImageFont;
class RenderOption;

class UIOption :public Component
{
public:
	UIOption();
	virtual ~UIOption();

private:
	struct ControlButton
	{
		int Value = 1;

		Button* Left;
		Button* Right;
	};

private:
	enum OPTION
	{
		GP_IBL,
		GP_SSAO,
		GP_SHADOW,
		GP_FOG,
		GP_BLOOM,
		GP_COLORGRADING,
		GP_FXAA,
		GP_HDR,
		SCREEN_SIZE,
		MASTER_VOL,
		BACKGROUND_VOL,
		SFX_VOL,
		OPTION_MAX,
	};

public:
	void Awake() override;
	void Start() override;

public:
	void SetOptionUIActive(bool active);
	void SetPrevTarget(int target);

private:
	void ClickButtonSetting(OPTION option, float left_x, float right_x, float y);
	
private:
	void SetScreenSize();
	void SetMasterVolumeUp();
	void SetMasterVolumeDown();
	void SetBackGroundVolumeUp();
	void SetBackGroundVolumeDown();
	void SetSFXVolumeUp();
	void SetSFXVolumeDown();
	void SetClearTime();
	void SetGraphicIBL();
	void SetGraphicSSAO();
	void SetGraphicShadow();
	void SetGraphicFog();
	void SetGraphicBloom();
	void SetGraphicColorGrading();
	void SetGraphicFXAA();
	void SetGraphicHDR();
	void SetReturnIn();
	void SetReturnOut();
	void SetReturnClick();

private:
	RenderOption* GraphicOption;

	bool IsClick;

	int PrevTarget;

	float ValueChangeTime;
	float NowTime;

	Image* BackGroundImage;
	Button* ReturnButton;

	Image* ScreenText;
	ImageFont* SoundText[3];
	Image* GraphicText[8];

	ControlButton ClickButton[12];
};

