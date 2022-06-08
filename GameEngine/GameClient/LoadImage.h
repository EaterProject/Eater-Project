#pragma once
#include "Component.h"

class Image;
class RectTransform;

class LoadingImage : public Component
{
public:
	LoadingImage();
	~LoadingImage();

public:
	void Awake() override;
	void Update() override;

private:
	Image* Icon_Image;
	Image* Loading_1_Image;
	Image* Loading_2_Image;
	Image* Text_Origin_Image;

	Image* Test_Image;

	RectTransform* Loading_RectTR_2;

	float Angle;
};

