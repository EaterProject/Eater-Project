#include "MonsterA.h"
#include "EaterEngineAPI.h"

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
	ModelName		= "MonsterA+";
	AnimationName	= "MonsterA+";
	Speed			= 10;
	/////////////////////////////////////////////
	MonsterComponent::SetUp();
}

void MonsterA::Update()
{
	MonsterComponent::Update();
}



