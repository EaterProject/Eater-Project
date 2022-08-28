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
#include "ParticleController.h"
#include "ParticleFactory.h"

MonsterComponent::MonsterComponent()
{
	ANIMATION_NAME[(int)MONSTER_STATE::IDLE]	= "idle";
	ANIMATION_NAME[(int)MONSTER_STATE::ATTACK]	= "attack";
	ANIMATION_NAME[(int)MONSTER_STATE::HIT]		= "hit";
	ANIMATION_NAME[(int)MONSTER_STATE::MOVE]	= "move";
	ANIMATION_NAME[(int)MONSTER_STATE::CHASE]	= "move";
	ANIMATION_NAME[(int)MONSTER_STATE::DEAD]	= "die";

	ANIMATION_TIME[(int)MONSTER_STATE::IDLE]	= 0.75f;
	ANIMATION_TIME[(int)MONSTER_STATE::ATTACK]	= 0.8f;
	ANIMATION_TIME[(int)MONSTER_STATE::HIT]		= 0.75f;
	ANIMATION_TIME[(int)MONSTER_STATE::MOVE]	= 0.75f;
	ANIMATION_TIME[(int)MONSTER_STATE::CHASE]	= 1.0f;
	ANIMATION_TIME[(int)MONSTER_STATE::DEAD]	= 0.75f;

	SOUND_NAME[(int)MONSTER_STATE::HIT] = "Monster_Hit";

	for (int i = 0; i < 6; i++){ STATE[i] = false;}

	MonsterState = (int)MONSTER_STATE::IDLE;
}	

MonsterComponent::~MonsterComponent()
{

}

void MonsterComponent::Awake()
{
	mTransform	= gameobject->GetTransform();
	mMeshFilter = gameobject->GetComponent<MeshFilter>();
	mAnimation	= gameobject->GetComponent<AnimationController>();
	mCollider	= gameobject->GetComponent<Collider>();
	mRigidbody  = gameobject->GetComponent<Rigidbody>();

	mHitController = ParticleFactory::Get()->CreateParticleController(PARTICLE_TYPE::CounterAttack);
	mHitController->SetScale(1);
}

void MonsterComponent::SetUp()
{
	//Collider����
	mCollider->SetBoxCollider(0.5f, 8, 0.5f);
	//mCollider->SetSphereCollider(0.5);
	mCollider->SetCenter(0, 8, 0);
	mCollider->SetMaterial_Dynamic(0);
	mCollider->SetMaterial_Restitution(0);
	mCollider->SetMaterial_Static(0);
	mRigidbody->SetFreezeRotation(true, true, true);

	mMeshFilter->SetModelName(ModelName);
	mMeshFilter->SetAnimationName(AnimationName);
	mAnimation->Play();

	//�÷��̾� ��ġ ����
	mPlayerTR = FindGameObjectTag("Player")->GetTransform();

	//���� �ʱ� ���� ����
	MonsterState = (int)MONSTER_STATE::IDLE;
	PointNumber = rand() % 5;
	mTransform->SetPosition(SearchPoint[PointNumber]);
	mTransform->SetScale(MonsterScale, MonsterScale, MonsterScale);
}

void MonsterComponent::Start()
{
	mMF_Setting.Setting(this->gameobject);
	GetRandomColor();

	if (mParticleController != nullptr)
	{
		mParticleController->gameobject->ChoiceParent(this->gameobject);
		mParticleController->gameobject->GetTransform()->AddPosition(0, 0.5f, 0);
	}
}

