#include "MonsterA.h"
#include "EaterEngineAPI.h"
#include "Collider.h"

MonsterA::MonsterA()
{

}

MonsterA::~MonsterA()
{

}

void MonsterA::Awake()
{
	MonsterComponent::Awake();
}
void MonsterA::SetUp()
{
	///������ �������� ���ʿ��� ����
	/////////////////////////////////////////////
	//��
	ModelName		= "MonsterA+";
	AnimationName	= "MonsterA+";
	PureManaCount	= 5;

	///��ġ
	MonsterScale	= 1.2f;
	HP				= 120.0f;
	Max_HP			= HP;
	Speed			= 1.5f;
	RotationSpeed	= 380.0f;
	ComboCount		= 2;
	Damage			= 200;
	PureManaCount	= 5;
	//AttackRange		= 1.5f;
	//ChaseRange		= 7.0f;

	ANIMATION_TIME[(int)MONSTER_STATE::MOVE] = 0.9f;
	ANIMATION_TIME[(int)MONSTER_STATE::CHASE] = 1.2f;
	ANIMATION_TIME[(int)MONSTER_STATE::ATTACK] = 0.8f;

	MonsterType		= MONSTER_TYPE_A;

	SOUND_NAME[int(MONSTER_STATE::HIT)] = "Monster_A_Hit";
	SOUND_NAME[int(MONSTER_STATE::MOVE)] = "Monster_A_Walking";
	SOUND_NAME[int(MONSTER_STATE::ATTACK)] = "Monster_A_Attack";


	//����
	/////////////////////////////////////////////
	MonsterComponent::SetUp();
}

void MonsterA::Start()
{
	MonsterComponent::Start();
	if (MonsterColor == MONSTER_COLOR_RED)
	{
		mMF_Setting.SetLimlightSetting(MeshFilterSetting::COLOR_TYPE::RED, 0.5f, 1);
		mMF_Setting.SetEmissiveSetting(245, 44, 13, 2.9);
	}
	else
	{
		mMF_Setting.SetLimlightSetting(MeshFilterSetting::COLOR_TYPE::BLUE, 0.5f, 1);
		mMF_Setting.SetEmissiveSetting(40,92,255, 2.9f);
	}
}

void MonsterA::Update()
{
	MonsterComponent::Update();
}

void MonsterA::Debug()
{
	MonsterComponent::Debug();
}



