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
	//�ݶ��̴� �� ����
	mColider->SetCenter(0, 0.25, 0);
	mColider->SetBoxCollider(0.25f);
	mColider->SetMaterial_Restitution(0);
	mRigidbody->SetFreezeRotation(true, true, true);
	mRigidbody->SetGravity(true);
	mColider->CreatePhys();

	//�Ž� ����
	mMeshFilter->SetModelName("MonsterA+");
	mMeshFilter->SetAnimationName("MonsterA+");
	mAnimation->Choice("move");
}

void MonsterA::Update()
{
	//if (mRigidbody->GetTriggerEnter())
	//{
	//	GameObject* Obj = mRigidbody->GetTriggerObject();
	//	int num = 0;
	//}

	if (mBase->isLife == true)
	{
		mRigidbody->SetVelocity(0, 0, -1);
	}
}

