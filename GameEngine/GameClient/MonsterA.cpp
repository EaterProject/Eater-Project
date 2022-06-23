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

	//���� ũ��
	MonsterScale	= 1.5f;
	MonsterType		= MONSTER_TYPE_A;

	//����
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



