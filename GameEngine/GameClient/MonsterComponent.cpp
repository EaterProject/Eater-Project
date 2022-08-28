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
	//Collider설정
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

	//플레이어 위치 설정
	mPlayerTR = FindGameObjectTag("Player")->GetTransform();

	//몬스터 초기 상태 지정
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
	//플레이어 충돌체와 충돌했을때
	if (HitStart == false)
	{
		//플레이어가 공격 상태일때
		if (Player::GetAttackState() == true)
		{
			if (MonsterState == (int)MONSTER_STATE::DEAD) { return; }

			//플레이어와 몬스터의 색이 같을때
			if (Player::GetPlayerColor() == MonsterColor)
			{
				//플레이어 콤보가 나의 콤보를 넘어갔을때
				if (Player::GetPlayerCombo() >= ComboCount)
				{
					//플레이어 콤보를 리셋 시킨다
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
			//플레이어 상태가 공격상태가 아닐떄
			if (Player::GetAttackState() == false)
			{
				HitStart = false;
			}
		}
	}
}

void MonsterComponent::Move()
{
	//처음 상태가 변경되었을때만 실행
	if (FirstState() == true)
	{
		mAnimation->Choice(ANIMATION_NAME[MonsterState], ANIMATION_TIME[MonsterState]);
		mAnimation->Play();
	}

	//추격상태 체크
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
		//목표지점의 도달하지 않았을때
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

	//공격 범위에 나갔을떄 다시 추격 상태로
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

		//이동 해야하는 넘버 설정
		PointNumber		= rand() % 5;
		//몇초까지 Idle상태로 있을것인지 설정
		Idle_MaxTime	= (rand() % Idle_MaxTime_Max) + Idle_MaxTime_Min;
		SetMovePoint(SearchPoint[PointNumber].x, 0, SearchPoint[PointNumber].z);
	}

	//추격상태 체크
	if (ChaseRange > PlayerDistance)
	{
		SetMonsterState(MONSTER_STATE::CHASE);
	}


	//대기시간이 지났는지 않지났는지 체크
	if (IdleTime <= Idle_MaxTime)
	{
		IdleTime += GetDeltaTime();
	}
	else
	{
		//대기시간이 지났다면 Move상태로 변경
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

		// 퓨어 마나 획득
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

	//공격 당했을때
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
	//방향값을 설정
	DirPoint = (Vector3(x, y, z) - mTransform->GetPosition());
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
	STATE[(int)State] = false;

	//타임 변수들도 초기화
	IdleTime	= 0;
	HitTime		= 0;
	AttackTime	= 0;
}

void MonsterComponent::SetState(MONSTER_STATE mState)
{
	//상태를 넣어준다
	STATE[(int)mState] = false;
	MonsterState = (int)mState;
}

bool MonsterComponent::FirstState()
{
	//현재 상태로 처음 들어왔을때를 체크
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
