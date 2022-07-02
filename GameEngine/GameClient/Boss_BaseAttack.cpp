#include "Boss_BaseAttack.h"
#include "AnimationController.h"
#include "ClientTypeOption.h"
#include "Transform.h"
#include "MessageManager.h"
#include "EaterEngineAPI.h"
Boss_BaseAttack::Boss_BaseAttack()
{



}

Boss_BaseAttack::~Boss_BaseAttack()
{


}

void Boss_BaseAttack::SetObject(GameObject* Object)
{
    State::SetObject(Object);
}

int Boss_BaseAttack::Play()
{
	mAnimation->Choice(AnimationName, AnimationSpeed);
	Phase_1();

    return mStateNumber;
}

int Boss_BaseAttack::Phase_1()
{
	mTransform->Slow_Y_Rotation(mTargetTR->GetPosition(), 150, false);
	if (RightAttack == true)
	{
		AnimationName = "attack5R";
	}
	else if (RightAttack == false)
	{
		AnimationName = "attack5L";
	}

	//플레이어 거리 체크
	float TargetDir = mTransform->GetDistance(mTargetTR->GetPosition());
	if (TargetDir < AttackRange)
	{
		if (mAnimation->EventCheck() == true)
		{
			if (isAttack == false)
			{
				int Damage = 100;
				MessageManager::GetGM()->SEND_Message(TARGET_PLAYER, MESSAGE_PLAYER_HIT, &Damage);
				MessageManager::GetGM()->SEND_Message(TARGET_UI, MESSAGE_UI_PLAYER_HIT, nullptr);
				isAttack = true;
			}
		}
	}

	int End = mAnimation->GetEndFrame();
	int Now = mAnimation->GetNowFrame();
	if (Now >= End)
	{
		isAttack = false;
		(RightAttack == true) ? RightAttack = false : RightAttack = true;
		return mStateNumber = (int)BOSS_STATE::IDLE;
	}
	else
	{
		mStateNumber = (int)BOSS_STATE::CLOSER_ATTACK_R;
		return mStateNumber;
	}
}

int Boss_BaseAttack::Phase_2()
{
    return 0;
}
