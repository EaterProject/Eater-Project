#include "UIOption.h"
#include "EaterEngineAPI.h"
#include "GameObject.h"
#include "RectTransform.h"
#include "Image.h"
#include "Button.h"
#include "ImageFont.h"
#include "ClientTypeOption.h"
#include "MessageManager.h"

UIOption::UIOption()
	: PrevTarget(-1)
{
}

UIOption::~UIOption()
{
}

void UIOption::Awake()
{
	// 그래픽 옵션 가져오기..
	GraphicOption = GetRenderOptionData();

	// 값이 바뀌는 시간..
	ValueChangeTime = 0.1f;

	GameObject* object = nullptr;
	Button* button = nullptr;
	Image* image = nullptr;
	ImageFont* font = nullptr;

	// 배경 화면
	object = Instance_Image();
	BackGroundImage = object->GetComponent<Image>();
	BackGroundImage->SetPivot(PIVOT_TYPE::PIVOT_MIDDLE_CENTER);
	BackGroundImage->SetTexture("Setting_Main");

	// 돌아가기
	object = Instance_Button();
	ReturnButton = object->GetComponent<Button>();
	ReturnButton->SetPivot(PIVOT_TYPE::PIVOT_RIGHT_BOTTOM);
	ReturnButton->SetTexture("Setting_Back_Off");
	ReturnButton->PushTextureList("Setting_Back_Off");
	ReturnButton->PushTextureList("Setting_Back_On");
	ReturnButton->SetPosition(-50, -100);
	ReturnButton->PushEvent(Eater::Bind(&UIOption::SetReturnIn, this), Button::IN_BUTTON);
	ReturnButton->PushEvent(Eater::Bind(&UIOption::SetReturnOut, this), Button::OUT_BUTTON);
	ReturnButton->PushEvent(Eater::Bind(&UIOption::SetReturnClick, this), Button::UP_BUTTON);

	// 버튼 생성..
	for (int i = 0; i < OPTION_MAX; i++)
	{
		object = Instance_Button();
		button = object->GetComponent<Button>();
		button->SetPivot(PIVOT_TYPE::PIVOT_MIDDLE_CENTER);
		button->SetTexture("Setting_Left");
		ClickButton[i].Left = button;

		object = Instance_Button();
		button = object->GetComponent<Button>();
		button->SetPivot(PIVOT_TYPE::PIVOT_MIDDLE_CENTER);
		button->SetTexture("Setting_Right");
		ClickButton[i].Right = button;
	}

	float left_offset = -295.0f;
	float right_offset = 125.0f;
	float start_height = -322.5f;
	float height_offset = 64.0f;
	float height_first_line = 97.5f;
	float height_second_line = 87.5f;

	// 왼쪽 오른쪽 버튼 위치 설정..
	ClickButton[SCREEN_SIZE].Value = 0;
	ClickButtonSetting(SCREEN_SIZE,left_offset, right_offset, start_height);
	start_height += height_first_line;
	ClickButtonSetting(MASTER_VOL,	left_offset, right_offset, start_height);
	start_height += height_offset;
	ClickButtonSetting(BACKGROUND_VOL,	left_offset, right_offset, start_height);
	start_height += height_offset;
	ClickButtonSetting(SFX_VOL, left_offset, right_offset, start_height);
	start_height += height_second_line;
	ClickButtonSetting(GP_IBL, left_offset, right_offset, start_height);
	start_height += height_offset;
	ClickButtonSetting(GP_SSAO, left_offset, right_offset, start_height);
	start_height += height_offset;
	ClickButtonSetting(GP_SHADOW, left_offset, right_offset, start_height);
	start_height += height_offset;
	ClickButtonSetting(GP_FOG, left_offset, right_offset, start_height);
	start_height += height_offset;
	ClickButtonSetting(GP_BLOOM, left_offset, right_offset, start_height);
	start_height += height_offset;
	ClickButtonSetting(GP_COLORGRADING, left_offset, right_offset, start_height);
	start_height += height_offset;
	ClickButtonSetting(GP_FXAA, left_offset, right_offset, start_height);
	start_height += height_offset;
	ClickButtonSetting(GP_HDR, left_offset, right_offset, start_height);

	// 시작 위치 초기화..
	start_height = -322.5f;
	float text_offset = -80.0f;

	// 화면 텍스트 이미지 생성..
	object = Instance_Button();
	image = object->GetComponent<Image>();
	image->SetPivot(PIVOT_TYPE::PIVOT_MIDDLE_CENTER);
	image->SetPosition(text_offset, start_height);
	image->PushTextureList("Setting_Window");
	image->PushTextureList("Setting_FullScreen");
	image->SetTexture("Setting_Window");
	ScreenText = image;

	start_height += height_first_line;

	// 사운드 폰트 이미지 생성..
	object = Instance_ImageFont();
	font = object->GetComponent<ImageFont>();
	font->SetFontPivot(FONT_PIVOT::FONT_CENTER);
	font->SetTexture("number_");
	font->SetPivot(PIVOT_TYPE::PIVOT_MIDDLE_CENTER);
	font->SetFontCount(3, false);
	font->SetOffset(18);
	font->SetScale(0.5f, 0.5f);
	font->SetPosition(text_offset, start_height);
	SoundText[0] = font;

	start_height += height_offset;

	object = Instance_ImageFont();
	font = object->GetComponent<ImageFont>();
	font->SetFontPivot(FONT_PIVOT::FONT_CENTER);
	font->SetTexture("number_");
	font->SetPivot(PIVOT_TYPE::PIVOT_MIDDLE_CENTER);
	font->SetFontCount(3, false);
	font->SetOffset(18);
	font->SetScale(0.5f, 0.5f);
	font->SetPosition(text_offset, start_height);
	SoundText[1] = font;

	start_height += height_offset;

	object = Instance_ImageFont();
	font = object->GetComponent<ImageFont>();
	font->SetFontPivot(FONT_PIVOT::FONT_CENTER);
	font->SetTexture("number_");
	font->SetPivot(PIVOT_TYPE::PIVOT_MIDDLE_CENTER);
	font->SetFontCount(3, false);
	font->SetOffset(18);
	font->SetScale(0.5f, 0.5f);
	font->SetPosition(text_offset, start_height);
	SoundText[2] = font; 


	start_height += height_second_line;

	// 그래픽 텍스트 이미지 생성..
	for (int i = 0; i < 8; i++)
	{
		object = Instance_Button();
		image = object->GetComponent<Image>();
		image->SetPivot(PIVOT_TYPE::PIVOT_MIDDLE_CENTER);
		image->SetPosition(text_offset, start_height);
		image->PushTextureList("Setting_Active");
		image->PushTextureList("Setting_Disable");
		image->SetTexture("Setting_Active");

		GraphicText[i] = image;

		start_height += height_offset;
	}

}										 

