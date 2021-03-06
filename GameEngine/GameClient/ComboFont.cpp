#include "ComboFont.h"
#include "GameObject.h"
#include "EaterEngineAPI.h"
#include "Image.h"
#include "RectTransform.h"
#include "Slider.h"
#include "MessageManager.h"
ComboFont::ComboFont()
{


}

ComboFont::~ComboFont()
{


}

void ComboFont::SetComboNumber(int Number)
{
	FontIdleTime = 0;
	FontAlpha = 255;
	for (int i = 0; i < 3; i++)
	{
		Com_Image[i]->SetColor(255, 255, 255, FontAlpha);
	}
	UI_Image->SetColor(255, 255, 255, FontAlpha);
	UI_Slider->SetFillColor(255, 255, 255, FontAlpha);
	UI_Slider->SetBackGroundColor(255, 255, 255, FontAlpha);
	UI_Slider->SetValueRange(0, MaxTime);
	SetFontNumber(Number);
	ComboTime = MaxTime;
	FontUpdte = true;
}

void ComboFont::SetComboTimeMax(int Number)
{
	MaxTime = Number;
}

void ComboFont::SetComboTime(int Number)
{
	NowTime = Number;
}

void ComboFont::Awake()
{
	Setting(500, 0, "combo_");
	FontAnimation(0.2, 1.0f);

	//콤보 이미지를 추가
	UI_Obj = Instance_UI();
	UI_Image	= UI_Obj->AddComponent<Image>();
	UI_Rect	= UI_Obj->GetComponent<RectTransform>();
	UI_Image->SetTexture("combo_title");
	UI_Rect->SetPosition(PosX + 240, PosY + 20);

	GameObject* Object  = Instance_Slider();
	UI_Slider = Object->GetComponent<Slider>();
	UI_Slider->SetBackGroundTexture("combo_Silder_Back");
	UI_Slider->SetFillTexture("combo_Silder");
	UI_Slider->SetPosition(PosX + 150, PosY + 70);
	UI_Slider->SetValueRange(0, 100);
	UI_Slider->SetFillRange(FILL_LEFT, 50);

	for (int i = 0; i < 3; i++)
	{
		Com_Image[i]->SetColor(255, 255, 255, 0);
	}
	UI_Slider->SetFillColor(255, 255, 255, 0);
	UI_Slider->SetBackGroundColor(255, 255, 255, 0);
	UI_Image->SetColor(255, 255, 255, 0);

	FontUpdte = false;
}

void ComboFont::UpdateFontAnimation()
{
	if (FontUpdte == true)
	{
		//폰트 사이즈 조절이 완료여부
		FontSizeUPTime += GetDeltaTime() * 3.0f;
		if (FontSizeUPTime >= FontSizeUPTimeMax)
		{
			Com_Rect[0]->SetScale(FontSizeUPTimeMax, FontSizeUPTimeMax);
			Com_Rect[1]->SetScale(FontSizeUPTimeMax, FontSizeUPTimeMax);
			Com_Rect[2]->SetScale(FontSizeUPTimeMax, FontSizeUPTimeMax);
			FontSizeUPTime = FontSizeUPTimeMin;

			IsAlpha = true;
			FontUpdte = false;
		}
		else
		{
			Com_Rect[0]->SetScale(FontSizeUPTime, FontSizeUPTime);
			Com_Rect[1]->SetScale(FontSizeUPTime, FontSizeUPTime);
			Com_Rect[2]->SetScale(FontSizeUPTime, FontSizeUPTime);
		}
	}
	else if(IsAlpha == true)
	{
		//사이즈 조절이 완료되고 알파값 조절
		if (FontIdleTime >= MaxTime)
		{
			if (FontAlpha > 0)
			{
				FontAlpha -= GetDeltaTime() * 255;
				for (int i = 0; i < 3; i++){Com_Image[i]->SetColor(255, 255, 255, FontAlpha);}
				UI_Slider->SetFillColor(255, 255, 255, FontAlpha);
				UI_Slider->SetBackGroundColor(255, 255, 255, FontAlpha);
				UI_Image->SetColor(255, 255, 255, FontAlpha);
				MessageManager::GetGM()->SEND_Message(TARGET_PLAYER, MESSAGE_PLAYER_COMBO_RESET);
				FontUpdte = false;
			}
		}
		else
		{
			FontIdleTime += GetDeltaTime();
			UI_Slider->SetFillRange(FILL_RIGHT, ComboTime);
		}
	}
	ComboTime -= GetDeltaTime();
}

void ComboFont::SetActive(bool Active)
{
	FontImage::SetActive(Active);

	UI_Image->SetActive(Active);
	UI_Slider->SetActive(Active);
}
