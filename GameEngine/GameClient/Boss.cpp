#include "Boss.h"
#include "MessageManager.h"
#include "EaterEngineAPI.h"
#include "EngineData.h"
#include "MessageManager.h"
#include "PhysRay.h"
#include "EngineData.h"
#include "MessageManager.h"
#include "BossStateManager.h"

//component
#include "GameObject.h"
#include "Transform.h"
#include "Player.h"
#include "Collider.h"
#include "Rigidbody.h"
#include "MeshFilter.h"
#include "AnimationController.h"
#include "BossWeapon.h"
#include "BossFriend.h"
#include "ParticleController.h"
#include "ParticleFactory.h"

#include "State.h"
#include "Boss_Idle.h"
#include "Boss_RendomAttack.h"
#include "Boss_ChaseAttack.h"
#include "Boss_BaseAttack.h"
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

	ANIMATION_NAME[(int)BOSS_STATE::CHASE_ATTACK_READY]		= "recovery_1";
	ANIMATION_NAME[(int)BOSS_STATE::CHASE_ATTACK_PLAY]		= "recovery_2";
	ANIMATION_NAME[(int)BOSS_STATE::CHASE_ATTACK_END]		= "recovery_3";
	//보스 애니메이션 
	ANIMATION_NAME[(int)BOSS_STATE::RENDOM_ATTACK_START]	= "attack4_1";
	ANIMATION_NAME[(int)BOSS_STATE::RENDOM_ATTACK_PLAY]		= "attack4_2";
	ANIMATION_NAME[(int)BOSS_STATE::RENDOM_ATTACK_END]		= "attack4_3";
	ANIMATION_NAME[(int)BOSS_STATE::RENDOM_ATTACK_RESET]	= "attack4_4";
	//텔레포트
	ANIMATION_NAME[(int)BOSS_STATE::TELEPORT_READY]			= "attack2_1";
	ANIMATION_NAME[(int)BOSS_STATE::TELEPORT_START]			= "attack2_2";
	//분신 생성
	ANIMATION_NAME[(int)BOSS_STATE::CREATE_FRIEND]			= "attack3";
	//애니메이션 시간 설정
	ANIMATION_TIME[(int)BOSS_STATE::GROGGY_START]			= 1.0f;
	ANIMATION_TIME[(int)BOSS_STATE::GROGGY_PLAY]			= 1.0f;
	ANIMATION_TIME[(int)BOSS_STATE::GROGGY_END]				= 1.0f;
	ANIMATION_TIME[(int)BOSS_STATE::IDLE]					= 1.0f;
	ANIMATION_TIME[(int)BOSS_STATE::DEAD]					= 1.0f;
	ANIMATION_TIME[(int)BOSS_STATE::CLOSER_ATTACK_L]		= 0.75f;
	ANIMATION_TIME[(int)BOSS_STATE::CLOSER_ATTACK_R]		= 0.75f;
	ANIMATION_TIME[(int)BOSS_STATE::CHASE_ATTACK_READY]		= 0.5f;
	ANIMATION_TIME[(int)BOSS_STATE::CHASE_ATTACK_PLAY]		= 1.0f;
	ANIMATION_TIME[(int)BOSS_STATE::CHASE_ATTACK_END]		= 1.0f;
	ANIMATION_TIME[(int)BOSS_STATE::RENDOM_ATTACK_START]	= 0.5f;
	ANIMATION_TIME[(int)BOSS_STATE::RENDOM_ATTACK_PLAY]		= 1.0f;
	ANIMATION_TIME[(int)BOSS_STATE::RENDOM_ATTACK_END]		= 1.0f;
	ANIMATION_TIME[(int)BOSS_STATE::RENDOM_ATTACK_RESET]	= 1.0f;
	ANIMATION_TIME[(int)BOSS_STATE::TELEPORT_READY]			= 0.5f;
	ANIMATION_TIME[(int)BOSS_STATE::TELEPORT_START]			= 0.5f;
	ANIMATION_TIME[(int)BOSS_STATE::CREATE_FRIEND]			= 1.0f;
	ANIMATION_TIME[(int)BOSS_STATE::RENDOM_ATTACK_END]		= 0.8f;

	mRay = new PhysRayCast();

	for (int i = 0; i < 15; i++)
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
	mParticle = ParticleFactory::Get()->CreateParticleController(PARTICLE_TYPE::BossMelee);

	for (int i = 0; i < 5; i++)
	{
		GameObject* WeaponObject = MessageManager::GetGM()->CREATE_MESSAGE(TARGET_BOSS_WEAPON);
		Weapon[i] = WeaponObject->GetComponent<BossWeapon>();
	}
	
	GameObject* FriendObject = MessageManager::GetGM()->CREATE_MESSAGE(TARGET_BOSS_FRIEND);
	Friend = FriendObject->GetComponent<BossFriend>();

	mBossIdle = new Boss_Idle();
	mBossRendomAttack = new Boss_RendomAttack();
	mBossChaseAttack = new Boss_ChaseAttack();
	mBossBaseAttack = new Boss_BaseAttack();

	mBossIdle->SetObject(this->gameobject);
	mBossRendomAttack->SetObject(this->gameobject);
	mBossChaseAttack->SetObject(this->gameobject);
	mBossBaseAttack->SetObject(this->gameobject);
}

