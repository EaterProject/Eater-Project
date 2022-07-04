#include "UIStore.h"
#include "EaterEngineAPI.h"

//Component
#include "GameObject.h"
#include "Image.h"
#include "ImageFont.h"
#include "Button.h"
#include "RectTransform.h"

#include "ClientTypeOption.h"
#include "MessageManager.h"

UIStore::UIStore()
{

}

UIStore::~UIStore()
{

}

void UIStore::Awake()
{
	GameObject* Object = nullptr;

	Object = Instance_Image();
	StoreTitle = Object->GetComponent<Image>();
	StoreTitle->SetTexture("Store");

	//마나 폰트
	Object = Instance_ImageFont();
	StoreMana[0] = Object->GetComponent<ImageFont>();
	StoreMana[0]->SetTexture("number_");
	StoreMana[0]->SetPivot(PIVOT_RIGHT_TOP);
	StoreMana[0]->SetFontCount(2);
	StoreMana[0]->SetOffset(35);
	StoreMana[0]->SetScale(1, 1);
	StoreMana[0]->SetPosition(-165, 65);
	StoreMana[0]->SetColor(65, 197, 198);
	StoreMana[0]->SetLayer(100);

	Object = Instance_ImageFont();
	StoreMana[1] = Object->GetComponent<ImageFont>();
	StoreMana[1]->SetTexture("number_");
	StoreMana[1]->SetPivot(PIVOT_RIGHT_TOP);
	StoreMana[1]->SetFontCount(2);
	StoreMana[1]->SetOffset(35);
	StoreMana[1]->SetScale(1, 1);
	StoreMana[1]->SetPosition(-65, 65);
	StoreMana[1]->SetColor(213, 138, 19);
	StoreMana[1]->SetLayer(100);

	// 강화 버튼..
	for (int i = 0; i < 6; i++)
	{
		Object = Instance_Button();
		Buttons[i]	= Object->GetComponent<Button>();
		Images[i]	= Object->GetComponent<Image>();
		mRect[i]	= Object->GetComponent<RectTransform>();

		mRect[i]->SetPivot(PIVOT_MIDDLE_LEFT);
		mRect[i]->SetPosition(OffsetX, OffsetY + (141 * i));
		Images[i]->SetLayer(1);

		SelectItem[i] = false;

		switch (i)
		{
		case 0:
			mRect[i]->SetPosition(OffsetX, 191 -540);
			Images[i]->PushTextureList("ChangeEmagin_OFF");
			Images[i]->PushTextureList("ChangeEmagin_ON");
			Images[i]->SetTexture("ChangeEmagin_OFF");
			ImageName_Off[i] = "ChangeEmagin_OFF";
			ImageName_Text[i] = "VendingM_Index_MaxEmagin";

			Buttons[i]->PushEvent(Eater::Bind(&UIStore::ChangeEmaginCount_Button_Enter, this), Button::IN_BUTTON);
			Buttons[i]->PushEvent(Eater::Bind(&UIStore::ChangeEmaginCount_Button_Exit, this), Button::OUT_BUTTON);
			Buttons[i]->PushEvent(Eater::Bind(&UIStore::ChangeEmaginCount_Button_Click, this), Button::DOWN_BUTTON);
			break;
		case 1:
			mRect[i]->SetPosition(OffsetX, 332 - 540);
			Images[i]->PushTextureList("MaxHP_OFF");
			Images[i]->PushTextureList("MaxHP_ON");
			Images[i]->SetTexture("MaxHP_OFF");
			ImageName_Off[i] = "MaxHP_OFF";
			ImageName_Text[i] = "VendingM_Index_MaxHp";

			Buttons[i]->PushEvent(Eater::Bind(&UIStore::MaxHP_Button_Enter, this), Button::IN_BUTTON);
			Buttons[i]->PushEvent(Eater::Bind(&UIStore::MaxHP_Button_Exit, this), Button::OUT_BUTTON);
			Buttons[i]->PushEvent(Eater::Bind(&UIStore::MaxHP_Button_Click, this), Button::DOWN_BUTTON);
			break;
		case 2:
			mRect[i]->SetPosition(OffsetX,485 - 540);
			Images[i]->PushTextureList("AttackSpeed_OFF");
			Images[i]->PushTextureList("AttackSpeed_ON");
			Images[i]->SetTexture("AttackSpeed_OFF");
			ImageName_Off[i] = "AttackSpeed_OFF";
			ImageName_Text[i] = "VendingM_Index_AttackSpeed";

			Buttons[i]->PushEvent(Eater::Bind(&UIStore::AttackSpeed_Button_Enter, this), Button::IN_BUTTON);
			Buttons[i]->PushEvent(Eater::Bind(&UIStore::AttackSpeed_Button_Exit, this), Button::OUT_BUTTON);
			Buttons[i]->PushEvent(Eater::Bind(&UIStore::AttackSpeed_Button_Click, this), Button::DOWN_BUTTON);
			break;
		case 3:
			mRect[i]->SetPosition(OffsetX, 635 - 540);
			Images[i]->PushTextureList("MoveSpeed_OFF");
			Images[i]->PushTextureList("MoveSpeed_ON");
			Images[i]->SetTexture("MoveSpeed_OFF");
			ImageName_Off[i] = "MoveSpeed_OFF";
			ImageName_Text[i] = "VendingM_Index_MoveSpeed";

			Buttons[i]->PushEvent(Eater::Bind(&UIStore::MoveSpeed_Button_Enter, this), Button::IN_BUTTON);
			Buttons[i]->PushEvent(Eater::Bind(&UIStore::MoveSpeed_Button_Exit, this), Button::OUT_BUTTON);
			Buttons[i]->PushEvent(Eater::Bind(&UIStore::MoveSpeed_Button_Click, this), Button::DOWN_BUTTON);
			break;
		case 4:
			mRect[i]->SetPosition( 1612 ,818 - 540);
			Images[i]->PushTextureList("Purchase_OFF");
			Images[i]->PushTextureList("Purchase_ON");
			Images[i]->SetTexture("Purchase_OFF");
			ImageName_Off[i] = "Purchase_OFF";

			Buttons[i]->PushEvent(Eater::Bind(&UIStore::Purchase_Item_Button_Enter, this), Button::IN_BUTTON);
			Buttons[i]->PushEvent(Eater::Bind(&UIStore::Purchase_Item_Button_Exit, this), Button::OUT_BUTTON);
			Buttons[i]->PushEvent(Eater::Bind(&UIStore::Purchase_Item_Button_Click, this), Button::DOWN_BUTTON);
			break;
		case 5:
			mRect[i]->SetPosition( 1612,909 - 540);
			Images[i]->PushTextureList("Store_OUT_OFF");
			Images[i]->PushTextureList("Store_OUT_ON");
			Images[i]->SetTexture("Store_OUT_OFF");
			ImageName_Off[i] = "Store_OUT_OFF";

			Buttons[i]->PushEvent(Eater::Bind(&UIStore::OutStore_Button_Enter, this), Button::IN_BUTTON);
			Buttons[i]->PushEvent(Eater::Bind(&UIStore::OutStore_Button_Exit, this), Button::OUT_BUTTON);
			Buttons[i]->PushEvent(Eater::Bind(&UIStore::OutStore_Button_Click, this), Button::DOWN_BUTTON);
			break;
		}
	}

	// 툴팁 이미지..
	Object = Instance_Image();
	ItemText = Object->GetComponent<Image>();
	ItemText->SetPivot(PIVOT_TYPE::PIVOT_LEFT_BOTTOM);
	ItemText->SetPosition(100, -100);
	ItemText->PushTextureList("VendingM_Index_AttackSpeed");
	ItemText->PushTextureList("VendingM_Index_MaxEmagin");
	ItemText->PushTextureList("VendingM_Index_MaxHp");
	ItemText->PushTextureList("VendingM_Index_MoveSpeed");
	ItemText->SetTexture("VendingM_Index_MaxEmagin");

	Set_Store_Active(false);
}

