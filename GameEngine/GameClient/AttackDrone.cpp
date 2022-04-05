#include "AttackDrone.h"
#include "MainHeader.h"
#include "GameObject.h"
#include "Transform.h"
#include "Bullet.h"
#include "MeshFilter.h"
#include "Collider.h"

AttackDrone::AttackDrone()
{
}

AttackDrone::~AttackDrone()
{


}


void AttackDrone::SetUp()
{
	MeshFilter* mMeshFilter = gameobject->GetComponent<MeshFilter>();
	Collider* mCollider = gameobject->GetComponent<Collider>();


	mTransform = gameobject->GetTransform();
	MonsterTag = FindTagNumber("Monster");

	mMeshFilter->SetModelName("drone");
	mCollider->SetSphereCollider(AttackRange);
	mCollider->SetTrigger(true);
}

void AttackDrone::Update()
{
	
}

void AttackDrone::ReSet()
{


}

void AttackDrone::OnTriggerStay(GameObject* Obj)
{
	//접촉한 오브젝트가 몬스터일때
	int num = Obj->GetTag();
	if (num == MonsterTag)
	{
		mMonsterTR = Obj->GetTransform();
		AttackTime += GetDeltaTime();
		if (AttackTime >= AttackMaxTime)
		{
			AttackTime -= AttackMaxTime;
			Vector3 AttackPos = mTransform->GetLocalPosition_Look();
			Bullet* mBullet = mObjectGM->GetBullet();
			if (mBullet == nullptr) { return; }

			Vector3 MY		= mTransform->Position;
			Vector3 Monster = mMonsterTR->Position;

			Vector3 Dir = (MY - Monster) * -1;
			Dir.Normalize();
			Dir.y = 0;
			mBullet->gameobject->GetTransform()->Position = mTransform->Position;
			mBullet->Shooting(Dir);
		}
		mTransform->Slow_Y_Rotation(mMonsterTR->Position, 100);
	}
}
