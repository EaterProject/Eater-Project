#include "Monster.h"
#include "MeshFilter.h"
#include "Transform.h"
#include "GameObject.h"
#include "AnimationController.h"
#include "MainHeader.h"
#include "Collider.h"
#include "Rigidbody.h"

Monster::Monster()
{
	mMeshFilter = nullptr;
	mTransform	= nullptr;
	mAnimation	= nullptr;
	mColider	= nullptr;
	mRigidbody	= nullptr;
}

Monster::~Monster()
{
	mMeshFilter = nullptr;
	mTransform	= nullptr;
	mAnimation	= nullptr;
	mColider = nullptr;
	mRigidbody = nullptr;
}

void Monster::Awake()
{
	mMeshFilter = gameobject->GetComponent<MeshFilter>();
	mTransform	= gameobject->GetComponent<Transform>();
	mAnimation	= gameobject->GetComponent<AnimationController>();
	mColider	= gameobject->GetComponent<Collider>();
	mRigidbody	= gameobject->GetComponent<Rigidbody>();
}
void Monster::SetUp()
{
	//�ݶ��̴� �� ����	
	mColider->SetCenter(0, 0, 0);
	mColider->SetBoxCollider(0.25f);
	mRigidbody->SetFreezeRotation(true, true, true);
	mColider->CreatePhys();

	//�Ž� ����
	mMeshFilter->SetModelName("MonsterA+");
	mMeshFilter->SetAnimationName("MonsterA+");
	mAnimation->Choice("Move");
}

void Monster::Update()
{
	
}

void Monster::MonsterUpdate(Vector3 Pos)
{
	mTransform->Position = {0,1,0};
}