void UIOption::Start()
{
	SoundText[0]->SetFontNumber(100);
	SoundText[1]->SetFontNumber(100);
	SoundText[2]->SetFontNumber(100);

	ClickButton[MASTER_VOL].Value = 100;
	ClickButton[BACKGROUND_VOL].Value = 100;
	ClickButton[SFX_VOL].Value = 100;
}

void UIOption::Set_OptionUI_Active(bool active)
{
	BackGroundImage->SetActive(active);
	ScreenText->SetActive(active);
	ReturnButton->SetActive(active);

	for (int i = 0; i < 3; i++)
	{
		SoundText[i]->SetActive(active);
	}

	for (int i = 0; i < 8; i++)
	{
		GraphicText[i]->SetActive(active);
	}

	for (int i = 0; i < 12; i++)
	{
		ClickButton[i].Left->SetActive(active);
		ClickButton[i].Right->SetActive(active);
	}

	// 전체 화면 블러..
	if (active)
	{
		SetFullScreenBlur(true, 2);
	}
}

void UIOption::SetPrevTarget(int target)
{
	PrevTarget = target;
}

void UIOption::ClickButtonSetting(OPTION option, float left_x, float right_x, float y)
{
	// 해당 이미지 위치 설정..
	ClickButton[option].Left->SetPosition(left_x, y);
	ClickButton[option].Left->SetBoundaryOffset(-20.0f, 20.0f, 0.0f, 0.0f);
	ClickButton[option].Right->SetPosition(right_x, y);
	ClickButton[option].Right->SetBoundaryOffset(-20.0f, 20.0f, 0.0f, 0.0f);

	// 해당 버튼에 대한 처리 추가..
	switch (option)
	{
	case UIOption::GP_IBL:
		ClickButton[option].Left->PushEvent(Eater::Bind(&UIOption::SetGraphicIBL, this), Button::UP_BUTTON);
		ClickButton[option].Right->PushEvent(Eater::Bind(&UIOption::SetGraphicIBL, this), Button::UP_BUTTON);
		break;
	case UIOption::GP_SSAO:
		ClickButton[option].Left->PushEvent(Eater::Bind(&UIOption::SetGraphicSSAO, this), Button::UP_BUTTON);
		ClickButton[option].Right->PushEvent(Eater::Bind(&UIOption::SetGraphicSSAO, this), Button::UP_BUTTON);
		break;
	case UIOption::GP_SHADOW:
		ClickButton[option].Left->PushEvent(Eater::Bind(&UIOption::SetGraphicShadow, this), Button::UP_BUTTON);
		ClickButton[option].Right->PushEvent(Eater::Bind(&UIOption::SetGraphicShadow, this), Button::UP_BUTTON);
		break;
	case UIOption::GP_FOG:
		ClickButton[option].Left->PushEvent(Eater::Bind(&UIOption::SetGraphicFog, this), Button::UP_BUTTON);
		ClickButton[option].Right->PushEvent(Eater::Bind(&UIOption::SetGraphicFog, this), Button::UP_BUTTON);
		break;
	case UIOption::GP_BLOOM:
		ClickButton[option].Left->PushEvent(Eater::Bind(&UIOption::SetGraphicBloom, this), Button::UP_BUTTON);
		ClickButton[option].Right->PushEvent(Eater::Bind(&UIOption::SetGraphicBloom, this), Button::UP_BUTTON);
		break;
	case UIOption::GP_COLORGRADING:
		ClickButton[option].Left->PushEvent(Eater::Bind(&UIOption::SetGraphicColorGrading, this), Button::UP_BUTTON);
		ClickButton[option].Right->PushEvent(Eater::Bind(&UIOption::SetGraphicColorGrading, this), Button::UP_BUTTON);
		break;
	case UIOption::GP_FXAA:
		ClickButton[option].Left->PushEvent(Eater::Bind(&UIOption::SetGraphicFXAA, this), Button::UP_BUTTON);
		ClickButton[option].Right->PushEvent(Eater::Bind(&UIOption::SetGraphicFXAA, this), Button::UP_BUTTON);
		break;
	case UIOption::GP_HDR:
		ClickButton[option].Left->PushEvent(Eater::Bind(&UIOption::SetGraphicHDR, this), Button::UP_BUTTON);
		ClickButton[option].Right->PushEvent(Eater::Bind(&UIOption::SetGraphicHDR, this), Button::UP_BUTTON);
	break;
	case UIOption::SCREEN_SIZE:
		ClickButton[option].Left->PushEvent(Eater::Bind(&UIOption::SetScreenSize, this), Button::UP_BUTTON);
		ClickButton[option].Right->PushEvent(Eater::Bind(&UIOption::SetScreenSize, this), Button::UP_BUTTON);
		break;
	case UIOption::MASTER_VOL:
		ClickButton[option].Left->PushEvent(Eater::Bind(&UIOption::SetMasterVolumeDown, this), Button::PRESS_DOWN_BUTTON);
		ClickButton[option].Right->PushEvent(Eater::Bind(&UIOption::SetMasterVolumeUp, this), Button::PRESS_DOWN_BUTTON);
		ClickButton[option].Left->PushEvent(Eater::Bind(&UIOption::SetClearTime, this), Button::UP_BUTTON);
		ClickButton[option].Right->PushEvent(Eater::Bind(&UIOption::SetClearTime, this), Button::UP_BUTTON);
		break;
	case UIOption::BACKGROUND_VOL:
		ClickButton[option].Left->PushEvent(Eater::Bind(&UIOption::SetBackGroundVolumeDown, this), Button::PRESS_DOWN_BUTTON);
		ClickButton[option].Right->PushEvent(Eater::Bind(&UIOption::SetBackGroundVolumeUp, this), Button::PRESS_DOWN_BUTTON);
		ClickButton[option].Left->PushEvent(Eater::Bind(&UIOption::SetClearTime, this), Button::UP_BUTTON);
		ClickButton[option].Right->PushEvent(Eater::Bind(&UIOption::SetClearTime, this), Button::UP_BUTTON);
		break;
	case UIOption::SFX_VOL:
		ClickButton[option].Left->PushEvent(Eater::Bind(&UIOption::SetSFXVolumeDown, this), Button::PRESS_DOWN_BUTTON);
		ClickButton[option].Right->PushEvent(Eater::Bind(&UIOption::SetSFXVolumeUp, this), Button::PRESS_DOWN_BUTTON);
		ClickButton[option].Left->PushEvent(Eater::Bind(&UIOption::SetClearTime, this), Button::UP_BUTTON);
		ClickButton[option].Right->PushEvent(Eater::Bind(&UIOption::SetClearTime, this), Button::UP_BUTTON);
		break;
	case UIOption::OPTION_MAX:
		break;
	default:
		break;
	}
}

