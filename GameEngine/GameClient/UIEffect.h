#pragma once
#include <Component.h>

class Image;
class RectTransform;
class UIEffect :public Component
{
public:
	UIEffect();
	virtual ~UIEffect();

	void Awake() override;
	void Start() override;
	void Update() override;

	void Fade_IN(void* Data);
	void Fade_OUT(void* Data);
	void PlayerHit(void* Data);

	void Set_UIEffect_Active(bool Active);

private:
	void CreateFade();

	void UpdateFade();
	void UpdateHit();

	//페이드 인/아웃 변수들
	Image* mImages[2] = { nullptr };
	RectTransform* mRect[2] = { nullptr };
	bool IsFade			= false;
	bool IsFade_IN		= false;
	bool IsFade_OUT		= false;
	float mAlpha		= 255;
	float mHitAlpha		= 255;
	int INDEX			= 0;

	bool* FadeIn_Return = nullptr;
	bool* FadeOut_Return = nullptr;

	bool IsHit			= false;


	const int FADE_IMAGE = 0;
	const int PLAYER_HIT = 1;
};

