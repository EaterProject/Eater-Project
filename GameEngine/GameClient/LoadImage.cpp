#include "LoadImage.h"
#include "GameObject.h"
#include "Transform.h"
#include "EaterEngineAPI.h"
#include "RectTransform.h"
#include "Image.h"

LoadingImage::LoadingImage()
	:Angle(0.0f), Alpha(0.0f), IsPlus(true)
{

}

LoadingImage::~LoadingImage()
{

}

void LoadingImage::Awake()
{
	GameObject* ui_object = nullptr;
	RectTransform* ui_rectTR = nullptr;

	ui_object = InstanceUI("Icon_UI");
	Icon_Image = ui_object->AddComponent<Image>();
	ui_rectTR = ui_object->GetComponent<RectTransform>();
	ui_rectTR->SetImagePivot(RECT_PIVOT::PIVOT_MIDDLE_CENTER);
	ui_rectTR->SetPosition(0.0f, -100.0f);
	ui_rectTR->SetScale(0.5f, 0.5f);

	ui_object = InstanceUI("Loading_1_UI");
	Loading_1_Image = ui_object->AddComponent<Image>();
	ui_rectTR = ui_object->GetComponent<RectTransform>();
	ui_rectTR->SetImagePivot(RECT_PIVOT::PIVOT_MIDDLE_BOTTOM);
	ui_rectTR->SetPosition(0.0f, -200.0f);
	ui_rectTR->SetScale(0.5f, 0.5f);

	ui_object = InstanceUI("Loading_2_UI");
	Loading_2_Image = ui_object->AddComponent<Image>();
	Loading_RectTR_2 = ui_object->GetComponent<RectTransform>();
	Loading_RectTR_2->SetImagePivot(RECT_PIVOT::PIVOT_MIDDLE_BOTTOM);
	Loading_RectTR_2->SetPosition(0.0f, -200.0f);
	Loading_RectTR_2->SetScale(0.5f, 0.5f);

	ui_object = InstanceUI("Text_Origin_UI");
	Text_Origin_Image = ui_object->AddComponent<Image>();
	ui_rectTR = ui_object->GetComponent<RectTransform>();
	ui_rectTR->SetImagePivot(RECT_PIVOT::PIVOT_MIDDLE_BOTTOM);
	ui_rectTR->SetPosition(0.0f, -150.0f);
	ui_rectTR->SetScale(0.5f, 0.5f);

	Icon_Image->SetTexture("Loading_Logo");
	Loading_1_Image->SetTexture("Loading_Icon_01");
	Loading_2_Image->SetTexture("Loading_Icon_02");
	Text_Origin_Image->SetTexture("Loading_Text");
}

void LoadingImage::Update()
{
	float dTime = GetDeltaTime();

	Angle += 100.0f * dTime;

	if (Angle >= 360.0f)
	{
		Angle = 0.0f;
	}

	Loading_RectTR_2->SetRotation(Angle);

	if (IsPlus)
	{
		Alpha += 100.0f * dTime;
		Text_Origin_Image->SetImageColor(255, 255, 255, Alpha);

		if (Alpha >= 255.0f)
		{
			Alpha = 255.0f;
			IsPlus = false;
		}
	}
	else
	{
		Alpha -= 100.0f * dTime;
		Text_Origin_Image->SetImageColor(255, 255, 255, Alpha);

		if (Alpha <= 0.0f)
		{
			Alpha = 0.0f;
			IsPlus = true;
		}
	}
}
