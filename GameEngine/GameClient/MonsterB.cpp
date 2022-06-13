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
	///변경할 변수들을 이쪽에다 선언
	/////////////////////////////////////////////
	//모델
	ModelName		= "MonsterB+";
	AnimationName	= "MonsterB+";
	
	//맞았을떄 줄어드는 값
	Speed = 1.5f;
	NowHitMonsterScale		= 3.0f;
	NowHitMonsterScale_F	= 0.1f;

	//사운드
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
