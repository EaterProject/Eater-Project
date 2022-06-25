#include "UIEffect.h"
#include "EaterEngineAPI.h"
//component
#include "GameObject.h"
#include "Image.h"
#include "RectTransform.h"
UIEffect::UIEffect()
{


}

UIEffect::~UIEffect()
{


}

void UIEffect::Awake()
{
	CreateFade();


}

void UIEffect::Start()
{


}

void UIEffect::Update()
{
	if (IsFade_IN == true)
	{
		mAlpha += GetDeltaTime() * 255;
		if (mAlpha >= 255)
		{
			mAlpha = 255;
			IsFade_IN = false;
			if (FadeIn_Return != nullptr)
			{
				*FadeIn_Return = true;
			}
		}
		mImages[FADE_IMAGE]->SetColor(255, 255, 255, mAlpha);
	}

	if (IsFade_OUT == true)
	{
		mAlpha -= GetDeltaTime() * 255;
		if (mAlpha <= 0)
		{
			mAlpha = 0;
			IsFade_OUT = false;
			if (FadeOut_Return != nullptr) 
			{
				*FadeOut_Return = true;
			}
		}
		mImages[FADE_IMAGE]->SetColor(255, 255, 255, mAlpha);
	}
}

void UIEffect::Fade_IN(void* Data)
{
	FadeIn_Return = reinterpret_cast<bool*>(Data);

	IsFade_IN = true;
	mAlpha = 0.0f;
}

void UIEffect::Fade_OUT(void* Data)
{
	FadeOut_Return = reinterpret_cast<bool*>(Data);
	IsFade_OUT = true;
	mAlpha = 255.0f;
}

void UIEffect::CreateFade()
{
	GameObject* Object = Instance_UI();
	mImages[FADE_IMAGE] = Object->AddComponent<Image>();
	mImages[FADE_IMAGE]->SetTexture("Black");
	mImages[FADE_IMAGE]->SetColor(255, 255, 255, 0);
	mImages[FADE_IMAGE]->SetLayer(3);
	mRect[FADE_IMAGE] = Object->GetComponent<RectTransform>();
	mRect[FADE_IMAGE]->SetPosition(0, 0);
	mRect[FADE_IMAGE]->SetScale(1, 1);
	mRect[FADE_IMAGE]->SetPivot(PIVOT_MIDDLE_CENTER);
}
