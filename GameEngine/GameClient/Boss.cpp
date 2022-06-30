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
	//�ִϸ��̼� �̸� ����
	ANIMATION_NAME[(int)BOSS_STATE::GROGGY_START]			= "weak_1";
	ANIMATION_NAME[(int)BOSS_STATE::GROGGY_PLAY]			= "weak_2";
	ANIMATION_NAME[(int)BOSS_STATE::GROGGY_END]				= "weak_3";
	ANIMATION_NAME[(int)BOSS_STATE::IDLE]					= "idle";
	ANIMATION_NAME[(int)BOSS_STATE::DEAD]					= "die";
	ANIMATION_NAME[(int)BOSS_STATE::CLOSER_ATTACK_L]		= "attack5L";
	ANIMATION_NAME[(int)BOSS_STATE::CLOSER_ATTACK_R]		= "attack5R";
	ANIMATION_NAME[(int)BOSS_STATE::CHASE_ATTACK_READY]		= "recovery_1";
	ANIMATION_NAME[(int)BOSS_STATE::CHASE_ATTACK_PLAY]		= "recovery_2";
	ANIMATION_NAME[(int)BOSS_STATE::RANDOM_ATTACK_READY]	= "attack4_1";
	ANIMATION_NAME[(int)BOSS_STATE::RANDOM_ATTACK_PLAY]		= "attack4_2";
	ANIMATION_NAME[(int)BOSS_STATE::RENDOM_ATTACK_END]		= "attack5L";
	ANIMATION_NAME[(int)BOSS_STATE::TELEPORT_READY]			= "attack2_1";
	ANIMATION_NAME[(int)BOSS_STATE::TELEPORT_START]			= "attack2_2";
	ANIMATION_NAME[(int)BOSS_STATE::CREATE_FRIEND]			= "attack3";

	//�ִϸ��̼� �ð� ����
	ANIMATION_TIME[(int)BOSS_STATE::GROGGY_START]			= 1.0f;
	ANIMATION_TIME[(int)BOSS_STATE::GROGGY_PLAY]			= 1.0f;
	ANIMATION_TIME[(int)BOSS_STATE::GROGGY_END]				= 1.0f;
	ANIMATION_TIME[(int)BOSS_STATE::IDLE]					= 1.0f;
	ANIMATION_TIME[(int)BOSS_STATE::DEAD]					= 1.0f;
	ANIMATION_TIME[(int)BOSS_STATE::CLOSER_ATTACK_L]		= 1.0f;
	ANIMATION_TIME[(int)BOSS_STATE::CLOSER_ATTACK_R]		= 1.0f;
	ANIMATION_TIME[(int)BOSS_STATE::CHASE_ATTACK_READY]		= 0.5f;
	ANIMATION_TIME[(int)BOSS_STATE::CHASE_ATTACK_PLAY]		= 1.0f;
	ANIMATION_TIME[(int)BOSS_STATE::RANDOM_ATTACK_READY]	= 0.5f;
	ANIMATION_TIME[(int)BOSS_STATE::RANDOM_ATTACK_PLAY]		= 1.0f;
	ANIMATION_TIME[(int)BOSS_STATE::TELEPORT_READY]			= 1.0f;
	ANIMATION_TIME[(int)BOSS_STATE::TELEPORT_START]			= 1.0f;
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

	//�÷��̾� ��ġ ����
	mPlayerTR = FindGameObjectTag("Player")->GetTransform();
	mState = (int)BOSS_STATE::IDLE;

	//��ġ�� ����
	mTransform->SetPosition(-45.09f, 6.8f, 70.0f);
	StartPoint = { -44.0f,6.0f,62.0f };
	mTransform->SetScale(1.5f, 1.5f, 1.5f);

	//��ų ����Ʈ�� ��ġ�� �����Ѵ�
	CreateSkillPoint();
}