void UIStore::Set_Store_Active(bool Active)
{
	for (int i = 0; i < 6; i++)
	{
		Buttons[i]->SetActive(Active);
	}

	for (int i = 0; i < 2; i++)
	{
		StoreMana[i]->SetActive(Active);
	}

	StoreTitle->SetActive(Active);
	ItemText->SetActive(false);

	if (Active)
	{
		// 전체 화면 블러..
		SetFullScreenBlur(true, 2);
	}
}

void UIStore::Set_Pure_Mana_Count(int Number)
{
	StoreMana[0]->SetFontNumber(Number);
}

void UIStore::Set_Core_Mana_Count(int Number)
{
	StoreMana[1]->SetFontNumber(Number);
}

void UIStore::ChangeEmaginCount_Button_Exit()
{
	//Emagin 교체 횟수 증가
	if (SelectItem[CHANGE_EMAGIN] == false)
	{
		Images[CHANGE_EMAGIN]->SetTexture("ChangeEmagin_OFF");
	}
}

void UIStore::ChangeEmaginCount_Button_Enter()
{
	//Emagin 교체 횟수 증가
	Images[CHANGE_EMAGIN]->SetTexture("ChangeEmagin_ON");
	Sound_Play_SFX("UI_Button_Overlay");
}

void UIStore::ChangeEmaginCount_Button_Click()
{
	//Emagin 교체 횟수 증가
	SelectItem[CHANGE_EMAGIN] ^= true;

	if (SelectItem[CHANGE_EMAGIN])
	{
		Images[CHANGE_EMAGIN]->SetTexture("ChangeEmagin_ON");
		ItemText->SetActive(true);
		ItemText->SetTexture(ImageName_Text[CHANGE_EMAGIN]);

		SelectItemID = CHANGE_EMAGIN;
		ResetItem(CHANGE_EMAGIN);
	}
	else
	{
		Images[CHANGE_EMAGIN]->SetTexture("ChangeEmagin_OFF");
		ItemText->SetActive(false);

		SelectItemID = -1;
	}

	Sound_Play_SFX("UI_Button_Click");
}

