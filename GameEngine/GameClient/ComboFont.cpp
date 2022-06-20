#include "ComboFont.h"
#include "GameObject.h"
#include "EaterEngineAPI.h"
#include "Image.h"
#include "RectTransform.h"
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
		UI_Image[i]->SetColor(255, 255, 255, FontAlpha);
	}
	SetFontNumber(Number);
}

void ComboFont::Awake()
{
	Setting(500, 0, "combo_");
	FontAnimation(0.1, 0.5f);

	//콤보 이미지를 추가
	UI_Obj[0]	= Instance_UI();
	UI_Image[0]	= UI_Obj[0]->AddComponent<Image>();
	UI_Rect[0]	= UI_Obj[0]->GetComponent<RectTransform>();
	UI_Image[0]->SetTexture("combo_title");
	UI_Rect[0]->SetPosition(PosX + 240, PosY + 20);

	//슬라이더
	UI_Obj[1]	= Instance_UI();
	UI_Image[1] = UI_Obj[1]->AddComponent<Image>();
	UI_Rect[1]  = UI_Obj[1]->GetComponent<RectTransform>();
	UI_Image[1]->SetTexture("combo_Silder");
	UI_Rect[1]->SetPosition(PosX + 150, PosY + 70);

	//슬라이더 Back
	UI_Obj[2]	= Instance_UI();
	UI_Image[2] = UI_Obj[2]->AddComponent<Image>();
	UI_Rect[2]	= UI_Obj[2]->GetComponent<RectTransform>();
	UI_Image[2]->SetTexture("combo_Silder_Back");
	UI_Rect[2]->SetPosition(PosX + 150, PosY + 70);
}

void ComboFont::Start()
{
	SetFontNumber(0);
}

void ComboFont::UpdateFontAnimation()
{
	if (FontUpdte == true)
	{
		FontSizeUPTime += GetDeltaTime() * 3.0f;
		if (FontSizeUPTime >= FontSizeUPTimeMax)
		{
			Com_Rect[0]->SetScale(FontSizeUPTimeMax, FontSizeUPTimeMax);
			Com_Rect[1]->SetScale(FontSizeUPTimeMax, FontSizeUPTimeMax);
			Com_Rect[2]->SetScale(FontSizeUPTimeMax, FontSizeUPTimeMax);
			FontSizeUPTime = FontSizeUPTimeMin;
			FontUpdte = false;
		}
		else
		{
			Com_Rect[0]->SetScale(FontSizeUPTime, FontSizeUPTime);
			Com_Rect[1]->SetScale(FontSizeUPTime, FontSizeUPTime);
			Com_Rect[2]->SetScale(FontSizeUPTime, FontSizeUPTime);
		}
	}
	else
	{
		if (FontIdleTime >= 5)
		{
			if (FontAlpha > 0)
			{
				FontAlpha -= GetDeltaTime() * 255;
				for (int i = 0; i < 3; i++)
				{
					Com_Image[i]->SetColor(255, 255, 255, FontAlpha);
					UI_Image[i]->SetColor(255, 255, 255, FontAlpha);
				}
			}
		}
		else
		{
			FontIdleTime += GetDeltaTime();
		}

	}
}
