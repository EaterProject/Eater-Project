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
	///변경할 변수들을 이쪽에다 선언
	/////////////////////////////////////////////
	ModelName		= "MonsterB+";
	AnimationName	= "MonsterB+";
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
