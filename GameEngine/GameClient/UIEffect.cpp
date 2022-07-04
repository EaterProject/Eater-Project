#include "UIEffect.h"
#include "EaterEngineAPI.h"
//component
#include "GameObject.h"
#include "Image.h"
#include "RectTransform.h"
#include "Player.h"

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
	UpdateFade();
	UpdateHit();
}

void UIEffect::Fade_IN(void* Data)
{
	FadeIn_Return = reinterpret_cast<bool*>(Data);

	IsFade		= true;
	IsFade_IN	= true;
	mAlpha = 0.0f;
}

void UIEffect::Fade_OUT(void* Data)
{
	FadeOut_Return = reinterpret_cast<bool*>(Data);

	IsFade		= true;
	IsFade_OUT	= true;
	mAlpha = 255.0f;
}

void UIEffect::PlayerHit(void* Data)
{
	if (Player::GetNoHitState() == false)
	{
		IsHit = true;
		mHitAlpha = 255.0f;
	}
}

void UIEffect::Set_UIEffect_Active(bool Active)
{
	for (int i = 0; i < 2; i++)
	{
		mImages[i]->SetActive(Active);
	}
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

	Object = Instance_UI();
	mImages[PLAYER_HIT] = Object->AddComponent<Image>();
	mImages[PLAYER_HIT]->SetTexture("PlayerDamage_01");
	mImages[PLAYER_HIT]->SetColor(255, 255, 255, 0);
	mImages[PLAYER_HIT]->SetLayer(3);
	mRect[PLAYER_HIT] = Object->GetComponent<RectTransform>();
	mRect[PLAYER_HIT]->SetPosition(0, 0);
	mRect[PLAYER_HIT]->SetScale(1, 1);
	mRect[PLAYER_HIT]->SetPivot(PIVOT_MIDDLE_CENTER);
}

void UIEffect::UpdateFade()
{
	if (IsFade == true)
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
}

void UIEffect::UpdateHit()
{
	//플레이어 피격시 화면에 피격 임펙트가 나온다
	if (IsHit == true)
	{
		mHitAlpha -= GetDeltaTime() * 255;
		if (mHitAlpha <= 0)
		{
			IsHit = false;
			mHitAlpha = 0.0f;
		}
		mImages[PLAYER_HIT]->SetColor(255, 0, 0, mHitAlpha);
	}
}
