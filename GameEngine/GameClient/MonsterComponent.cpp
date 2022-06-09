#include "MonsterComponent.h"
#include "GameObject.h"
#include "Player.h"
#include "EngineData.h"
#include "EaterEngineAPI.h"


//Component
#include "Transform.h"
#include "MeshFilter.h"
#include "AnimationController.h"
#include "Collider.h"
#include "Rigidbody.h"
#include "PhysData.h"

MonsterComponent::MonsterComponent():
	mRay(new PhysRayCast)
{

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
	mRigidbody  = gameobject->GetComponent<Rigidbody>();
}

void MonsterComponent::SetUp()
{
	//Collider설정
	mColider->SetCenter(0, 0.25f, 0);
	mColider->SetSphereCollider(0.25f);
	mRigidbody->SetFreezeRotation(true, true, true);

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
	PlayerDistanceCheck();
	UpdateColor();

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
}

void MonsterComponent::OnTriggerStay(GameObject* Obj)
{
	if (HitStart == false)
	{
		if (Player::GetAttackState() == true)
		{
			mSkinFilter = gameobject->GetChildMesh(0)->GetComponent<MeshFilter>();
			mSkinFilter->SetMaterialPropertyBlock(true);
			MPB = mSkinFilter->GetMaterialPropertyBlock();
			MPB->LimLightColor = NowLimLightColor;
			MPB->LimLightFactor = NowLimLightFactor;
			MPB->LimLightWidth = NowLimLightWidth;
			SetLimLightColor(MONSTER_COLOR::RED);

			MonsterState = (int)MONSTER_STATE::HIT;
			HitStart	 = true;
		}
	}
}

void MonsterComponent::Move()
{
	//추격상태 체크
	if (ChaseRange > PlayerDistance)
	{
		SetMonsterState(MONSTER_STATE::CHASE);
	}

	//처음 한번 만 실행
	if (MoveStart == false)
	{
		//현재 목표지점을 저장
		mAnimation->Choice(Animation_Move,Animation_Move_Speed);
		MoveStart = true;
	}
	
	if (GetStopPoint(SearchPoint[PointNumber]) == false)
	{
		//목표지점의 도달하지 않았을때
		mTransform->Slow_Y_Rotation(SearchPoint[PointNumber], RotationSpeed, MonsterFront_Z);
		mRigidbody->SetVelocity(DirPoint.x, 0, DirPoint.z);
	}
	else
	{
		SetMonsterState(MONSTER_STATE::IDLE);
	}
}

void MonsterComponent::Attack()
{
	if (AttackStart == false)
	{
		mAnimation->Choice(Animation_Attack, Animation_Attack_Speed);
		AttackStart = true;
	}

	//공격 범위에 나갔을떄 다시 추격 상태로
	if (AttackRange < PlayerDistance)
	{
		SetMonsterState(MONSTER_STATE::CHASE);
	}

	mTransform->Slow_Y_Rotation(mPlayerTR->Position, RotationSpeed, MonsterFront_Z);
}

void MonsterComponent::Idle()
{
	//추격상태 체크
	if (ChaseRange > PlayerDistance)
	{
		SetMonsterState(MONSTER_STATE::CHASE);
	}

	//처음 한번 만 실행
	if (IdleStart == false)
	{
		//애니메이션 설정
		mAnimation->Choice(Animation_Idel,Animation_Idle_Speed);
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
		SetMonsterState(MONSTER_STATE::MOVE);
	}
}

void MonsterComponent::Dead()
{



}

