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
	UI_Rect[0]->SetScale(0.5f, 0.5f);

	//슬라이더
	UI_Obj[1]	= Instance_UI();
	UI_Image[1] = UI_Obj[1]->AddComponent<Image>();
	UI_Rect[1]  = UI_Obj[1]->GetComponent<RectTransform>();
	UI_Image[1]->SetTexture("combo_Silder");
	UI_Rect[1]->SetPosition(PosX + 150, PosY + 70);
	UI_Rect[1]->SetScale(0.5f, 0.5f);

	//슬라이더 Back
	UI_Obj[2]	= Instance_UI();
	UI_Image[2] = UI_Obj[2]->AddComponent<Image>();
	UI_Rect[2]	= UI_Obj[2]->GetComponent<RectTransform>();
	UI_Image[2]->SetTexture("combo_Silder_Back");
	UI_Rect[2]->SetPosition(PosX + 150, PosY + 70);
	UI_Rect[2]->SetScale(0.5f, 0.5f);

}

void ComboFont::Start()
{
	SetFontNumber(0);
}

void ComboFont::TransformUpdate()
{
	UpdateFontAnimation();
}
