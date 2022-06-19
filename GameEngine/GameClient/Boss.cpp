#include "Boss.h"
#include "MessageManager.h"
#include "EaterEngineAPI.h"
#include "EngineData.h"
#include "MessageManager.h"
#include "PhysRay.h"

//component
#include "GameObject.h"
#include "Transform.h"
#include "Player.h"
#include "Collider.h"
#include "Rigidbody.h"
#include "MeshFilter.h"
#include "AnimationController.h"

Boss::Boss()
{
	//애니메이션 이름 셋팅
	ANIMATION_NAME[(int)BOSS_STATE::GROGGY_START]			= "weak_1";
	ANIMATION_NAME[(int)BOSS_STATE::GROGGY_PLAY]			= "weak_2";
	ANIMATION_NAME[(int)BOSS_STATE::GROGGY_END]				= "weak_3";
	ANIMATION_NAME[(int)BOSS_STATE::IDLE]					= "idle";
	ANIMATION_NAME[(int)BOSS_STATE::DEAD]					= "die";
	ANIMATION_NAME[(int)BOSS_STATE::CLOSER_ATTACK_L]		= "attack5L";
	ANIMATION_NAME[(int)BOSS_STATE::CLOSER_ATTACK_R]		= "attack5R";
	ANIMATION_NAME[(int)BOSS_STATE::CHASE_ATTACK_READY]		= "attack1_1";
	ANIMATION_NAME[(int)BOSS_STATE::CHASE_ATTACK_PLAY]		= "attack1_2";
	ANIMATION_NAME[(int)BOSS_STATE::RENDOM_ATTACK_READY]	= "attack4_1";
	ANIMATION_NAME[(int)BOSS_STATE::RENDOM_ATTACK_PLAY]		= "attack4_2";
	ANIMATION_NAME[(int)BOSS_STATE::TELEPORT_READY]			= "attack2_1";
	ANIMATION_NAME[(int)BOSS_STATE::TELEPORT_START]			= "attack2_2";
	ANIMATION_NAME[(int)BOSS_STATE::CREATE_FRIEND]			= "attack3";

	//애니메이션 시간 설정
	ANIMATION_TIME[(int)BOSS_STATE::GROGGY_START]			= 1.0f;
	ANIMATION_TIME[(int)BOSS_STATE::GROGGY_PLAY]			= 1.0f;
	ANIMATION_TIME[(int)BOSS_STATE::GROGGY_END]				= 1.0f;
	ANIMATION_TIME[(int)BOSS_STATE::IDLE]					= 1.0f;
	ANIMATION_TIME[(int)BOSS_STATE::DEAD]					= 1.0f;
	ANIMATION_TIME[(int)BOSS_STATE::CLOSER_ATTACK_L]		= 1.0f;
	ANIMATION_TIME[(int)BOSS_STATE::CLOSER_ATTACK_R]		= 1.0f;
	ANIMATION_TIME[(int)BOSS_STATE::CHASE_ATTACK_READY]		= 1.0f;
	ANIMATION_TIME[(int)BOSS_STATE::CHASE_ATTACK_PLAY]		= 1.0f;
	ANIMATION_TIME[(int)BOSS_STATE::RENDOM_ATTACK_READY]	= 0.5f;
	ANIMATION_TIME[(int)BOSS_STATE::RENDOM_ATTACK_PLAY]		= 1.0f;
	ANIMATION_TIME[(int)BOSS_STATE::TELEPORT_READY]			= 1.0f;
	ANIMATION_TIME[(int)BOSS_STATE::TELEPORT_START]			= 1.0f;
	ANIMATION_TIME[(int)BOSS_STATE::CREATE_FRIEND]			= 1.0f;

	mRay = new PhysRayCast();

	for (int i = 0; i < 14; i++)
	{
		STATE_START[i] = false;
	}
}

Boss::~Boss()
{
	delete mRay;
}

void Boss::Awake()
{
	mTransform	= gameobject->GetTransform();
	mAnimation	= gameobject->GetComponent<AnimationController>();
	mMeshFilter = gameobject->GetComponent<MeshFilter>();
	mColider	= gameobject->GetComponent<Collider>();
	mRigidbody	= gameobject->GetComponent<Rigidbody>();
}

