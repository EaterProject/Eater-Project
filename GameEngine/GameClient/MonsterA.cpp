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
	MonsterScale	= 1.5f;
	MonsterType		= MONSTER_TYPE_A;

	//사운드
	Sound_Attack	= "Monster_A_Attack";
	Sound_move		= "Monster_A_Walking";
	/////////////////////////////////////////////
	MonsterComponent::SetUp();
}

void MonsterA::Update()
{
	MonsterComponent::Update();
}

void MonsterA::Debug()
{
	MonsterComponent::Debug();
}



