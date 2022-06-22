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
	void Set_HP_Now(int Number);
	void Set_HP_Max(int Number);
	void Set_Emagin_Now(int Number);
	void Set_Emagin_Max(int Number);

	
	void Set_Monster_UI_ON(void* Emagin);		//���� UI�� �Ҵ�
	void Set_Monster_UI_OFF(void* Emagin);		//���� UI�� ����
	void Set_Monster_UI_SET_DATA(void* Emagin);	//���� �����͸� �ѱ��

	void Set_ALLRender(bool Render);

	
private:
	//���� �Լ�
	void Create_Combo_UI();
	void Create_HP_UI();
	void Create_Skill_UI();
	void Create_Emagin_Color_UI();
	void Create_Emagin_UI();
	void Create_Effect_UI();
	void Create_Monster_UI();


	void Update_Hit_Check();	//���İ� ó��
	void Update_Combo_Check();	//ũ�� ó��
	
	int GetActiveMonsterUI();
	bool UseCheck(GameObject* Obj);

	RectTransform*	RT[15]		= { nullptr };
	Image*			Images[15]	= { nullptr };
	FontImage*		mFont[5]	= { nullptr };
	ComboFont*		mCombo		= nullptr;


	//���� UI��
	Image*			Monster_Emagin_Back[5]	= { nullptr };
	Image*			Monster_Emagin_Front[5] = { nullptr };
	RectTransform*	MonSterTR_Back[5]		= { nullptr };
	RectTransform*	MonsterTR_Front[5]		= { nullptr };
	Slider*			MonsterSlider[5]		= { nullptr };
	ImageFont*		MonsterFont[5]			= { nullptr };
	GameObject*		MonsterObject[5]		= { nullptr };
	bool			MonsterActiveUI[5]		= { false };

	bool isHit  = false;
	float Alpha = 0;

	float PosX = 0;
	float PosY = 350;


	//��Ʈ ����Ʈ
	const int Font_Emagin_Now		= 0;
	const int Font_Emagin_Max		= 1;
	const int Font_HP_Now			= 2;
	const int Font_HP_Max			= 3;
	const int Font_Monster_Emagin	= 4;

	//�̹��� HP ����
	const int Image_Space_Index		= 0;
	const int Image_HP_Space		= 1;
	const int Image_HP_Slider		= 2;

	//�̹��� �̸��� ����
	const int Image_Emagin_Back		= 3;
	const int Image_Emagin_Front	= 4;
	const int Image_Emagin_Change	= 5;
	const int Image_Emagin_Space	= 6;

	//�̹��� ��ų ����
	const int Image_Skill_Space		= 7;
	const int Image_Skill_E			= 8;
	const int Image_Skill_Right		= 9;

	//�̹��� ��ų ����
	const int Image_PlayerHit_Effect	= 10;

	const int Image_Monster_Emagin_Back	= 11;
	const int Image_Monster_Emagin_Front= 12;

	const int Image_Monster_HP_Front	= 13;
	const int Image_Monster_HP_Back		= 14;
};