void Boss::Start()
{
	mMF_Setting.Setting(this->gameobject);
	mMF_Setting.SetLimlightSetting(1.0f, 1.0f, 1.0f, 0.5f, 0.5f);
	mMF_Setting.SetEmissiveSetting(231,39,9, 2.9f);

	GameObject* Hand = gameobject->GetChildBone("t1.L");
	mParticle->gameobject->ChoiceParent(Hand);
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
	case (int)BOSS_STATE::GROGGY_START:	//�׷α� ����
		Boss_Groggy_Start();
		break;
	case (int)BOSS_STATE::GROGGY_PLAY:	//�׷α� ��
		Boss_Groggy_Play();
		break;
	case (int)BOSS_STATE::GROGGY_END:	//�׷α� ��
		Boss_Groggy_End();
		break;
	case (int)BOSS_STATE::TELEPORT_READY:	//�ڷ���Ʈ �غ�
		Boss_Teleport_Ready();
		break;
	case (int)BOSS_STATE::TELEPORT_START:	//�ڷ���Ʈ ����
		Boss_Teleport_Start();
		break;
	case (int)BOSS_STATE::CREATE_FRIEND:	//�н� ����
		Boss_Create();
		break;
	case (int)BOSS_STATE::CLOSER_ATTACK_L:		//���� ���� ����
		Boss_Closer_Attack_L();
		break;
	case (int)BOSS_STATE::CLOSER_ATTACK_R:		//���� ������ ����
		Boss_Closer_Attack_R();
		break;
	case (int)BOSS_STATE::CHASE_ATTACK_READY:	//�߰� �߻�ü ����
		Boss_Chase_Attack_Ready();
		break;
	case (int)BOSS_STATE::CHASE_ATTACK_PLAY:	//�߰� �߻�ü ����
		Boss_Chase_Attack_Play();
		break;
	case (int)BOSS_STATE::RANDOM_ATTACK_READY:	//������ �߻�ü ����
		Boss_Rendom_Attack_Ready();
		break;
	case (int)BOSS_STATE::RANDOM_ATTACK_PLAY:	//������ �߻�ü ����
		Boss_Rendom_Attack_Play();
		break;
	case (int)BOSS_STATE::HIT:	//������ �߻�ü ����
		Boss_Hit();
		break;
	case (int)BOSS_STATE::RENDOM_ATTACK_END:	//������ �߻�ü ����
		Boss_Rendom_Attack_End();
		break;
	}

	mAnimation->Choice(ANIMATION_NAME[mState], ANIMATION_TIME[mState]);
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
	//������ �´°� �����ϱ�����
	if (IsHit == false)
	{
		//�÷��̾� ���°� �����̰ų�,��ų�����϶�
		if (Player::GetAttackState() == true)
		{
			HP -= 10;
			MessageManager::GetGM()->SEND_Message(TARGET_PLAYER, MESSAGE_PLAYER_ATTACK_OK);
			SetState(BOSS_STATE::GROGGY_START);
			Sound_Play_SFX("Monster_Hit");
			IsHit = true;
		}
	}
	else
	{
		//�÷��̾� ���°� ���ݻ��°� �ƴҋ�
		if (Player::GetAttackState() == false){IsHit = false;}
	}

}