void Boss::SetUp()
{
	mColider->SetSphereCollider(0.5f);
	mColider->SetCenter(0, 0.75f, 0);
	mColider->SetMaterial_Restitution(0);
	mColider->SetMaterial_Dynamic(0);
	mRigidbody->SetFreezeRotation(true, true, true);
	mRigidbody->SetGravity(false);

	mMeshFilter->SetModelName("BossB+");
	mMeshFilter->SetAnimationName("BossB+");
	mAnimation->Play();

	//플레이어 위치 설정
	mPlayerTR = FindGameObjectTag("Player")->GetTransform();
	mState = (int)BOSS_STATE::IDLE;

	//위치값 설정
	mTransform->SetPosition(-45.09f, 6.8f, 70.0f);
	StartPoint = { -44.0f,6.0f,62.0f };
	mTransform->SetScale(1.5f, 1.5f, 1.5f);

	//스킬 포인트의 위치를 생성한다
	CreateSkillPoint();
}

void Boss::Start()
{
	mMF_Setting.Setting(this->gameobject);
	mMF_Setting.SetLimlightSetting(1.0f, 1.0f, 1.0f, 0.5f, 0.5f);
	mMF_Setting.SetEmissiveSetting(231,39,9, 2.9f);

	GameObject* Hand = gameobject->GetChildBone("t1.L");
	mParticle->gameobject->ChoiceParent(Hand);

	//테스트용 보스 대기상태
	mBossIdle->SetTarget(mPlayerTR);
	mBossIdle->SetAnimationOption(ANIMATION_NAME[(int)BOSS_STATE::IDLE], ANIMATION_TIME[(int)BOSS_STATE::IDLE]);

	mBossRendomAttack->SetAnimationOption(ANIMATION_NAME[(int)BOSS_STATE::RENDOM_ATTACK_START], ANIMATION_TIME[(int)BOSS_STATE::RENDOM_ATTACK_START]);

	mBossChaseAttack->SetTarget(mPlayerTR);
	mBossChaseAttack->SetAnimationOption(ANIMATION_NAME[(int)BOSS_STATE::CHASE_ATTACK_READY], ANIMATION_TIME[(int)BOSS_STATE::CHASE_ATTACK_READY]);

	mBossBaseAttack->SetTarget(mPlayerTR);
}

void Boss::Update()
{
	switch (mState)
	{
	case (int)BOSS_STATE::IDLE:
		mState = mBossIdle->Play();
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
	case (int)BOSS_STATE::RENDOM_ATTACK_START:
		mState = mBossRendomAttack->Play();
		break;
	case (int)BOSS_STATE::CLOSER_ATTACK_R:		//근접 오른쪽 공격
		mState = mBossBaseAttack->Play();
		break;
	case (int)BOSS_STATE::CHASE_ATTACK_READY:	//추격 발사체 시작
		mState = mBossChaseAttack->Play();
		break;
	case (int)BOSS_STATE::HIT:
		Boss_Hit();
		break;
	}

	//mAnimation->Choice(ANIMATION_NAME[mState], ANIMATION_TIME[mState]);
	mAnimation->Play();
}