void MonsterComponent::Update()
{
	mRigidbody->SetVelocity(0.0f,-9.8f,0.0f);
	switch (MonsterState)
	{
	case (int)MONSTER_STATE::IDLE:
		PlayerDistanceCheck();
		Idle();
		break;
	case (int)MONSTER_STATE::MOVE:
		PlayerDistanceCheck();
		Move();
		break;
	case (int)MONSTER_STATE::ATTACK:
		PlayerDistanceCheck();
		Attack();
		break;
	case (int)MONSTER_STATE::CHASE:
		PlayerDistanceCheck();
		Chase();
		break;
	case (int)MONSTER_STATE::HIT:
		PlayerDistanceCheck();
		Hit();
		break;
	case (int)MONSTER_STATE::DEAD:
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
			if (MonsterState == (int)MONSTER_STATE::DEAD) { return; }

			//�÷��̾�� ������ ���� ������
			if (Player::GetPlayerColor() == MonsterColor)
			{
				//�÷��̾� �޺��� ���� �޺��� �Ѿ����
				if (Player::GetPlayerCombo() >= ComboCount)
				{
					//�÷��̾� �޺��� ���� ��Ų��
					MessageManager::GetGM()->SEND_Message(TARGET_PLAYER, MESSAGE_PLAYER_COMBO_RESET);
					MessageManager::GetGM()->SEND_Message(TARGET_PLAYER, MESSAGE_PLAYER_HEAL);

					Vector3 Pos = mTransform->GetPosition();
					Pos.y += 1.0f;
					mHitController->SetPosition(Pos);
					mHitController->SetScale(0.5f);
					mHitController->Play();
					HP -= 1000;
				}
				else
				{
					MessageManager::GetGM()->SEND_Message(TARGET_PLAYER, MESSAGE_PLAYER_ATTACK_OK);
					HP -= Player::GetPlayerComboPower();
				}
			}
			else
			{
				HP -= Player::GetPlayerPower();
			}

			SetMonsterColor();
			SetMonsterState(MONSTER_STATE::HIT);
			if (Player::GetNoHitState() == false)
			{
				Sound_Play_SFX(SOUND_NAME[(int)MONSTER_STATE::HIT]);
			}
			HitStart	 = true;
		}
	}
	else
	{
		if (Player::GetAttackState() == false)
		{
			//�÷��̾� ���°� ���ݻ��°� �ƴҋ�
			if (Player::GetAttackState() == false)
			{
				HitStart = false;
			}
		}
	}
}

void MonsterComponent::Move()
{
	//ó�� ���°� ����Ǿ������� ����
	if (FirstState() == true)
	{
		mAnimation->Choice(ANIMATION_NAME[MonsterState], ANIMATION_TIME[MonsterState]);
		mAnimation->Play();
	}

	//�߰ݻ��� üũ
	if (ChaseRange > PlayerDistance)
	{
		SetMonsterState(MONSTER_STATE::CHASE);
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
		mRigidbody->SetVelocity(DirPoint.x, -9.8f, DirPoint.z);
	}
	else
	{
		SetMonsterState(MONSTER_STATE::IDLE);
	}
}

void MonsterComponent::Attack()
{
	if (FirstState() == true)
	{
		mAnimation->Choice(ANIMATION_NAME[MonsterState], ANIMATION_TIME[MonsterState]);
		mAnimation->Play();
	}

	if (mAnimation->EventCheck() == true)
	{
		if(mParticleController != nullptr) { mParticleController->Play(); }
		if (IsAttack == false)
		{
			MessageManager::GetGM()->SEND_Message(TARGET_PLAYER, MESSAGE_PLAYER_HIT, &Damage);

			if (Player::GetNoHitState() == false)
			{
				Sound_Play_SFX(SOUND_NAME[(int)MONSTER_STATE::ATTACK]);
			}

			IsAttack = true;
		}
	}
	else
	{
		IsAttack = false;

		if (mParticleController != nullptr)
		{
			mParticleController->Stop();
		}
	}

	//���� ������ �������� �ٽ� �߰� ���·�
	if (AttackRange < PlayerDistance)
	{
		SetMonsterState(MONSTER_STATE::CHASE);
	}

	mTransform->Slow_Y_Rotation(mPlayerTR->GetPosition(), RotationSpeed, MonsterFront_Z);
}

void MonsterComponent::Idle()
{
	if (FirstState() == true)
	{
		mAnimation->Choice(ANIMATION_NAME[MonsterState], ANIMATION_TIME[MonsterState]);
		mAnimation->Play();

		//�̵� �ؾ��ϴ� �ѹ� ����
		PointNumber		= rand() % 5;
		//���ʱ��� Idle���·� ���������� ����
		Idle_MaxTime	= (rand() % Idle_MaxTime_Max) + Idle_MaxTime_Min;
		SetMovePoint(SearchPoint[PointNumber].x, 0, SearchPoint[PointNumber].z);
	}

	//�߰ݻ��� üũ
	if (ChaseRange > PlayerDistance)
	{
		SetMonsterState(MONSTER_STATE::CHASE);
	}


	//���ð��� �������� ���������� üũ
	if (IdleTime <= Idle_MaxTime)
	{
		IdleTime += GetDeltaTime();
	}
	else
	{
		//���ð��� �����ٸ� Move���·� ����
		SetMonsterState(MONSTER_STATE::MOVE);
	}
}

