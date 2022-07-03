#include "UIManual.h"
#include "EaterEngineAPI.h"
#include "GameObject.h"
#include "Image.h"
#include "Button.h"
#include "ClientTypeOption.h"
#include "MessageManager.h"

UIManual::UIManual()
{

}

UIManual::~UIManual()
{

}

void UIManual::Awake()
{
	GameObject* object = nullptr;

	// 매뉴얼 이미지..
	object = Instance_Image();
	ManualImage = object->GetComponent<Image>();
	ManualImage->SetPivot(PIVOT_TYPE::PIVOT_MIDDLE_CENTER);
	ManualImage->PushTextureList("Manual_Key");
	ManualImage->PushTextureList("Manual_Combo");
	ManualImage->SetTexture("Manual_Key");

	// 다음 버튼..
	object = Instance_Button();
	ReturnButton[KEY] = object->GetComponent<Button>();
	ReturnButton[KEY]->SetPivot(PIVOT_TYPE::PIVOT_RIGHT_BOTTOM);
	ReturnButton[KEY]->SetTexture("Manual_Next_Off");
	ReturnButton[KEY]->PushTextureList("Manual_Next_Off");
	ReturnButton[KEY]->PushTextureList("Manual_Next_On");
	ReturnButton[KEY]->SetPosition(-50, -100);
	ReturnButton[KEY]->PushEvent(Eater::Bind(&UIManual::SetNextIn, this), Button::IN_BUTTON);
	ReturnButton[KEY]->PushEvent(Eater::Bind(&UIManual::SetNextOut, this), Button::OUT_BUTTON);
	ReturnButton[KEY]->PushEvent(Eater::Bind(&UIManual::SetNextClick, this), Button::UP_BUTTON);

	// 나가기 버튼..
	object = Instance_Button();
	ReturnButton[COMBO] = object->GetComponent<Button>();
	ReturnButton[COMBO]->SetPivot(PIVOT_TYPE::PIVOT_RIGHT_BOTTOM);
	ReturnButton[COMBO]->SetTexture("Manual_Exit_Off");
	ReturnButton[COMBO]->PushTextureList("Manual_Exit_Off");
	ReturnButton[COMBO]->PushTextureList("Manual_Exit_On");
	ReturnButton[COMBO]->SetPosition(-50, -100);
	ReturnButton[COMBO]->PushEvent(Eater::Bind(&UIManual::SetReturnIn, this), Button::IN_BUTTON);
	ReturnButton[COMBO]->PushEvent(Eater::Bind(&UIManual::SetReturnOut, this), Button::OUT_BUTTON);
	ReturnButton[COMBO]->PushEvent(Eater::Bind(&UIManual::SetReturnClick, this), Button::UP_BUTTON);
}

void UIManual::SetManualUIActive(bool active)
{
	ManualImage->SetActive(active);

	if (active)
	{
		// 전체 화면 블러..
		SetFullScreenBlur(true, 2);

		ReturnButton[KEY]->SetActive(true);
		ReturnButton[COMBO]->SetActive(false);
	}
	else
	{
		ReturnButton[KEY]->SetActive(false);
		ReturnButton[COMBO]->SetActive(false);
	}
}

void UIManual::SetNextIn()
{
	Sound_Play_SFX("UI_Button_Overlay");
	ReturnButton[KEY]->SetTexture("Manual_Next_On");
}

void UIManual::SetNextOut()
{
	ReturnButton[KEY]->SetTexture("Manual_Next_Off");
}

void UIManual::SetNextClick()
{
	ManualImage->SetTexture("Manual_Combo");

	ReturnButton[KEY]->SetActive(false);
	ReturnButton[COMBO]->SetActive(true);
}

void UIManual::SetReturnIn()
{
	Sound_Play_SFX("UI_Button_Overlay");
	ReturnButton[COMBO]->SetTexture("Manual_Exit_On");
}

void UIManual::SetReturnOut()
{
	ReturnButton[COMBO]->SetTexture("Manual_Exit_Off");
}

void UIManual::SetReturnClick()
{
	ManualImage->SetTexture("Manual_Key");

	ManualImage->SetActive(false);
	ReturnButton[KEY]->SetActive(false);
	ReturnButton[COMBO]->SetActive(false);

	// 화면 블러 해제..
	SetFullScreenBlur(false);

	// 혹시 모르니 모든 키 리셋..
	KeyReset();

	// 게임상으로 복귀..
	MessageManager::GetGM()->SEND_Message(TARGET_GLOBAL, MESSAGE_GLOBAL_RESUME);
}
