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

	//���� UI
	void Set_Monster_UI_ON(void* Emagin);		//���� UI�� �Ҵ�
	void Set_Monster_UI_OFF(void* Emagin);		//���� UI�� ����
	void Set_Monster_UI_SET_DATA(void* Emagin);	//���� �����͸� �ѱ��

	//�÷��̾� ���� ü�� ����
	void Set_Player_HP(int Number);
	void Set_Player_HP_Max(int Number);
	void Set_Player_UI_Active(bool Active);

	//�÷��̾� �̸��� ����
	void Set_Player_Emagin(int Number);
	void Set_Player_Emagin_Max(int Number);

	//�÷��̾� ��ų ����
	void Set_Skill_E(float value);
	void Set_Skill_MR(float value);
	void Set_Skill_SPC(float value);

	//�ʷ� ���� ī��Ʈ ����
	void Set_Pure_Mana_Count(int Number);
	void Set_Core_Mana_Count(int Number);

	//��� �ؽ���
	void Set_Drone_Text(int Number);

	void Set_InGameUI_Active(bool Active);
private:
	//���� �Լ�
	void Create_Player_Emagin(float X,float Y);
	void Create_Player_HP(float X,float Y);
	void Create_Player_Mana(float X,float Y);
	void Create_Player_Emagin_Color(float X, float Y);
	void Create_Dron_Text(float X, float Y);
	void Create_Monster_UI();
	void Create_Skill_UI(float X,float Y);

	//���߱� �Լ�
	void Active_Player_HP(bool Active);
	void Active_Player_Emagin(bool Active);
	void Active_Player_Emagin_Color(bool Active);
	void Active_Player_Skill(bool Active);
	void Active_Mana_Count(bool Active);
	void Active_Dron_Text(bool Active);


	void Create_Combo_UI();

	void Update_Combo_Check();	//ũ�� ó��
	
	int GetActiveMonsterUI();
	void SetMonsterUIActive(int index, bool IsActive);
	void SetMonsterUIDraw(int index, bool IsActive);
	bool UseCheck(GameObject* Obj);

	ComboFont*		mCombo		= nullptr;

	//�÷��̾� ü�� UI
	ImageFont*		Player_HP[2];			//�÷��̾� ü�� ��Ʈ
	Slider*			Player_HP_Slider;		//�÷��̾� ü�� ��Ʈ
	Image*			Player_HP_BAR;
	RectTransform*	Player_RECT;

	//�÷��̾� �̸��� UI
	ImageFont*		Player_EMAGIN[2];	//�÷��̾� �̸��� ��Ʈ
	RectTransform*	Player_EMAGIN_RECT[2];
	Image*			Player_EMAGIN_CHANGE;
	Image*			Player_EMAGIN_BAR;

	//�÷��̾� �̸��� Color UI
	Image*			Player_EMAGIN_COLOR[2];

	//�÷��̾� ������
	ImageFont*		Player_MANA[2];
	Image*			Player_MANA_ICON;
	RectTransform*	Player_MANA_RECT;

	//��� �ؽ���
	Image*			Dron_Text;
	RectTransform*	Dron_Rect;

	//�÷��̾� ��ų
	Slider*			Player_Skill[3];

	//���� UI��
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

