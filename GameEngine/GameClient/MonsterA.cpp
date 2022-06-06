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
#include "PhysData.h"
#include <time.h>

MonsterA::MonsterA()
{
	mMeshFilter = nullptr;
	mTransform	= nullptr;
	mAnimation	= nullptr;
	mColider	= nullptr;
}

MonsterA::~MonsterA()
{
	mMeshFilter = nullptr;
	mTransform	= nullptr;
	mAnimation	= nullptr;
	mColider	= nullptr;
}

void MonsterA::Awake()
{
	MonsterComponent::Awake();
}
void MonsterA::SetUp()
{
	ModelName		= "MonsterA+";
	AnimationName	= "MonsterA+";
	MonsterComponent::SetUp();
}

void MonsterA::Update()
{
	MonsterComponent::Update();

	//if (HP <= 0) { State = (int)MONSTER_STATE::DEAD; }
	//mRay->Direction = Vector3(0, -5, 0);
	//mRay->Origin = mTransform->Position;
	//mRay->Origin.y += 2;
	//mRay->MaxDistance = 10;
	//if (RayCast(mRay)) 
	//{
	//	Pos_Y = mRay->Hit.HitPoint.y;
	//};
	//
	//switch (State)
	//{
	//case (int)MONSTER_STATE::IDLE:
	//	Idle();
	//	break;
	//case (int)MONSTER_STATE::MOVE:
	//	Move();
	//	break;
	//case (int)MONSTER_STATE::ATTACK:
	//	Attack();
	//	break;
	//case (int)MONSTER_STATE::CHASE:
	//	Chase();
	//	break;
	//case (int)MONSTER_STATE::DEAD:
	//	Dead();
	//	break;
	//}
	//
	//Debug();
}

//void MonsterA::OnTriggerStay(GameObject* Obj)
//{
//	//if (Obj->GetTag() == 6 && Player::GetAttackState() == true)
//	//{
//	//	mAnimation->Choice("die");
//	//	HP -= 50;
//	//}
//}
//
//void MonsterA::Move()
//{
//	//if (MoveStart == false)
//	//{
//	//	//���� ��ǥ������ ����
//	//	mAnimation->Choice("move");
//	//	MoveStart = true;
//	//}
//	//
//	//if (GetStopPoint() == false)
//	//{
//	//	//��ǥ������ �������� �ʾ�����
//	//	mTransform->Slow_Y_Rotation(MovePoint, 100,true);
//	//	mTransform->SetTranlate(DirPoint.x * GetDeltaTime(), Pos_Y, DirPoint.z *GetDeltaTime());
//	//}
//	//else
//	//{
//	//	//��ǥ���� ���� �� ���� ��ȭ
//	//	State = (int)MONSTER_STATE::IDLE;
//	//	PointNumber = -1;
//	//	MoveStart = false;
//	//}
//}
//
//void MonsterA::Attack()
//{
//
//	//if (mAnimation->GetNowFrame() >= mAnimation->GetEndFrame())
//	//{
//	//	State = (int)MONSTER_STATE::CHASE;
//	//}
//	//
//	//if (AttackStart == false) 
//	//{
//	//	mAnimation->Choice("attack");
//	//	AttackStart = true;
//	//}
//}
//
//void MonsterA::Idle()
//{
//	//if (mTransform->GetDistance(mPlayer->GetTransform()->Position) <= ChaseRange)
//	//{
//	//	State = (int)MONSTER_STATE::CHASE;
//	//	return;
//	//}
//	//
//	////�⺻ �� ����
//	//if (IdleStart == false)
//	//{
//	//	mAnimation->Choice("idle");
//	//	PointNumber		= rand() % 5;
//	//	Idle_MaxTime	= (rand() % Idle_MaxTime_Max) + Idle_MaxTime_Min;
//	//	Vector3 Point	= Mana->GetPoint(PointIndex, PointNumber);
//	//	SetMovePoint(Point.x, Point.y, Point.z);
//	//	IdleStart = true;
//	//}
//	//
//	//
//	//if (IdleTime <= Idle_MaxTime)
//	//{
//	//	IdleTime += GetDeltaTime();
//	//}
//	//else
//	//{
//	//	//���� ��ȭ �⺻�� �ʱ�ȭ
//	//	Speed = IdleSpeed;
//	//	State = (int)MONSTER_STATE::MOVE;
//	//	IdleStart	= false;
//	//	IdleTime	= 0;
//	//}
//}
//
//void MonsterA::Dead()
//{
//	//mAnimation->Choice("die");
//	//if (mAnimation->GetNowFrame() >= mAnimation->GetEndFrame())
//	//{
//	//	gameobject->SetActive(false);
//	//}
//}
//
//void MonsterA::Chase()
//{
//	//mAnimation->Choice("move");
//	//
//	////���� �Ÿ��� ������ ������ �Ѵ�
//	//if (mTransform->GetDistance(mPlayer->GetTransform()->Position) <= AttackRange)
//	//{
//	//	mTransform->SetTranlate(0,0,0);
//	//	State = (int)MONSTER_STATE::ATTACK;
//	//	return;
//	//}
//	//
//	////�߰ݽð��� ������� �����ִ� ��ġ�� ���ư���
//	//ChaseTime += GetDeltaTime();
//	//if (ChaseTime >= ChaseEndTime)
//	//{
//	//	ChaseTime -= ChaseEndTime;
//	//	State = (int)MONSTER_STATE::MOVE;
//	//}
//	//else
//	//{
//	//	Speed = ChaseSpeed;
//	//	SetMovePoint(mPlayerTR->Position.x, mPlayerTR->Position.y, mPlayerTR->Position.z);
//	//	mTransform->Slow_Y_Rotation(MovePoint, 100, false);
//	//	mTransform->SetTranlate(DirPoint.x * GetDeltaTime() , Pos_Y, DirPoint.z * GetDeltaTime());
//	//}
//}
//
//void MonsterA::Debug()
//{
//	//DebugDrawCircle(ChaseRange, mTransform->Position, Vector3(0, 0, 0), Vector3(1, 0, 0));
//	//DebugDrawCircle(AttackRange, mTransform->Position, Vector3(0, 0, 0), Vector3(0, 1, 0));
//}



