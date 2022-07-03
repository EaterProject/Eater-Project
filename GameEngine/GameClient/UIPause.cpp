#include "UIPause.h"
#include "EaterEngineAPI.h"
#include "GameObject.h"
#include "Image.h"
#include "Button.h"
#include "ClientTypeOption.h"
#include "MessageManager.h"

UIPause::UIPause()
{

}

UIPause::~UIPause()
{

}

void UIPause::Awake()
{
	GameObject* object = nullptr;
	Button* button = nullptr;

	// 일시정지
	object = Instance_Image();
	PauseImage = object->GetComponent<Image>();
	PauseImage->SetPivot(PIVOT_TYPE::PIVOT_MIDDLE_CENTER);
	PauseImage->SetPosition(0.0, -150.0f);
	PauseImage->SetTexture("Pause_Main");

	// 계속하기
	object = Instance_Button();
	button = object->GetComponent<Button>();
	button->SetPivot(PIVOT_TYPE::PIVOT_MIDDLE_CENTER);
	button->SetTexture("Pause_Resume_Off");
	button->PushTextureList("Pause_Resume_Off");
	button->PushTextureList("Pause_Resume_On");
	button->SetPosition(0, 0);
	button->PushEvent(Eater::Bind(&UIPause::SetResumeIn, this), Button::IN_BUTTON);
	button->PushEvent(Eater::Bind(&UIPause::SetResumeOut, this), Button::OUT_BUTTON);
	button->PushEvent(Eater::Bind(&UIPause::SetResumeClick, this), Button::UP_BUTTON);
	PauseButton[PAUSE::RESUME] = button;

	// 설정
	object = Instance_Button();
	button = object->GetComponent<Button>();
	button->SetPivot(PIVOT_TYPE::PIVOT_MIDDLE_CENTER);
	button->SetTexture("Pause_Set_Off");
	button->PushTextureList("Pause_Set_Off");
	button->PushTextureList("Pause_Set_On");
	button->SetPosition(0, 75);
	button->PushEvent(Eater::Bind(&UIPause::SetOptionIn, this), Button::IN_BUTTON);
	button->PushEvent(Eater::Bind(&UIPause::SetOptionOut, this), Button::OUT_BUTTON);
	button->PushEvent(Eater::Bind(&UIPause::SetOptionClick, this), Button::UP_BUTTON);
	PauseButton[PAUSE::OPTION] = button;

	// 게임 종료
	object = Instance_Button();
	button = object->GetComponent<Button>();
	button->SetPivot(PIVOT_TYPE::PIVOT_MIDDLE_CENTER);
	button->SetTexture("Pause_Exit_Off");
	button->PushTextureList("Pause_Exit_Off");
	button->PushTextureList("Pause_Exit_On");
	button->SetPosition(0, 150);
	button->PushEvent(Eater::Bind(&UIPause::SetExitIn, this), Button::IN_BUTTON);
	button->PushEvent(Eater::Bind(&UIPause::SetExitOut, this), Button::OUT_BUTTON);
	button->PushEvent(Eater::Bind(&UIPause::SetExitClick, this), Button::UP_BUTTON);
	PauseButton[PAUSE::EXIT] = button;
}

void UIPause::SetPauseUIActive(bool active)
{
	PauseImage->SetActive(active);

	for (int i = 0; i < 3; i++)
	{
		PauseButton[i]->SetActive(active);
	}

	if (active)
	{
		// 전체 화면 블러..
		SetFullScreenBlur(true, 2);
	}
}

void UIPause::SetResumeIn()
{
	Sound_Play_SFX("UI_Button_Overlay");
	PauseButton[PAUSE::RESUME]->SetTexture("Pause_Resume_On");
}

void UIPause::SetResumeOut()
{
	PauseButton[PAUSE::RESUME]->SetTexture("Pause_Resume_Off");
}

void UIPause::SetResumeClick()
{
	Sound_Play_SFX("UI_Button_Click");

	/// 게임상으로 복귀
	MessageManager::GetGM()->SEND_Message(TARGET_GLOBAL, MESSAGE_GLOBAL_RESUME);

	// 전체 화면 블러 해제..
	SetFullScreenBlur(false);

	// 혹시 모르니 모든 키 리셋..
	KeyReset();
}

void UIPause::SetOptionIn()
{
	Sound_Play_SFX("UI_Button_Overlay");
	PauseButton[PAUSE::OPTION]->SetTexture("Pause_Set_On");
}

void UIPause::SetOptionOut()
{
	PauseButton[PAUSE::OPTION]->SetTexture("Pause_Set_Off");
}

void UIPause::SetOptionClick()
{
	Sound_Play_SFX("UI_Button_Click");

	int target = MESSAGE_GLOBAL_PAUSE;

	/// 옵션으로 이동
	MessageManager::GetGM()->SEND_Message(TARGET_GLOBAL, MESSAGE_GLOBAL_OPTION, &target);
}

void UIPause::SetExitIn()
{
	Sound_Play_SFX("UI_Button_Overlay");
	PauseButton[PAUSE::EXIT]->SetTexture("Pause_Exit_On");
}

void UIPause::SetExitOut()
{
	PauseButton[PAUSE::EXIT]->SetTexture("Pause_Exit_Off");
}

void UIPause::SetExitClick()
{
	Sound_Play_SFX("UI_Button_Click");

	/// 게임 종료
	MessageManager::GetGM()->SEND_Message(TARGET_GLOBAL, MESSAGE_GLOBAL_GAMEEND);
}