void UIOption::SetScreenSize()
{
	Sound_Play_SFX("UI_Button_Click");

	ClickButton[SCREEN_SIZE].Value ^= true;

	if (ClickButton[SCREEN_SIZE].Value)
	{
		ScreenText->SetTexture("Setting_FullScreen");
		SetWindowFullScreen();
	}
	else
	{
		ScreenText->SetTexture("Setting_Window");
		
		SetWindowSize(1920, 1080);
	}
}

void UIOption::SetMasterVolumeUp()
{
	// 첫 클릭시 증가..
	if (IsClick == false)
	{
		Sound_Play_SFX("UI_Button_Click");
	
		IsClick = true;

		ClickButton[MASTER_VOL].Value += 1;

		if (ClickButton[MASTER_VOL].Value > 100)
		{
			ClickButton[MASTER_VOL].Value = 100;
		}

		SoundText[0]->SetFontNumber(ClickButton[MASTER_VOL].Value);
	}

	NowTime += GetDeltaTime();

	// 설정한 누르고있는 시간이 지낫다면 증가..
	if (NowTime < ValueChangeTime) return;

	Sound_Play_SFX("UI_Button_Click");

	NowTime = 0.0f;

	ClickButton[MASTER_VOL].Value += 1;

	if (ClickButton[MASTER_VOL].Value > 100)
	{
		ClickButton[MASTER_VOL].Value = 100;
	}

	// 사운드 설정..
	Sound_VolumeSet_Master(ClickButton[MASTER_VOL].Value * 0.01f);

	// 볼륨에 맞는 숫자 출력..
	SoundText[0]->SetFontNumber(ClickButton[MASTER_VOL].Value);
}

