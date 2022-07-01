#pragma once
#include <Component.h>

class Image;
class RectTransform;
class Slider;
class ImageFont;
class UIBoss :public Component
{
public:
	UIBoss();
	~UIBoss();
public:
	void Awake() override;
	void Start() override;
	void SetUp() override;

	void SetActive(bool mActive);
private:
	Image* mBossName_Img;
	RectTransform* mBossName_Rect;

	Image* mBossColor_Img;
	RectTransform* mBossColor_Rect;

	Slider* mBossHP_Slider;
	RectTransform* mBossHP_Rect;

	ImageFont* mBossCount_Font;
};

