#pragma once
#include <Component.h>

class RectTransform;
class Image;
class GameObject;
class FontImage :public Component
{
public:
	FontImage();
	virtual ~FontImage();

	void SetActive(bool Active);
	void SetFontNumber(int Number);
	void SetSize(float x, float Y);
	virtual void UpdateFontAnimation();
	void Setting(int mPosX,int mPosY, std::string mFontName,float SizeX = 0.5f,float SizeY = 0.5f,float NumberOffset = 60 ,PIVOT_TYPE pivot = PIVOT_TYPE::PIVOT_MIDDLE_CENTER);
	void FontAnimation(float MinSize, float MaxSize);
protected:
	GameObject*		ImageObject[3];
	Image*			Com_Image[3];
	RectTransform*	Com_Rect[3];

	bool FontUpdte = false;

	float FontOriginalSize  = 0.0f;

	float FontSizeUPTime	= 0.1f;
	float FontSizeUPTimeMin	= 0.2f;
	float FontSizeUPTimeMax = 2.0f;

	float FontAlpha		= 255;
	float FontIdleTime	= 0;

	const int COMBO_01		= 0;
	const int COMBO_02		= 1;
	const int COMBO_TITLE	= 2;

	float PosX = 500.0f;
	float PosY = 0.0f;
	float OffsetX = 0;
	float OffsetY = 0;
	std::string FontName;
};

