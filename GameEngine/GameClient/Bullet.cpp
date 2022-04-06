#include "Bullet.h"
#include "Transform.h"
#include "GameObject.h"
#include "MainHeader.h"
#include "Collider.h"
#include "MeshFilter.h"

Bullet::Bullet()
{
	mTransform = nullptr;
	isLife = false;
	Direction	= { 0,0,0 };
	MonsterTag	= 0;
}

Bullet::~Bullet()
{
	mTransform = nullptr;
	isLife = false;
	Direction = { 0,0,0 };
}

void Bullet::SetUp()
{
	mTransform = gameobject->GetTransform();
	MonsterTag = FindTagNumber("Monster");

	MeshFilter* mMeshFilter = gameobject->GetComponent<MeshFilter>();
	Collider*	mCollider	= gameobject->GetComponent<Collider>();

	mMeshFilter->SetModelName("Bullet");
	mCollider->SetSphereCollider(0.25f);
	mCollider->SetTrigger(true);
}

void Bullet::Shooting(Vector3 Dir)
{
	Direction = Dir;
}

void Bullet::Update()
{
	if (isLife == true)
	{
		if (LifeTime >= ResetTime)
		{
			ReSet();
			LifeTime -= ResetTime;
			return;
		}
		mTransform->SetTranlate(Direction * GetDeltaTime() * BulletSpeed);
		LifeTime += GetDeltaTime();
	}
}

void Bullet::ReSet()
{
	isLife = false;
}

void Bullet::OnTriggerEnter(GameObject* Obj)
{
	//if (Obj->GetTag() == MonsterTag)
	//{
	//	isLife = false;
	//	mTransform->Position = { 0,0,0 };
	//}
}
