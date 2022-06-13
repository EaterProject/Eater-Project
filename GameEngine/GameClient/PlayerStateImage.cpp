#include "PlayerStateImage.h"
#include "FontImage.h"
#include "RectTransform.h"
#include "Image.h"
#include "EaterEngineAPI.h"
#include "GameObject.h"
PlayerStateImage::PlayerStateImage()
{

}

PlayerStateImage::~PlayerStateImage()
{

}

void PlayerStateImage::Awake()
{
	GameObject* Object = nullptr;
	RectTransform* RT = nullptr;
	float OffsetX = 50;

	//이메진 변경 회전 이미지
	Object = Instance_UI();
	Change = Object->AddComponent<Image>();
	Change->SetTexture("ingame_switch");
	RT = Object->GetComponent<RectTransform>();
	RT->SetPosition(OffsetX, PosY);
	RT->SetScale(0.5f, 0.5f);
	RT->SetImagePivot(PIVOT_MIDDLE_LEFT);

	//이메진 변경 카운트 폰트
	Object = Instance();
	OffsetX += 75;
	mFont[0] = Object->AddComponent<FontImage>();
	mFont[0]->Setting(PosX + OffsetX, PosY + 0, "number_", 0.4f, 0.4f, 30, PIVOT_MIDDLE_LEFT);

	//이메진 카운터 스페이스
	Object = Instance_UI();
	Space  = Object->AddComponent<Image>();
	Space->SetTexture("Count_Space");
	RT = Object->GetComponent<RectTransform>();
	OffsetX += 60;
	RT->SetPosition(PosX + OffsetX, PosY + 10);
	RT->SetScale(0.35f, 0.35f);
	RT->SetImagePivot(PIVOT_MIDDLE_LEFT);
	//
	////이메진
	Object = Instance();
	OffsetX += 10;
	mFont[1] = Object->AddComponent<FontImage>();
	mFont[1]->Setting(PosX + OffsetX, PosY + 7, "number_", 0.35f, 0.35f, 20, PIVOT_MIDDLE_LEFT);


	OffsetX = 50;
	//HP 폰트
	Object = Instance();
	mFont[2] = Object->AddComponent<FontImage>();
	mFont[2]->Setting(PosX+ OffsetX, PosY + 50, "number_", 0.2f, 0.2f, 15, PIVOT_MIDDLE_LEFT);

	Object = Instance_UI();
	HP_Space = Object->AddComponent<Image>();
	HP_Space->SetTexture("hp_Space");
	RT = Object->GetComponent<RectTransform>();
	OffsetX += 70;
	RT->SetPosition(PosX + OffsetX, PosY + 50);
	RT->SetScale(0.75f, 0.75f);
	RT->SetImagePivot(PIVOT_MIDDLE_LEFT);

	//HP 폰트 맥스
	Object = Instance();
	OffsetX += 10;
	mFont[3] = Object->AddComponent<FontImage>();
	mFont[3]->Setting(PosX + OffsetX, PosY + 50, "number_", 0.2f, 0.2f, 15, PIVOT_MIDDLE_LEFT);

	OffsetX = 50;
	Object = Instance_UI();
	HP_Bar = Object->AddComponent<Image>();
	HP_Bar->SetTexture("ingame_player_hp");
	RT = Object->GetComponent<RectTransform>();
	RT->SetPosition(PosX + OffsetX, PosY + 100);
	RT->SetScale(0.75f, 0.75f);
	RT->SetImagePivot(PIVOT_MIDDLE_LEFT);
	OffsetX += 450;




	Object = Instance_UI();
	Skill_01 = Object->AddComponent<Image>();
	Skill_01->SetTexture("ingame_skill_rb");
	RT = Object->GetComponent<RectTransform>();
	OffsetX += 60;
	RT->SetPosition(PosX + OffsetX, PosY + 100);
	RT->SetScale(0.35f, 0.35f);
	RT->SetImagePivot(PIVOT_MIDDLE_LEFT);

	Object = Instance_UI();
	Skill_02 = Object->AddComponent<Image>();
	Skill_02->SetTexture("ingame_skill_space");
	RT = Object->GetComponent<RectTransform>();
	OffsetX += 60;
	RT->SetPosition(PosX + OffsetX, PosY + 100);
	RT->SetScale(0.35f, 0.35f);
	RT->SetImagePivot(PIVOT_MIDDLE_LEFT);

	Object = Instance_UI();
	Skill_03 = Object->AddComponent<Image>();
	Skill_03->SetTexture("ingame_skill_e");
	RT = Object->GetComponent<RectTransform>();
	OffsetX += 60;
	RT->SetPosition(PosX + OffsetX, PosY + 100);
	RT->SetScale(0.35f, 0.35f);
	RT->SetImagePivot(PIVOT_MIDDLE_LEFT);
}

void PlayerStateImage::Start()
{
	mFont[ColorCount_Index]->SetFontNumber(0);
	mFont[ColorCountMax_Index]->SetFontNumber(14);
	mFont[HPCount_Index]->SetFontNumber(150);
	mFont[HPCountMax_Index]->SetFontNumber(150);
}

void PlayerStateImage::SetHP(int Number)
{
	mFont[HPCount_Index]->SetFontNumber(Number);
}

void PlayerStateImage::SetChangeCount(int Number)
{
	mFont[ColorCount_Index]->SetFontNumber(Number);
}
