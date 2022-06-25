#pragma once
#include <Component.h>
class Image;
class Button;
class RectTransform;
class UITitle :public Component
{
public:
	UITitle();
	virtual ~UITitle();

	void Awake() override;
	void Update() override;
	
	void SetTitleUIActive(bool Active);
private:
	void StartButton_Enter();
	void StartButton_Click();
	void StartButton_Exit();

	void OptionButton_Enter();
	void OptionButton_Click();
	void OptionButton_Exit();

	void EndButton_Enter();
	void EndButton_Click();
	void EndButton_Exit();


	Button* Title_Button[3];
	Image*	Title_IMG[3];
	RectTransform* Title_Rect[3];


	Image* TitleMainImage;
	RectTransform* Title_IMG_Rect;

	bool IsStart	= false;
	bool IsEnd		= false;
	bool IsOption	= false;

	bool IsFade		= false;

	float ButtonUI_PosX = 200;
};

