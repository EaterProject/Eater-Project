#include "MonsterComponent.h"
#include "GameObject.h"
#include "EaterEngineAPI.h"
#include "ClientTypeOption.h"

//Component
#include "Transform.h"
#include "MeshFilter.h"
#include "AnimationController.h"
#include "Collider.h"
#include "PhysData.h"

MonsterComponent::MonsterComponent()
{
	mMeshFilter = nullptr;
	mTransform	= nullptr;
	mAnimation	= nullptr; 
	mColider	= nullptr;
	mPlayerTR	= nullptr;

	mRay = new PhysRayCast();
}

MonsterComponent::~MonsterComponent()
{
	delete mRay;
}

void MonsterComponent::Awake()
{
	mTransform	= gameobject->GetTransform();
	mMeshFilter = gameobject->GetComponent<MeshFilter>();
	mAnimation	= gameobject->GetComponent<AnimationController>();
	mColider	= gameobject->GetComponent<Collider>();
}

void MonsterComponent::SetUp()
{
	//Collider����
	mColider->SetCenter(0, 0.5f, 0);
	mColider->SetSphereCollider(0.25f);

	mMeshFilter->SetModelName(ModelName);
	mMeshFilter->SetAnimationName(AnimationName);
	mAnimation->Play();

	//�÷��̾� ��ġ ����
	mPlayerTR = FindGameObjectTag("Player")->GetTransform();

	//���� �ʱ� ���� ����
	MonsterState = (int)MONSTER_STATE::IDLE;

	PointNumber = rand() % 5;
	mTransform->Position = SearchPoint[PointNumber];
}

void MonsterComponent::Update()
{
	switch (MonsterState)
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
	case (int)MONSTER_STATE::HIT:
		Hit();
		break;
	case (int)MONSTER_STATE::DEAD:
		Dead();
		break;
	}

	//��üũ 
	GroundCheck();
}

void MonsterComponent::OnTriggerStay(GameObject* Obj)
{
	if (HitStart == false)
	{
		MonsterState = (int)MONSTER_STATE::HIT;
		HitStart	 = true;
	}
}

void MonsterComponent::Move()
{
	//ó�� �ѹ� �� ����
	if (MoveStart == false)
	{
		//���� ��ǥ������ ����
		mAnimation->Choice(Animation_Move);
		MoveStart = true;
	}
	
	if (GetStopPoint(PointNumber) == false)
	{
		//��ǥ������ �������� �ʾ�����
		mTransform->Slow_Y_Rotation(SearchPoint[PointNumber], 150, MonsterFront_Z);
		mTransform->SetTranlate(DirPoint.x * GetDeltaTime(), 0, DirPoint.z *GetDeltaTime());
	}
	else
	{
		//��ǥ���� ���� �� ���� ��ȭ
		MonsterState	= (int)MONSTER_STATE::IDLE;
		PointNumber		= -1;
		MoveStart		= false;
	}
}

void MonsterComponent::Attack()
{


}

void MonsterComponent::Idle()
{
	//ó�� �ѹ� �� ����
	if (IdleStart == false)
	{
		//�ִϸ��̼� ����
		mAnimation->Choice(Animation_Idel);
		//�̵� �ؾ��ϴ� �ѹ� ����
		PointNumber		= rand() % 5;
		//���ʱ��� Idle���·� ���������� ����
		Idle_MaxTime	= (rand() % Idle_MaxTime_Max) + Idle_MaxTime_Min;

		SetMovePoint(SearchPoint[PointNumber].x, 0, SearchPoint[PointNumber].z);
		IdleStart = true;
	}

	//���ð��� �������� ���������� üũ
	if (IdleTime <= Idle_MaxTime)
	{
		IdleTime += GetDeltaTime();
	}
	else
	{
		//���ð��� �����ٸ� Move���·� ����
		Speed			= IdleSpeed;
		MonsterState	= (int)MONSTER_STATE::MOVE;
		IdleStart		= false;
		IdleTime		= 0;
	}
}

void MonsterComponent::Dead()
{



}

void MonsterComponent::Chase()
{



}

void MonsterComponent::Hit()
{
	if (HP > 0)
	{
		mAnimation->Choice(Animation_hit);
		HitTime += GetDeltaTime();

		if (HitTime >= HitMaxTime)
		{
			HitStart = false;
			HitTime = 0;
		}
	}
	else
	{
		mAnimation->Choice(Animation_Die);
		MonsterState = (int)MONSTER_STATE::DEAD;
	}
}

void MonsterComponent::Debug()
{
	DebugDrawCircle(ChaseRange, mTransform->Position, Vector3(0, 0, 0), Vector3(1, 0, 0));
	DebugDrawCircle(AttackRange, mTransform->Position, Vector3(0, 0, 0), Vector3(0, 1, 0));
}

void MonsterComponent::SetSearchPoint(int Index, Vector3 Point)
{
	SearchPoint[Index] = Point;
}
 
void MonsterComponent::SetMovePoint(float x, float y, float z)
{
	DirPoint = (gameobject->GetTransform()->Position - Vector3(x, y, z)) * -1;
	DirPoint.Normalize();
	DirPoint *= Speed;
	MovePoint.x = x;
	MovePoint.y = 0;
	MovePoint.z = z;
	ReturnPoint = MovePoint;
}

bool MonsterComponent::GetStopPoint(int Index)
{
	Transform* mTransform = gameobject->GetTransform();
	if (mTransform->Position.x > (SearchPoint[Index].x - 0.1f) &&
		mTransform->Position.x < (SearchPoint[Index].x + 0.1f) &&
		mTransform->Position.z > (SearchPoint[Index].z - 0.1f) &&
		mTransform->Position.z < (SearchPoint[Index].z + 0.1f))
	{
		return true;

	}
	else
	{
		return false;
	}
}

void MonsterComponent::GroundCheck()
{
	//�Ʒ� �������� Ray�� ����Ͽ� ��üũ
	Vector3 RayStartPoint	= mTransform->Position;
	RayStartPoint.y			= mTransform->Position.y+0.1f;
	mRay->Origin			= RayStartPoint;
	mRay->Direction			= { 0,-1,0 };
	mRay->MaxDistance		= 10;
	if (RayCast(mRay) == true)
	{
		mTransform->Position.y = mRay->Hit.HitPoint.y;
	}
}


