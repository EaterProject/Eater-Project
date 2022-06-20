#include "UICanvas.h"
#include "ClientTypeOption.h"
#include "FontImage.h"
#include "RectTransform.h"
#include "Image.h"
#include "EaterEngineAPI.h"
#include "GameObject.h"
#include "ComboFont.h"
UICanvas::UICanvas()
{
	

}

UICanvas::~UICanvas()
{

}

void UICanvas::Awake()
{
	Create_Emagin_Color_UI();
	Create_Emagin_UI();
	Create_Combo_UI();
	Create_HP_UI();
	Create_Skill_UI();
	Create_Effect_UI();
	Create_Monster_UI();
}

void UICanvas::Start()
{
	mFont[Font_Emagin_Now]->SetFontNumber(0);
	mFont[Font_Emagin_Max]->SetFontNumber(14);
	mFont[Font_HP_Now]->SetFontNumber(150);
	mFont[Font_HP_Max]->SetFontNumber(150);
	mFont[Font_Monster_Emagin]->SetFontNumber(7);
}

void UICanvas::Update()
{
	Update_Hit_Check();
	Update_Combo_Check();
}

void UICanvas::Set_Combo_Now(int Number)
{
	mCombo->SetComboNumber(Number);
}

void UICanvas::Set_HP_Now(int Number)
{
	Alpha = 255;
	mFont[Font_HP_Now]->SetFontNumber(Number);
	Images[Image_PlayerHit_Effect]->SetColor(255, 0, 0, Alpha);
	isHit = true;
}

void UICanvas::Set_HP_Max(int Number)
{
	mFont[Font_HP_Max]->SetFontNumber(Number);
}

void UICanvas::Set_Emagin_Now(int Number)
{
	int Count = Number % 4;
	switch (Count)
	{
	case 0:
		Images[Image_Emagin_Front]->SetColor(255, 0, 0, 200);
		break;
	case 1:
		Images[Image_Emagin_Front]->SetColor(0, 255, 0, 200);
		break;
	case 2:
		Images[Image_Emagin_Front]->SetColor(0, 0, 255, 200);
		break;
	case 3:
		Images[Image_Emagin_Front]->SetColor(255, 255, 0, 200);
		break;
	}

	mFont[Font_Emagin_Now]->SetFontNumber(Number);
}

void UICanvas::Set_Emagin_Max(int Number)
{
	mFont[Font_Emagin_Max]->SetFontNumber(Number);
}

void UICanvas::Set_Monster_EMAGINE(void* Emagin)
{
	MONSTER_EMAGIN* mEmagin = reinterpret_cast<MONSTER_EMAGIN*>(Emagin);

	mFont[Font_Monster_Emagin]->SetFontNumber(mEmagin->ComboCount);
	Images[Image_Monster_Emagin_Front]->SetColor(mEmagin->R, mEmagin->G, mEmagin->B, 255);
	Images[Image_Monster_HP_Front];
}

void UICanvas::Set_ALLRender(bool Render)
{
	if (Render == true)
	{
		//모든 이미지 On
		for (int i = 0; i < 14; i++)
		{

		}
	}
	else
	{
		//모든 이미지 On
		for (int i = 0; i < 14; i++)
		{

		}
	}
}

void UICanvas::Create_Combo_UI()
{
	GameObject* Object = Instance();
	mCombo = Object->AddComponent<ComboFont>();
}

void UICanvas::Create_HP_UI()
{
	GameObject*		Object	= nullptr;
	RectTransform*	Rect	= nullptr;
	Image*			Img		= nullptr;

	float OffsetX = 50;

	//HP 폰트
	Object = Instance();
	mFont[Font_HP_Now] = Object->AddComponent<FontImage>();
	mFont[Font_HP_Now]->Setting(PosX + OffsetX, PosY + 50, "number_", 0.4f, 0.4f, 15, PIVOT_MIDDLE_LEFT);

	Object = Instance_UI();
	Images[Image_HP_Space] = Object->AddComponent<Image>();
	Images[Image_HP_Space]->SetTexture("hp_Space");
	Rect = Object->GetComponent<RectTransform>();
	OffsetX += 70;
	Rect->SetPosition(PosX + OffsetX, PosY + 50);
	Rect->SetScale(1.5f, 1.5f);
	Rect->SetPivot(PIVOT_MIDDLE_LEFT);
	
	//HP 폰트 맥스
	Object = Instance();
	OffsetX += 10;
	mFont[Font_HP_Max] = Object->AddComponent<FontImage>();
	mFont[Font_HP_Max]->Setting(PosX + OffsetX, PosY + 50, "number_", 0.4f, 0.4f, 15, PIVOT_MIDDLE_LEFT);

	OffsetX = 50;
	Object = Instance_UI();
	Images[Image_HP_Slider] = Object->AddComponent<Image>();
	Images[Image_HP_Slider]->SetTexture("ingame_player_hp");
	Rect = Object->GetComponent<RectTransform>();
	Rect->SetPosition(PosX + OffsetX, PosY + 100);
	Rect->SetScale(1.5f, 1.5f);
	Rect->SetPivot(PIVOT_MIDDLE_LEFT);
}

