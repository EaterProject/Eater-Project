#include "Boss.h"
#include "MessageManager.h"
#include "EaterEngineAPI.h"
#include "EngineData.h"
#include "MessageManager.h"
#include "PhysRay.h"
#include "EngineData.h"

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

	ANIMATION_NAME[(int)BOSS_STATE::RENDOM_ATTACK_START]	= "attack4_1";
	ANIMATION_NAME[(int)BOSS_STATE::RENDOM_ATTACK_PLAY]		= "attack4_2";
	ANIMATION_NAME[(int)BOSS_STATE::RENDOM_ATTACK_END]		= "attack4_3";
	ANIMATION_NAME[(int)BOSS_STATE::RENDOM_ATTACK_RESET]	= "attack4_4";
	ANIMATION_NAME[(int)BOSS_STATE::RENDOM_ATTACK_ADDSKILL]	= "attack4_4";


	ANIMATION_NAME[(int)BOSS_STATE::TELEPORT_READY]			= "attack2_1";
	ANIMATION_NAME[(int)BOSS_STATE::TELEPORT_START]			= "attack2_2";
	ANIMATION_NAME[(int)BOSS_STATE::CREATE_FRIEND]			= "attack3";

	ANIMATION_NAME[(int)BOSS_STATE::PHASE_UP_START]			= "attack4_1";
	ANIMATION_NAME[(int)BOSS_STATE::PHASE_UP_END]			= "attack4_3";

	ANIMATION_NAME[(int)BOSS_STATE::MISSILE_START]			= "attack3";
	ANIMATION_NAME[(int)BOSS_STATE::MISSILE_PLAY]			= "attack3";
	ANIMATION_NAME[(int)BOSS_STATE::MISSILE_END]			= "attack3";
	ANIMATION_NAME[(int)BOSS_STATE::RENDOM_ATTACK_BIG]		= "attack3";

	//애니메이션 시간 설정
	ANIMATION_TIME[(int)BOSS_STATE::GROGGY_START]			= 1.0f;
	ANIMATION_TIME[(int)BOSS_STATE::GROGGY_PLAY]			= 1.0f;
	ANIMATION_TIME[(int)BOSS_STATE::GROGGY_END]				= 1.0f;
	ANIMATION_TIME[(int)BOSS_STATE::IDLE]					= 1.0f;
	ANIMATION_TIME[(int)BOSS_STATE::DEAD]					= 1.0f;

	ANIMATION_TIME[(int)BOSS_STATE::CLOSER_ATTACK_L]		= 1.0f;
	ANIMATION_TIME[(int)BOSS_STATE::CLOSER_ATTACK_R]		= 1.0f;

	ANIMATION_TIME[(int)BOSS_STATE::CHASE_ATTACK_READY]		= 0.5f;
	ANIMATION_TIME[(int)BOSS_STATE::CHASE_ATTACK_PLAY]		= 1.0f;
	ANIMATION_TIME[(int)BOSS_STATE::CHASE_ATTACK_END]		= 1.0f;

	ANIMATION_TIME[(int)BOSS_STATE::RENDOM_ATTACK_START]	= 0.5f;
	ANIMATION_TIME[(int)BOSS_STATE::RENDOM_ATTACK_PLAY]		= 1.0f;
	ANIMATION_TIME[(int)BOSS_STATE::RENDOM_ATTACK_END]		= 1.0f;
	ANIMATION_TIME[(int)BOSS_STATE::RENDOM_ATTACK_RESET]	= 1.0f;

	ANIMATION_TIME[(int)BOSS_STATE::TELEPORT_READY]			= 1.0f;
	ANIMATION_TIME[(int)BOSS_STATE::TELEPORT_START]			= 1.0f;
	ANIMATION_TIME[(int)BOSS_STATE::CREATE_FRIEND]			= 1.0f;
	ANIMATION_TIME[(int)BOSS_STATE::RENDOM_ATTACK_END]		= 0.8f;

	ANIMATION_TIME[(int)BOSS_STATE::PHASE_UP_START]			= 1.0f;
	ANIMATION_TIME[(int)BOSS_STATE::PHASE_UP_END]			= 0.5f;

	ANIMATION_TIME[(int)BOSS_STATE::MISSILE_START]			= 0.5f;
	ANIMATION_TIME[(int)BOSS_STATE::MISSILE_START]			= 0.5f;
	ANIMATION_TIME[(int)BOSS_STATE::MISSILE_START]			= 0.5f;

	ANIMATION_TIME[(int)BOSS_STATE::RENDOM_ATTACK_ADDSKILL] = 1.0f;
	ANIMATION_TIME[(int)BOSS_STATE::RENDOM_ATTACK_BIG]		= 1.0f;

	mRay = new PhysRayCast();

	for (int i = 0; i < 15; i++)
	{
		STATE_START[i] = false;
	}

	SkillTime[Rendom_Time]		= 10.0f;

	SkillTimeMax[Rendom_Time]	= 8.0f;		//장판 공격
	SkillTimeMax[Chase_Time]	= 6.0f;		//일직선 공격
	SkillTimeMax[Base_Time]		= 5.0f;		//기본 공격
	SkillTimeMax[Teleport_Time] = 4.0f;		//텔레포트
	SkillTimeMax[Groggy_Time]	= 10.0f;	//그로기 상태
	SkillTimeMax[Missile_Time]	= 6.0f;		//미사일 상태
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

	mBaseAttackParticle = ParticleFactory::Get()->CreateParticleController(PARTICLE_TYPE::BossMelee);
	mPushParticle		= ParticleFactory::Get()->CreateParticleController(PARTICLE_TYPE::BossPush);
	mCountAttackParticle = ParticleFactory::Get()->CreateParticleController(PARTICLE_TYPE::CounterAttack);

	for (int i = 0; i < 6; i++)
	{
		if (i < 5)
		{
			GameObject* WeaponObject = MessageManager::GetGM()->CREATE_MESSAGE(TARGET_BOSS_WEAPON);
			Weapon[i] = WeaponObject->GetComponent<BossWeapon>();
		}
		else
		{
			GameObject* WeaponObject = MessageManager::GetGM()->CREATE_MESSAGE(TARGET_BOSS_WEAPON);
			BigWeapon = WeaponObject->GetComponent<BossWeapon>();
		}
	}
	

	GameObject* FriendObject = MessageManager::GetGM()->CREATE_MESSAGE(TARGET_BOSS_FRIEND);
	Friend = FriendObject->GetComponent<BossFriend>();
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

	//스킬 포인트의 위치를 생성한다
	CreateSkillPoint();
}

