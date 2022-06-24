#include "MonsterB.h"
#include "EaterEngineAPI.h"
#include "Transform.h"
#include "Collider.h"
MonsterB::MonsterB()
{
	
}

MonsterB::~MonsterB()
{
	
}

void MonsterB::Awake()
{
	MonsterComponent::Awake();
}
void MonsterB::SetUp()
{
	///������ �������� ���ʿ��� ����
	/////////////////////////////////////////////
	//��
	ModelName		= "MonsterB+";
	AnimationName	= "MonsterB+";
	
	//�¾����� �پ��� ��
	Speed = 1.5f;
	MonsterScale = 3.0f;
	MonsterType = MONSTER_TYPE_B;

	SOUND_NAME[int(MONSTER_STATE::HIT)] = "Monster_B_Hit";
	SOUND_NAME[int(MONSTER_STATE::MOVE)] = "Monster_B_Walking";
	SOUND_NAME[int(MONSTER_STATE::ATTACK)] = "Monster_B_Attack";

	//����
	Sound_Attack = "Monster_B_Attack";
	Sound_move	 = "Monster_B_Walking";
	/////////////////////////////////////////////
	MonsterComponent::SetUp();
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
