#include "MonsterB.h"
#include "MeshFilter.h"
#include "Transform.h"
#include "GameObject.h"
#include "AnimationController.h"
#include "EaterEngineAPI.h"
#include "Collider.h"
#include "Rigidbody.h"

MonsterB::MonsterB()
{
	mMeshFilter		= nullptr;
	mTransform		= nullptr;
	mAnimation		= nullptr;
	mColider		= nullptr;
	mRigidbody		= nullptr;
}

MonsterB::~MonsterB()
{
	mMeshFilter		= nullptr;
	mTransform		= nullptr;
	mAnimation		= nullptr;
	mColider		= nullptr;
	mRigidbody		= nullptr;
}

void MonsterB::Awake()
{
	mMeshFilter	= gameobject->GetComponent<MeshFilter>();
	mTransform	= gameobject->GetComponent<Transform>();
	mAnimation	= gameobject->GetComponent<AnimationController>();
	mColider	= gameobject->GetComponent<Collider>();
	mRigidbody	= gameobject->GetComponent<Rigidbody>();
	//mBase		= gameobject->GetComponent<MonsterBase>();
	//Create(100, 10, 5);
}
void MonsterB::SetUp()
{
	//�ݶ��̴� �� ����	
	mColider->SetCenter(0, 0.25, 0);
	mColider->SetBoxCollider(0.25f);
	mColider->SetMaterial_Restitution(0);
	mRigidbody->SetFreezeRotation(true, true, true);
	mRigidbody->SetGravity(false);
	mColider->CreatePhys();

	//�Ž� ����
	mMeshFilter->SetModelName("MonsterB+");
	mMeshFilter->SetAnimationName("MonsterB+");
	mAnimation->Choice("move");
}

void MonsterB::Update()
{
	//if (isLife == true) 
	//{
	//	mRigidbody->SetVelocity(0, 0, -1);
	//}
	//
	////���� ��ġ�� Y�� -10���� ������ Reset��Ų�� 
	//if (mTransform->Position.y < -10)
	//{
	//	mRigidbody->SetPosition(0, 0, 0);
	//}
}

void MonsterB::ReSet()
{
}