void UIOption::SetMasterVolumeDown()
{
	// 첫 클릭시 감소..
	if (IsClick == false)
	{
		Sound_Play_SFX("UI_Button_Click");

		IsClick = true;

		ClickButton[MASTER_VOL].Value -= 1;

		if (ClickButton[MASTER_VOL].Value < 0)
		{
			ClickButton[MASTER_VOL].Value = 0;
		}

		SoundText[0]->SetFontNumber(ClickButton[MASTER_VOL].Value);
	}

	NowTime += GetDeltaTime();

	// 설정한 누르고있는 시간이 지낫다면 감소..
	if (NowTime < ValueChangeTime) return;

	Sound_Play_SFX("UI_Button_Click");

	NowTime = 0.0f;

	ClickButton[MASTER_VOL].Value -= 1;

	if (ClickButton[MASTER_VOL].Value < 0)
	{
		ClickButton[MASTER_VOL].Value = 0;
	}

	// 사운드 설정..
	Sound_VolumeSet_Master(ClickButton[MASTER_VOL].Value * 0.01f);

	// 볼륨에 맞는 숫자 출력..
	SoundText[0]->SetFontNumber(ClickButton[MASTER_VOL].Value);
}

void UIOption::SetBackGroundVolumeUp()
{
	// 첫 클릭시 증가..
	if (IsClick == false)
	{
		Sound_Play_SFX("UI_Button_Click");

		IsClick = true;

		ClickButton[BACKGROUND_VOL].Value += 1;

		if (ClickButton[BACKGROUND_VOL].Value > 100)
		{
			ClickButton[BACKGROUND_VOL].Value = 100;
		}

		SoundText[1]->SetFontNumber(ClickButton[BACKGROUND_VOL].Value);
	}

	NowTime += GetDeltaTime();

	// 설정한 누르고있는 시간이 지낫다면 증가..
	if (NowTime < ValueChangeTime) return;

	Sound_Play_SFX("UI_Button_Click");

	NowTime = 0.0f;

	ClickButton[BACKGROUND_VOL].Value += 1;

	if (ClickButton[BACKGROUND_VOL].Value > 100)
	{
		ClickButton[BACKGROUND_VOL].Value = 100;
	}

	// 사운드 설정..
	Sound_VolumeSet_BGM(ClickButton[MASTER_VOL].Value * 0.01f);

	// 볼륨에 맞는 숫자 출력..
	SoundText[1]->SetFontNumber(ClickButton[BACKGROUND_VOL].Value);
}