void Boss::Start()
{
	mMF_Setting.Setting(this->gameobject);
	mMF_Setting.SetLimlightSetting(1.0f, 1.0f, 1.0f, 0.5f, 0.5f);
	mMF_Setting.SetEmissiveSetting(231,39,9, 2.9f);

	GameObject* Hand = gameobject->GetChildBone("t1.L");
	mBaseAttackParticle->gameobject->ChoiceParent(Hand);

	mTransform->SetScale(1.7f, 1.7f, 1.7f);
	mAnimation->Play();
}

void Boss::Update()
{
	StartFight();
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
		Boss_Chase_Attack_Ready(BossPhase);
		break;
	case (int)BOSS_STATE::CHASE_ATTACK_PLAY:	//추격 발사체 공격
		Boss_Chase_Attack_Play(BossPhase);
		break;
	case (int)BOSS_STATE::CHASE_ATTACK_END:	//추격 발사체 공격
		Boss_Chase_Attack_End(BossPhase);
		break;
	case (int)BOSS_STATE::RENDOM_ATTACK_START:	//장판형 발사체 시작
		Boss_Rendom_Attack_Ready(BossPhase);
		break;
	case (int)BOSS_STATE::RENDOM_ATTACK_PLAY:	//장판형 발사체 공격
		Boss_Rendom_Attack_Play(BossPhase);
		break;
	case (int)BOSS_STATE::RENDOM_ATTACK_END:	//장판형 발사체 공격
		Boss_Rendom_Attack_End(BossPhase);
		break;
	case (int)BOSS_STATE::RENDOM_ATTACK_RESET:	//장판형 발사체 공격
		Boss_Rendom_Attack_Reset(BossPhase);
		break;
	case (int)BOSS_STATE::RENDOM_ATTACK_ADDSKILL:
		Boss_Rendom_Attack_AddSkill();
		break;
	case (int)BOSS_STATE::HIT:	//장판형 발사체 공격
		Boss_Hit();
		break;
	case (int)BOSS_STATE::PHASE_UP_START:	//페이즈 변경 시작
		Phase_UP_Start();
		break;
	case (int)BOSS_STATE::PHASE_UP_END: //페이즈 변경 끝
		Phase_UP_End();
		break;
	case (int)BOSS_STATE::MISSILE_START: //페이즈 변경 끝
		Boss_Guidedmissile_Attack_Start();
		break;
	case (int)BOSS_STATE::MISSILE_PLAY: //페이즈 변경 끝
		Boss_Guidedmissile_Attack_Play();
		break;
	case (int)BOSS_STATE::MISSILE_END: //페이즈 변경 끝
		Boss_Guidedmissile_Attack_End();
		break;
	case (int)BOSS_STATE::RENDOM_ATTACK_BIG: //페이즈 변경 끝
		Boss_Rendom_Attack_Big();
		break;
	}

	mAnimation->Choice(ANIMATION_NAME[mState], ANIMATION_TIME[mState]);
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
			if (Player::GetPlayerColor() == ColorType)
			{
				if (Player::GetPlayerCombo() >= 30)
				{
					HP -= Player::GetPlayerComboPower();
					MessageManager::GetGM()->SEND_Message(TARGET_PLAYER, MESSAGE_PLAYER_ATTACK_OK);
					MessageManager::GetGM()->SEND_Message(TARGET_PLAYER, MESSAGE_PLAYER_COMBO_RESET);
					MessageManager::GetGM()->SEND_Message(TARGET_UI, MESSAGE_UI_BOSS_HP, &HP);
					ColorType == 0 ? ColorType = 1 : ColorType = 0;
					Vector3 Pos = mTransform->GetPosition();
					Pos.y += 3.0f;
					mCountAttackParticle->SetPosition(Pos);
					mCountAttackParticle->Play();
					SetState(BOSS_STATE::GROGGY_START);

					if (ColorType == 0)
					{
						mMF_Setting.SetLimlightSetting(1.0f, 1.0f, 1.0f, 0.5f, 0.5f);
						mMF_Setting.SetEmissiveSetting(231, 39, 9, 2.9f);
					}
					else
					{
						mMF_Setting.SetLimlightSetting(1.0f, 1.0f, 1.0f, 0.5f, 0.5f);
						mMF_Setting.SetEmissiveSetting(40, 92, 255, 2.9f);
					}
				}
				else
				{
					HP -= Player::GetPlayerComboPower();
					MessageManager::GetGM()->SEND_Message(TARGET_PLAYER, MESSAGE_PLAYER_ATTACK_OK);
					MessageManager::GetGM()->SEND_Message(TARGET_UI, MESSAGE_UI_BOSS_HP, &HP);
				}
			}
			else
			{
				HP -= Player::GetPlayerPower();
				MessageManager::GetGM()->SEND_Message(TARGET_UI, MESSAGE_UI_BOSS_HP, &HP);
			}
			Sound_Play_SFX("Boss_Hit");

			if (BossPhase == 0 && HP <= 1500){SetState(BOSS_STATE::PHASE_UP_START);}
			if (HP <= 0) { SetState(BOSS_STATE::DEAD); }

			IsUpdateColor = true;
			IsHit = true;

		}
	}
	else
	{
		//플레이어 상태가 공격상태가 아닐떄
		if (Player::GetAttackState() == false){IsHit = false;}
	}

}

