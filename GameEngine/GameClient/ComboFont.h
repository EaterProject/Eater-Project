#pragma once
#include "FontImage.h"

class RectTransform;
class Image;
class GameObject;
class Slider;
class ComboFont : public FontImage
{
public:
	ComboFont();
	virtual ~ComboFont();


	void Awake() override;
	void UpdateFontAnimation();

	void SetComboNumber(int Number);
	void SetComboTimeMax(int Number);
	void SetComboTime(int Number);
private:
	GameObject*		UI_Obj;
	Image*			UI_Image;
	RectTransform*	UI_Rect;
	Slider*			UI_Slider;

	float MaxTime = 10.0f;
	float NowTime = 0.0f;

	float ComboTime = 0.0f;

	bool IsAlpha = false;
};

