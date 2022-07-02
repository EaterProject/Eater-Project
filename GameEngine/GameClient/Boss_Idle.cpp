#include "Boss_Idle.h"
#include "AnimationController.h"
#include "ClientTypeOption.h"
#include "GameObject.h"
#include "Transform.h"
#include "MessageManager.h"
#include "EaterEngineAPI.h"

Boss_Idle::Boss_Idle()
{

}

Boss_Idle::~Boss_Idle()
{

}

void Boss_Idle::SetMessage(void* Message)
{
	BossPhase = *(reinterpret_cast<int*>(Message));
}

void Boss_Idle::SetObject(GameObject* Object)
{
	mObject = Object;
	mAnimation = mObject->GetComponent<AnimationController>();
	mTransform = mObject->GetTransform();
}

int Boss_Idle::Play()
{
	mAnimation->Choice(AnimationName, AnimationSpeed);

	if (BossPhase == 0)
	{
		return  Phase_1();
	}
	else
	{
		return Phase_2();
	}
}

int Boss_Idle::Phase_1()
{
	TargetDistance();

	if (PlayerDistance < AttackRange)
	{
		return (int)BOSS_STATE::CLOSER_ATTACK_R;
	}
	else if (PlayerDistance < FightRange && PlayerDistance > AttackRange)
	{
		if (mTargetTR != nullptr) { mTransform->Slow_Y_Rotation(mTargetTR->GetPosition(), 150); }
	}


	if (GetKeyDown(VK_NUMPAD7))
	{
		return (int)BOSS_STATE::TELEPORT_READY;
	}

	if (GetKeyDown(VK_NUMPAD8))
	{
		return (int)BOSS_STATE::RENDOM_ATTACK_START;
	}

	if (GetKeyDown(VK_NUMPAD9))
	{
		return (int)BOSS_STATE::CREATE_FRIEND;
	}

	if (GetKeyDown(VK_NUMPAD4))
	{
		return (int)BOSS_STATE::CHASE_ATTACK_READY;
	}

	if (GetKeyDown(VK_NUMPAD5))
	{
		return (int)BOSS_STATE::CLOSER_ATTACK_R;
	}

	return (int)BOSS_STATE::IDLE;
}

int Boss_Idle::Phase_2()
{




	return 0;
}

void Boss_Idle::TargetDistance()
{
	if (mTargetTR == nullptr) { return; }

	PlayerDistance = mTransform->GetDistance(mTargetTR->GetPosition());
	if (PlayerDistance <= FightRange && IsUI_ON == false)
	{
		IsUI_ON = true;
		MessageManager::GetGM()->SEND_Message(TARGET_UI, MESSAGE_UI_BOSS_ACTIVE, &IsUI_ON);
	}
	else if (PlayerDistance > FightRange && IsUI_ON == true)
	{
		IsUI_ON = false;
		MessageManager::GetGM()->SEND_Message(TARGET_UI, MESSAGE_UI_BOSS_ACTIVE, &IsUI_ON);
	}
}
