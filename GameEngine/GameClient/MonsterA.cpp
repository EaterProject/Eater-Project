#include "MonsterA.h"
#include "MeshFilter.h"
#include "Transform.h"
#include "GameObject.h"
#include "AnimationController.h"
#include "EaterEngineAPI.h"
#include "Collider.h"
#include "Rigidbody.h"
#include "Player.h"
#include "ManaStone.h"
#include "ClientTypeOption.h"
#include <time.h>

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

void MonsterA::Create(ManaStone* mMana, int mCreatePointIndex)
{
	Mana = mMana;
	PointIndex = mCreatePointIndex;
}

void MonsterA::Awake()
{
	mMeshFilter = gameobject->GetComponent<MeshFilter>();
	mTransform	= gameobject->GetTransform();
	mAnimation	= gameobject->GetComponent<AnimationController>();
	mColider	= gameobject->GetComponent<Collider>();
	mRigidbody  = gameobject->GetComponent<Rigidbody>();
}
void MonsterA::SetUp()
{
	//�ݶ��̴� �� ����
	mColider->SetCenter(0, 0.25f, 0);
	mColider->SetSphereCollider(0.25f);
	mColider->SetMaterial_Restitution(0);
	mRigidbody->SetFreezeRotation(true, true, true);
	mRigidbody->SetGravity(true);
	mColider->CreatePhys();

	//�Ž� ����a
	mMeshFilter->SetModelName("MonsterA+");
	mMeshFilter->SetAnimationName("MonsterA+");
	mAnimation->Play();

	//�÷��̾�
	mPlayer = FindGameObjectTag("Player");
	mPlayerTR = mPlayer->GetTransform();

	//�̵� ��ġ
	Vector3 Point = Mana->GetPoint(PointIndex, 1);
	SetMovePoint(Point.x, Point.y, Point.z);

	srand((unsigned int)time(NULL));
	State = (int)MONSTER_STATE::IDLE;
}

void MonsterA::Update()
{
	switch (State)
	{
	case (int)MONSTER_STATE::IDLE:
		Idle();
		break;
	case (int)MONSTER_STATE::MOVE:
		Move();
		break;
	case (int)MONSTER_STATE::ATTACK:
		Attack();
		break;
	case (int)MONSTER_STATE::CHASE:
		Chase();
		break;
	}

	Debug();
}

void MonsterA::OnTriggerStay(GameObject* Obj)
{
	if (Obj->GetTag() == 6 && Player::GetAttackState() == true)
	{
		mAnimation->Choice("die");
		gameobject->SetActive(false);
	}
}

void MonsterA::Move()
{
	if (MoveStart == false)
	{
		mAnimation->Choice("move");
		MoveStart = true;
	}
	
	if (GetStopPoint() == false)
	{
		//��ǥ������ �������� �ʾ�����
		mTransform->Slow_Y_Rotation(MovePoint, 100,false);
		mRigidbody->SetVelocity(DirPoint.x, DirPoint.y, DirPoint.z);
	}
	else
	{
		//��ǥ���� ���� �� ���� ��ȭ
		State = (int)MONSTER_STATE::IDLE;
		PointNumber = -1;
		MoveStart = false;
	}
}

void MonsterA::Attack()
{
	if (AttackStart == false) 
	{

		
		AttackStart = true;
	}
}

void MonsterA::Idle()
{
	if (mTransform->GetDistance(mPlayer->GetTransform()->Position) <= AttackDir)
	{

		return;
	}

	//�⺻ �� ����
	if (IdleStart == false)
	{
		mAnimation->Choice("idle");
		PointNumber		= rand() % 5;
		Idle_MaxTime	= (rand() % Idle_MaxTime_Max) + Idle_MaxTime_Min;
		Vector3 Point	= Mana->GetPoint(PointIndex, PointNumber);
		SetMovePoint(Point.x, Point.y, Point.z);
		IdleStart = true;
	}


	if (IdleTime <= Idle_MaxTime)
	{
		IdleTime += GetDeltaTime();
	}
	else
	{
		//���� ��ȭ �⺻�� �ʱ�ȭ
		State = (int)MONSTER_STATE::MOVE;
		IdleStart	= false;
		IdleTime	= 0;
	}
}

void MonsterA::Chase()
{
	mTransform->Slow_Y_Rotation(MovePoint, 100, false);
	mRigidbody->SetVelocity(mPlayerTR->Position.x, DirPoint.y, mPlayerTR->Position.z);
}

void MonsterA::Debug()
{
	DebugDrawCircle(AttackDir, mTransform->Position, Vector3(0, 0, 0), Vector3(1, 0, 0));
}



