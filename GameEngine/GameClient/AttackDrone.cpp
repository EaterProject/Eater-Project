#include "AttackDrone.h"
#include "MainHeader.h"
#include "GameObject.h"
#include "Transform.h"
#include "Bullet.h"
#include "MeshFilter.h"
#include "Collider.h"

AttackDrone::AttackDrone()
{
	mTransform = nullptr;
	mTargetMonsterTR = nullptr;
	mMeshFilter = nullptr;
	mCollider = nullptr;

}

AttackDrone::~AttackDrone()
{
	mTransform = nullptr;
	mTargetMonsterTR = nullptr;
	mMeshFilter = nullptr;
	mCollider = nullptr;
}


void AttackDrone::Awake()
{
	mMeshFilter = gameobject->GetComponent<MeshFilter>();
	mCollider = gameobject->GetComponent<Collider>();
}

void AttackDrone::SetUp()
{
	mTransform = gameobject->GetTransform();
	MonsterTag = FindTagNumber("Monster");

	mMeshFilter->SetModelName("drone");
	mCollider->SetSphereCollider(AttackRange);
	mCollider->SetTrigger(true);
}

void AttackDrone::Update()
{
	if (mTargetMonsterTR != nullptr)
	{
		AttackTime += GetDeltaTime();
		if (AttackTime >= AttackMaxTime)
		{
			AttackTime -= AttackMaxTime;
			Bullet* mBullet = mObjectGM->GetBullet();
			if (mBullet == nullptr) { return; }
			Vector3 Dir = (mTransform->Position - mTargetMonsterTR->Position) * -1;
			Dir.Normalize();
			Dir.y = 0;

			//ó�� �������� ����
			mBullet->gameobject->GetTransform()->Position = mTransform->Position;
			mBullet->Shooting(Dir);
		}
		mTransform->Slow_Y_Rotation(mTargetMonsterTR->Position, 150);
	}
}

void AttackDrone::ReSet()
{


}

void AttackDrone::OnTriggerStay(GameObject* Obj)
{
	//������ ������Ʈ�� �����϶�
	if (Obj->GetTag() == MonsterTag)
	{
		if (mTargetMonsterTR == nullptr)
		{
			mTargetMonsterTR = Obj->GetTransform();
		}
	}
}

void AttackDrone::OnTriggerExit(GameObject* Obj)
{
	mTargetMonsterTR = nullptr;
}
