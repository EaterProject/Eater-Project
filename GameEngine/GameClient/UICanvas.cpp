#include "UICanvas.h"
#include "ClientTypeOption.h"
#include "FontImage.h"
#include "RectTransform.h"
#include "Image.h"
#include "EaterEngineAPI.h"
#include "GameObject.h"
#include "ComboFont.h"
#include "Slider.h"
#include "ImageFont.h";
#include "MiniMapSystem.h"
UICanvas::UICanvas()
{
	

}

UICanvas::~UICanvas()
{

}

void UICanvas::Awake()
{
	//Create_Emagin_Color_UI();
	//Create_Emagin_UI();
	//Create_Player_HP();
	//Create_Skill_UI();
	//Create_Effect_UI();

	//플레이어 관련 UI
	Create_Player_HP(50,400);
	Create_Player_Emagin(50,350);
	Create_Player_Mana(100, 400);
	Create_Player_Emagin_Color(-200, 0);
	Create_Dron_Text(0, -200);
	Create_Skill_UI(375, 450);

	Create_Combo_UI();
	Create_Monster_UI();
}

void UICanvas::Start()
{
	Set_Player_HP(80);
	Set_Player_HP_Max(150);
	
	Set_Player_Emagin(0);
	Set_Player_Emagin_Max(14);
	
	Set_Mana_Green_Count(13);
	Set_Mana_Orange_Count(41);
	
	Set_Drone_Text(0);
}

void UICanvas::Update()
{
	//Update_Hit_Check();
	Update_Combo_Check();
}

void UICanvas::Set_Combo_Now(int Number)
{
	mCombo->SetComboNumber(Number);
}

void UICanvas::Set_Player_Emagin(int Number)
{
	int Count = Number % 2;
	switch (Count)
	{
	case 0:
		Player_EMAGIN_COLOR[1]->SetColor(255, 0, 0, 200);
		break;
	case 1:
		Player_EMAGIN_COLOR[1]->SetColor(0, 0, 255, 200);
		break;
	}

	Player_EMAGIN[0]->SetFontNumber(Number);
}

void UICanvas::Set_Player_Emagin_Max(int Number)
{
	Player_EMAGIN[1]->SetFontNumber(Number);
}

void UICanvas::Set_Mana_Green_Count(int Number)
{
	Player_MANA[0]->SetFontNumber(Number);
}

void UICanvas::Set_Mana_Orange_Count(int Number)
{
	Player_MANA[1]->SetFontNumber(Number);
}

void UICanvas::Set_Drone_Text(int Number)
{
	Sound_Play_SFX("Drone_Text_01");
	switch (Number)
	{
	case 0 :
		Dron_Text->SetTexture("00_DroneText_GameStart");
		break;
	case 1:
		Dron_Text->SetTexture("01_DroneText_GameStart1");
		break;
	case 2:
		Dron_Text->SetTexture("02_DroneText_GameStart2");
		break;
	case 3:
		Dron_Text->SetTexture("03_DroneText_FirstOutDoom");
		break;
	case 4:
		Dron_Text->SetTexture("04_DroneText_FirstOutDoom1");
		break;
	case 5:
		Dron_Text->SetTexture("05_DroneText_VendingMashine");
		break;
	case 6:
		Dron_Text->SetTexture("06_DroneText_Sell");
		break;
	case 7:
		Dron_Text->SetTexture("07_DroneText_SellFalse");
		break;
	case 8:
		Dron_Text->SetTexture("08_DroneText_BossSpawn");
		break;
	case 9:
		Dron_Text->SetTexture("09_DroneText_BossZone");
		break;
	case 10:
		Dron_Text->SetTexture("10_DroneText_BossZone1");
		break;
	case 11:
		Dron_Text->SetTexture("11_DroneText_ManaSpawn");
		break;
	case 12:
		Dron_Text->SetTexture("12_DroneText_PureMana");
		break;
	case 13:
		Dron_Text->SetTexture("13_DroneText_CoreMana");
		break;
	case 14:
		Dron_Text->SetTexture("14_DroneText_PlayerDead");
		break;
	case 15:
		Dron_Text->SetTexture("15_DroneText_PlayerHeal");
		break;
	}
}

void UICanvas::Set_InGameUI_Active(bool Active)
{
	Active_Player_HP(Active);
	Active_Player_Emagin(Active);
	Active_Mana_Count(Active);
	Active_Dron_Text(Active);
	Active_Player_Emagin_Color(Active);
	Active_Player_Skill(Active);
	MiniMapSystem::Get()->SetMiniMapActive(Active);
	MiniMapSystem::Get()->SetIconListActive(Active);
}

