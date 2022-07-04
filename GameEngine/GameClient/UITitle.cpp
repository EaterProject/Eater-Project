#include "UITitle.h"
#include "GameObject.h"
#include "EaterEngineAPI.h"
#include "Utility.h"
#include "EaterEngineAPI.h"
#include "MessageManager.h"

#include <functional>

#include "Button.h"
#include "Image.h"
#include "RectTransform.h"

UITitle::UITitle()
{



}

UITitle::~UITitle()
{



}

void UITitle::Awake()
{
	GameObject* Title_object = Instance_Image();
	TitleMainImage = Title_object->GetComponent<Image>();
	Title_IMG_Rect = Title_object->GetComponent<RectTransform>();

	TitleMainImage->SetTexture("Title_Eater");
	Title_IMG_Rect->SetPivot(PIVOT_MIDDLE_LEFT);
	Title_IMG_Rect->SetPosition(ButtonUI_PosX, -50);

	float Y = 0;
	for (int i = 0; i < 3; i++)
	{
		GameObject* Object = Instance_Button();
		Title_Button[i] = Object->GetComponent<Button>();
		Title_IMG[i] = Object->GetComponent<Image>();
		Title_Rect[i] = Object->GetComponent<RectTransform>();

		//Eater::Bind(&Component::Awake, mComponent)
		Title_Rect[i]->SetPivot(PIVOT_MIDDLE_LEFT);

		switch (i)
		{
		case 0:
			Title_IMG[i]->PushTextureList("Title_GameStart_OFF");
			Title_IMG[i]->PushTextureList("Title_GameStart_ON");
			Title_IMG[i]->SetTexture("Title_GameStart_OFF");

			Title_Button[i]->PushEvent(Eater::Bind(&UITitle::StartButton_Enter, this) , Button::IN_BUTTON);
			Title_Button[i]->PushEvent(Eater::Bind(&UITitle::StartButton_Exit, this),  Button::OUT_BUTTON);
			Title_Button[i]->PushEvent(Eater::Bind(&UITitle::StartButton_Click, this),  Button::DOWN_BUTTON);

			Y += 60;
			break;
		case 1:
			Title_IMG[i]->PushTextureList("Title_GameEnd_OFF");
			Title_IMG[i]->PushTextureList("Title_GameEnd_ON");
			Title_IMG[i]->SetTexture("Title_GameEnd_OFF");

			Title_Button[i]->PushEvent(Eater::Bind(&UITitle::EndButton_Enter, this), Button::IN_BUTTON);
			Title_Button[i]->PushEvent(Eater::Bind(&UITitle::EndButton_Exit, this), Button::OUT_BUTTON);
			Title_Button[i]->PushEvent(Eater::Bind(&UITitle::EndButton_Click, this), Button::DOWN_BUTTON);

			Y += 60;
			break;
		case 2:
			Title_IMG[i]->PushTextureList("Title_GameOption_OFF");
			Title_IMG[i]->PushTextureList("Title_GameOption_ON");
			Title_IMG[i]->SetTexture("Title_GameOption_OFF");

			Title_Button[i]->PushEvent(Eater::Bind(&UITitle::OptionButton_Enter, this), Button::IN_BUTTON);
			Title_Button[i]->PushEvent(Eater::Bind(&UITitle::OptionButton_Exit, this), Button::OUT_BUTTON);
			Title_Button[i]->PushEvent(Eater::Bind(&UITitle::OptionButton_Click, this), Button::DOWN_BUTTON);

			Y += 60;
			break;
		}
		Title_Rect[i]->SetPosition(ButtonUI_PosX, Y);
	}
}

