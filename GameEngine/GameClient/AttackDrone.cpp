#include "AttackDrone.h"
#include "MainHeader.h"
#include "GameObject.h"
#include "Transform.h"
AttackDrone::AttackDrone()
{
}

AttackDrone::~AttackDrone()
{


}


void AttackDrone::SetUp()
{
	mTransform = gameobject->GetTransform();
	MonsterTag = FindTagNumber("Monster");
}

void AttackDrone::Update()
{
	if (mMonsterTR != nullptr)
	{
		mTransform->Slow_Y_Rotation(mMonsterTR->Position, 100);
	}
}

void AttackDrone::OnTriggerStay(GameObject* Obj)
{
	int Tag = Obj->GetTag();
	if (Tag == MonsterTag)
	{
		mMonsterTR = Obj->GetTransform();
	}
}