void UICanvas::Set_Monster_UI_ON(void* Emagin)
{
	MONSTER_EMAGIN* mEmagin = reinterpret_cast<MONSTER_EMAGIN*>(Emagin);
	GameObject* Object = reinterpret_cast<GameObject*>(mEmagin->Object);

	if (UseCheck(Object) == true) { return; }

	int OffsetY = 0.0f;
	if(mEmagin->Type == MONSTER_TYPE_A)
	{
		OffsetY = 2.0f;
	}
	else
	{
		OffsetY = 2.5f;
	}
	int MonsterIndex = GetActiveMonsterUI();
	SetMonsterUIActive(MonsterIndex, true);


	MonsterTR_Back[MonsterIndex]->SetPivot(PIVOT_OBJECT);
	MonsterTR_Back[MonsterIndex]->SetPositionObject(Object, Vector3(0.0, OffsetY, 0.0f));
	MonsterTR_Back[MonsterIndex]->SetPosition(-60, 0);

	MonsterTR_Front[MonsterIndex]->SetPivot(PIVOT_OBJECT);
	MonsterTR_Front[MonsterIndex]->SetPositionObject(Object, Vector3(0.0, OffsetY, 0.0f));
	MonsterTR_Front[MonsterIndex]->SetPosition(-60, 0);

	//색 설정
	Monster_Emagin_Front[MonsterIndex]->SetColor(mEmagin->R, mEmagin->G, mEmagin->B, 255);
	
	MonsterSlider[MonsterIndex]->SetValueRange(0, 100);
	MonsterSlider[MonsterIndex]->SetFillRange(FILL_LEFT, mEmagin->HP);
	MonsterSlider[MonsterIndex]->SetPivot(PIVOT_OBJECT);
	MonsterSlider[MonsterIndex]->SetPositionObject(Object, Vector3(0.0, OffsetY, 0.0f));

	MonsterFont[MonsterIndex]->SetPivot(PIVOT_OBJECT);
	MonsterFont[MonsterIndex]->SetPositionObject(Object, Vector3(0.0, OffsetY, 0.0f));
	MonsterFont[MonsterIndex]->SetFontNumber(mEmagin->ComboCount);

	//레이어 설정
	Monster_Emagin_Back[MonsterIndex]->SetLayer(1);
	Monster_Emagin_Front[MonsterIndex]->SetLayer(1);
	MonsterFont[MonsterIndex]->SetLayer(3);
	MonsterObject[MonsterIndex] = Object;
}

void UICanvas::Set_Monster_UI_OFF(void* Emagin)
{
	MONSTER_EMAGIN* mEmagin = reinterpret_cast<MONSTER_EMAGIN*>(Emagin);
	GameObject* Object = reinterpret_cast<GameObject*>(mEmagin->Object);

	for (int i = 0; i < 5; i++)
	{
		if (Object == MonsterObject[i])
		{
			MonsterObject[i] = nullptr;
			MonsterActiveUI[i] = false;

			SetMonsterUIActive(i, false);

			MonsterTR_Back[i]->SetPositionObject(Object, Vector3(0.0, 20.0f, 0.0f));
			MonsterTR_Front[i]->SetPositionObject(Object, Vector3(0.0, 20.0f, 0.0f));
			MonsterSlider[i]->SetPositionObject(Object, Vector3(0.0, 20.0f, 0.0f));
			MonsterFont[i]->SetPositionObject(Object, Vector3(0.0, 20.0f, 0.0f));
			break;
		}
	}
}

