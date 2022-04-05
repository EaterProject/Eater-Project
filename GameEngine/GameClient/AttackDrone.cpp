#include "AttackDrone.h"
#include "MainHeader.h"
#include "GameObject.h"
#include "Transform.h"
#include "Bullet.h"

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
		AttackTime += GetDeltaTime();
		if (AttackTime >= AttackMaxTime)
		{
			Vector3 AttackPos = mTransform->Position;
			//mObjectGM->GetObjectPool<Bullet>();
			

			AttackTime -= AttackMaxTime;
		}

		mTransform->Slow_Y_Rotation(mMonsterTR->Position, 100);
	}
}

void AttackDrone::ReSet()
{
}

void AttackDrone::OnTriggerStay(GameObject* Obj)
{
	if (Obj->GetTag() == MonsterTag)
	{
		mMonsterTR = Obj->GetTransform();
	}
}