void MonsterComponent::Dead()
{
	if (FirstState() == true)
	{
		mAnimation->Choice(ANIMATION_NAME[MonsterState], ANIMATION_TIME[MonsterState]);
		mAnimation->Play();
		mMF_Setting.SetDissolveOption(DISSOLVE_FADEOUT);
		mMF_Setting.SetDissolveTexture("Dissolve_1");
		mMF_Setting.SetDissolveColor(255.0f, 0, 0);
		mMF_Setting.SetDissolveColorFactor(10.0f);
		mMF_Setting.SetDissolvePlayTime(8.0f);
		mMF_Setting.SetDissolveWidth(0.1f);
		mMF_Setting.SetDissolveInnerFactor(100.0f);
		mMF_Setting.SetDissolveOuterFactor(25.0f);
		
		mCollider->DeletePhysCollider();

		// ǻ�� ���� ȹ��
		MessageManager::GetGM()->SEND_Message(TARGET_PLAYER, MESSAGE_PLAYER_GET_PUREMANA, &PureManaCount);
	}


	int End = mAnimation->GetEndFrame();
	int Now = mAnimation->GetNowFrame();
	if (Now == End)
	{
		mAnimation->Pause();
		MONSTER_EMAGIN Data;
		Data.Object = this->gameobject;
		MessageManager::GetGM()->SEND_Message(TARGET_UI, MESSAGE_UI_MONSTER_UI_OFF, &Data);
		IsUI_ON = false;
		mMF_Setting.PlayDissolve();
		DissolveStart = true;
	}
	else
	{
		mMF_Setting.LimLightUpdate(1);
	}

	if (DissolveStart == true)
	{
		if (mMF_Setting.EndDissolve() == false)
		{
			DissolveStart = false;
			mTransform->SetPosition(0, 0, -400);
			//gameobject->SetActive(false);
		}
	}
}

void MonsterComponent::Chase()
{
	if (FirstState() == true)
	{
		mAnimation->Choice(ANIMATION_NAME[MonsterState], ANIMATION_TIME[MonsterState]);
		mAnimation->Play();
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
			Vector3 position = mPlayerTR->GetPosition();
			SetMovePoint(position.x, 0, position.z);
			mTransform->Slow_Y_Rotation(position, 150, MonsterFront_Z);
			mRigidbody->SetVelocity(DirPoint.x, 0, DirPoint.z);
		}
	}
	IsAttack = false;
}

void MonsterComponent::Hit()
{
	if (FirstState() == true)
	{
		mAnimation->Choice(ANIMATION_NAME[MonsterState], ANIMATION_TIME[MonsterState]);
		mAnimation->Play();
	}

	//���� ��������
	if (HP > 0)
	{
		float End = mAnimation->GetEndFrame();
		float Now = mAnimation->GetNowFrame();
		mMF_Setting.LimLightUpdate(2.0f);
		if (Now >= End)
		{
			HitStart = false;
			SetMonsterState(MONSTER_STATE::CHASE);
		}
	}
	else
	{
		SetMonsterState(MONSTER_STATE::DEAD);
	}
}

void MonsterComponent::Debug()
{
	DebugDrawCircle(ChaseRange, mTransform->GetPosition() + Vector3(0, 0.25f, 0), Vector3(0, 0, 0), Vector3(1, 0, 0));
	DebugDrawCircle(AttackRange, mTransform->GetPosition() + Vector3(0, 0.25f, 0), Vector3(0, 0, 0), Vector3(0, 1, 0));
}