void UIStore::MaxHP_Button_Exit()
{
	//Max HP 증가
	if (SelectItem[MAX_HP] == false)
	{
		Images[MAX_HP]->SetTexture("MaxHP_OFF");
	}
}

void UIStore::MaxHP_Button_Enter()
{
	//Max HP 증가
	Images[MAX_HP]->SetTexture("MaxHP_ON");
	Sound_Play_SFX("UI_Button_Overlay");
}

void UIStore::MaxHP_Button_Click()
{
	//Max HP 증가
	SelectItem[MAX_HP] ^= true;

	if (SelectItem[MAX_HP])
	{
		Images[MAX_HP]->SetTexture("MaxHP_ON");
		ItemText->SetActive(true);
		ItemText->SetTexture(ImageName_Text[MAX_HP]);

		SelectItemID = MAX_HP;
		ResetItem(MAX_HP);
	}
	else
	{
		Images[MAX_HP]->SetTexture("MaxHP_OFF");
		ItemText->SetActive(false);

		SelectItemID = -1;
	}

	Sound_Play_SFX("UI_Button_Click");
}

void UIStore::AttackSpeed_Button_Exit()
{
	if (SelectItem[ATTACK_SPEED] == false)
	{
		Images[ATTACK_SPEED]->SetTexture("AttackSpeed_OFF");
	}
}

void UIStore::AttackSpeed_Button_Enter()
{
	Images[ATTACK_SPEED]->SetTexture("AttackSpeed_ON");
	Sound_Play_SFX("UI_Button_Overlay");
}

void UIStore::AttackSpeed_Button_Click()
{
	//공격 속도 증가
	SelectItem[ATTACK_SPEED] ^= true;

	if (SelectItem[ATTACK_SPEED])
	{
		Images[ATTACK_SPEED]->SetTexture("AttackSpeed_ON");
		ItemText->SetActive(true);
		ItemText->SetTexture(ImageName_Text[ATTACK_SPEED]);

		SelectItemID = ATTACK_SPEED;
		ResetItem(ATTACK_SPEED);
	}
	else
	{
		Images[ATTACK_SPEED]->SetTexture("AttackSpeed_OFF");
		ItemText->SetActive(false);

		SelectItemID = -1;
	}

	Sound_Play_SFX("UI_Button_Click");
}

