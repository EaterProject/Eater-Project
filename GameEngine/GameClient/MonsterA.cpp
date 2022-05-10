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
	mTransform	= gameobject->GetTransform();
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

	//매쉬 생성
	mMeshFilter->SetModelName("MonsterA+");
	mMeshFilter->SetAnimationName("MonsterA+");
	mAnimation->Choice("idle");

	//이동 위치
	Vector3 Point = Mana->GetPoint(0, 1);
	SetMovePoint(Point.x, Point.y, Point.z);
}

void MonsterA::Update()
{
	if (GetKeyDown(VK_NUMPAD0))
	{
		Vector3 Point = Mana->GetPoint(0, 0);
		SetMovePoint(Point.x, Point.y, Point.z);
	}
	else if (GetKeyDown(VK_NUMPAD1))
	{
		Vector3 Point = Mana->GetPoint(0, 1);
		SetMovePoint(Point.x, Point.y, Point.z);
	}
	else if (GetKeyDown(VK_NUMPAD2))
	{
		Vector3 Point = Mana->GetPoint(0, 2);
		SetMovePoint(Point.x, Point.y, Point.z);
	}
	else if (GetKeyDown(VK_NUMPAD3))
	{
		Vector3 Point = Mana->GetPoint(0, 3);
		SetMovePoint(Point.x, Point.y, Point.z);
	}
	else if (GetKeyDown(VK_NUMPAD4))
	{
		Vector3 Point = Mana->GetPoint(0, 4);
		SetMovePoint(Point.x, Point.y, Point.z);
	}




	if (GetStopPoint() == false)
	{
		mTransform->Slow_Y_Rotation(MovePoint, 200, true);
		mRigidbody->SetVelocity(DirPoint.x, DirPoint.y, DirPoint.z);
	}

	DebugDrawCircle(2.5f, mTransform->Position, Vector3(0, 0, 0), Vector3(1, 0, 0));
}

void MonsterA::SetMovePoint(float x, float y, float z)
{
	DirPoint = (gameobject->GetTransform()->Position - Vector3(x, y, z)) * -1;
	DirPoint.Normalize();
	DirPoint *= Speed;
	MovePoint.x = x;
	MovePoint.y = y;
	MovePoint.z = z;
}

bool MonsterA::GetStopPoint()
{
	if (mTransform->Position.x > (MovePoint.x - 0.5f) && mTransform->Position.x < (MovePoint.x + 0.5f) &&
		mTransform->Position.z >(MovePoint.z - 0.5f) &&  mTransform->Position.z < (MovePoint.z + 0.5f))
	{
		return true;

	}
	else
	{
		return false;
	}
}

