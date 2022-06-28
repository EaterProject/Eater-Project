#include "UIOption.h"
#include "EaterEngineAPI.h"
#include "GameObject.h"
#include "RectTransform.h"
#include "Image.h"
#include "Button.h"
#include "ImageFont.h"

UIOption::UIOption()
	:IsUpdate(false)
{
}

UIOption::~UIOption()
{
}

void UIOption::Awake()
{
	// 그래픽 옵션 가져오기..
	GraphicOption = GetRenderOptionData();

	GameObject* object = nullptr;
	RectTransform* rt = nullptr;
	Button* button = nullptr;
	Image* image = nullptr;

	// 배경 화면
	object = Instance_Image();
	rt = object->GetComponent<RectTransform>();
	rt->SetPivot(PIVOT_TYPE::PIVOT_MIDDLE_CENTER);
	BackGround = object->GetComponent<Image>();
	BackGround->SetTexture("Setting_Main");

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
	image->SetTexture("Setting_FullScreen");

	ScreenText = image;

	start_height += height_first_line;
	start_height += height_offset;
	start_height += height_offset;

	// 사운드 폰트 이미지 생성..


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

void UIOption::Update()
{
	if (IsUpdate)
	{


		IsUpdate = false;
	}
}

void UIOption::ClickButtonSetting(OPTION option, float left_x, float right_x, float y)
{
	// 해당 이미지 위치 설정..
	ClickButton[option].Left->SetPosition(left_x, y);
	ClickButton[option].Left->SetBoundaryOffset(-5.0f, 5.0f, 0.0f, 0.0f);
	ClickButton[option].Right->SetPosition(right_x, y);
	ClickButton[option].Right->SetBoundaryOffset(-5.0f, 5.0f, 0.0f, 0.0f);

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
		break;
	case UIOption::BACKGROUND_VOL:
		ClickButton[option].Left->PushEvent(Eater::Bind(&UIOption::SetBackGroundVolumeDown, this), Button::PRESS_DOWN_BUTTON);
		ClickButton[option].Right->PushEvent(Eater::Bind(&UIOption::SetBackGroundVolumeUp, this), Button::PRESS_DOWN_BUTTON);
		break;
	case UIOption::SFX_VOL:
		ClickButton[option].Left->PushEvent(Eater::Bind(&UIOption::SetSFXVolumeDown, this), Button::PRESS_DOWN_BUTTON);
		ClickButton[option].Right->PushEvent(Eater::Bind(&UIOption::SetSFXVolumeUp, this), Button::PRESS_DOWN_BUTTON);
		break;
	case UIOption::OPTION_MAX:
		break;
	default:
		break;
	}
}

void UIOption::SetScreenSize()
{
	ClickButton[SCREEN_SIZE].Value ^= true;

	if (ClickButton[SCREEN_SIZE].Value)
	{
		ScreenText->SetTexture("Setting_FullScreen");
	}
	else
	{
		ScreenText->SetTexture("Setting_Window");
	}

	// 해당 상태의 해상도로 화면 변경..
}

void UIOption::SetMasterVolumeUp()
{
	ClickButton[MASTER_VOL].Value += 1;

	if (ClickButton[MASTER_VOL].Value > 100)
	{
		ClickButton[MASTER_VOL].Value = 100;
	}

	// 볼륨에 맞는 숫자 출력..
}

void UIOption::SetMasterVolumeDown()
{
	ClickButton[MASTER_VOL].Value -= 1;

	if (ClickButton[MASTER_VOL].Value < 0)
	{
		ClickButton[MASTER_VOL].Value = 0;
	}

	// 볼륨에 맞는 숫자 출력..
}

void UIOption::SetBackGroundVolumeUp()
{
	ClickButton[BACKGROUND_VOL].Value += 1;

	if (ClickButton[BACKGROUND_VOL].Value > 100)
	{
		ClickButton[BACKGROUND_VOL].Value = 100;
	}

	// 볼륨에 맞는 숫자 출력..
}

void UIOption::SetBackGroundVolumeDown()
{
	ClickButton[BACKGROUND_VOL].Value -= 1;

	if (ClickButton[BACKGROUND_VOL].Value < 0)
	{
		ClickButton[BACKGROUND_VOL].Value = 0;
	}

	// 볼륨에 맞는 숫자 출력..
}

void UIOption::SetSFXVolumeUp()
{
	ClickButton[SFX_VOL].Value += 1;

	if (ClickButton[SFX_VOL].Value > 100)
	{
		ClickButton[SFX_VOL].Value = 100;
	}

	// 볼륨에 맞는 숫자 출력..
}

void UIOption::SetSFXVolumeDown()
{
	ClickButton[SFX_VOL].Value -= 1;

	if (ClickButton[SFX_VOL].Value < 0)
	{
		ClickButton[SFX_VOL].Value = 0;
	}

	// 볼륨에 맞는 숫자 출력..
}

void UIOption::SetGraphicIBL()
{
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
