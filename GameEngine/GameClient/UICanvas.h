#pragma once
#include "Component.h"

class Image;
class RectTransform;
class FontImage;
class ImageFont;
class ComboFont;
class Slider;
class MONSTER_EMAGIN;
class UICanvas :public Component
{
public:
	UICanvas();
	virtual ~UICanvas();

	void Awake() override;
	void Start() override;
	void Update() override;

	void Set_Combo_Now(int Number);

	//몬스터 UI
	void Set_Monster_UI_ON(void* Emagin);		//몬스터 UI를 켠다
	void Set_Monster_UI_OFF(void* Emagin);		//몬스터 UI를 끈다
	void Set_Monster_UI_SET_DATA(void* Emagin);	//몬스터 데이터를 넘긴다

	//플레이어 현재 체력 설정
	void Set_Player_HP(int Number);
	void Set_Player_HP_Max(int Number);
	void Set_Player_UI_Active(bool Active);

	//플레이어 이메진 설정
	void Set_Player_Emagin(int Number);
	void Set_Player_Emagin_Max(int Number);

	//플레이어 스킬 설정
	void Set_Skill_E(float value);
	void Set_Skill_MR(float value);
	void Set_Skill_SPC(float value);

	//초록 마나 카운트 설정
	void Set_Pure_Mana_Count(int Number);
	void Set_Core_Mana_Count(int Number);

	//드론 텍스쳐
	void Set_Drone_Text(int Number);

	void Set_InGameUI_Active(bool Active);
private:
	//생성 함수
	void Create_Player_Emagin(float X,float Y);
	void Create_Player_HP(float X,float Y);
	void Create_Player_Mana(float X,float Y);
	void Create_Player_Emagin_Color(float X, float Y);
	void Create_Dron_Text(float X, float Y);
	void Create_Monster_UI();
	void Create_Skill_UI(float X,float Y);

	//감추기 함수
	void Active_Player_HP(bool Active);
	void Active_Player_Emagin(bool Active);
	void Active_Player_Emagin_Color(bool Active);
	void Active_Player_Skill(bool Active);
	void Active_Mana_Count(bool Active);
	void Active_Dron_Text(bool Active);


	void Create_Combo_UI();

	void Update_Combo_Check();	//크기 처리
	
	int GetActiveMonsterUI();
	void SetMonsterUIActive(int index, bool IsActive);
	void SetMonsterUIDraw(int index, bool IsActive);
	bool UseCheck(GameObject* Obj);

	ComboFont*		mCombo		= nullptr;

	//플레이어 체력 UI
	ImageFont*		Player_HP[2];			//플레이어 체력 폰트
	Slider*			Player_HP_Slider;		//플레이어 체력 폰트
	Image*			Player_HP_BAR;
	RectTransform*	Player_RECT;

	//플레이어 이매진 UI
	ImageFont*		Player_EMAGIN[2];	//플레이어 이메진 폰트
	RectTransform*	Player_EMAGIN_RECT[2];
	Image*			Player_EMAGIN_CHANGE;
	Image*			Player_EMAGIN_BAR;

	//플레이어 이메진 Color UI
	Image*			Player_EMAGIN_COLOR[2];

	//플레이어 마나석
	ImageFont*		Player_MANA[2];
	Image*			Player_MANA_ICON;
	RectTransform*	Player_MANA_RECT;

	//드론 텍스쳐
	Image*			Dron_Text;
	RectTransform*	Dron_Rect;

	//플레이어 스킬
	Slider*			Player_Skill[3];

	//몬스터 UI들
	Image*			Monster_Emagin_Back[5]	= { nullptr };
	Image*			Monster_Emagin_Front[5] = { nullptr };
	RectTransform*	MonsterTR_Back[5]		= { nullptr };
	RectTransform*	MonsterTR_Front[5]		= { nullptr };
	Slider*			MonsterSlider[5]		= { nullptr };
	ImageFont*		MonsterFont[5]			= { nullptr };
	GameObject*		MonsterObject[5]		= { nullptr };
	bool			MonsterActiveUI[5]		= { false };

	bool isHit		= false;
	bool isCombo	= false;
	float Alpha = 0;

	float PosX = 0;
	float PosY = 350;

	bool isActive = true;
};