void Boss::SetUp()
{
	mColider->SetSphereCollider(0.5f);
	mColider->SetCenter(0, 0.5f, 0);
	mColider->SetMaterial_Restitution(0);
	mColider->SetMaterial_Dynamic(0);
	mRigidbody->SetFreezeRotation(true, true, true);

	mMeshFilter->SetModelName("BossB+");
	mMeshFilter->SetAnimationName("BossB+");
	mAnimation->Play();

	//플레이어 위치 설정
	mPlayerTR = FindGameObjectTag("Player")->GetTransform();
	mState = (int)BOSS_STATE::IDLE;

	//위치값 설정
	mTransform->Position	= { -44.0f,6.0f,62.0f };
	mTransform->Scale		= { 1.5f,1.5f,1.5f};

	//스킬 포인트의 위치를 생성한다
	CreateSkillPoint();

	for (int i = 0; i < 5; i++)
	{
		Weapon[i] = MessageManager::GetGM()->CREATE_MESSAGE(TARGET_BOSS_WEAPON);
		Weapon[i]->GetComponent<MeshFilter>()->SetModelName("Sphere");
		Transform* mTransform =	Weapon[i]->GetTransform();
		mTransform->Position	= { -44.0f,6.0f,62.0f };
		mTransform->Scale		= { 1,1,1 };
	}
}

void Boss::Update()
{
	switch (mState)
	{
	case (int)BOSS_STATE::IDLE:
		Boss_Idle();
		break;
	case (int)BOSS_STATE::DEAD:
		Boss_DEAD();
		break;
	case (int)BOSS_STATE::GROGGY_START:	//그로기 시작
		Boss_Groggy_Start();
		break;
	case (int)BOSS_STATE::GROGGY_PLAY:	//그로기 중
		Boss_Groggy_Play();
		break;
	case (int)BOSS_STATE::GROGGY_END:	//그로기 끝
		Boss_Groggy_End();
		break;
	case (int)BOSS_STATE::TELEPORT_READY:	//텔레포트 준비
		Boss_Teleport_Ready();
		break;
	case (int)BOSS_STATE::TELEPORT_START:	//텔레포트 시작
		Boss_Teleport_Start();
		break;
	case (int)BOSS_STATE::CREATE_FRIEND:	//분신 생성
		Boss_Create();
		break;
	case (int)BOSS_STATE::CLOSER_ATTACK_L:		//근접 왼쪽 공격
		Boss_Closer_Attack_L();
		break;
	case (int)BOSS_STATE::CLOSER_ATTACK_R:		//근접 오른쪽 공격
		Boss_Closer_Attack_R();
		break;
	case (int)BOSS_STATE::CHASE_ATTACK_READY:	//추격 발사체 시작
		Boss_Chase_Attack_Ready();
		break;
	case (int)BOSS_STATE::CHASE_ATTACK_PLAY:	//추격 발사체 공격
		Boss_Chase_Attack_Play();
		break;
	case (int)BOSS_STATE::RENDOM_ATTACK_READY:	//장판형 발사체 시작
		Boss_Rendom_Attack_Ready();
		break;
	case (int)BOSS_STATE::RENDOM_ATTACK_PLAY:	//장판형 발사체 공격
		Boss_Rendom_Attack_Play();
		break;
	case (int)BOSS_STATE::HIT:	//장판형 발사체 공격
		Boss_Hit();
		break;
	}

	mAnimation->Choice(ANIMATION_NAME[mState], ANIMATION_TIME[mState]);
	mAnimation->Play();
}

void Boss::Debug()
{
	DebugDrawCircle(AttackRange, mTransform->Position + Vector3(0, 1, 0), Vector3(0, 0, 0), Vector3(0, 1, 0));
	DebugDrawCircle(FightRange, mTransform->Position + Vector3(0, 1, 0), Vector3(0, 0, 0), Vector3(1, 0, 0));
	for(int i = 0; i < 5;i++)
	{
		DebugDrawCircle(1.5f, SkillPoint[i] + Vector3(0, 0.25f, 0), Vector3(0, 0, 0), Vector3(0, 1, 0));
	}
}

