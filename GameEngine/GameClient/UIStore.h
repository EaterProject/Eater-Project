#pragma once
#include <Component.h>

class Image;
class Button;
class RectTransform;
class UIStore :public Component
{
public:
	UIStore();
	virtual ~UIStore();

	void Awake() override;
	void Start() override;
	void Update() override;

	void Set_Store_Active(bool Active);

	
	void PurchaseItem(int Type);	//아이템 구매
private:
	void Store_ON();		//상점 들어가기
	void Store_OFF();		//상점 나가기

	void ChangeEmaginCount_Button_Exit();
	void ChangeEmaginCount_Button_Enter();
	void ChangeEmaginCount_Button_Click();

	void MaxHP_Button_Exit();
	void MaxHP_Button_Enter();
	void MaxHP_Button_Click();

	void AttackSpeed_Button_Exit();
	void AttackSpeed_Button_Enter();
	void AttackSpeed_Button_Click();

	void MoveSpeed_Button_Exit();
	void MoveSpeed_Button_Enter();
	void MoveSpeed_Button_Click();

	void GateOpen_Button_Exit();
	void GateOpen_Button_Enter();
	void GateOpen_Button_Click();

	void OutStore_Button_Exit();
	void OutStore_Button_Enter();
	void OutStore_Button_Click();

	void Purchase_Item_Button_Exit();
	void Purchase_Item_Button_Enter();
	void Purchase_Item_Button_Click();
private:
	Button* Buttons[7];
	Image*	Images[7];
	RectTransform* mRect[7];

	Image* StoreTitle;

	const int OffsetX = 454;
	const int OffsetY = 191;

	const int ChangeEmagin	= 0;
	const int MaxHP			= 1;
	const int AttackSpeed	= 2;
	const int MoveSpeed		= 3;
	const int GateOpen		= 4;
	const int Purchase_Item	= 5;
	const int StoreOFF		= 6;
};