void UICanvas::Set_Monster_UI_SET_DATA(void* Emagin)
{
	MONSTER_EMAGIN* mEmagin = reinterpret_cast<MONSTER_EMAGIN*>(Emagin);
	GameObject* Object = reinterpret_cast<GameObject*>(mEmagin->Object);

	for (int i = 0; i < 5; i++)
	{
		if (MonsterObject[i] == nullptr) { return; }
		if (Object == MonsterObject[i])
		{
			Monster_Emagin_Front[i]->SetColor(mEmagin->R, mEmagin->G, mEmagin->B, 255);

			if (mEmagin->Type == MONSTER_TYPE_A)
			{
				MonsterSlider[i]->SetFillRange(FILL_LEFT, mEmagin->HP);
				MonsterSlider[i]->SetPositionObject(Object, Vector3(0.0, 2.0f, 0.0f));

				MonsterFont[i]->SetPivot(PIVOT_OBJECT);
				MonsterFont[i]->SetPositionObject(Object, Vector3(0.0, 2.0f, 0.0f));
				MonsterFont[i]->SetFontNumber(mEmagin->ComboCount);

				MonsterTR_Front[i]->SetPositionObject(Object, Vector3(0.0, 2.0f, 0.0f));
				MonsterTR_Back[i]->SetPositionObject(Object, Vector3(0.0, 2.0f, 0.0f));
			}
			else
			{
				MonsterSlider[i]->SetFillRange(FILL_LEFT, mEmagin->HP);
				MonsterSlider[i]->SetPositionObject(Object, Vector3(0.0, 2.5f, 0.0f));

				MonsterFont[i]->SetPivot(PIVOT_OBJECT);
				MonsterFont[i]->SetPositionObject(Object, Vector3(0.0, 2.5f, 0.0f));
				MonsterFont[i]->SetFontNumber(mEmagin->ComboCount);

				MonsterTR_Front[i]->SetPositionObject(Object, Vector3(0.0, 2.5f, 0.0f));
				MonsterTR_Back[i]->SetPositionObject(Object, Vector3(0.0, 2.5f, 0.0f));
			}
			break;
		}
	}
}

void UICanvas::Set_Player_HP(int Number)
{
	Player_HP[0]->SetFontNumber(Number);
	Player_HP_Slider->SetFillRange(FILL_LEFT, Number);
	isHit = true;
}

void UICanvas::Set_Player_HP_Max(int Number)
{
	Player_HP[1]->SetFontNumber(Number);
	Player_HP_Slider->SetValueRange(0, Number);
}

void UICanvas::Set_Player_UI_Active(bool Active)
{
	Player_HP[0]->SetActive(Active);				//플레이어 체력 폰트	Now
	Player_HP[1]->SetActive(Active);				//플레이어 체력 폰트 Max
	Player_HP_Slider->SetActive(Active);				//플레이어 체력 슬라이더
	Player_HP_BAR->gameobject->SetActive(Active);	//플레이어 체력 언더바
}

void UICanvas::Create_Player_Emagin(float X, float Y)
{
	GameObject* Object = nullptr;
	RectTransform* Rect = nullptr;
	Image* Img = nullptr;

	//현재 이메진 폰트
	Object = Instance_UI();
	Player_EMAGIN_CHANGE = Object->AddComponent<Image>();
	Player_EMAGIN_CHANGE->SetTexture("ingame_switch");
	Player_EMAGIN_RECT[0] = Object->GetComponent<RectTransform>();
	Player_EMAGIN_RECT[0]->SetPosition(X, Y - 10);
	Player_EMAGIN_RECT[0]->SetScale(0.75f, 0.75f);
	Player_EMAGIN_RECT[0]->SetPivot(PIVOT_MIDDLE_LEFT);

	Object = Instance_ImageFont();
	Player_EMAGIN[0] = Object->GetComponent<ImageFont>();
	Player_EMAGIN[0]->SetTexture("number_");
	Player_EMAGIN[0]->SetPosition(X + 50,  Y -5);
	Player_EMAGIN[0]->SetFontCount(2);
	Player_EMAGIN[0]->SetOffset(30);
	Player_EMAGIN[0]->SetScale(0.75f, 0.75f);
	Player_EMAGIN[0]->SetPivot(PIVOT_MIDDLE_LEFT);

	//폰트 사이의 언더바 이미지
	Object = Instance_UI();
	Player_HP_BAR = Object->AddComponent<Image>();
	Player_HP_BAR->SetTexture("Count_Space");
	Player_EMAGIN_RECT[1] = Object->GetComponent<RectTransform>();
	Player_EMAGIN_RECT[1]->SetPosition(X + 110, Y);
	Player_EMAGIN_RECT[1]->SetScale(0.75f, 0.75f);
	Player_EMAGIN_RECT[1]->SetPivot(PIVOT_MIDDLE_LEFT);

	//최대 이메진 폰트
	Object = Instance_ImageFont();
	Player_EMAGIN[1] = Object->GetComponent<ImageFont>();
	Player_EMAGIN[1]->SetTexture("number_");
	Player_EMAGIN[1]->SetPosition(X+125, Y);
	Player_EMAGIN[1]->SetFontCount(2);
	Player_EMAGIN[1]->SetOffset(20);
	Player_EMAGIN[1]->SetScale(0.5f, 0.5f);
	Player_EMAGIN[1]->SetPivot(PIVOT_MIDDLE_LEFT);
}

