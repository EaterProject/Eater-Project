#include "UICredit.h"
#include "EaterEngineAPI.h"
#include "GameObject.h"
#include "RectTransform.h"
#include "Image.h"
#include "ClientTypeOption.h"
#include "MessageManager.h"

UICredit::UICredit()
	:StartCredit(false), MoveSpeed(-200.0f), StartWidth(-50.0f), StartHeight(1758.5f), EndHeight(-2328.0f)
{

}

UICredit::~UICredit()
{

}

void UICredit::Awake()
{
	GameObject* object = nullptr;

	// 매뉴얼 이미지..
	object = Instance_Image();
	CreditTransform = object->GetComponent<RectTransform>();
	CreditImage = object->GetComponent<Image>();
	CreditImage->SetPivot(PIVOT_TYPE::PIVOT_MIDDLE_RIGHT);
	CreditImage->SetTexture("Credit_Image");
	CreditImage->SetPosition(StartWidth, StartHeight);
}

void UICredit::Update()
{
	if (StartCredit)
	{
		float dTime = GetDeltaTime() * MoveSpeed;

		CreditTransform->AddPosition(0.0f, dTime);

		if (CreditTransform->GetPosition().y < EndHeight)
		{
			StartCredit = false;
			SetFullScreenBlur(false);

			CreditTransform->SetPosition(StartWidth, StartHeight);

			MessageManager::GetGM()->SEND_Message(TARGET_GLOBAL, MESSAGE_GLOBAL_RESUME);
		}
	}
}

void UICredit::Set_CreditUI_Active(bool active)
{
	CreditImage->SetActive(active);

	StartCredit = active;

	// 전체 화면 블러..
	if (active)
	{
		SetFullScreenBlur(true, 4);
	}
}
