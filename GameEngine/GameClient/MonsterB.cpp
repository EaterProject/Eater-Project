#include "MonsterB.h"
#include "EaterEngineAPI.h"
#include "Transform.h"
#include "Collider.h"
#include "ParticleFactory.h"
MonsterB::MonsterB()
{
	
}

MonsterB::~MonsterB()
{
	
}

void MonsterB::Awake()
{
	mParticleController = ParticleFactory::Get()->CreateParticleController(PARTICLE_TYPE::MonsterExplode);
	mParticleController->SetScale(3);
	MonsterComponent::Awake();
}
void MonsterB::SetUp()
{
	///변경할 변수들을 이쪽에다 선언
	/////////////////////////////////////////////
	//모델
	ModelName		= "MonsterB+";
	AnimationName	= "MonsterB+";
	
	///수치
	MonsterScale	= 3.0f;
	HP				= 240.0f;
	Max_HP			= HP;
	Speed			= 0.8f;
	RotationSpeed	= 380.0f;
	ComboCount		= 4;
	Damage			= 400;
	PureManaCount	= 10;
	//AttackRange		= 4.0f;
	//ChaseRange		= 7.0f;

	ANIMATION_TIME[(int)MONSTER_STATE::MOVE] = 0.4f;
	ANIMATION_TIME[(int)MONSTER_STATE::CHASE] = 0.6f;
	ANIMATION_TIME[(int)MONSTER_STATE::ATTACK] = 0.4f;

	MonsterType = MONSTER_TYPE_B;

	SOUND_NAME[int(MONSTER_STATE::HIT)] = "Monster_B_Hit";
	SOUND_NAME[int(MONSTER_STATE::MOVE)] = "Monster_B_Walking";
	SOUND_NAME[int(MONSTER_STATE::ATTACK)] = "Monster_B_Attack";

	if (MonsterColor == MONSTER_COLOR_RED)
	{
		mMF_Setting.SetLimlightSetting(MeshFilterSetting::COLOR_TYPE::RED, 0.5f, 1);
		mMF_Setting.SetEmissiveSetting(245, 81, 61, 4.1f *10);
	}
	else
	{
		mMF_Setting.SetLimlightSetting(MeshFilterSetting::COLOR_TYPE::BLUE, 0.5f, 1);
		mMF_Setting.SetEmissiveSetting(26, 68, 236, 4.1f * 10);
	}

	/////////////////////////////////////////////
	MonsterComponent::SetUp();
}

void MonsterB::Start()
{
	MonsterComponent::Start();
	if (MonsterColor == MONSTER_COLOR_RED)
	{
		mMF_Setting.SetLimlightSetting(MeshFilterSetting::COLOR_TYPE::RED, 0.5f, 1);
		mMF_Setting.SetEmissiveSetting(245, 81, 61, 4.1f);
	}
	else
	{
		mMF_Setting.SetLimlightSetting(MeshFilterSetting::COLOR_TYPE::BLUE, 0.5f, 1);
		mMF_Setting.SetEmissiveSetting(26, 68, 236, 4.1f);
	}
}

void MonsterB::Update()
{
	MonsterComponent::Update();
}

void MonsterB::Debug()
{
	MonsterComponent::Debug();
}

void MonsterB::OnTriggerStay(GameObject* Obj)
{
	MonsterComponent::OnTriggerStay(Obj);
}