void Boss::Boss_Idle()
{
	if (FirstState() == true)
	{
		if (ColorType == 0)
		{
			mMF_Setting.SetLimlightSetting(1.0f, 1.0f, 1.0f, 0.5f, 0.5f);
			mMF_Setting.SetEmissiveSetting(231, 39, 9, 2.9f);
		}
		else
		{
			mMF_Setting.SetLimlightSetting(1.0f, 1.0f, 1.0f, 0.5f, 0.5f);
			mMF_Setting.SetEmissiveSetting(40, 92, 255, 2.9f);
		}
	}
	mMF_Setting.LimLightUpdate(1);
	GroundCheck();
	PlayerDistanceCheck();

	if (mTransform->GetPosition().y > PositionY)
	{
		mTransform->SetPosition_Y(PositionY);
	}
	
	SkillCheck();
}

void Boss::Boss_DEAD()
{
	if (FirstState() == true)
	{
		mMF_Setting.SetDissolveOption(DISSOLVE_FADEOUT);
		mMF_Setting.SetDissolveTexture("Dissolve_1");
		mMF_Setting.SetDissolveColor(255.0f, 0, 0);
		mMF_Setting.SetDissolveColorFactor(10.0f);
		mMF_Setting.SetDissolvePlayTime(1.0f);
		mMF_Setting.SetDissolveWidth(0.1f);
		mMF_Setting.SetDissolveInnerFactor(100.0f);
		mMF_Setting.SetDissolveOuterFactor(25.0f);
	}

	int End = mAnimation->GetEndFrame();
	int Now = mAnimation->GetNowFrame();
	if (Now >= End)
	{
		mAnimation->Pause();
		mMF_Setting.PlayDissolve();
		IsCredit = true;
	}

	if (mMF_Setting.EndDissolve() == false)
	{
		if (IsCredit)
		{
			IsCredit = false;
			gameobject->SetActive(false);
			Destroy(this->gameobject);

			bool Active = false;
			MessageManager::GetGM()->SEND_Message(TARGET_UI, MESSAGE_UI_BOSS_ACTIVE, &Active);
			MessageManager::GetGM()->SEND_Message(TARGET_GLOBAL, MESSAGE_GLOBAL_CREDIT);
		}
	}
}