void MonsterComponent::Chase()
{
	if (ChaseStart == false)
	{
		mAnimation->Choice(Animation_Move, Animation_Chase_Speed);
		ChaseStart = true;
	}

	ChaseTime += GetDeltaTime();
	if (AttackRange > PlayerDistance)
	{
		//공격범위에 들어왔을경우 공격 상태로 변경
		mRigidbody->SetVelocity(0, 0, 0);
		SetMonsterState(MONSTER_STATE::ATTACK);
	}
	else
	{
		//추격시간 계산
		if (ChaseTime >= ChaseEndTime) 
		{
			//추격시간을 넘었다면 원래 상태로 돌아간다
			SetMovePoint(SearchPoint[PointNumber].x, 0, SearchPoint[PointNumber].z);
			SetMonsterState(MONSTER_STATE::MOVE);
			ChaseTime = 0;
		}
		else
		{
			//계속 추격
			SetMovePoint(mPlayerTR->Position.x, 0, mPlayerTR->Position.z);
			mTransform->Slow_Y_Rotation(mPlayerTR->Position, 150, MonsterFront_Z);
			mRigidbody->SetVelocity(DirPoint.x, 0, DirPoint.z);
		}
	}
}

void MonsterComponent::Hit()
{

	if (HP > 0)
	{
		mAnimation->Choice(Animation_hit, Animation_hit_Speed);
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
	DebugDrawCircle(ChaseRange, mTransform->Position  + Vector3(0, 0.25f, 0), Vector3(0, 0, 0), Vector3(1, 0, 0));
	DebugDrawCircle(AttackRange, mTransform->Position + Vector3(0, 0.25f, 0), Vector3(0, 0, 0), Vector3(0, 1, 0));
}

void MonsterComponent::PlayerDistanceCheck()
{
	PlayerDistance = mTransform->GetDistance(mPlayerTR->Position);
}

void MonsterComponent::SetSearchPoint(int Index, Vector3 Point)
{
	SearchPoint[Index] = Point;
}
 
void MonsterComponent::SetMovePoint(float x, float y, float z)
{
	//방향값을 설정
	DirPoint = (Vector3(x, y, z) - mTransform->Position);
	DirPoint.Normalize();
	DirPoint *= Speed;

	//이동 위치값을 설정
	MovePoint.x = x;
	MovePoint.y = 0;
	MovePoint.z = z;
	ReturnPoint = MovePoint;
}

void MonsterComponent::SetMonsterState(MONSTER_STATE State)
{
	//몬스터의 상태를 변경한다
	MonsterState = (int)State;

	//모든 상태를 false로 변경
	IdleStart	= false;		//Idle	 상태 시작 변수
	AttackStart = false;		//Attack 상태 시작 변수
	MoveStart	= false;		//Move   상태 시작 변수
	ChaseStart	= false;		//Chase  상태 시작 변수
	//HitStart	= false;		//Hit	 상태 시작 변수

	//타임 변수들도 초기화
	IdleTime	= 0;
	HitTime		= 0;
	AttackTime	= 0;

	//속력 제로
	mRigidbody->SetVelocity(0, 0, 0);
}

void MonsterComponent::SetLimLightColor(MONSTER_COLOR mColor)
{
	switch (mColor)
	{
	case MONSTER_COLOR::RED:
		NowLimLightColor = { 1,0,0 };
		break;
	case MONSTER_COLOR::GREEN:
		NowLimLightColor = { 0,1,0 };
		break;
	case MONSTER_COLOR::YELLOW:
		NowLimLightColor = { 1,1,0 };
		break;
	case MONSTER_COLOR::BLUE:
		NowLimLightColor = { 0,0,1 };
		break;
	}

	NowUpdateColor = true;
}

void MonsterComponent::UpdateColor()
{
	if (NowUpdateColor == false) { return; }

	if (NowLimLightFactor <= 0)
	{
		NowLimLightFactor = 0;
		NowUpdateColor = false;
	}
	else
	{

		NowLimLightFactor -= GetDeltaTime();
	}


}

bool MonsterComponent::GetStopPoint(const Vector3& Pos)
{
	Transform* mTransform = gameobject->GetTransform();
	if (mTransform->Position.x > (Pos.x - 0.25f) &&
		mTransform->Position.x < (Pos.x + 0.25f) &&
		mTransform->Position.z > (Pos.z - 0.25f) &&
		mTransform->Position.z < (Pos.z + 0.25f))
	{
		return true;

	}
	else
	{
		return false;
	}
}