void Boss::Debug()
{
	DebugDrawCircle(AttackRange, mTransform->GetPosition() + Vector3(0, 1, 0), Vector3(0, 0, 0), Vector3(0, 1, 0));
	DebugDrawCircle(FightRange, mTransform->GetPosition() + Vector3(0, 1, 0), Vector3(0, 0, 0), Vector3(1, 0, 0));
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
			float PlayerPower		= Player::GetPlayerPower();
			float PlayerComboPower	= Player::GetPlayerComboPower();

			//보스의 컬러와 같을떄
			if (Player::GetPlayerColor() == ColorIndex)
			{
				//보스가 가지고잇는 콤보가 플레이어 콤보보다 낮을경우
				if (Player::GetPlayerCombo() >= 10)
				{
					HP -= Player::GetPlayerComboPower();
					SetState(BOSS_STATE::GROGGY_START);
					MessageManager::GetGM()->SEND_Message(TARGET_UI, MESSAGE_UI_BOSS_HP,&HP);
					MessageManager::GetGM()->SEND_Message(TARGET_PLAYER, MESSAGE_PLAYER_ATTACK_OK);
					MessageManager::GetGM()->SEND_Message(TARGET_PLAYER, MESSAGE_PLAYER_COMBO_RESET);
				}
				else
				{
					HP -= Player::GetPlayerComboPower();
					MessageManager::GetGM()->SEND_Message(TARGET_UI, MESSAGE_UI_BOSS_HP, &HP);
					MessageManager::GetGM()->SEND_Message(TARGET_PLAYER, MESSAGE_PLAYER_ATTACK_OK);
				}
			}
			else
			{
				HP -= Player::GetPlayerPower();
				MessageManager::GetGM()->SEND_Message(TARGET_UI, MESSAGE_UI_BOSS_HP, &HP);
				MessageManager::GetGM()->SEND_Message(TARGET_PLAYER, MESSAGE_PLAYER_ATTACK_OK);
			}

			Sound_Play_SFX("Monster_Hit");
			IsHit = true;
		}
	}
	else
	{
		//플레이어 상태가 공격상태가 아닐떄
		if (Player::GetAttackState() == false){IsHit = false;}
	}

}

void Boss::Boss_DEAD()
{
	






}

void Boss::Boss_Groggy_Start()
{
	int End = mAnimation->GetEndFrame();
	int Now = mAnimation->GetNowFrame();
	if (Now >= End)
	{
		SetState(BOSS_STATE::GROGGY_PLAY);
	}
}

void Boss::Boss_Groggy_Play()
{
	int End = mAnimation->GetEndFrame();
	int Now = mAnimation->GetNowFrame();
	if (Now >= End)
	{
		SetState(BOSS_STATE::GROGGY_END);
	}
}

void Boss::Boss_Groggy_End()
{
	int End = mAnimation->GetEndFrame();
	int Now = mAnimation->GetNowFrame();
	if (Now >= End)
	{
		SetState(BOSS_STATE::IDLE);
	}
}

void Boss::Boss_Teleport_Ready()
{
	int now = mAnimation->GetNowFrame();
	int end = mAnimation->GetEndFrame();

	if (now >= end) 
	{
		SetState(BOSS_STATE::TELEPORT_START);
	}
}

void Boss::Boss_Teleport_Start()
{
	int now = mAnimation->GetNowFrame();
	int end = mAnimation->GetEndFrame();


	if (FriendIndex == -1)
	{
		mTransform->SetPosition(StartPoint);
	}
	else
	{
		mTransform->SetPosition(SkillPoint[FriendIndex]);
	}

	if (now >= end)
	{
		SetState(BOSS_STATE::IDLE);
	}
}