void Boss::Boss_Groggy_Start()
{
	int End = mAnimation->GetEndFrame();
	int Now = mAnimation->GetNowFrame();
	if (Now >= End)
	{
		MessageManager::GetGM()->SEND_Message(TARGET_UI, MESSAGE_UI_BOSS_COLOR,&ColorType);
		SetState(BOSS_STATE::GROGGY_PLAY);

		for (int i = 0; i < 5; i++)
		{
			Weapon[i]->Reset();
		}
		BigWeapon->Reset();
	}
}

void Boss::Boss_Groggy_Play()
{
	if (SkillTime[Groggy_Time] >= SkillTimeMax[Groggy_Time])
	{
		SetState(BOSS_STATE::GROGGY_END);
		SkillTime[Groggy_Time] = 0;
	}
	else
	{
		SkillTime[Groggy_Time] += GetDeltaTime();
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
	if (FirstState() == true)
	{
		mMF_Setting.SetDissolveOption(DISSOLVE_FADEOUT);
		mMF_Setting.SetDissolveTexture("Dissolve_8");
		mMF_Setting.SetDissolveColor(255.0f, 0, 0);
		mMF_Setting.SetDissolveColorFactor(10.0f);
		mMF_Setting.SetDissolvePlayTime(0.6f);
		mMF_Setting.SetDissolveWidth(0.1f);
		mMF_Setting.SetDissolveInnerFactor(100.0f);
		mMF_Setting.SetDissolveOuterFactor(25.0f);
		mMF_Setting.PlayDissolve();
	}
	
	float End = mAnimation->GetEndFrame();
	float Now = mAnimation->GetNowFrame();
	bool IsTest = mMF_Setting.EndDissolve();
	if (Now >= End)
	{
		Sound_Play_SFX("Boss_Teleport");
		SetState(BOSS_STATE::TELEPORT_START);
	}
}

void Boss::Boss_Teleport_Start()
{

	if (FriendIndex == -1)
	{
		mTransform->SetPosition(StartPoint);
	}
	else
	{
		mTransform->SetPosition(SkillPoint[FriendIndex]);
		Friend->Reset();
	}

	float End = mAnimation->GetEndFrame();
	float Now = mAnimation->GetNowFrame();
	if (Now >= End)
	{
		SetState(BOSS_STATE::IDLE);
		IsBossFriend = false;
	}
}

void Boss::Boss_Create()
{
	if (FirstState() == true)
	{
		Vector3 Pos = mTransform->GetPosition();
		Pos.y += 5.0f;
		mPushParticle->SetPosition(Pos);
		mPushParticle->Play();
		PushPlayer();
		Sound_Play_SFX("Boss_Push");
	}

	int Now = mAnimation->GetNowFrame();
	int End = mAnimation->GetEndFrame();

	if (Now >= End)
	{
		IsBossFriend = true;

		//플레이어와 가장 먼 위치에 생성
		float	Max = 0;
		int		Index = 0;
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
		mBaseAttackParticle->gameobject->ChoiceParent(Hand);
		mBaseAttackParticle->Play();
		IsAttack = false;
	}

	//왼쪽 근접 공격
	int End = mAnimation->GetEndFrame();
	int Now = mAnimation->GetNowFrame();
	if (mAnimation->EventCheck() == true)
	{
		if (IsAttack == false && PlayerDistance <= AttackRange)
		{
			int Damage = 150;
			MessageManager::GetGM()->SEND_Message(TARGET_PLAYER, MESSAGE_PLAYER_HIT, &Damage);
			MessageManager::GetGM()->SEND_Message(TARGET_UI, MESSAGE_UI_PLAYER_HIT);
			Sound_Play_SFX("Boss_Attack_5L");
			IsAttack = true;
		}
	}

	if (Now >= End)
	{
		SetState(BOSS_STATE::IDLE);
		mBaseAttackParticle->Stop();
	}
	mTransform->Slow_Y_Rotation(mPlayerTR->GetPosition(), 150, false);
}

void Boss::Boss_Closer_Attack_R()
{
	if (FirstState() == true)
	{
		GameObject* Hand = gameobject->GetChildBone("t1.R");
		mBaseAttackParticle->gameobject->ChoiceParent(Hand);
		mBaseAttackParticle->Play();
		IsAttack = false;
	}

	//오른쪽 근접 공격
	int End = mAnimation->GetEndFrame();
	int Now = mAnimation->GetNowFrame();
	if (mAnimation->EventCheck() == true)
	{
		if (IsAttack == false && PlayerDistance <= AttackRange)
		{
			int Damage = 10;
			MessageManager::GetGM()->SEND_Message(TARGET_PLAYER, MESSAGE_PLAYER_HIT, &Damage);
			MessageManager::GetGM()->SEND_Message(TARGET_UI, MESSAGE_UI_PLAYER_HIT, nullptr);
			Sound_Play_SFX("Boss_Attack_5L");
			IsAttack = true;
		}
	}
	if (Now >= End)
	{
		SetState(BOSS_STATE::IDLE);
		mBaseAttackParticle->Stop();
	}
	mTransform->Slow_Y_Rotation(mPlayerTR->GetPosition(), 150, false);
}

void Boss::Boss_Chase_Attack_Ready(int Phase)
{
	int End = mAnimation->GetEndFrame();
	int Now = mAnimation->GetNowFrame();
	if (Now >= End)
	{
		SetState(BOSS_STATE::CHASE_ATTACK_PLAY);
	}
	mTransform->Slow_Y_Rotation(mPlayerTR->GetPosition(), 150, false);
}

void Boss::Boss_Chase_Attack_Play(int Phase)
{
	if (FirstState() == true)
	{
		CreateSkillPoint_Chase();
	}

	static int WeaponIndex = 0;
	if (IsShooting == false)
	{
		Vector3 Look = mTransform->GetLocalPosition_Look();
		Look.z *= -1;

		Vector3 Start;
		
		Start = mTransform->GetPosition() + (Look * 3);
		Start.y += 3.0f;

		if (WeaponIndex < 5)
		{
			Weapon[WeaponIndex]->SetShootingPoistion(Start, SkillPoint_01[WeaponIndex], 9, (WeaponIndex + 2) * 6,true);
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
		if (RendomSkillPlayTime >= RendomSkillPlayTimeMax)
		{
			SetState(BOSS_STATE::CHASE_ATTACK_END);
			RendomSkillPlayTime = 0;
			WeaponIndex = 0;
			IsShooting = false;
			for (int i = 0; i < 5; i++)
			{
				Weapon[i]->Reset();
			}
		}
		else
		{
			RendomSkillPlayTime += GetDeltaTime();
		}
	}
}

void Boss::Boss_Chase_Attack_End(int Phase)
{
	int End = mAnimation->GetEndFrame();
	int Now = mAnimation->GetNowFrame();

	if (Now >= End)
	{
		SetState(BOSS_STATE::IDLE);
	}
}

void Boss::Boss_Rendom_Attack_Ready(int Phase)
{
	if (FirstState() == true)
	{
		if (ColorType == 0)
		{
			mMF_Setting.SetLimlightSetting(1, 1, 1, 0.5f, 0.5f);
			mMF_Setting.SetLimlightSettingMax(1, 0, 0, 3, 0.5f);
		}
		else
		{
			mMF_Setting.SetLimlightSetting(1, 1, 1, 0.5f, 0.5f);
			mMF_Setting.SetLimlightSettingMax(0, 0, 1, 3, 0.5f);
		}
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
}

void Boss::Boss_Rendom_Attack_Play(int Phase)
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
			if (Phase == 0)
			{
				SetState(BOSS_STATE::RENDOM_ATTACK_END);
			}
			else
			{
				SetState(BOSS_STATE::RENDOM_ATTACK_ADDSKILL);
			}

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

void Boss::Boss_Rendom_Attack_End(int Phase)
{
	if (FirstState() == true)
	{
		if(ColorType == 0)
		{
			mMF_Setting.SetLimlightSetting(1, 0, 0, 3, 0.5f);
			mMF_Setting.SetLimlightSettingMax(1.0f, 1.0f, 1.0f, 0.5f, 0.5f);
		}
		else
		{
			mMF_Setting.SetLimlightSetting(0, 0, 1, 3, 0.5f);
			mMF_Setting.SetLimlightSettingMax(1.0f, 1.0f, 1.0f, 0.5f, 0.5f);
		}

	}
	else
	{
		mMF_Setting.LimLightUpdate(0.5);
	}

	int End = mAnimation->GetEndFrame();
	int NOW = mAnimation->GetNowFrame();

	if (NOW >= End)
	{
		SetState(BOSS_STATE::RENDOM_ATTACK_RESET);
	}
}

void Boss::Boss_Rendom_Attack_Reset(int Phase)
{
	int End = mAnimation->GetEndFrame();
	int NOW = mAnimation->GetNowFrame();
	if (mAnimation->EventCheck() == true)
	{
		if (FirstState() == true)
		{
			for (int i = 0; i < 5; i++)
			{
				Weapon[i]->Explosion();
			}
		}
	}
	if (NOW >= End)
	{
		SetState(BOSS_STATE::IDLE);
	}
}

void Boss::Boss_Rendom_Attack_AddSkill()
{
	if (FirstState() == true)
	{
		RendomSkillPlayTime = 0.0f;
	}

	static int WeaponIndex = 0;

	if (IsShooting == false)
	{
		Vector3 Start = mTransform->GetPosition();
		Start.y += 7.0f;
		if (WeaponIndex < 5)
		{
			Weapon[WeaponIndex]->SetShootingPoistion(Weapon[WeaponIndex]->gameobject->GetTransform()->GetPosition(), Start);
			IsShooting = true;
		}
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
		if (RendomSkillPlayTime >= RendomSkillPlayTimeMax)
		{
			SetState(BOSS_STATE::RENDOM_ATTACK_BIG);
			RendomSkillPlayTime = 0;
			WeaponIndex = 0;
			IsShooting = false;
			for (int i = 0; i < 5; i++)
			{
				Weapon[i]->Reset();
			}
		}
		else
		{
			RendomSkillPlayTime += GetDeltaTime();
		}
	}
}

void Boss::Boss_Rendom_Attack_Big()
{
	if (FirstState() == true)
	{
		BigWeapon->SetScale(10);
		Vector3 Start = mTransform->GetPosition();
		Start.y += 9.0f;
		BigWeapon->SetShootingPoistion(Start, mPlayerTR->GetPosition(), 5, 10, true);
	}


	if (BigWeapon->ShootingReady() == true)
	{
		SetState(BOSS_STATE::IDLE);
	}
}

void Boss::Boss_Guidedmissile_Attack_Start()
{
	int End = mAnimation->GetEndFrame();
	int Now = mAnimation->GetNowFrame();
	if (Now >= 30)
	{
		mAnimation->Pause();
		SetState(BOSS_STATE::MISSILE_PLAY);
	}
}

void Boss::Boss_Guidedmissile_Attack_Play()
{
	static int WeaponIndex = 0;
	mTransform->Slow_Y_Rotation(mPlayerTR->GetPosition(), 150, false);
	if (IsShooting == false)
	{
		Vector3 Right = mTransform->GetLocalPosition_Right();
		Right.z *= -1;
		Vector3 Start;
		Start = mTransform->GetPosition() + (Right * 3);
		Start.y += 8.0f;

		if (WeaponIndex < 5)
		{
			Weapon[WeaponIndex]->SetShootingPoistion(Start,mPlayerTR->GetPosition(), 9, 20,true);
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
		if (RendomSkillPlayTime >= RendomSkillPlayTimeMax)
		{
			SetState(BOSS_STATE::MISSILE_END);
			RendomSkillPlayTime = 0;
			WeaponIndex = 0;
			IsShooting = false;
			mAnimation->Play();
		}
		else
		{
			RendomSkillPlayTime += GetDeltaTime();
		}
	}
}

void Boss::Boss_Guidedmissile_Attack_End()
{
	int End = mAnimation->GetEndFrame();
	int Now = mAnimation->GetNowFrame();
	if (Now >= End)
	{
		for (int i = 0; i < 5; i++){Weapon[i]->Reset();}
		SetState(BOSS_STATE::IDLE);
	}

}

void Boss::Boss_Hit()
{
	


}

void Boss::Phase_UP_Start()
{
	if (FirstState() == true)
	{
		for (int i = 0; i < 5; i++)
		{
			Weapon[i]->Reset();
		}
		BigWeapon->Reset();
		Sound_Stop_BGM();
		Sound_Play_BGM("BossZone_Phase2");
	}

	//페이즈 변경시작
	int End = mAnimation->GetEndFrame();
	int NOW = mAnimation->GetNowFrame();
	if (NOW >= End)
	{
		SetState(BOSS_STATE::PHASE_UP_END);
		BossPhase = 1;
		Sound_Play_SFX("Boss_Voice_Attack3");
	}
}

void Boss::Phase_UP_End()
{
	int End = mAnimation->GetEndFrame();
	int NOW = mAnimation->GetNowFrame();
	if (NOW >= End)
	{
		SetState(BOSS_STATE::IDLE);
	}
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
	if (IsStart == false && PlayerDistance <= FightRange)
	{
		IsStart = true;
		MessageManager::GetGM()->SEND_Message(TARGET_UI, MESSAGE_UI_BOSS_ACTIVE, &IsStart);
	}


	return PlayerDistance;
}

void Boss::CreateSkillPoint()
{
	float Angle = 360 / 5;
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

void Boss::CreateSkillPoint_Chase()
{
	Vector3 Look = mTransform->GetLocalPosition_Look();
	Look.z *= -1;
	for (int i = 0; i < 5; i++)
	{
		Vector3 mStart;
		mStart = mTransform->GetPosition() + (Look * ((i+1) * 4));
		SkillPoint_01[i] = mStart;

		mRay->Direction = { 0,-1,0 };
		mRay->MaxDistance = 10;
		mRay->Origin = mStart;
		mRay->Origin.y += 5;
		//충돌된 곳에Y축만 가져온다
		RayCast(mRay);
		SkillPoint_01[i].y = mRay->Hit.HitPoint.y;
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

void Boss::SkillCheck()
{
	float DTime = GetDeltaTime();

	if (PlayerDistance < FightRange)
	{
		///근접 스킬 쿨계산
		if (PlayerDistance < AttackRange)
		{
			if (SkillTime[Base_Time] >= SkillTimeMax[Base_Time])
			{
				SetState(BOSS_STATE::CLOSER_ATTACK_R);
				SkillTime[Base_Time] = 0;
			}
			else
			{
				SkillTime[Base_Time] += DTime;
			}

			if (SkillTime[Teleport_Time] >= SkillTimeMax[Teleport_Time])
			{
				if (IsBossFriend == true)
				{
					SetState(BOSS_STATE::TELEPORT_READY);
				}
				else
				{
					SetState(BOSS_STATE::CREATE_FRIEND);
				}
				SkillTime[Teleport_Time] = 0;
			}
			else
			{
				SkillTime[Teleport_Time] += DTime;
			}
		}
		else
		{
			///원거리 스킬을 체크

			//장판형
			if (SkillTime[Rendom_Time] >= SkillTimeMax[Rendom_Time])
			{
				SetState(BOSS_STATE::RENDOM_ATTACK_START);
				SkillTime[Rendom_Time] = 0;
			}
			else
			{
				SkillTime[Rendom_Time] += DTime;
			}

			//일직선
			if (SkillTime[Chase_Time] >= SkillTimeMax[Chase_Time])
			{
				SetState(BOSS_STATE::CHASE_ATTACK_READY);
				SkillTime[Chase_Time] = 0;
			}
			else
			{
				SkillTime[Chase_Time] += DTime;
			}


			if (BossPhase == 1)
			{
				if (SkillTime[Missile_Time] >= SkillTimeMax[Missile_Time])
				{
					SetState(BOSS_STATE::MISSILE_START);
					SkillTime[Missile_Time] = 0;
				}
				else
				{
					SkillTime[Missile_Time] += DTime;
				}
			}
		}

		mTransform->Slow_Y_Rotation(mPlayerTR->GetPosition(), 150);

		bool Active = true;
		MessageManager::GetGM()->SEND_Message(TARGET_UI, MESSAGE_UI_BOSS_ACTIVE, &Active);

		/// 들어왔을때 한번만해야함
		MessageManager::GetGM()->SEND_Message(TARGET_DRONE, MESSAGE_DRONE_BOSS_ZONE_IN);
	}
}

void Boss::PushPlayer()
{
	Vector3 Look = mTransform->GetLocalPosition_Look();
	Look.z *= -1;
	Vector3 Point;

	int Index = 1;
	while (true)
	{
		Vector3 mStart;
		mStart = mTransform->GetPosition() + (Look * Index);

		mRay->Direction = { 0,-1,0 };
		mRay->MaxDistance = 2;
		mRay->Origin = mStart;
		mRay->Origin.y += 0.5f;
		//충돌된 곳에Y축만 가져온다
		bool Hit = RayCast(mRay);
		
		if (Hit == false)
		{
			//mPlayerTR->SetPosition(Point);
			MessageManager::GetGM()->SEND_Message(TARGET_PLAYER, MESSAGE_PLAYER_PUSH, &Point);
			break;
		}
		else
		{
			Point = mRay->Hit.HitPoint;
			Index += 3;
		}
	}

}

void Boss::BossColorUpdate()
{
	if (IsUpdateColor == false) { return; }
	static float DTime = 0.0f;
	if (ColorType == 0)
	{
		if (DTime >= 1.0f)
		{
			mMF_Setting.SetLimlightSetting(1.0f, 1.0f, 1.0f, 0.5f, 0.5f);
			mMF_Setting.SetEmissiveSetting(231, 39, 9, 2.9f);

			IsUpdateColor = false;
			DTime = 0;
		}
		else
		{
			DTime += GetDeltaTime();
		}
	}
	else
	{
		if (DTime >= 1.0f)
		{
			IsUpdateColor = false;
			DTime = 0;
		}
		else
		{
			DTime += GetDeltaTime();
		}
	}
}

void Boss::StartFight()
{
	if (IsStartFight == true) { return; }
	//보스와 싸울수 있는거리에 들어왔을떄 한번 들어옴
	MessageManager::GetGM()->SEND_Message(TARGET_UI, MESSAGE_UI_BOSS_COLOR, &ColorType);
	bool Active = true;
	MessageManager::GetGM()->SEND_Message(TARGET_UI, MESSAGE_UI_BOSS_ACTIVE, &Active);

	IsStartFight = true;
}

void Boss::EndFight()
{
	if (IsEndFight == true) { return; }
	//보스와 싸울수 있는 거리에 나갔을때




	IsEndFight = true;
}
