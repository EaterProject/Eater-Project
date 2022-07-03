#include "UIStore.h"
#include "EaterEngineAPI.h"

//Component
#include "GameObject.h"
#include "Image.h"
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
	for (int i = 0; i < 6; i++)
	{
		GameObject* Object = Instance_Button();
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

	GameObject* Title = Instance_Image();
	StoreTitle = Title->GetComponent<Image>();
	StoreTitle->SetTexture("Store");

	GameObject* TextImage = Instance_Image();
	ItemText = TextImage->GetComponent<Image>();
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

	StoreTitle->SetActive(Active);
	ItemText->SetActive(false);

	if (Active)
	{
		// 전체 화면 블러..
		SetFullScreenBlur(true, 2);
	}
}

void UIStore::PurchaseItem(int Type)
{

}

void UIStore::ChangeEmaginCount_Button_Exit()
{
	//Emagin 교체 횟수 증가
	if (SelectItem[ChangeEmagin] == false)
	{
		Images[ChangeEmagin]->SetTexture("ChangeEmagin_OFF");
	}
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
	SelectItem[ChangeEmagin] ^= true;

	if (SelectItem[ChangeEmagin])
	{
		Images[ChangeEmagin]->SetTexture("ChangeEmagin_ON");
		ItemText->SetActive(true);
		ItemText->SetTexture(ImageName_Text[ChangeEmagin]);
		ResetItem(ChangeEmagin);
	}
	else
	{
		ItemText->SetActive(false);
		Images[ChangeEmagin]->SetTexture("ChangeEmagin_OFF");
	}
}

void UIStore::MaxHP_Button_Exit()
{
	//Max HP 증가
	if (SelectItem[MaxHP] == false)
	{
		Images[MaxHP]->SetTexture("MaxHP_OFF");
	}
}

void UIStore::MaxHP_Button_Enter()
{
	//Max HP 증가
	Images[MaxHP]->SetTexture("MaxHP_ON");
	Sound_Play_SFX("UI_Button_Click");
}

void UIStore::MaxHP_Button_Click()
{
	//Max HP 증가
	SelectItem[MaxHP] ^= true;

	if (SelectItem[MaxHP])
	{
		Images[MaxHP]->SetTexture("MaxHP_ON");
		ItemText->SetActive(true);
		ItemText->SetTexture(ImageName_Text[MaxHP]);
		ResetItem(MaxHP);
	}
	else
	{
		ItemText->SetActive(false);
		Images[MaxHP]->SetTexture("MaxHP_OFF");
	}
}

void UIStore::AttackSpeed_Button_Exit()
{
	if (SelectItem[AttackSpeed] == false)
	{
		Images[AttackSpeed]->SetTexture("AttackSpeed_OFF");
	}
}

void UIStore::AttackSpeed_Button_Enter()
{
	Images[AttackSpeed]->SetTexture("AttackSpeed_ON");
	Sound_Play_SFX("UI_Button_Click");
}

void UIStore::AttackSpeed_Button_Click()
{
	//공격 속도 증가
	SelectItem[AttackSpeed] ^= true;

	if (SelectItem[AttackSpeed])
	{
		Images[AttackSpeed]->SetTexture("AttackSpeed_ON");
		ItemText->SetActive(true);
		ItemText->SetTexture(ImageName_Text[AttackSpeed]);
		ResetItem(AttackSpeed);
	}
	else
	{
		ItemText->SetActive(false);
		Images[AttackSpeed]->SetTexture("AttackSpeed_OFF");
	}
}

void UIStore::MoveSpeed_Button_Exit()
{
	if (SelectItem[MoveSpeed] == false)
	{
		Images[MoveSpeed]->SetTexture("MoveSpeed_OFF");
	}
}

void UIStore::MoveSpeed_Button_Enter()
{
	Images[MoveSpeed]->SetTexture("MoveSpeed_ON");
	Sound_Play_SFX("UI_Button_Click");
}

void UIStore::MoveSpeed_Button_Click()
{
	// 이동속도 증가..
	SelectItem[MoveSpeed] ^= true;

	if (SelectItem[MoveSpeed])
	{
		Images[MoveSpeed]->SetTexture("MoveSpeed_ON");
		ItemText->SetActive(true);
		ItemText->SetTexture(ImageName_Text[MoveSpeed]);
		ResetItem(MoveSpeed);
	}
	else
	{
		ItemText->SetActive(false);
		Images[MoveSpeed]->SetTexture("MoveSpeed_OFF");
	}
}

void UIStore::OutStore_Button_Exit()
{
	Images[StoreOFF]->SetTexture("Store_OUT_OFF");
}

void UIStore::OutStore_Button_Enter()
{
	Images[StoreOFF]->SetTexture("Store_OUT_ON");
	Sound_Play_SFX("UI_Button_Click");
}

void UIStore::OutStore_Button_Click()
{
	// 해당 아이템 관련 이미지 초기화..
	for (int i = 0; i < 6; i++)
	{
		SelectItem[i] = false;
		Images[i]->SetTexture(ImageName_Off[i]);
	}

	// 스토어 UI 해제..
	Set_Store_Active(false);

	// 전체화면 블러 해제..
	SetFullScreenBlur(false);

	MessageManager::GetGM()->SEND_Message(TARGET_GLOBAL, MESSAGE_GLOBAL_RESUME);
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
	/// 선택한 아이템에 대한 구매 및 능력치 적용..



	// 해당 아이템 관련 이미지 초기화..
	for (int i = 0; i < 6; i++)
	{
		SelectItem[i] = false;
		Images[i]->SetTexture(ImageName_Off[i]);
	}

	// 스토어 UI 해제..
	Set_Store_Active(false);

	// 전체화면 블러 해제..
	SetFullScreenBlur(false);

	MessageManager::GetGM()->SEND_Message(TARGET_GLOBAL, MESSAGE_GLOBAL_RESUME);
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
