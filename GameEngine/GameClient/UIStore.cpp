#include "UIStore.h"
#include "EaterEngineAPI.h"


//Component
#include "GameObject.h"
#include "Image.h"
#include "Button.h"
#include "RectTransform.h"

UIStore::UIStore()
{

}

UIStore::~UIStore()
{

}

void UIStore::Awake()
{
	for (int i = 0; i < 7; i++)
	{
		GameObject* Object = Instance_Button();
		Buttons[i]	= Object->GetComponent<Button>();
		Images[i]	= Object->GetComponent<Image>();
		mRect[i]	= Object->GetComponent<RectTransform>();

		mRect[i]->SetPivot(PIVOT_MIDDLE_LEFT);
		mRect[i]->SetPosition(OffsetX, OffsetY + (141 * i));
		Images[i]->SetLayer(1);

		switch (i)
		{
		case 0:
			mRect[i]->SetPosition(OffsetX, 191 -540);
			Images[i]->PushTextureList("ChangeEmagin_OFF");
			Images[i]->PushTextureList("ChangeEmagin_ON");
			Images[i]->SetTexture("ChangeEmagin_OFF");

			Buttons[i]->PushEvent(Eater::Bind(&UIStore::ChangeEmaginCount_Button_Enter, this), Button::IN_BUTTON);
			Buttons[i]->PushEvent(Eater::Bind(&UIStore::ChangeEmaginCount_Button_Exit, this), Button::OUT_BUTTON);
			Buttons[i]->PushEvent(Eater::Bind(&UIStore::ChangeEmaginCount_Button_Click, this), Button::DOWN_BUTTON);
			break;
		case 1:
			mRect[i]->SetPosition(OffsetX, 332 - 540);
			Images[i]->PushTextureList("MaxHP_OFF");
			Images[i]->PushTextureList("MaxHP_ON");
			Images[i]->SetTexture("MaxHP_OFF");

			Buttons[i]->PushEvent(Eater::Bind(&UIStore::MaxHP_Button_Enter, this), Button::IN_BUTTON);
			Buttons[i]->PushEvent(Eater::Bind(&UIStore::MaxHP_Button_Exit, this), Button::OUT_BUTTON);
			Buttons[i]->PushEvent(Eater::Bind(&UIStore::MaxHP_Button_Click, this), Button::DOWN_BUTTON);
			break;
		case 2:
			mRect[i]->SetPosition(OffsetX,485 - 540);
			Images[i]->PushTextureList("AttackSpeed_OFF");
			Images[i]->PushTextureList("AttackSpeed_ON");
			Images[i]->SetTexture("AttackSpeed_OFF");

			Buttons[i]->PushEvent(Eater::Bind(&UIStore::AttackSpeed_Button_Enter, this), Button::IN_BUTTON);
			Buttons[i]->PushEvent(Eater::Bind(&UIStore::AttackSpeed_Button_Exit, this), Button::OUT_BUTTON);
			Buttons[i]->PushEvent(Eater::Bind(&UIStore::AttackSpeed_Button_Click, this), Button::DOWN_BUTTON);
			break;
		case 3:
			mRect[i]->SetPosition(OffsetX, 635 - 540);
			Images[i]->PushTextureList("MoveSpeed_OFF");
			Images[i]->PushTextureList("MoveSpeed_ON");
			Images[i]->SetTexture("MoveSpeed_OFF");

			Buttons[i]->PushEvent(Eater::Bind(&UIStore::MoveSpeed_Button_Enter, this), Button::IN_BUTTON);
			Buttons[i]->PushEvent(Eater::Bind(&UIStore::MoveSpeed_Button_Exit, this), Button::OUT_BUTTON);
			Buttons[i]->PushEvent(Eater::Bind(&UIStore::MoveSpeed_Button_Click, this), Button::DOWN_BUTTON);
			break;
		case 4:
			mRect[i]->SetPosition(940, 191 - 540);
			Images[i]->PushTextureList("GateOpen_OFF");
			Images[i]->PushTextureList("GateOpen_ON");
			Images[i]->SetTexture("GateOpen_OFF");

			Buttons[i]->PushEvent(Eater::Bind(&UIStore::GateOpen_Button_Enter, this), Button::IN_BUTTON);
			Buttons[i]->PushEvent(Eater::Bind(&UIStore::GateOpen_Button_Exit, this), Button::OUT_BUTTON);
			Buttons[i]->PushEvent(Eater::Bind(&UIStore::GateOpen_Button_Click, this), Button::DOWN_BUTTON);
			break;
		case 5:
			mRect[i]->SetPosition( 1612 ,818 - 540);
			Images[i]->PushTextureList("Purchase_OFF");
			Images[i]->PushTextureList("Purchase_ON");
			Images[i]->SetTexture("Purchase_OFF");

			Buttons[i]->PushEvent(Eater::Bind(&UIStore::Purchase_Item_Button_Enter, this), Button::IN_BUTTON);
			Buttons[i]->PushEvent(Eater::Bind(&UIStore::Purchase_Item_Button_Exit, this), Button::OUT_BUTTON);
			Buttons[i]->PushEvent(Eater::Bind(&UIStore::Purchase_Item_Button_Click, this), Button::DOWN_BUTTON);
			break;
		case 6:
			mRect[i]->SetPosition( 1612,909 - 540);
			Images[i]->PushTextureList("Store_OUT_OFF");
			Images[i]->PushTextureList("Store_OUT_ON");
			Images[i]->SetTexture("Store_OUT_OFF");

			Buttons[i]->PushEvent(Eater::Bind(&UIStore::OutStore_Button_Enter, this), Button::IN_BUTTON);
			Buttons[i]->PushEvent(Eater::Bind(&UIStore::OutStore_Button_Exit, this), Button::OUT_BUTTON);
			Buttons[i]->PushEvent(Eater::Bind(&UIStore::OutStore_Button_Click, this), Button::DOWN_BUTTON);
			break;
		}
	}

	GameObject* Title = Instance_Image();
	StoreTitle = Title->GetComponent<Image>();
	StoreTitle->SetTexture("Store");

	Set_Store_Active(false);
}

