#include "MonsterB.h"
#include "EaterEngineAPI.h"
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
	ModelName		= "MonsterB+";
	AnimationName	= "MonsterB+";
	MonsterFront_Z	= true;
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