void UICanvas::Create_Skill_UI()
{
	float OffsetX = 450;
	GameObject*		Object		= nullptr;
	RectTransform*	Rect		= nullptr;
	Image*			Img			= nullptr;


	Object = Instance_UI();
	Images[Image_Skill_Space] = Object->AddComponent<Image>();
	Images[Image_Skill_Space]->SetTexture("ingame_skill_space");
	Rect = Object->GetComponent<RectTransform>();
	OffsetX += 60;
	Rect->SetPosition(PosX + OffsetX, PosY + 100);
	Rect->SetScale(0.65f, 0.65f);
	Rect->SetPivot(PIVOT_MIDDLE_LEFT);
	
	Object = Instance_UI();
	Images[Image_Skill_E] = Object->AddComponent<Image>();
	Images[Image_Skill_E]->SetTexture("ingame_skill_e");
	Rect = Object->GetComponent<RectTransform>();
	OffsetX += 60;
	Rect->SetPosition(PosX + OffsetX, PosY + 100);
	Rect->SetScale(0.65f, 0.65f);
	Rect->SetPivot(PIVOT_MIDDLE_LEFT);
	
	Object = Instance_UI();
	Images[Image_Skill_Right] = Object->AddComponent<Image>();
	Images[Image_Skill_Right]->SetTexture("ingame_skill_rb");
	Rect = Object->GetComponent<RectTransform>();
	OffsetX += 60;
	Rect->SetPosition(PosX + OffsetX, PosY + 100);
	Rect->SetScale(0.65f, 0.65f);
	Rect->SetPivot(PIVOT_MIDDLE_LEFT);
}

void UICanvas::Create_Emagin_Color_UI()
{
	//이메진 UI 생성
	GameObject*		Object	= nullptr;
	RectTransform*	Rect	= nullptr;
	Image*			Img		= nullptr;

	Object = Instance_UI();
	Images[Image_Emagin_Back] = Object->AddComponent<Image>();
	Images[Image_Emagin_Back]->SetTexture("Emagin_Back");
	Images[Image_Emagin_Back]->SetColor(255, 255, 255, 255);
	Rect = Object->GetComponent<RectTransform>();
	Rect->SetPosition(-200, 0);
	Rect->SetScale(1,1);
	Rect->SetPivot(PIVOT_MIDDLE_CENTER);

	Object = Instance_UI();
	Images[Image_Emagin_Front] = Object->AddComponent<Image>();
	Images[Image_Emagin_Front]->SetTexture("Emagin_Front");
	Images[Image_Emagin_Front]->SetColor(255, 255, 255, 255);
	Rect = Object->GetComponent<RectTransform>();
	Rect->SetPosition(-200, 0);
	Rect->SetScale(1, 1);
	Rect->SetPivot(PIVOT_MIDDLE_CENTER);
}

