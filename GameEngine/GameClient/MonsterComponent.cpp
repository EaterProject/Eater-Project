#include "MonsterComponent.h"
#include "GameObject.h"
#include "Player.h"
#include "EngineData.h"
#include "EaterEngineAPI.h"
#include "MessageManager.h"


//Component
#include "Transform.h"
#include "MeshFilter.h"
#include "AnimationController.h"
#include "Collider.h"
#include "Rigidbody.h"
#include "PhysData.h"

MonsterComponent::MonsterComponent()
{

}

MonsterComponent::~MonsterComponent()
{
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
	//Collider����
	mColider->SetSphereCollider(0.5f);
	mColider->SetCenter(0, 0.5f, 0);
	mColider->SetMaterial_Restitution(0);
	mRigidbody->SetFreezeRotation(true, true, true);

	mMeshFilter->SetModelName(ModelName);
	mMeshFilter->SetAnimationName(AnimationName);
	mAnimation->Play();

	//�÷��̾� ��ġ ����
	mPlayerTR = FindGameObjectTag("Player")->GetTransform();

	//���� �ʱ� ���� ����
	MonsterState = (int)MONSTER_STATE::IDLE;

	PointNumber = rand() % 5;
	mTransform->Position = SearchPoint[PointNumber];

	float Scale = NowHitMonsterScale + NowHitMonsterScale_F;
	mTransform->Scale = { Scale,Scale,Scale };
}

void MonsterComponent::Update()
{
	switch (MonsterState)
	{
	case (int)MONSTER_STATE::IDLE:
		PlayerDistanceCheck();
		UpdateColor();
		Idle();
		break;
	case (int)MONSTER_STATE::MOVE:
		PlayerDistanceCheck();
		UpdateColor();
		Move();
		break;
	case (int)MONSTER_STATE::ATTACK:
		PlayerDistanceCheck();
		UpdateColor();
		Attack();
		break;
	case (int)MONSTER_STATE::CHASE:
		PlayerDistanceCheck();
		UpdateColor();
		Chase();
		break;
	case (int)MONSTER_STATE::HIT:
		PlayerDistanceCheck();
		UpdateColor();
		Hit();
		break;
	case (int)MONSTER_STATE::DEAD:
		UpdateColor();
		Dead();
		break;
	}
}

void MonsterComponent::OnTriggerStay(GameObject* Obj)
{
	//�÷��̾� �浹ü�� �浹������
	if (HitStart == false)
	{
		//�÷��̾ ���� �����϶�
		if (Player::GetAttackState() == true)
		{
			MessageManager::GetGM()->SEND_Message(TARGET_PLAYER, MESSAGE_PLAYER_ATTACK_OK);
			//���� �ٲٴ� �Լ������͸� �ְ� ���º�ȭ
			HitFunction = std::bind(&MonsterComponent::SetLimLightColor, this);
			SetMonsterState(MONSTER_STATE::HIT);
			
			HP -= 20;
			HitStart	 = true;
			//���� ���
			Sound_Play_SFX(Sound_Hit);
		}
	}
}

void MonsterComponent::Move()
{
	//�߰ݻ��� üũ
	if (ChaseRange > PlayerDistance)
	{
		SetMonsterState(MONSTER_STATE::CHASE);
	}

	//ó�� �ѹ� �� ����
	if (MoveStart == false)
	{
		//���� ��ǥ������ ����
		mAnimation->Choice(Animation_Move,Animation_Move_Speed);
		MoveStart = true;
	}
	
	if (MoveSoundTime >= MoveSoundTimeMax)
	{
		MoveSoundTime = 0;
		//Sound_Play_SFX(Sound_move);
	}
	else
	{
		MoveSoundTime += GetDeltaTime();
	}

	if (GetStopPoint(SearchPoint[PointNumber]) == false)
	{
		//��ǥ������ �������� �ʾ�����
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

	//���� ������ �������� �ٽ� �߰� ���·�
	if (AttackRange < PlayerDistance)
	{
		SetMonsterState(MONSTER_STATE::CHASE);
	}

	mTransform->Slow_Y_Rotation(mPlayerTR->Position, RotationSpeed, MonsterFront_Z);
}

void MonsterComponent::Idle()
{
	//�߰ݻ��� üũ
	if (ChaseRange > PlayerDistance)
	{
		SetMonsterState(MONSTER_STATE::CHASE);
	}

	//ó�� �ѹ� �� ����
	if (IdleStart == false)
	{
		//�ִϸ��̼� ����
		mAnimation->Choice(Animation_Idel,Animation_Idle_Speed);
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
		//Speed			= IdleSpeed;
		SetMonsterState(MONSTER_STATE::MOVE);
	}
}

void MonsterComponent::Dead()
{
	if(DeadStart == false)
	{
		mAnimation->Choice(Animation_Die, Animation_hit_Speed);
		DeadStart = true;
	}

	float End = mAnimation->GetEndFrame();
	float Now = mAnimation->GetNowFrame();
	if (Now >= End)
	{
		gameobject->SetActive(false);
	}
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
		//���ݹ����� ��������� ���� ���·� ����
		mRigidbody->SetVelocity(0, 0, 0);
		SetMonsterState(MONSTER_STATE::ATTACK);
	}
	else
	{
		//�߰ݽð� ���
		if (ChaseTime >= ChaseEndTime) 
		{
			//�߰ݽð��� �Ѿ��ٸ� ���� ���·� ���ư���
			SetMovePoint(SearchPoint[PointNumber].x, 0, SearchPoint[PointNumber].z);
			SetMonsterState(MONSTER_STATE::MOVE);
			ChaseTime = 0;
		}
		else
		{
			//��� �߰�
			SetMovePoint(mPlayerTR->Position.x, 0, mPlayerTR->Position.z);
			mTransform->Slow_Y_Rotation(mPlayerTR->Position, 150, MonsterFront_Z);
			mRigidbody->SetVelocity(DirPoint.x, 0, DirPoint.z);
		}
	}
}