void UIOption::SetBackGroundVolumeDown()
{
	// 첫 클릭시 감소..
	if (IsClick == false)
	{
		Sound_Play_SFX("UI_Button_Click");

		IsClick = true;

		ClickButton[BACKGROUND_VOL].Value -= 1;

		if (ClickButton[BACKGROUND_VOL].Value < 0)
		{
			ClickButton[BACKGROUND_VOL].Value = 0;
		}

		SoundText[1]->SetFontNumber(ClickButton[BACKGROUND_VOL].Value);
	}

	NowTime += GetDeltaTime();

	// 설정한 누르고있는 시간이 지낫다면 감소..
	if (NowTime < ValueChangeTime) return;

	Sound_Play_SFX("UI_Button_Click");

	NowTime = 0.0f;

	ClickButton[BACKGROUND_VOL].Value -= 1;

	if (ClickButton[BACKGROUND_VOL].Value < 0)
	{
		ClickButton[BACKGROUND_VOL].Value = 0;
	}

	// 사운드 설정..
	Sound_VolumeSet_BGM(ClickButton[MASTER_VOL].Value * 0.01f);

	// 볼륨에 맞는 숫자 출력..
	SoundText[1]->SetFontNumber(ClickButton[BACKGROUND_VOL].Value);
}

void UIOption::SetSFXVolumeUp()
{
	// 첫 클릭시 증가..
	if (IsClick == false)
	{
		Sound_Play_SFX("UI_Button_Click");

		IsClick = true;

		ClickButton[SFX_VOL].Value += 1;
		
		if (ClickButton[SFX_VOL].Value > 100)
		{
			ClickButton[SFX_VOL].Value = 100;
		}

		SoundText[2]->SetFontNumber(ClickButton[SFX_VOL].Value);
	}

	NowTime += GetDeltaTime();

	// 설정한 누르고있는 시간이 지낫다면 증가..
	if (NowTime < ValueChangeTime) return;

	Sound_Play_SFX("UI_Button_Click");

	NowTime = 0.0f;

	ClickButton[SFX_VOL].Value += 1;

	if (ClickButton[SFX_VOL].Value > 100)
	{
		ClickButton[SFX_VOL].Value = 100;
	}

	// 사운드 설정..
	Sound_VolumeSet_SFX(ClickButton[MASTER_VOL].Value * 0.01f);

	// 볼륨에 맞는 숫자 출력..
	SoundText[2]->SetFontNumber(ClickButton[SFX_VOL].Value);
}

void UIOption::SetSFXVolumeDown()
{
	// 첫 클릭시 감소..
	if (IsClick == false)
	{
		Sound_Play_SFX("UI_Button_Click");
	
		IsClick = true;

		ClickButton[SFX_VOL].Value -= 1;

		if (ClickButton[SFX_VOL].Value < 0)
		{
			ClickButton[SFX_VOL].Value = 0;
		}

		SoundText[2]->SetFontNumber(ClickButton[SFX_VOL].Value);
	}
	
	NowTime += GetDeltaTime();

	// 설정한 누르고있는 시간이 지낫다면 감소..
	if (NowTime < ValueChangeTime) return;

	Sound_Play_SFX("UI_Button_Click");
	
	NowTime = 0.0f;

	ClickButton[SFX_VOL].Value -= 1;

	if (ClickButton[SFX_VOL].Value < 0)
	{
		ClickButton[SFX_VOL].Value = 0;
	}

	// 사운드 설정..
	Sound_VolumeSet_SFX(ClickButton[MASTER_VOL].Value * 0.01f);

	// 볼륨에 맞는 숫자 출력..
	SoundText[2]->SetFontNumber(ClickButton[SFX_VOL].Value);
}

void UIOption::SetClearTime()
{
	IsClick = false;
	NowTime = 0.0f;
}

void UIOption::SetGraphicIBL()
{
	Sound_Play_SFX("UI_Button_Click");

	GraphicOption->RenderingOption ^= RENDER_OPTION::RENDER_IBL;

	if (GraphicOption->RenderingOption & RENDER_OPTION::RENDER_IBL)
	{
		GraphicText[GP_IBL]->SetTexture("Setting_Active");
	}
	else
	{
		GraphicText[GP_IBL]->SetTexture("Setting_Disable");
	}

	RenderSetting();
}

void UIOption::SetGraphicSSAO()
{
	Sound_Play_SFX("UI_Button_Click");

	GraphicOption->RenderingOption ^= RENDER_OPTION::RENDER_SSAO;

	if (GraphicOption->RenderingOption & RENDER_OPTION::RENDER_SSAO)
	{
		GraphicText[GP_SSAO]->SetTexture("Setting_Active");
	}
	else
	{
		GraphicText[GP_SSAO]->SetTexture("Setting_Disable");
	}

	RenderSetting();
}

