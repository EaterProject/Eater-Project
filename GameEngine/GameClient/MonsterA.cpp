#include "MonsterA.h"
#include "MeshFilter.h"
#include "Transform.h"
#include "GameObject.h"
#include "AnimationController.h"
#include "MainHeader.h"
#include "Collider.h"
#include "Rigidbody.h"
#include "MonsterBase.h"

MonsterA::MonsterA()
{
	mMeshFilter = nullptr;
	mTransform	= nullptr;
	mAnimation	= nullptr;
	mColider	= nullptr;
	mRigidbody	= nullptr;
	mBase		= nullptr;
}

MonsterA::~MonsterA()
{
	mMeshFilter = nullptr;
	mTransform	= nullptr;
	mAnimation	= nullptr;
	mColider	= nullptr;
	mRigidbody	= nullptr;
	mBase		= nullptr;
}

void MonsterA::Awake()
{
	mMeshFilter = gameobject->GetComponent<MeshFilter>();
	mTransform	= gameobject->GetComponent<Transform>();
	mAnimation	= gameobject->GetComponent<AnimationController>();
	mColider	= gameobject->GetComponent<Collider>();
	mRigidbody	= gameobject->GetComponent<Rigidbody>();
	mBase		= gameobject->GetComponent<MonsterBase>();

	//Create(100, 10, 5);
}
void MonsterA::SetUp()
{
	//콜라이더 값 조정
	mColider->SetCenter(0, 0.25, 0);
	mColider->SetBoxCollider(0.25f);
	mColider->SetMaterial_Restitution(0);
	mRigidbody->SetFreezeRotation(true, true, true);
	mColider->CreatePhys();

	//매쉬 생성
	mMeshFilter->SetModelName("MonsterA+");
	mMeshFilter->SetAnimationName("MonsterA+");
	mAnimation->Choice("Move");
}

void MonsterA::Update()
{
	if (mBase->isLife == true)
	{
		mRigidbody->SetVelocity(0, 0, -1);
	}
	//
	//
	////만약 위치값 Y가 -10보다 작으면 Reset시킨다 
	//if (mTransform->Position.y < -10)
	//{
	//	mRigidbody->SetPosition(0, 0, 0);
	//	State = STATE::DEAD;
	//}
}
