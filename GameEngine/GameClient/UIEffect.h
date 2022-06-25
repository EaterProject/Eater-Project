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
private:
	void CreateFade();

	//º¯¼öµé
	Image* mImages[3] = { nullptr };
	RectTransform* mRect[3] = { nullptr };

	bool IsFade_IN		= false;
	bool IsFade_OUT		= false;
	float mAlpha		= 255;
	int INDEX			= 0;


	bool* FadeIn_Return		= nullptr;
	bool* FadeOut_Return	= nullptr;

	const int FADE_IMAGE = 0;
};

