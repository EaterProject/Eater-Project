#include "Boss_ChaseAttack.h"

#include "AnimationController.h"
#include "ClientTypeOption.h"
#include "Transform.h"
#include "MessageManager.h"
#include "EaterEngineAPI.h"
#include "BossWeapon.h"
#include "PhysRay.h"

Boss_ChaseAttack::Boss_ChaseAttack()
{
	mRay = new PhysRayCast();
}

Boss_ChaseAttack::~Boss_ChaseAttack()
{
	delete mRay;

	for (int i = 0; i < 5; i++)
	{
		Destroy(Weapon[i]->gameobject);
	}
}

void Boss_ChaseAttack::SetObject(GameObject* Object)
{
	State::SetObject(Object);
	for (int i = 0; i < 5; i++)
	{
		GameObject* Object = MessageManager::GetGM()->CREATE_MESSAGE(TARGET_BOSS_WEAPON);
		Weapon[i] = Object->GetComponent<BossWeapon>();
	}
}

int Boss_ChaseAttack::Phase_1()
{

	switch (SkillNumber)
	{
	case 0:
		SkillStart();
		break;
	case 1:
		SkillPlay();
		break;
	case 2:
		SkillEnd();
		break;
	}
	return mStateNumber;
}

int Boss_ChaseAttack::Phase_2()
{
	return 0;
}

void Boss_ChaseAttack::SkillStart()
{
	if (StartOnePlay[SkillNumber] == false)
	{
		mStateNumber = (int)BOSS_STATE::CHASE_ATTACK_READY;
		StartOnePlay[SkillNumber] = true;
	}



	mTransform->Slow_Y_Rotation(mTargetTR->GetPosition(), 150, false);
	int End = mAnimation->GetEndFrame();
	int Now = mAnimation->GetNowFrame();

	if (Now >= End)
	{
		SkillNumber++;
		AnimationName = "recovery_2";
	}
}

void Boss_ChaseAttack::SkillPlay()
{
	if (StartOnePlay[SkillNumber] == false)
	{
		CreateSkillPoint();
		StartOnePlay[SkillNumber] = true;
	}

	mTransform->Slow_Y_Rotation(mTargetTR->GetPosition(), 150, false);
	if (IsShooting == false)
	{
		Vector3 Start;
		Start = mTransform->GetPosition();
		Start.y += 5.0f;

		if (WeaponIndex < 5)
		{
			Weapon[WeaponIndex]->SetShootingPoistion(Start, SkillPoint[WeaponIndex],8,(WeaponIndex + 1) *5);
		}

		IsShooting = true;
	}

	if (IsShooting == true)
	{
		if (Weapon[WeaponIndex]->ShootingReady() == true)
		{
			if(WeaponIndex <= 4)
			{
				WeaponIndex++;
				IsShooting = false;
			}
		}
	}

	if (WeaponIndex >= 4)
	{
		if (ChaseSkillPlayTime >= ChaseSkillPlayTimeMax)
		{
			SkillNumber++;
			ChaseSkillPlayTime = 0;
			WeaponIndex = 0;
			IsShooting = false;
			AnimationName = "recovery_3";
			for (int i = 0; i < 5; i++)
			{
				Weapon[i]->Reset();
			}
		}
		else
		{
			ChaseSkillPlayTime += GetDeltaTime();
		}
	}
}

void Boss_ChaseAttack::SkillEnd()
{
	int End = mAnimation->GetEndFrame();
	int Now = mAnimation->GetNowFrame();
	if (Now >= End)
	{
		SkillNumber = 0;
		AnimationName = "recovery_1";
		mStateNumber = (int)BOSS_STATE::IDLE;

		for (int i = 0; i < 3; i++)
		{
			StartOnePlay[i] = false;
		}
	}
}

void Boss_ChaseAttack::CreateSkillPoint()
{
	Vector3 Look = mTransform->GetLocalPosition_Look();
	Look.z *= -1;
	for (int i = 0; i < 10; i++)
	{
		Vector3 Start;
		Start = mTransform->GetPosition() + (Look * (i*8));
		SkillPoint[i] = Start;

		mRay->Direction = { 0,-1,0 };
		mRay->MaxDistance = 10;
		mRay->Origin = Start;
		//충돌된 곳에Y축만 가져온다
		RayCast(mRay);
		SkillPoint[i].y = mRay->Hit.HitPoint.y;
	}
}

int Boss_ChaseAttack::Play()
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