void UIStore::Start()
{
}

void UIStore::Update()
{


}

void UIStore::Set_Store_Active(bool Active)
{
	if (Active == true)
	{
		Store_ON();
	}
	else
	{
		Store_OFF();
	}
}

void UIStore::Store_ON()
{
	for (int i = 0; i < 7; i++)
	{
		Buttons[i]->gameobject->SetActive(true);
	}
	StoreTitle->gameobject->SetActive(true);
}

void UIStore::Store_OFF()
{
	for (int i = 0; i < 7; i++)
	{
		Buttons[i]->gameobject->SetActive(false);
	}
	StoreTitle->gameobject->SetActive(false);
}

void UIStore::PurchaseItem(int Type)
{

}

void UIStore::ChangeEmaginCount_Button_Exit()
{
	//Emagin 교체 횟수 증가
	Images[ChangeEmagin]->SetTexture("ChangeEmagin_OFF");
}

void UIStore::ChangeEmaginCount_Button_Enter()
{
	//Emagin 교체 횟수 증가
	Images[ChangeEmagin]->SetTexture("ChangeEmagin_ON");
	Sound_Play_SFX("UI_Button_Click");
}

void UIStore::ChangeEmaginCount_Button_Click()
{
	//Emagin 교체 횟수 증가
}

void UIStore::MaxHP_Button_Exit()
{
	Images[MaxHP]->SetTexture("MaxHP_OFF");
	//Max HP 증가
}

void UIStore::MaxHP_Button_Enter()
{
	Sound_Play_SFX("UI_Button_Click");
	Images[MaxHP]->SetTexture("MaxHP_ON");
	//Max HP 증가
}

void UIStore::MaxHP_Button_Click()
{
	//Max HP 증가
}

void UIStore::AttackSpeed_Button_Exit()
{
	Images[AttackSpeed]->SetTexture("AttackSpeed_OFF");
}

void UIStore::AttackSpeed_Button_Enter()
{
	Sound_Play_SFX("UI_Button_Click");
	Images[AttackSpeed]->SetTexture("AttackSpeed_ON");
}

void UIStore::AttackSpeed_Button_Click()
{


}

void UIStore::MoveSpeed_Button_Exit()
{
	Images[MoveSpeed]->SetTexture("MoveSpeed_OFF");
}

void UIStore::MoveSpeed_Button_Enter()
{
	Sound_Play_SFX("UI_Button_Click");
	Images[MoveSpeed]->SetTexture("MoveSpeed_ON");
}

void UIStore::MoveSpeed_Button_Click()
{

}

void UIStore::GateOpen_Button_Exit()
{
	Images[GateOpen]->SetTexture("GateOpen_OFF");
}

void UIStore::GateOpen_Button_Enter()
{
	Sound_Play_SFX("UI_Button_Click");
	Images[GateOpen]->SetTexture("GateOpen_ON");
}

void UIStore::GateOpen_Button_Click()
{

}

void UIStore::OutStore_Button_Exit()
{
	Images[StoreOFF]->SetTexture("Store_OUT_OFF");
}

void UIStore::OutStore_Button_Enter()
{
	Sound_Play_SFX("UI_Button_Click");
	Images[StoreOFF]->SetTexture("Store_OUT_ON");
}

void UIStore::OutStore_Button_Click()
{
	Store_OFF();
}

void UIStore::Purchase_Item_Button_Exit()
{
	Images[Purchase_Item]->SetTexture("Purchase_OFF");
}

void UIStore::Purchase_Item_Button_Enter()
{
	Sound_Play_SFX("UI_Button_Click");
	Images[Purchase_Item]->SetTexture("Purchase_ON");
}

void UIStore::Purchase_Item_Button_Click()
{

}