void MonsterComponent::Hit()
{
	//���� ��������
	if (HP > 0)
	{
		mAnimation->Choice(Animation_hit, Animation_hit_Speed);
		float End = mAnimation->GetEndFrame();
		float Now = mAnimation->GetNowFrame();
		if (Now >= End)
		{
			HitStart = false;
			SetMonsterState(MONSTER_STATE::CHASE);
		}
	}
	else
	{
		mAnimation->Choice(Animation_Die);
		SetMonsterState(MONSTER_STATE::DEAD);
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
	//���Ⱚ�� ����
	DirPoint = (Vector3(x, y, z) - mTransform->Position);
	DirPoint.Normalize();
	DirPoint *= Speed;

	//�̵� ��ġ���� ����
	MovePoint.x = x;
	MovePoint.y = 0;
	MovePoint.z = z;
	ReturnPoint = MovePoint;
}

void MonsterComponent::SetMonsterState(MONSTER_STATE State)
{
	//������ ���¸� �����Ѵ�
	MonsterState = (int)State;

	//��� ���¸� false�� ����
	IdleStart	= false;		//Idle	 ���� ���� ����
	AttackStart = false;		//Attack ���� ���� ����
	MoveStart	= false;		//Move   ���� ���� ����
	ChaseStart	= false;		//Chase  ���� ���� ����

	//Ÿ�� �����鵵 �ʱ�ȭ
	IdleTime	= 0;
	HitTime		= 0;
	AttackTime	= 0;

	//�ӷ� ����
	mRigidbody->SetVelocity(0, 0, 0);
}

void MonsterComponent::SetLimLightColor()
{
	if (HitFXStart == false)
	{
		//��Ų ������Ʈ ��������
		if (mSkinFilter == nullptr){mSkinFilter = gameobject->GetChildMesh(0)->GetComponent<MeshFilter>();}

		//���׸��� ��� ��������
		mSkinFilter->SetMaterialPropertyBlock(true);
		MPB = mSkinFilter->GetMaterialPropertyBlock();
		//�� ����Ʈ ���� ���� ����
		MPB->LimLightColor	= NowLimLightColor;
		MPB->LimLightFactor = NowLimLightFactor;
		MPB->LimLightWidth	= NowLimLightWidth;
		//���� ũ�� ����
		float Scale = NowHitMonsterScale + NowHitMonsterScale_F;
		mTransform->Scale = { Scale,Scale,Scale };

		HitFXStart = true;
	}

	//�ð��� ���� ������Ʈ ȿ���� �پ���
	if (MPB->LimLightFactor <= 0)
	{
		float Scale = NowHitMonsterScale + NowHitMonsterScale_F;
		mTransform->Scale.x = Scale;
		mTransform->Scale.y = Scale;
		mTransform->Scale.z = Scale;
		HitFunction = nullptr;
		HitFXStart  = false;
		mSkinFilter->SetMaterialPropertyBlock(false);
	}
	else
	{
		float DTime = GetDeltaTime();
		mTransform->Scale.x -= DTime * NowHitMonsterScale_F;
		mTransform->Scale.y -= DTime * NowHitMonsterScale_F;
		mTransform->Scale.z -= DTime * NowHitMonsterScale_F; 
		MPB->LimLightFactor -= DTime * NowLimLightFactor;
	}
}

void MonsterComponent::UpdateColor()
{
	if (HitFunction != nullptr)
	{
		HitFunction();
	}
	else
	{
		return;
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
