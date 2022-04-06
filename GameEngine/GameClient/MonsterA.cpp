#include "MonsterA.h"
#include "MeshFilter.h"
#include "Transform.h"
#include "GameObject.h"
#include "AnimationController.h"
#include "MainHeader.h"
#include "Collider.h"
#include "Rigidbody.h"
#include "Player.h"

MonsterA::MonsterA()
{
	mMeshFilter = nullptr;
	mTransform	= nullptr;
	mAnimation	= nullptr;
	mColider	= nullptr;
	mRigidbody	= nullptr;
}

MonsterA::~MonsterA()
{
	mMeshFilter = nullptr;
	mTransform	= nullptr;
	mAnimation	= nullptr;
	mColider	= nullptr;
	mRigidbody	= nullptr;
}

void MonsterA::Awake()
{
	mMeshFilter = gameobject->GetComponent<MeshFilter>();
	mTransform	= gameobject->GetComponent<Transform>();
	mAnimation	= gameobject->GetComponent<AnimationController>();
	mColider	= gameobject->GetComponent<Collider>();
	mRigidbody	= gameobject->GetComponent<Rigidbody>();
}
void MonsterA::SetUp()
{
	//콜라이더 값 조정
	mColider->SetCenter(0, 0.5, 0);
	mColider->SetBoxCollider(0.25f, 0.5f, 0.75f);
	mColider->SetMaterial_Restitution(0);
	mRigidbody->SetFreezeRotation(true, true, true);
	mRigidbody->SetGravity(true);
	mColider->CreatePhys();

	BulletTag = FindTagNumber("Bullet");

	//매쉬 생성
	mMeshFilter->SetModelName("MonsterA+");
	mMeshFilter->SetAnimationName("MonsterA+");
	mAnimation->Choice("move");
}

void MonsterA::Update()
{
	if (isLife == true)
	{
		mRigidbody->SetVelocity(0, 0, 1);
	}
}

void MonsterA::OnTriggerStay(GameObject* Obj)
{
	if (Player::GetState() == PLAYER_STATE::ATTACK)
	{
		Vector3 Look = Player::GetPlayerTransform()->GetLocalPosition_Look() * 10;
		mRigidbody->SetAddForce(Look.x, Look.y + 10, Look.z * -1);
	}

	//if (Player::GetState() == PLAYER_STATE::ATTACK)
	//{
	//	Vector3 Look = Player::GetPlayerTransform()->GetLocalPosition_Look() * 10;
	//	mRigidbody->SetAddForce(Look.x, Look.y + 10, Look.z * -1);
	//}
}

void MonsterA::OnTriggerEnter(GameObject* Obj)
{
	int num = Obj->GetTag();
	if (Obj->GetTag() == BulletTag)
	{
		HP -= 20;
		if(HP <= 0)
		{
			isLife = false;
			mAnimation->Choice("die");
		}
	}
}

void MonsterA::ReSet()
{


}

