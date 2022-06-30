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
	///변경할 변수들을 이쪽에다 선언
	/////////////////////////////////////////////
	//모델
	ModelName		= "MonsterA+";
	AnimationName	= "MonsterA+";

	//몬스터 크기
	MonsterScale	= 1.25f;
	MonsterType		= MONSTER_TYPE_A;
	SOUND_NAME[int(MONSTER_STATE::HIT)] = "Monster_A_Hit";
	SOUND_NAME[int(MONSTER_STATE::MOVE)] = "Monster_A_Walking";
	SOUND_NAME[int(MONSTER_STATE::ATTACK)] = "Monster_A_Attack";


	//사운드
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