void Boss::Boss_Idle()
{
	GroundCheck();
	PlayerDistanceCheck();

	if (PlayerDistance < AttackRange)
	{
		//���� ������ ������ ����
		mTransform->Slow_Y_Rotation(mPlayerTR->GetPosition(), 150, false);
		if (IsRight == false)
		{
			SetState(BOSS_STATE::CLOSER_ATTACK_L);
			IsRight = true;
		}
		else
		{
			SetState(BOSS_STATE::CLOSER_ATTACK_R);
			IsRight = false; 
		}
	}
	else if(PlayerDistance < FightRange && PlayerDistance > AttackRange)
	{
		//������ ������ ����
		mTransform->Slow_Y_Rotation(mPlayerTR->GetPosition(), 150, false);
	}


	if (mTransform->GetPosition().y > PositionY)
	{
		mTransform->AddPosition_Y(-GetDeltaTime());
	}
	else
	{
		mTransform->SetPosition_Y(PositionY);
	}


	if (GetKeyDown(VK_NUMPAD7))
	{
		SetState(BOSS_STATE::TELEPORT_READY);
	}

	if (GetKeyDown(VK_NUMPAD8))
	{
		SetState(BOSS_STATE::RANDOM_ATTACK_READY);
	}

	if (GetKeyDown(VK_NUMPAD9))
	{
		SetState(BOSS_STATE::CREATE_FRIEND);
	}

	if (GetKeyDown(VK_NUMPAD4))
	{
		SetState(BOSS_STATE::CHASE_ATTACK_READY);
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
	if (FirstState() == true)
	{
		mMF_Setting.SetLimlightSetting(1,1,1, 1, 10);
		mMF_Setting.SetLimlightSettingMax(1, 1, 1, 50, 10);
	}
	else
	{
		mMF_Setting.LimLightUpdate(10);
	}


	if (mTransform->GetScale().x > 0.0f)
	{
		float mDTime = GetDeltaTime();
		mTransform->AddScale_X(-GetDeltaTime() * 10);
	}
	else
	{
		mTransform->SetScale_X(0.0f);
		Friend->gameobject->SetActive(false);
		SetState(BOSS_STATE::TELEPORT_START);
	}
}

void Boss::Boss_Teleport_Start()
{
	if (FirstState() == true)
	{
		mMF_Setting.SetLimlightSetting(1, 1, 1, 50, 10);
		mMF_Setting.SetLimlightSettingMax(1, 1, 1, 1, 10);
	}
	else
	{
		mMF_Setting.LimLightUpdate(10);
	}


	if (FriendIndex == -1)
	{
		mTransform->SetPosition(StartPoint);
	}
	else
	{
		mTransform->SetPosition(SkillPoint[FriendIndex]);
	}

	if (mTransform->GetScale().x < 1.5f)
	{
		float mDTime = GetDeltaTime();
		mTransform->AddScale_X(mDTime*10);
	}
	else
	{
		mTransform->SetScale_X(1.5f);
		SetState(BOSS_STATE::IDLE);

		mMF_Setting.Default();
	}
}

void Boss::Boss_Create()
{
	int Now = mAnimation->GetNowFrame();
	int End = mAnimation->GetEndFrame();

	if (Now >= End)
	{
		//�÷��̾�� ���� �� ��ġ�� ����
		float	Max = 0;
		int		Index = 0;
		//��ų ����Ʈ ��ġ�� ���Ӱ� ���Ѵ�
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



	//���� ���� ����
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

	//������ ���� ����
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
	static int WeaponIndex = 0;
	mTransform->Slow_Y_Rotation(mPlayerTR->GetPosition(), 150, false);
	if (IsShooting == false)
	{
		Vector3 Look = mTransform->GetLocalPosition_Look();
		Look.z *= -1;

		Vector3 Start;
		
		Start = mTransform->GetPosition() + (Look * 3);
		Start.y += 3.0f;

		Weapon[WeaponIndex]->SetShootingPoistion(Start, mPlayerTR->GetPosition());

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
			SetState(BOSS_STATE::IDLE);
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

void Boss::Boss_Rendom_Attack_Ready()
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
		SetState(BOSS_STATE::RANDOM_ATTACK_PLAY);
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
	int NOW = mAnimation->GetNowFrame();

	if (mAnimation->EventCheck() == true)
	{
		for (int i = 0; i < 5; i++)
		{
			Weapon[i]->Reset();
		}
	}

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
	//���¸� �־��ش�
	STATE_START[mState] = false;
	mState = (int)State;
}

bool Boss::FirstState()
{
	//���� ���·� ó�� ���������� üũ
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

		//Ray�� ���
		mRay->Direction = { 0,-1,0 };
		mRay->MaxDistance = 10;
		mRay->Origin = EndPoint;

		//�浹�� ����Y�ุ �����´�
		RayCast(mRay);
		EndPoint.y = mRay->Hit.HitPoint.y;
		SkillPoint[i] = EndPoint;
	}
}

void Boss::GroundCheck()
{
	//Ray�� ���
	mRay->Direction = { 0,-1,0 };
	mRay->MaxDistance = 10;
	mRay->Origin = mTransform->GetPosition();
	mRay->Origin.y += 0.2f;
	//�浹�� ����Y�ุ �����´�
	RayCast(mRay);
	PositionY = mRay->Hit.HitPoint.y + 0.5f;
}