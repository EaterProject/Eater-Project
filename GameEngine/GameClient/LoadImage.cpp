#include "LoadImage.h"
#include "GameObject.h"
#include "Transform.h"
#include "EaterEngineAPI.h"
#include "RectTransform.h"
#include "Image.h"
#include "Slider.h"

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
	ui_rectTR->SetPivot(RECT_PIVOT::PIVOT_MIDDLE_CENTER);
	ui_rectTR->SetPosition(0.0f, -100.0f);

	ui_object = InstanceUI("Loading_1_UI");
	Loading_1_Image = ui_object->AddComponent<Image>();
	ui_rectTR = ui_object->GetComponent<RectTransform>();
	ui_rectTR->SetPivot(RECT_PIVOT::PIVOT_MIDDLE_BOTTOM);
	ui_rectTR->SetPosition(0.0f, -200.0f);

	ui_object = InstanceUI("Loading_2_UI");
	Loading_2_Image = ui_object->AddComponent<Image>();
	Loading_RectTR_2 = ui_object->GetComponent<RectTransform>();
	Loading_RectTR_2->SetPivot(RECT_PIVOT::PIVOT_MIDDLE_BOTTOM);
	Loading_RectTR_2->SetPosition(0.0f, -200.0f);

	ui_object = InstanceUI("Text_Origin_UI");
	Text_Origin_Image = ui_object->AddComponent<Image>();
	ui_rectTR = ui_object->GetComponent<RectTransform>();
	ui_rectTR->SetPivot(RECT_PIVOT::PIVOT_MIDDLE_BOTTOM);
	ui_rectTR->SetPosition(0.0f, -220.0f);

	//ui_object = InstanceUI("Test_Slider_Back");
	//Test_Slider_Back = ui_object->AddComponent<Image>();
	//ui_rectTR = ui_object->GetComponent<RectTransform>();
	//ui_rectTR->SetPivot(RECT_PIVOT::PIVOT_LEFT_BOTTOM);
	//ui_rectTR->SetPosition(50.0f, -50.0f);
	//
	//ui_object = InstanceUI("Test_Slider_Fill");
	//Test_Slider_Fill = ui_object->AddComponent<Image>();
	//Test_Slider_Fill->SetImageColor(1.0f, 0.0f, 0.0f, 1.0f);
	//ui_rectTR = ui_object->GetComponent<RectTransform>();
	//ui_rectTR->SetPivot(RECT_PIVOT::PIVOT_LEFT_BOTTOM);
	//ui_rectTR->SetPosition(50.0f, -50.0f);

	GameObject* obj = InstanceSlider();
	Test_Slider = obj->GetComponent<Slider>();
	Test_Slider->SetValueRange(0.0f, 255.0f);
	Test_Slider->SetPivot(RECT_PIVOT::PIVOT_LEFT_BOTTOM);
	Test_Slider->SetPosition(50.0f, -50.0f);
	Test_Slider->SetBackGroundColor(0.0f, 1.0f, 0.0f, 1.0f);
	Test_Slider->SetFillColor(1.0f, 0.0f, 0.0f, 1.0f);
	//Test_Slider->SetFillRange();
	Test_Slider->SetBackGroundTexture("ingame_player_hp_back");
	Test_Slider->SetFillTexture("ingame_player_hp");

	Icon_Image->SetTexture("Loading_Logo");
	Loading_1_Image->SetTexture("Loading_Icon_01");
	Loading_2_Image->SetTexture("Loading_Icon_02");
	Text_Origin_Image->SetTexture("Loading_Text");

	//Test_Slider_Back->SetTexture("ingame_player_hp_back");
	//Test_Slider_Fill->SetTexture("ingame_player_hp");
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
		Text_Origin_Image->SetImageColor(1.0f, 1.0f, 1.0f, Alpha / 255.0f);
		Text_Origin_Image->SetFillRange(FILL_TYPE::FILL_RIGHT, Alpha / 255.0f);
		Test_Slider->SetFillRange(FILL_TYPE::FILL_LEFT, Alpha);

		if (Alpha >= 255.0f)
		{
			Alpha = 255.0f;
			IsPlus = false;
		}
	}
	else
	{
		Alpha -= 100.0f * dTime;
		Text_Origin_Image->SetImageColor(1.0f, 1.0f, 1.0f, Alpha / 255.0f);
		Text_Origin_Image->SetFillRange(FILL_TYPE::FILL_RIGHT, Alpha / 255.0f);
		Test_Slider->SetFillRange(FILL_TYPE::FILL_LEFT, Alpha);

		if (Alpha <= 50.0f)
		{
			Alpha = 50.0f;
			IsPlus = true;
		}
	}
}
