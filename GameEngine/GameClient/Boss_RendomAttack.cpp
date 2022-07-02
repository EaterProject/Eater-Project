#include "Boss_RendomAttack.h"
#include "AnimationController.h"
#include "ClientTypeOption.h"
#include "Transform.h"
#include "MessageManager.h"
#include "EaterEngineAPI.h"
#include "BossWeapon.h"
#include "PhysRay.h"

Boss_RendomAttack::Boss_RendomAttack()
{
	mRay = new PhysRayCast();
	StartPoint = { -44.0f,6.0f,62.0f };
}

Boss_RendomAttack::~Boss_RendomAttack()
{
	delete mRay;
}

void Boss_RendomAttack::SetObject(GameObject* Object)
{
	State::SetObject(Object);
	for (int i = 0; i < 5; i++)
	{
		GameObject* Object = MessageManager::GetGM()->CREATE_MESSAGE(TARGET_BOSS_WEAPON);
		Weapon[i] = Object->GetComponent<BossWeapon>();
	}
}

int Boss_RendomAttack::Play()
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

	return 0;
}

int Boss_RendomAttack::Phase_1()
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
	case 3:
		SkillReset();
		break;
	}

	return mStateNumber;
}

int Boss_RendomAttack::Phase_2()
{











	return 0;
}
void Boss_RendomAttack::CreateSkillPoint()
{
	float Angle = 360.0f / 5.0f;
	float NowAngle = mTransform->GetRotation().y;
	for (int i = 0; i < 5; i++)
	{
		NowAngle += Angle;
		float X = cos(NowAngle * (3.141592f / 180));
		float y = sin(NowAngle * (3.141592f / 180));

		Vector3 EndPoint = Vector3(0, 0, 0);
		EndPoint.x += (X * SkillRange) + StartPoint.x;
		EndPoint.y += StartPoint.y + 2;
		EndPoint.z += (y * SkillRange) + StartPoint.z;

		//Ray를 쏜다
		mRay->Direction = { 0,-1,0 };
		mRay->MaxDistance = 10;
		mRay->Origin = EndPoint;

		//충돌된 곳에Y축만 가져온다
		RayCast(mRay);
		EndPoint.y = mRay->Hit.HitPoint.y;
		SkillPoint[i] = EndPoint;
	}
}

void Boss_RendomAttack::SkillStart()
{
	if (StartOnePlay[SkillNumber] == false)
	{
		mStateNumber = (int)BOSS_STATE::RENDOM_ATTACK_START;
		//스킬을 날릴 위치를 지정한다
		CreateSkillPoint();

		mTransform->SetPosition_X(StartPoint.x);
		mTransform->SetPosition_Z(StartPoint.z);
		AnimationName = "attack4_1";
		StartOnePlay[SkillNumber] = true;
	}


	int End = mAnimation->GetEndFrame();
	int Now = mAnimation->GetNowFrame();
	if (Now >= End)
	{
		SkillNumber++;
		AnimationName = "attack4_2";
	}
	//else
	//{
	//	mTransform->AddPosition_Y(GetDeltaTime());
	//}
}

void Boss_RendomAttack::SkillPlay()
{
	
	if (IsShooting == false)
	{
		Vector3 Start = mTransform->GetPosition();
		Start.y += 7.0f;
		if (WeaponIndex < 5)
		{
			Weapon[WeaponIndex]->SetShootingPoistion(Start, SkillPoint[WeaponIndex]);
			IsShooting = true;
		}
	}

	if (IsShooting == true)
	{
		if (Weapon[WeaponIndex]->ShootingReady() == true)
		{
			WeaponIndex++;
			IsShooting = false;
		}
	}

	if (WeaponIndex >= 5)
	{
		if (RendomSkillPlayTime >= RendomSkillPlayTimeMax)
		{
			SkillNumber++;
			RendomSkillPlayTime = 0;
			WeaponIndex			= 0;
			AnimationName = "attack4_3";
			IsShooting			= false;
		}
		else
		{
			RendomSkillPlayTime += GetDeltaTime();
		}
	}
}

void Boss_RendomAttack::SkillEnd()
{
	if (StartOnePlay[SkillNumber] == false)
	{
		//스킬을 날릴 위치를 지정한다
		StartOnePlay[SkillNumber] = true;
	}

	int End = mAnimation->GetEndFrame();
	int Now = mAnimation->GetNowFrame();

	if (Now >= End)
	{
		AnimationName = "attack4_4";
		SkillNumber++;
	}
}

void Boss_RendomAttack::SkillReset()
{
	if (StartOnePlay[SkillNumber] == false)
	{
		//스킬을 날릴 위치를 지정한다
		StartOnePlay[SkillNumber] = true;
	}



	int End = mAnimation->GetEndFrame();
	int NOW = mAnimation->GetNowFrame();

	if (NOW >= End)
	{
		mStateNumber = (int)BOSS_STATE::IDLE;

		SkillNumber = 0;
		WeaponIndex = 0;
		AnimationName = "attack4_1";
		for (int i = 0; i < 4; i++)
		{
			StartOnePlay[i] = false;
		}

		for (int i = 0; i < 5; i++)
		{
			Weapon[i]->Reset();
		}
	}
}
