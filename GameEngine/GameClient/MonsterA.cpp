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
	///������ �������� ���ʿ��� ����
	/////////////////////////////////////////////
	ModelName		= "MonsterA+";
	AnimationName	= "MonsterA+";
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