void MonsterComponent::PlayerDistanceCheck()
{
	PlayerDistance = mTransform->GetDistance(mPlayerTR->GetPosition());

	
	if (PlayerDistance <= ChaseRange && IsUI_ON == false) 
	{
		MONSTER_EMAGIN Data;
		if (MonsterColor == MONSTER_COLOR_RED)
		{
			Data.R = 255;
			Data.G = 0;
			Data.B = 0;
			Data.HP = HP;
			Data.MaxHP = Max_HP;
			Data.ComboCount = ComboCount;
			Data.Object = this->gameobject;
			MessageManager::GetGM()->SEND_Message(TARGET_UI, MESSAGE_UI_MONSTER_UI_ON, &Data);
		}
		else
		{
			Data.R = 0;
			Data.G = 0;
			Data.B = 255;
			Data.HP = HP;
			Data.MaxHP = Max_HP;
			Data.Type = MONSTER_TYPE_A;
			Data.ComboCount = ComboCount;
			Data.Object = this->gameobject;
			MessageManager::GetGM()->SEND_Message(TARGET_UI, MESSAGE_UI_MONSTER_UI_ON, &Data);
		}
		IsUI_ON		= true;
	}

	if (PlayerDistance >= ChaseRange && IsUI_ON == true)
	{
		MONSTER_EMAGIN Data;
		Data.Object = this->gameobject;
		MessageManager::GetGM()->SEND_Message(TARGET_UI, MESSAGE_UI_MONSTER_UI_OFF, &Data);
		IsUI_ON		= false;
	}
}

void MonsterComponent::SetSearchPoint(int Index, Vector3 Point)
{
	SearchPoint[Index] = Point;
}
 
void MonsterComponent::SetMovePoint(float x, float y, float z)
{
	//���Ⱚ�� ����
	DirPoint = (Vector3(x, y, z) - mTransform->GetPosition());
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
	STATE[(int)State] = false;

	//Ÿ�� �����鵵 �ʱ�ȭ
	IdleTime	= 0;
	HitTime		= 0;
	AttackTime	= 0;
}

void MonsterComponent::SetState(MONSTER_STATE mState)
{
	//���¸� �־��ش�
	STATE[(int)mState] = false;
	MonsterState = (int)mState;
}

bool MonsterComponent::FirstState()
{
	//���� ���·� ó�� ���������� üũ
	if (STATE[MonsterState] == false)
	{
		STATE[MonsterState] = true;
		return true;
	}
	else
	{
		return false;
	}
}

void MonsterComponent::GetRandomColor()
{
	MonsterColor = rand() % 2;
}

void MonsterComponent::SetMonsterColor()
{
	if (MonsterColor == MONSTER_COLOR_RED)
	{
		MONSTER_EMAGIN Data;
		Data.R = 255;
		Data.G = 0;
		Data.B = 0;
		Data.HP = HP;
		Data.MaxHP = Max_HP;
		Data.Type = MonsterType;
		Data.ComboCount = ComboCount;
		Data.Object = this->gameobject;
		MessageManager::GetGM()->SEND_Message(TARGET_UI, MESSAGE_UI_MONSTER_UI_UPDATE, &Data);
		mMF_Setting.SetLimlightSetting(1, 1, 1, 2.2f, 1.0f);
		mMF_Setting.SetLimlightSettingMax(1, 0, 0, 0.4f, 0.7);
	}
	else
	{
		MONSTER_EMAGIN Data;
		Data.R = 0;
		Data.G = 0;
		Data.B = 255;
		Data.HP = HP;
		Data.MaxHP = Max_HP;
		Data.Type = MonsterType;
		Data.ComboCount = ComboCount;
		Data.Object = this->gameobject;
		MessageManager::GetGM()->SEND_Message(TARGET_UI, MESSAGE_UI_MONSTER_UI_UPDATE, &Data);
		mMF_Setting.SetLimlightSetting(1, 1, 1, 2.2f, 1.0f);
		mMF_Setting.SetLimlightSettingMax(0, 0, 1, 0.4f, 0.7f);
	}
}

bool MonsterComponent::GetStopPoint(const Vector3& Pos)
{
	Transform* mTransform = gameobject->GetTransform();
	Vector3 position = mTransform->GetPosition();
	if (position.x >= (Pos.x - 0.5f) &&
		position.x <= (Pos.x + 0.5f) &&
		position.z >= (Pos.z - 0.5f) &&
		position.z <= (Pos.z + 0.5f))
	{
		return true;

	}
	else
	{
		return false;
	}
}