void UICanvas::Create_Combo_UI()
{
	GameObject* Object = Instance();
	mCombo = Object->AddComponent<ComboFont>();
}

void UICanvas::Create_Player_HP(float X, float Y)
{
	GameObject*		Object	= nullptr;
	RectTransform*	Rect	= nullptr;
	Image*			Img		= nullptr;

	//현재 체력 폰트
	Object = Instance_ImageFont();
	Player_HP[0] = Object->GetComponent<ImageFont>();
	Player_HP[0]->SetTexture("number_");
	Player_HP[0]->SetPivot(PIVOT_MIDDLE_LEFT);
	Player_HP[0]->SetFontCount(3);
	Player_HP[0]->SetOffset(18);
	Player_HP[0]->SetScale(0.5f,0.5f);
	Player_HP[0]->SetPosition(X, Y);
	
	//폰트 사이의 언더바 이미지
	Object = Instance_UI();
	Player_HP_BAR = Object->AddComponent<Image>();
	Player_HP_BAR->SetTexture("hp_Space");
	Player_RECT = Object->GetComponent<RectTransform>();
	Player_RECT->SetPosition(X+ 65, Y);
	Player_RECT->SetScale(1.75f, 1.5f);
	Player_RECT->SetPivot(PIVOT_MIDDLE_LEFT);

	//최대 체력의 폰트
	Object = Instance_ImageFont();
	Player_HP[1] = Object->GetComponent<ImageFont>();
	Player_HP[1]->SetTexture("number_");
	Player_HP[1]->SetFontCount(3);
	Player_HP[1]->SetOffset(18);
	Player_HP[1]->SetScale(0.5f, 0.5f);
	Player_HP[1]->SetPosition(X + 75, Y);
	Player_HP[1]->SetPivot(PIVOT_MIDDLE_LEFT);

	//체력 슬라이더바 
	Object = Instance_Slider();
	Player_HP_Slider = Object->GetComponent<Slider>();
	Player_HP_Slider->SetBackGroundTexture("ingame_player_hp_back");
	Player_HP_Slider->SetFillTexture("ingame_player_hp");
	Player_HP_Slider->SetPosition(X, Y + 50);
	Player_HP_Slider->SetPivot(PIVOT_MIDDLE_LEFT);
	Player_HP_Slider->SetValueRange(0, 150);
	Player_HP_Slider->SetFillRange(FILL_LEFT, 100);
}

void UICanvas::Create_Player_Mana(float X, float Y)
{
	GameObject* Object = nullptr;
	RectTransform* Rect = nullptr;
	Image* Img = nullptr;

	//현재 체력 폰트
	Object = Instance_ImageFont();
	Player_MANA[0] = Object->GetComponent<ImageFont>();
	Player_MANA[0]->SetTexture("number_");
	Player_MANA[0]->SetPivot(PIVOT_MIDDLE_RIGHT);
	Player_MANA[0]->SetFontCount(2);
	Player_MANA[0]->SetOffset(35);
	Player_MANA[0]->SetScale(1, 1);
	Player_MANA[0]->SetPosition(X-300, Y);
	Player_MANA[0]->SetColor(65, 197, 198);

	Object = Instance_ImageFont();
	Player_MANA[1] = Object->GetComponent<ImageFont>();
	Player_MANA[1]->SetTexture("number_");
	Player_MANA[1]->SetPivot(PIVOT_MIDDLE_RIGHT);
	Player_MANA[1]->SetFontCount(2);
	Player_MANA[1]->SetOffset(35);
	Player_MANA[1]->SetScale(1, 1);
	Player_MANA[1]->SetPosition(X-200, Y);
	Player_MANA[1]->SetColor(213, 138, 19);


	Object = Instance_UI();
	Player_MANA_ICON = Object->AddComponent<Image>();
	Player_MANA_ICON->SetTexture("Minimap_Mana");
	Player_MANA_RECT = Object->GetComponent<RectTransform>();
	Player_MANA_RECT->SetPosition(X - 350, Y);
	Player_MANA_RECT->SetScale(1, 1);
	Player_MANA_RECT->SetPivot(PIVOT_MIDDLE_RIGHT);
}