void Boss::Boss_Create()
{

	int Now = mAnimation->GetNowFrame();
	int End = mAnimation->GetEndFrame();

	if (Now >= End)
	{
		//플레이어와 가장 먼 위치에 생성
		float	Max = 0;
		int		Index = 0;
		//스킬 포인트 위치를 새롭게 구한다
		for (int i = 0; i < 5; i++)
		{
			int Dir = mPlayerTR->GetDistance(SkillPoint[i]);
			if (Dir > Max && i != FriendIndex)
			{
				Max = Dir;
				Index = i;
			}
		}
		Friend->gameobject->SetActive(true);
		Friend->SetPosition(SkillPoint[Index]);
		FriendIndex = Index;
		SetState(BOSS_STATE::IDLE);
	}
}

void Boss::Boss_Closer_Attack_L()
{
	if (FirstState() == true)
	{
		GameObject* Hand = gameobject->GetChildBone("t1.L");
		mParticle->gameobject->ChoiceParent(Hand);
		mParticle->Play();
	}

	//왼쪽 근접 공격
	int End = mAnimation->GetEndFrame();
	int Now = mAnimation->GetNowFrame();

	if (Now >= End)
	{
		SetState(BOSS_STATE::IDLE);
		mParticle->Stop();
	}
	mTransform->Slow_Y_Rotation(mPlayerTR->GetPosition(), 150, false);
}

void Boss::Boss_Closer_Attack_R()
{
	if (FirstState() == true)
	{
		GameObject* Hand = gameobject->GetChildBone("t1.R");
		mParticle->gameobject->ChoiceParent(Hand);
		mParticle->Play();
	}

	//오른쪽 근접 공격
	int End = mAnimation->GetEndFrame();
	int Now = mAnimation->GetNowFrame();
	if (Now >= End)
	{
		SetState(BOSS_STATE::IDLE);
		mParticle->Stop();
	}
	mTransform->Slow_Y_Rotation(mPlayerTR->GetPosition(), 150, false);
}

void Boss::Boss_Chase_Attack_Ready()
{
	int End = mAnimation->GetEndFrame();
	int Now = mAnimation->GetNowFrame();
	if (Now >= End)
	{
		SetState(BOSS_STATE::CHASE_ATTACK_PLAY);
	}
	mTransform->Slow_Y_Rotation(mPlayerTR->GetPosition(), 150, false);
}

void Boss::Boss_Chase_Attack_Play()
{
	mTransform->Slow_Y_Rotation(mPlayerTR->GetPosition(), 150, false);
	if (IsShooting == false)
	{
		Vector3 Look = mTransform->GetLocalPosition_Look();
		Look.z *= -1;

		Vector3 Start;
		
		Start = mTransform->GetPosition() + (Look * 3);
		Start.y += 3.0f;

		if (WeaponIndex <= 5)
		{
			Weapon[WeaponIndex]->SetShootingPoistion(Start, mPlayerTR->GetPosition());
		}

		IsShooting = true;
	}

	if (IsShooting == true)
	{
		if (Weapon[WeaponIndex]->ShootingReady() == true)
		{
			WeaponIndex++;
			IsShooting = false;
		}
	}

	if (WeaponIndex >= 5)
	{
		if (ChaseSkillPlayTime >= RendomSkillPlayTimeMax)
		{
			SetState(BOSS_STATE::CHASE_ATTACK_END);
			ChaseSkillPlayTime = 0;
			WeaponIndex = 0;
			IsShooting = false;
			for (int i = 0; i < 5; i++)
			{
				Weapon[i]->Reset();
			}
		}
		else
		{
			ChaseSkillPlayTime += GetDeltaTime();
		}
	}
}

void Boss::Boss_Chase_Attack_End()
{
	int End = mAnimation->GetEndFrame();
	int Now = mAnimation->GetNowFrame();
	if (Now >= End)
	{
		SetState(BOSS_STATE::IDLE);
	}
}

void Boss::Boss_Rendom_Attack_Start()
{
	if (FirstState() == true)
	{
		mMF_Setting.SetLimlightSetting(1, 1, 1, 0.5f, 0.5f);
		mMF_Setting.SetLimlightSettingMax(1, 0, 0, 3, 0.5f);
	}
	else
	{
		mMF_Setting.LimLightUpdate(0.5f);
	}

	mTransform->SetPosition_X(StartPoint.x);
	mTransform->SetPosition_Z(StartPoint.z);

	int End = mAnimation->GetEndFrame();
	int Now = mAnimation->GetNowFrame();
	if (Now >= End)
	{
		SetState(BOSS_STATE::RENDOM_ATTACK_PLAY);
	}
	else
	{
		mTransform->AddPosition_Y(GetDeltaTime());
	}
}