void UITitle::Update()
{
	if (IsStart == true)
	{
		ButtonUI_PosX -= GetDeltaTime() * 600;
		for (int i = 0; i < 3; i++)
		{
			Title_Rect[i]->SetPosition(ButtonUI_PosX, Title_Rect[i]->Position.y);
			Title_IMG_Rect->SetPosition(ButtonUI_PosX, Title_IMG_Rect->Position.y);
		}

		if (ButtonUI_PosX < -600)
		{
			IsStart = false;
			ButtonUI_PosX = -600;
		}
	}

	if (IsFade == true)
	{
		IsFade = false;
		MessageManager::GetGM()->SEND_Message(TARGET_UI, MESSAGE_UI_FADE_OUT);
		//MessageManager::GetGM()->SEND_Message(TARGET_CAMERA_MANAGER, MESSAGE_CAMERA_CINEMATIC_GAME_START, &IsGameStart);
		MessageManager::GetGM()->SEND_Message(TARGET_GLOBAL, MESSAGE_GLOBAL_GAME_START);
	}

	//if (IsGameStart == true)
	//{
	//	IsGameStart = false;
	//	MessageManager::GetGM()->SEND_Message(TARGET_UI, MESSAGE_UI_FADE_IN, &IsGameStartFadeIn);
	//}
	//
	//if (IsGameStartFadeIn == true)
	//{
	//	IsGameStartFadeIn = false;
	//	MessageManager::GetGM()->SEND_Message(TARGET_GLOBAL, MESSAGE_GLOBAL_GAME_START);
	//	MessageManager::GetGM()->SEND_Message(TARGET_UI, MESSAGE_UI_FADE_OUT, &IsGameStartFadeOut);
	//}
	//
	//if (IsGameStartFadeOut == true)
	//{
	//	IsGameStartFadeOut = false;
	//}
}

void UITitle::Set_TitleUI_Active(bool Active)
{
	for (int i = 0; i < 3; i++)
	{
		Title_Button[i]->SetActive(Active);
		Title_Rect[i]->SetPosition(200, Title_Rect[i]->Position.y);
	}
	TitleMainImage->SetActive(Active);
	Title_IMG_Rect->SetPosition(200, Title_IMG_Rect->Position.y);

	if (Active)
	{
		SetColorGradingBlendFactor(1.0f);
	}
}

void UITitle::StartButton_Enter()
{
	Sound_Play_SFX("UI_Button_Overlay");
	Title_IMG[0]->SetTexture("Title_GameStart_ON");
}

void UITitle::StartButton_Click()
{
	IsStart = true;
	Sound_Play_SFX("UI_Button_Click");
	Sound_Stop_BGM();

	MessageManager::GetGM()->SEND_Message(TARGET_UI, MESSAGE_UI_FADE_IN, &IsFade);
	MessageManager::GetGM()->SEND_Message(TARGET_CAMERA_MANAGER, MESSAGE_CAMERA_CINEMATIC_TITLE);

	// 혹시 모르니 모든 키 리셋..
	KeyReset();
}

void UITitle::StartButton_Exit()
{
	Title_IMG[0]->SetTexture("Title_GameStart_OFF");
}

void UITitle::OptionButton_Enter()
{
	Sound_Play_SFX("UI_Button_Overlay");
	Title_IMG[2]->SetTexture("Title_GameOption_ON");
}

void UITitle::OptionButton_Click()
{
	int Message_Number = MESSAGE_GLOBAL_TITLE;
	Sound_Play_SFX("UI_Button_Click");
	MessageManager::GetGM()->SEND_Message(TARGET_GLOBAL, MESSAGE_GLOBAL_OPTION, &Message_Number);
}

void UITitle::OptionButton_Exit()
{
	Title_IMG[2]->SetTexture("Title_GameOption_OFF");
}

void UITitle::EndButton_Enter()
{
	Sound_Play_SFX("UI_Button_Overlay");
	Title_IMG[1]->SetTexture("Title_GameEnd_ON");
}

void UITitle::EndButton_Click()
{
	Sound_Play_SFX("UI_Button_Click");

	/// 게임 종료
	MessageManager::GetGM()->SEND_Message(TARGET_GLOBAL, MESSAGE_GLOBAL_GAME_END);
}

void UITitle::EndButton_Exit()
{
	Title_IMG[1]->SetTexture("Title_GameEnd_OFF");
}