void UICanvas::Create_Player_Emagin_Color(float X, float Y)
{
	//이메진 UI 생성
	GameObject* Object = nullptr;
	RectTransform* Rect = nullptr;
	Image* Img = nullptr;

	Object = Instance_UI();
	Player_EMAGIN_COLOR[0] = Object->AddComponent<Image>();
	Player_EMAGIN_COLOR[0]->SetTexture("Emagin_Back");
	Player_EMAGIN_COLOR[0]->SetColor(255, 255, 255, 255);
	Rect = Object->GetComponent<RectTransform>();
	Rect->SetPosition(X, Y);
	Rect->SetScale(1, 1);
	Rect->SetPivot(PIVOT_MIDDLE_CENTER);

	Object = Instance_UI();
	Player_EMAGIN_COLOR[1] = Object->AddComponent<Image>();
	Player_EMAGIN_COLOR[1]->SetTexture("Emagin_Front");
	Player_EMAGIN_COLOR[1]->SetColor(255, 255, 255, 255);
	Rect = Object->GetComponent<RectTransform>();
	Rect->SetPosition(X, Y);
	Rect->SetScale(1, 1);
	Rect->SetPivot(PIVOT_MIDDLE_CENTER);
}

void UICanvas::Create_Dron_Text(float X, float Y)
{
	GameObject*	Object = Instance_UI();
	Dron_Text = Object->AddComponent<Image>();
	Dron_Rect = Object->GetComponent<RectTransform>();
	Dron_Rect->SetPosition(X, Y);
	Dron_Rect->SetScale(1,1);
	Dron_Rect->SetPivot(PIVOT_MIDDLE_LEFT);

	Dron_Text->PushTextureList("00_DroneText_GameStart");
	Dron_Text->PushTextureList("01_DroneText_GameStart1");
	Dron_Text->PushTextureList("02_DroneText_GameStart2");
	Dron_Text->PushTextureList("03_DroneText_FirstOutDoom");
	Dron_Text->PushTextureList("04_DroneText_FirstOutDoom1");
	Dron_Text->PushTextureList("05_DroneText_VendingMashine");
	Dron_Text->PushTextureList("06_DroneText_Sell");
	Dron_Text->PushTextureList("07_DroneText_SellFalse");
	Dron_Text->PushTextureList("08_DroneText_BossSpawn");
	Dron_Text->PushTextureList("09_DroneText_BossZone");
	Dron_Text->PushTextureList("10_DroneText_BossZone1");
	Dron_Text->PushTextureList("11_DroneText_ManaSpawn");
	Dron_Text->PushTextureList("12_DroneText_PureMana");
	Dron_Text->PushTextureList("13_DroneText_CoreMana");
	Dron_Text->PushTextureList("15_DroneText_PlayerHeal");

	Dron_Text->SetTexture("00_DroneText_GameStart");
}

void UICanvas::Active_Player_HP(bool Active)
{
	Player_HP[0]->SetActive(Active);
	Player_HP[1]->SetActive(Active);

	Player_HP_Slider->SetActive(Active);
	Player_HP_BAR->gameobject->SetActive(Active);
}

void UICanvas::Active_Player_Emagin(bool Active)
{
	Player_EMAGIN[0]->SetActive(Active);
	Player_EMAGIN[1]->SetActive(Active);
	Player_EMAGIN_CHANGE->gameobject->SetActive(Active);
}

void UICanvas::Active_Player_Emagin_Color(bool Active)
{
	Player_EMAGIN_COLOR[0]->gameobject->SetActive(Active);
	Player_EMAGIN_COLOR[1]->gameobject->SetActive(Active);
}

void UICanvas::Active_Player_Skill(bool Active)
{
	Player_Skill[0]->SetActive(Active);
	Player_Skill[1]->SetActive(Active);
	Player_Skill[2]->SetActive(Active);
}

void UICanvas::Active_Mana_Count(bool Active)
{
	Player_MANA[0]->SetActive(Active);
	Player_MANA[1]->SetActive(Active);
	Player_MANA_ICON->gameobject->SetActive(Active);
}

void UICanvas::Active_Dron_Text(bool Active)
{
	Dron_Text->gameobject->SetActive(Active);
}

