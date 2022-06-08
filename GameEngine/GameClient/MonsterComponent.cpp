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
	//Collider설정
	mColider->SetCenter(0, 0.5f, 0);
	mColider->SetSphereCollider(0.25f);

	mMeshFilter->SetModelName(ModelName);
	mMeshFilter->SetAnimationName(AnimationName);
	mAnimation->Play();

	//플레이어 위치 설정
	mPlayerTR = FindGameObjectTag("Player")->GetTransform();

	//몬스터 초기 상태 지정
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

	//땅체크 
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
	//처음 한번 만 실행
	if (MoveStart == false)
	{
		//현재 목표지점을 저장
		mAnimation->Choice(Animation_Move);
		MoveStart = true;
	}
	
	if (GetStopPoint(PointNumber) == false)
	{
		//목표지점의 도달하지 않았을때
		mTransform->Slow_Y_Rotation(SearchPoint[PointNumber], 150, MonsterFront_Z);
		mTransform->SetTranlate(DirPoint.x * GetDeltaTime(), 0, DirPoint.z *GetDeltaTime());
	}
	else
	{
		//목표지점 도달 후 상태 변화
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
	//처음 한번 만 실행
	if (IdleStart == false)
	{
		//애니메이션 설정
		mAnimation->Choice(Animation_Idel);
		//이동 해야하는 넘버 설정
		PointNumber		= rand() % 5;
		//몇초까지 Idle상태로 있을것인지 설정
		Idle_MaxTime	= (rand() % Idle_MaxTime_Max) + Idle_MaxTime_Min;

		SetMovePoint(SearchPoint[PointNumber].x, 0, SearchPoint[PointNumber].z);
		IdleStart = true;
	}

	//대기시간이 지났는지 않지났는지 체크
	if (IdleTime <= Idle_MaxTime)
	{
		IdleTime += GetDeltaTime();
	}
	else
	{
		//대기시간이 지났다면 Move상태로 변경
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
	//아래 방향으로 Ray를 사용하여 땅체크
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