void UICanvas::Create_Emagin_UI()
{
	GameObject*		Object	= nullptr;
	RectTransform*	Rect	= nullptr;
	Image*			Img		= nullptr;


	float OffsetX = 50;

	//이메진 변경 회전 이미지
	Object = Instance_UI();
	Images[Image_Emagin_Change] = Object->AddComponent<Image>();
	Images[Image_Emagin_Change]->SetTexture("ingame_switch");
	Rect = Object->GetComponent<RectTransform>();
	Rect->SetPosition(OffsetX, PosY);
	Rect->SetScale(1, 1);
	Rect->SetPivot(PIVOT_MIDDLE_LEFT);

	//이메진 Now Count
	Object = Instance();
	OffsetX += 75;
	mFont[Font_Emagin_Now] = Object->AddComponent<FontImage>();
	mFont[Font_Emagin_Now]->Setting(PosX + OffsetX, PosY + 0, "number_", 0.8f, 0.8f, 30, PIVOT_MIDDLE_LEFT);
	
	//이메진 카운터 스페이스
	Object = Instance_UI();
	Images[Image_Emagin_Space] = Object->AddComponent<Image>();
	Images[Image_Emagin_Space]->SetTexture("Count_Space");
	Rect = Object->GetComponent<RectTransform>();
	OffsetX += 60;
	Rect->SetPosition(PosX + OffsetX, PosY + 10);
	Rect->SetScale(0.7f, 0.7f);
	Rect->SetPivot(PIVOT_MIDDLE_LEFT);
	
	//이메진 Max Count
	Object = Instance();
	OffsetX += 10;
	mFont[Font_Emagin_Max] = Object->AddComponent<FontImage>();
	mFont[Font_Emagin_Max]->Setting(PosX + OffsetX, PosY + 7, "number_", 0.7f, 0.7f, 20, PIVOT_MIDDLE_LEFT);
}

void UICanvas::Create_Effect_UI()
{
	GameObject*		Object	= nullptr;
	RectTransform*	Rect	= nullptr;
	Image*			Img		= nullptr;

	Object = Instance_UI();
	Images[Image_PlayerHit_Effect] = Object->AddComponent<Image>();
	Images[Image_PlayerHit_Effect]->SetTexture("PlayerDamage_01");
	Images[Image_PlayerHit_Effect]->SetColor(255, 0, 0, 0);
	Rect = Object->GetComponent<RectTransform>();
	Rect->SetPosition(0, 0);
	Rect->SetScale(1,1);
	Rect->SetPivot(PIVOT_MIDDLE_CENTER);
}

void UICanvas::Create_Monster_UI()
{
	//폰트 하나
	//체력바 하나
	//이미지 두개
	GameObject* Object	= nullptr;
	RectTransform* Rect = nullptr;

	Object = Instance_UI();
	Images[Image_Monster_HP_Front] = Object->AddComponent<Image>();
	Images[Image_Monster_HP_Front]->SetTexture("Monster_HP");
	Images[Image_Monster_HP_Front]->SetColor(255, 255, 255, 255);
	Rect = Object->GetComponent<RectTransform>();
	Rect->SetPosition(0, -400);
	Rect->SetScale(1, 1);
	Rect->SetPivot(PIVOT_MIDDLE_CENTER);

	Object = Instance_UI();
	Images[Image_Monster_Emagin_Back] = Object->AddComponent<Image>();
	Images[Image_Monster_Emagin_Back]->SetTexture("Monster_Emagin_Back");
	Images[Image_Monster_Emagin_Back]->SetColor(255, 255, 255, 255);
	Rect = Object->GetComponent<RectTransform>();
	Rect->SetPosition(-100,-400);
	Rect->SetScale(1, 1);
	Rect->SetPivot(PIVOT_MIDDLE_CENTER);

	Object = Instance_UI();
	Images[Image_Monster_Emagin_Front] = Object->AddComponent<Image>();
	Images[Image_Monster_Emagin_Front]->SetTexture("Monster_Emagin_Front");
	Images[Image_Monster_Emagin_Front]->SetColor(255, 0, 0, 255);
	Rect = Object->GetComponent<RectTransform>();
	Rect->SetPosition(-100, -400);
	Rect->SetScale(1, 1);
	Rect->SetPivot(PIVOT_MIDDLE_CENTER);


	Object = Instance();
	mFont[Font_Monster_Emagin] = Object->AddComponent<FontImage>();
	mFont[Font_Monster_Emagin]->Setting(-110, -400, "number_", 0.4f, 0.4f, 15, PIVOT_MIDDLE_CENTER);
}

void UICanvas::Update_Hit_Check()
{
	if (isHit == false) { return; }

	float DTime = GetDeltaTime();
	Alpha -= (100.0f * DTime);
	Images[Image_PlayerHit_Effect]->SetColor(255, 0, 0, Alpha);
	if (Alpha <= 0)
	{
		isHit = false;
	}
}

void UICanvas::Update_Combo_Check()
{
	if (mCombo != nullptr)
	{
		mCombo->UpdateFontAnimation();
	}
}
