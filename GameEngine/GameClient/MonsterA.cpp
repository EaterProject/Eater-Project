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
	MonsterScale	= 1.25f;
	MonsterType		= MONSTER_TYPE_A;
	SOUND_NAME[int(MONSTER_STATE::HIT)] = "Monster_A_Hit";
	SOUND_NAME[int(MONSTER_STATE::MOVE)] = "Monster_A_Walking";
	SOUND_NAME[int(MONSTER_STATE::ATTACK)] = "Monster_A_Attack";


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