void Boss::OnTriggerStay(GameObject* Obj)
{
	//여러번 맞는걸 방지하기위해
	if (IsHit == false)
	{
		//플레이어 상태가 공격이거나,스킬상태일때
		if (Player::GetAttackState() == true)
		{
			HP -= 10;
			MessageManager::GetGM()->SEND_Message(TARGET_PLAYER, MESSAGE_PLAYER_ATTACK_OK);
			Sound_Play_SFX("Monster_Hit");
			IsHit = true;
		}
	}
	else
	{
		//플레이어 상태가 공격상태가 아닐떄
		if (Player::GetAttackState() == false)
		{
			IsHit = false;
		}
	}

}

void Boss::Boss_Idle()
{
	PlayerDistanceCheck();
	if (PlayerDistance < AttackRange)
	{
		//근접 공격이 가능한 상태
		mTransform->Slow_Y_Rotation(mPlayerTR->Position, 150, false);
		SetState(BOSS_STATE::CLOSER_ATTACK_L);
	}
	else if(PlayerDistance < FightRange && PlayerDistance > AttackRange)
	{
		//전투가 가능한 상태
		mTransform->Slow_Y_Rotation(mPlayerTR->Position, 150, false);
	}

	if (GetKeyDown(VK_SPACE))
	{
		SetState(BOSS_STATE::RENDOM_ATTACK_READY);
	}
}

void Boss::Boss_DEAD()
{







}

void Boss::Boss_Groggy_Start()
{





}

void Boss::Boss_Groggy_Play()
{








}

void Boss::Boss_Groggy_End()
{





}

void Boss::Boss_Teleport_Ready()
{




}

void Boss::Boss_Teleport_Start()
{





}

void Boss::Boss_Create()
{





}

void Boss::Boss_Closer_Attack_L()
{
	//왼쪽 근접 공격
	int End = mAnimation->GetEndFrame();
	int Now = mAnimation->GetNowFrame();
	if (Now >= End)
	{
		SetState(BOSS_STATE::IDLE);
	}
	mTransform->Slow_Y_Rotation(mPlayerTR->Position, 150, false);
}

void Boss::Boss_Closer_Attack_R()
{
	//오른쪽 근접 공격
	int End = mAnimation->GetEndFrame();
	int Now = mAnimation->GetNowFrame();
	if (Now >= End)
	{
		SetState(BOSS_STATE::IDLE);
	}
	mTransform->Slow_Y_Rotation(mPlayerTR->Position, 150, false);
}

void Boss::Boss_Chase_Attack_Ready()
{





}

void Boss::Boss_Chase_Attack_Play()
{



}

void Boss::Boss_Rendom_Attack_Ready()
{
	int End = mAnimation->GetEndFrame();
	int Now = mAnimation->GetNowFrame();
	if (Now >= End) { SetState(BOSS_STATE::RENDOM_ATTACK_PLAY); }
	else { mTransform->Position.y += GetDeltaTime(); }
}

void Boss::Boss_Rendom_Attack_Play()
{
	mTransform->Position.y;


}

void Boss::Boss_Hit()
{
	


}

void Boss::SetState(BOSS_STATE State)
{
	//상태를 넣어준다
	STATE_START[mState] = false;
	mState = (int)State;
}

bool Boss::FirstState()
{
	//현재 상태로 처음 들어왔을때를 체크
	if (STATE_START[mState] == false)
	{
		STATE_START[mState] = true;
		return true;
	}
	else
	{
		return false;
	}
}

float Boss::PlayerDistanceCheck()
{
	PlayerDistance = mTransform->GetDistance(mPlayerTR->Position);
	return PlayerDistance;
}

void Boss::CreateSkillPoint()
{
	float Angle = 360 / 5;
	float NowAngle = mTransform->Rotation.y;
	for (int i = 0; i < 5; i++)
	{
		NowAngle += Angle;
		float X = cos(NowAngle * (3.141592f / 180));
		float y = sin(NowAngle * (3.141592f / 180));

		Vector3 EndPoint = Vector3(0, 0, 0);
		EndPoint.x += (X * SkillRange) + mTransform->Position.x;
		EndPoint.y += mTransform->Position.y + 2;
		EndPoint.z += (y * SkillRange) + mTransform->Position.z;

		//Ray를 쏜다
		mRay->Direction = { 0,-1,0 };
		mRay->MaxDistance = 10;
		mRay->Origin = EndPoint;

		//충돌된 곳에Y축만 가져온다
		RayCast(mRay);
		EndPoint.y = mRay->Hit.HitPoint.y;
		SkillPoint[i] = EndPoint;
	}
}
