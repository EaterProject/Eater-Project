#pragma once
#include "Component.h"

class Image;
class RectTransform;
class FontImage;
class PlayerStateImage :public Component
{
public:
	PlayerStateImage();
	virtual ~PlayerStateImage();

	void Awake() override;
	void Start() override;
	void Update() override;

	void SetHP(int Number);
	void SetChangeCount(int Number);
private:
	FontImage* mFont[4];
	Image* Space;
	Image* Change;
	Image* HP_Space;
	Image* HP_Bar;

	Image* Skill_01;
	Image* Skill_02;
	Image* Skill_03;

	Image* ChangePlayerColor;
	Image* HitBackGround;

	bool isHit  = false;
	float Alpha = 0;

	float PosX = 0;
	float PosY = 350;

	const int ColorCount_Index		= 0;
	const int ColorCountMax_Index	= 1;
	const int HPCount_Index			= 2;
	const int HPCountMax_Index		= 3;
};