void UIStore::MoveSpeed_Button_Exit()
{
	if (SelectItem[MOVE_SPEED] == false)
	{
		Images[MOVE_SPEED]->SetTexture("MoveSpeed_OFF");
	}
}

void UIStore::MoveSpeed_Button_Enter()
{
	Images[MOVE_SPEED]->SetTexture("MoveSpeed_ON");
	Sound_Play_SFX("UI_Button_Overlay");
}

void UIStore::MoveSpeed_Button_Click()
{
	// 이동속도 증가..
	SelectItem[MOVE_SPEED] ^= true;

	if (SelectItem[MOVE_SPEED])
	{
		Images[MOVE_SPEED]->SetTexture("MoveSpeed_ON");
		ItemText->SetActive(true);
		ItemText->SetTexture(ImageName_Text[MOVE_SPEED]);

		SelectItemID = MOVE_SPEED;
		ResetItem(MOVE_SPEED);
	}
	else
	{
		Images[MOVE_SPEED]->SetTexture("MoveSpeed_OFF");
		ItemText->SetActive(false);

		SelectItemID = -1;
	}

	Sound_Play_SFX("UI_Button_Click");
}

void UIStore::OutStore_Button_Exit()
{
	Images[STORE_OFF]->SetTexture("Store_OUT_OFF");
}

void UIStore::OutStore_Button_Enter()
{
	Images[STORE_OFF]->SetTexture("Store_OUT_ON");
	Sound_Play_SFX("UI_Button_Overlay");
}

void UIStore::OutStore_Button_Click()
{
	Sound_Play_SFX("UI_Button_Click");
	
	// 해당 아이템 관련 이미지 초기화..
	for (int i = 0; i < 6; i++)
	{
		SelectItem[i] = false;
		Images[i]->SetTexture(ImageName_Off[i]);
	}

	// 선택중인 아이템 아이디 초기화..
	SelectItemID = -1;

	// 스토어 UI 해제..
	Set_Store_Active(false);

	// 전체화면 블러 해제..
	SetFullScreenBlur(false);

	MessageManager::GetGM()->SEND_Message(TARGET_GLOBAL, MESSAGE_GLOBAL_RESUME);
}

void UIStore::Purchase_Item_Button_Exit()
{
	Images[PURCHASE_ITEM]->SetTexture("Purchase_OFF");
}

void UIStore::Purchase_Item_Button_Enter()
{
	Sound_Play_SFX("UI_Button_Overlay");
	Images[PURCHASE_ITEM]->SetTexture("Purchase_ON");
}

void UIStore::Purchase_Item_Button_Click()
{
	// 만약 선택중인 아이템이 없다면..
	if (SelectItemID == -1)
	{
		Sound_Play_SFX("UI_Button_Click");
		return;
	}

	Sound_Play_SFX("VendingMachine");

	/// 선택한 아이템에 대한 구매 및 능력치 적용..
	switch (SelectItemID)
	{
	case CHANGE_EMAGIN:
		MessageManager::GetGM()->SEND_Message(TARGET_PLAYER, MESSAGE_PLAYER_UPGRADE_EMAGIN);
		break;
	case MAX_HP:
		MessageManager::GetGM()->SEND_Message(TARGET_PLAYER, MESSAGE_PLAYER_UPGRADE_HP);
		break;
	case ATTACK_SPEED:
		MessageManager::GetGM()->SEND_Message(TARGET_PLAYER, MESSAGE_PLAYER_UPGRADE_ATTACK);
		break;
	case MOVE_SPEED:
		MessageManager::GetGM()->SEND_Message(TARGET_PLAYER, MESSAGE_PLAYER_UPGRADE_MOVE);
		break;
	default:
		break;
	}

}

void UIStore::ResetItem(int nowItem)
{
	for (int i = 0; i < 6; i++)
	{
		if (i == nowItem) continue;

		SelectItem[i] = false;
		Buttons[i]->SetTexture(ImageName_Off[i]);
	}
}
