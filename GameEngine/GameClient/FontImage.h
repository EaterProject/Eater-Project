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

	void Awake() override;
	void Update() override;
	void SetCombo(int Number);

private:
	GameObject*		ImageObject[3];
	Image*			Com_Image[3];
	RectTransform*	Com_Rect[3];

	const int COMBO_01		= 0;
	const int COMBO_02		= 1;
	const int COMBO_TITLE	= 2;
};