void Boss::Boss_Rendom_Attack_Play()
{
	static int WeaponIndex = 0;

	if (IsShooting == false)
	{
		Vector3 Start = mTransform->GetPosition();
		Start.y += 7.0f;
		if (WeaponIndex < 5)
		{
			Weapon[WeaponIndex]->SetShootingPoistion(Start, SkillPoint[WeaponIndex]);
			IsShooting = true;
		}
	}

	if(IsShooting == true)
	{
		if (Weapon[WeaponIndex]->ShootingReady() == true)
		{
			WeaponIndex++;
			IsShooting = false;
		}
	}
	
	if (WeaponIndex >= 5)
	{
		if (RendomSkillPlayTime >= RendomSkillPlayTimeMax)
		{
			SetState(BOSS_STATE::RENDOM_ATTACK_END);
			RendomSkillPlayTime = 0;
			WeaponIndex = 0;
			IsShooting = false;
		}
		else
		{
			RendomSkillPlayTime += GetDeltaTime();
		}
	}
}

void Boss::Boss_Rendom_Attack_End()
{
	if (FirstState() == true)
	{
		mMF_Setting.SetLimlightSetting(1, 0, 0, 3, 0.5f);
		mMF_Setting.SetLimlightSettingMax(1.0f, 1.0f, 1.0f, 0.5f, 0.5f);
	}
	else
	{
		mMF_Setting.LimLightUpdate(0.5);
	}


	int End = mAnimation->GetEndFrame();
	int Now = mAnimation->GetNowFrame();

	if (mAnimation->EventCheck() == true)
	{
		for (int i = 0; i < 5; i++)
		{
			Weapon[i]->Reset();
		}
	}

	if (Now >= End)
	{
		SetState(BOSS_STATE::RENDOM_ATTACK_RESET);
	}
}

void Boss::Boss_Rendom_Attack_Reset()
{
	int End = mAnimation->GetEndFrame();
	int NOW = mAnimation->GetNowFrame();

	if (NOW >= End)
	{
		SetState(BOSS_STATE::IDLE);
	}
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
	PlayerDistance = mTransform->GetDistance(mPlayerTR->GetPosition());
	if (PlayerDistance <= FightRange && IsUI_ON == false)
	{
		IsUI_ON = true;
		MessageManager::GetGM()->SEND_Message(TARGET_UI, MESSAGE_UI_BOSS_ACTIVE, &IsUI_ON);
	}
	else if (PlayerDistance > FightRange && IsUI_ON == true)
	{
		IsUI_ON = false;
		MessageManager::GetGM()->SEND_Message(TARGET_UI, MESSAGE_UI_BOSS_ACTIVE, &IsUI_ON);
	}

	return PlayerDistance;
}

void Boss::CreateSkillPoint()
{
	float Angle = 360.0f / 5.0f;
	float NowAngle = mTransform->GetRotation().y;
	for (int i = 0; i < 5; i++)
	{
		NowAngle += Angle;
		float X = cos(NowAngle * (3.141592f / 180));
		float y = sin(NowAngle * (3.141592f / 180));

		Vector3 EndPoint = Vector3(0, 0, 0);
		EndPoint.x += (X * SkillRange) + StartPoint.x;
		EndPoint.y += StartPoint.y + 2;
		EndPoint.z += (y * SkillRange) + StartPoint.z;

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

void Boss::GroundCheck()
{
	//Ray를 쏜다
	mRay->Direction = { 0,-1,0 };
	mRay->MaxDistance = 10;
	mRay->Origin = mTransform->GetPosition();
	mRay->Origin.y += 0.2f;
	//충돌된 곳에Y축만 가져온다
	RayCast(mRay);
	PositionY = mRay->Hit.HitPoint.y + 0.5f;
}