void UICanvas::Create_Skill_UI(float X, float Y)
{
	float OffsetX = 450;
	GameObject*		Object		= nullptr;
	RectTransform*	Rect		= nullptr;
	Image*			Img			= nullptr;


	Object = Instance_Slider();
	Player_Skill[0] = Object->GetComponent<Slider>();
	Player_Skill[0]->SetBackGroundTexture("ingame_skill_rb");
	Player_Skill[0]->SetFillTexture("ingame_skill_rb_1");
	Player_Skill[0]->SetPosition(X, Y );
	Player_Skill[0]->SetPivot(PIVOT_MIDDLE_LEFT);
	Player_Skill[0]->SetValueRange(0, 100);
	Player_Skill[0]->SetFillRange(FILL_DOWN, 50);

	Object = Instance_Slider();
	Player_Skill[1] = Object->GetComponent<Slider>();
	Player_Skill[1]->SetBackGroundTexture("ingame_skill_space");
	Player_Skill[1]->SetFillTexture("ingame_skill_space_1");
	Player_Skill[1]->SetPosition(X + 80, Y);
	Player_Skill[1]->SetPivot(PIVOT_MIDDLE_LEFT);
	Player_Skill[1]->SetValueRange(0, 100);
	Player_Skill[1]->SetFillRange(FILL_DOWN, 50);

	Object = Instance_Slider();
	Player_Skill[2] = Object->GetComponent<Slider>();
	Player_Skill[2]->SetBackGroundTexture("ingame_skill_e");
	Player_Skill[2]->SetFillTexture("ingame_skill_e_1");
	Player_Skill[2]->SetPosition(X + 160, Y);
	Player_Skill[2]->SetPivot(PIVOT_MIDDLE_LEFT);
	Player_Skill[2]->SetValueRange(0, 100);
	Player_Skill[2]->SetFillRange(FILL_DOWN, 50);
}

void UICanvas::Create_Monster_UI()
{
	for(int i = 0; i < 5; i++)
	{
		GameObject* Object	= nullptr;
		RectTransform* Rect = nullptr;

		//몬스터 체력바 
		Object = Instance_Slider();
		MonsterSlider[i] = Object->GetComponent<Slider>();
		MonsterSlider[i]->SetBackGroundTexture("Monster_HP_Back");
		MonsterSlider[i]->SetFillTexture("Monster_HP_Front");
		MonsterSlider[i]->SetPosition(0, 0);
		
		//몬스터 이메진 뒤 이미지
		Object = Instance_UI();
		Monster_Emagin_Back[i] = Object->AddComponent<Image>();
		Monster_Emagin_Back[i]->SetTexture("Monster_Emagin_Back");
		Monster_Emagin_Back[i]->SetColor(255, 255, 255, 255);
		MonsterTR_Back[i] = Object->GetComponent<RectTransform>();
		MonsterTR_Back[i]->SetPosition(0, 0);
		
		//몬스터 이메진 앞 이미지
		Object = Instance_UI();
		Monster_Emagin_Front[i] = Object->AddComponent<Image>();
		Monster_Emagin_Front[i]->SetTexture("Monster_Emagin_Front");
		Monster_Emagin_Front[i]->SetColor(255, 255, 255, 255);
		MonsterTR_Front[i] = Object->GetComponent<RectTransform>();
		MonsterTR_Front[i]->SetPosition(0, 0);

		Object = Instance_ImageFont();
		MonsterFont[i] = Object->GetComponent<ImageFont>();
		MonsterFont[i]->SetTexture("number_");
		MonsterFont[i]->SetFontCount(3);
		MonsterFont[i]->SetOffset(25);
		MonsterFont[i]->SetScale(0.5f, 0.5f);
		MonsterFont[i]->SetPosition(-112, 0);

		SetMonsterUIActive(i, false);
	}
}

void UICanvas::Update_Combo_Check()
{
	if (mCombo != nullptr)
	{
		mCombo->UpdateFontAnimation();
	}
}

int UICanvas::GetActiveMonsterUI()
{
	for (int i = 0; i < 5; i++)
	{
		if (MonsterObject[i] == nullptr)
		{ 
			return i;
		}
	}
}

void UICanvas::SetMonsterUIActive(int index, bool IsActive)
{
	Monster_Emagin_Back[index]->gameobject->SetActive(IsActive);
	Monster_Emagin_Front[index]->gameobject->SetActive(IsActive);
	MonsterFont[index]->SetActive(IsActive);
	MonsterSlider[index]->SetActive(IsActive);
}

bool UICanvas::UseCheck(GameObject* Obj)
{
	for (int i = 0; i < 5; i++)
	{
		if (MonsterObject[i] == Obj) 
		{
			return true;
		}
	}
	return false;
}
