#include "MonsterA.h"
#include "MeshFilter.h"
#include "Transform.h"
#include "GameObject.h"
#include "AnimationController.h"
#include "MainHeader.h"
#include "Collider.h"
#include "Rigidbody.h"
#include "MonsterBase.h"
#include "Player.h"

MonsterA::MonsterA()
{
	mMeshFilter = nullptr;
	mTransform	= nullptr;
	mAnimation	= nullptr;
	mColider	= nullptr;
	mRigidbody	= nullptr;
	mBase		= nullptr;

	BackAttackSlipPower = 10.0f;
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
	mColider->SetSphereCollider(0.25f);
	mColider->SetMaterial_Restitution(0);
	mRigidbody->SetFreezeRotation(true, true, true);
	mRigidbody->SetGravity(true);
	mColider->CreatePhys();

	//매쉬 생성
	mMeshFilter->SetModelName("monsterA+");
	mMeshFilter->SetAnimationName("monsterA+");
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

void MonsterA::OnTriggerStay(GameObject* Obj)
{
	//충돌중일때 플레이어 상태가 Attack 상태면 공격을당한다
	if (Player::GetState() == PLAYER_STATE::ATTACK)
	{
		Vector3 Look = Player::GetPlayerTransform()->GetLocalPosition_Look()  * BackAttackSlipPower;
		mRigidbody->SetAddForce(Look.x, Look.y+ BackAttackSlipPower, Look.z *-1);
	}
}

