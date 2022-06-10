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

	//�¾����� �پ��� ��
	NowHitMonsterScale	 = 1.25f;
	NowHitMonsterScale_F = 0.15f;

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



