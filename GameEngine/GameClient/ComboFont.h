#pragma once
#include "FontImage.h"

class RectTransform;
class Image;
class GameObject;
class ComboFont : public FontImage
{
public:
	ComboFont();
	virtual ~ComboFont();

	void SetComboNumber(int Number);

	void Awake() override;
	void Start() override;
	void TransformUpdate() override;

	GameObject*		UI_Obj[3];
	Image*			UI_Image[3];
	RectTransform*	UI_Rect[3];
};