void UIOption::SetGraphicShadow()
{
	Sound_Play_SFX("UI_Button_Click");

	GraphicOption->RenderingOption ^= RENDER_OPTION::RENDER_SHADOW;

	if (GraphicOption->RenderingOption & RENDER_OPTION::RENDER_SHADOW)
	{
		GraphicText[GP_SHADOW]->SetTexture("Setting_Active");
	}
	else
	{
		GraphicText[GP_SHADOW]->SetTexture("Setting_Disable");
	}

	RenderSetting();
}

void UIOption::SetGraphicFog()
{
	Sound_Play_SFX("UI_Button_Click");

	GraphicOption->RenderingOption ^= RENDER_OPTION::RENDER_FOG;

	if (GraphicOption->RenderingOption & RENDER_OPTION::RENDER_FOG)
	{
		GraphicText[GP_FOG]->SetTexture("Setting_Active");
	}
	else
	{
		GraphicText[GP_FOG]->SetTexture("Setting_Disable");
	}

	RenderSetting();
}

void UIOption::SetGraphicBloom()
{
	Sound_Play_SFX("UI_Button_Click");

	GraphicOption->PostProcessOption ^= POSTPROCESS_OPTION::POSTPROCESS_BLOOM;

	if (GraphicOption->PostProcessOption & POSTPROCESS_OPTION::POSTPROCESS_BLOOM)
	{
		GraphicText[GP_BLOOM]->SetTexture("Setting_Active");
	}
	else
	{
		GraphicText[GP_BLOOM]->SetTexture("Setting_Disable");
	}

	RenderSetting();
}

void UIOption::SetGraphicColorGrading()
{
	Sound_Play_SFX("UI_Button_Click");

	GraphicOption->PostProcessOption ^= POSTPROCESS_OPTION::POSTPROCESS_COLORGRADING;

	if (GraphicOption->PostProcessOption & POSTPROCESS_OPTION::POSTPROCESS_COLORGRADING)
	{
		GraphicText[GP_COLORGRADING]->SetTexture("Setting_Active");
	}
	else
	{
		GraphicText[GP_COLORGRADING]->SetTexture("Setting_Disable");
	}

	RenderSetting();
}

void UIOption::SetGraphicFXAA()
{
	Sound_Play_SFX("UI_Button_Click");

	GraphicOption->PostProcessOption ^= POSTPROCESS_OPTION::POSTPROCESS_FXAA;

	if (GraphicOption->PostProcessOption & POSTPROCESS_OPTION::POSTPROCESS_FXAA)
	{
		GraphicText[GP_FXAA]->SetTexture("Setting_Active");
	}
	else
	{
		GraphicText[GP_FXAA]->SetTexture("Setting_Disable");
	}

	RenderSetting();
}

void UIOption::SetGraphicHDR()
{
	Sound_Play_SFX("UI_Button_Click");

	GraphicOption->PostProcessOption ^= POSTPROCESS_OPTION::POSTPROCESS_HDR;

	if (GraphicOption->PostProcessOption & POSTPROCESS_OPTION::POSTPROCESS_HDR)
	{
		GraphicText[GP_HDR]->SetTexture("Setting_Active");
	}
	else
	{
		GraphicText[GP_HDR]->SetTexture("Setting_Disable");
	}

	RenderSetting();
}

void UIOption::SetReturnIn()
{
	Sound_Play_SFX("UI_Button_Overlay");
	ReturnButton->SetTexture("Setting_Back_On");
}

void UIOption::SetReturnOut()
{
	ReturnButton->SetTexture("Setting_Back_Off");
}

void UIOption::SetReturnClick()
{
	Sound_Play_SFX("UI_Button_Click");

	// 어느 상황에 들어왔는지에 따른 복귀 메세지 전송..
	/// 일시 정지 상황도 있음 추가해야됨
	switch (PrevTarget)
	{
	case MESSAGE_GLOBAL_TITLE:
		MessageManager::GetGM()->SEND_Message(TARGET_GLOBAL, MESSAGE_GLOBAL_TITLE);
		SetFullScreenBlur(false);
		break;
	case MESSAGE_GLOBAL_PAUSE:
		MessageManager::GetGM()->SEND_Message(TARGET_GLOBAL, MESSAGE_GLOBAL_PAUSE);
		break;
	default:
		break;
	}
}
