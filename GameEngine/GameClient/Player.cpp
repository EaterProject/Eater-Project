#include "Player.h"
#include "AnimationController.h"
#include "Transform.h"
#include "MeshFilter.h"
#include "GameObject.h"
#include "EaterEngineAPI.h"
#include "Camera.h"
#include "Collider.h"

#include "PhysData.h"
#include "PhysRay.h"
#include "MessageManager.h"
#include "ClientTypeOption.h"


#include "PlayerCamera.h"

Transform* Player::mTransform = nullptr;
bool	Player::IsAttackTime = false;
//PLAYER_STATE Player::mState;

#define LERP(prev, next, time) ((prev * (1.0f - time)) + (next * time))
Player::Player()
{
	mAnimation	= nullptr;
	mTransform	= nullptr;
	mMeshFilter = nullptr;
	mCameraTR = nullptr;
	AttackColliderObject= nullptr;
	AttackCollider = nullptr;

	RayCastHit = new PhysRayCast[5]();
	BasePos = Vector3(0, 0, 0);
	Start_Order = FUNCTION_ORDER_LAST;
}

Player::~Player()
{
	mAnimation = nullptr;
	mTransform = nullptr;
	mMeshFilter = nullptr;
	mCameraTR = nullptr;
	AttackColliderObject = nullptr;
	AttackCollider = nullptr;
	delete[] RayCastHit;
}

void Player::Awake()
{
	//���۳�Ʈ ��������
	mTransform	= gameobject->GetTransform();
	mMeshFilter = gameobject->GetComponent<MeshFilter>();
	mAnimation	= gameobject->GetComponent<AnimationController>();
	
	//���� ������Ʈ ��������
	WeaponObject = Instance();
	WeaponObject->AddComponent<MeshFilter>()->SetModelName("player_Weapon");

	//�浹 ���� ��������
	AttackColliderObject = FindGameObjectTag("PlayerCollider");
	AttackCollider = AttackColliderObject->GetComponent<Collider>();

	IsCreate = true;
}

void Player::SetUp()
{
	//Player ���۳�Ʈ�� �־��ֱ�
	mCameraTR = FindGameObjectTag("PlayerCamera")->GetTransform();
	mMeshFilter->SetModelName("Player+");
	mMeshFilter->SetAnimationName("Player+");
	//AttackCollider ����
	AttackCollider->SetSphereCollider(AttackRange);
	AttackCollider->SetTrigger(true);
	//�ִϸ��̼� �⺻ 
	mAnimation->Play();
}

void Player::Start()
{
	//����� ���� ����
	GameObject* Hand = gameobject->GetChildBone("hand.l");
	//
	Hand->ChoiceChild(WeaponObject);
	WeaponObject->ChoiceParent(Hand);
	WeaponTR = WeaponObject->GetTransform();
}

void Player::Update()
{
	if (IsCreate == false) { return; }

	//�÷��̾� �� üũ
	PlayerGroundCheck();

	//�÷��̾� Ű�Է�
	PlayerKeyinput();

	//���� �浹ü�� ��ġ�� ����
	PlayerAttackColliderUpdate();

	//�÷��̾� ���ݴ������� �����ð�
	PlayerHitTimeCheck();

	//���ݻ����϶� �ƴҋ��� ���� üũ
	if (mState & (PLAYER_STATE_ATTACK_01 | PLAYER_STATE_ATTACK_02 | PLAYER_STATE_SKILL_01 | PLAYER_STATE_SKILL_02))
	{
		PlayerState_Attack();
	}
	else
	{
		PlayerState_Base();
	}

	DirPos = { 0,0,0 };
}

void Player::SetMessageRECV(int Type, void* Data)
{
	//�ٸ� ��ü���� �޼����� �޴´� ���� �޼����� 
	//UI������ �ٽ� ������

	switch (Type)
	{
	case MESSAGE_PLAYER_HIT:
		Player_Hit(*(reinterpret_cast<int*>(Data)));
		break;
	case MESSAGE_PLAYER_HILL:
		break;
	case MESSAGE_PLAYER_ATTACK_OK:
		ComboCount++;
		MessageManager::GetGM()->SEND_Message(TARGET_UI,MESSAGE_UI_COMBO,&ComboCount);
		break;
	}
}

Transform* Player::GetPlayerTransform()
{
	return mTransform;
}

bool Player::GetAttackState()
{
	return IsAttackTime;
}

void Player::Healing(float HealingPower)
{
	float MaxHP = LERP(0, HP, 0.7f);
	if (HP <= MaxHP)
	{
		HP += HealingPower;
	}
}

void Player::PlayerKeyinput()
{
	if (mCameraTR == nullptr) 
	{
		DebugPrint("ī�޶� ���� �ȵǾ�����");
		return;
	}

	if (GetKey('D'))
	{
		if (IsRightRayCheck == true)
		{
			DirPos += mCameraTR->GetLocalPosition_Right();
		}
		DirRot += mCameraTR->GetLocalPosition_Right();
	}

	if (GetKey('A'))
	{
		if (IsLeftRayCheck == true)
		{
			DirPos += -mCameraTR->GetLocalPosition_Right();
		}
		DirRot += -mCameraTR->GetLocalPosition_Right();
	}

	if (GetKey('W'))
	{
		if (IsFrontRayCheck == true)
		{
			Vector3 Pos = mCameraTR->GetLocalPosition_Look();
			Pos.y = 0;
			DirPos += Pos;
		}
		DirRot += mCameraTR->GetLocalPosition_Look();
	}

	if (GetKey('S'))
	{
		if (IsBackRayCheck == true)
		{
			Vector3 Pos = mCameraTR->GetLocalPosition_Look();
			Pos.y = 0;
			DirPos += -Pos;
		}
		DirRot += mCameraTR->GetLocalPosition_Look()* -1;
	}

	if (GetKeyDown(VK_LBUTTON))
	{
		AttackKeyDownCount++;
		mState |= PLAYER_STATE_ATTACK_01;
	}
	else if (GetKeyDown(VK_RBUTTON))
	{
		if (IsAttack == false)
		{
			 mState |= PLAYER_STATE_SKILL_01;
		}
	}
	else if (GetKeyDown('E'))
	{
		mState |= PLAYER_STATE_SKILL_02;
	}
	else if (GetKeyDown('Q'))
	{
		ChangeCount++;
		if (ChangeCount > 14){ChangeCount = 0;}
		MessageManager::GetGM()->SEND_Message(TARGET_UI, MESSAGE_UI_EMAGIN_NOW, &ChangeCount);
		Sound_Play_SFX("ChangeEmagin");
	}
	else if (GetKeyDown(VK_SPACE))
	{
		mState |= PLAYER_STATE_JUMP;
	}

	//�̹������ӿ� �̵��ؾ��ϴ� ����
	DirPos.Normalize();
	DirRot.Normalize();

	Vector3 position = mTransform->GetPosition();

	if (IsAttack == false)
	{

		mTransform->Slow_Y_Rotation(DirRot + position, 450);

		//�������� �ƴϰ� ȸ������ �������
		if (DirPos != Vector3(0, 0, 0))
		{
			mTransform->AddPosition(DirPos * Speed * GetDeltaTime());
		}
		Attack_Rot = false;
	}
	else
	{
		if (Attack_Rot == false)
		{
			AttackStartRot = mCameraTR->GetLocalPosition_Look();
			Attack_Rot = true;
		}
		mTransform->Slow_Y_Rotation(AttackStartRot + position, 450);
	}
}

void Player::PlayerState_Attack()
{
	IsAttack = true;
	IsMove = false;

	/// �÷��̾� ���ݻ����϶� ���ɴϴ�

	Speed = 0;
	if (mState & PLAYER_STATE_ATTACK_01)
	{
		AnimationName = "attack1";
		mAnimation->Choice(AnimationName, Animation_Attack01_Speed, false);
		Player_Attack_01();
	}
	else if (mState & PLAYER_STATE_ATTACK_02)
	{
		AnimationName = "attack2";
		mAnimation->Choice(AnimationName,Animation_Attack02_Speed, false);
		Player_Attack_02();
	}
	else if (mState & PLAYER_STATE_SKILL_01)
	{
		AnimationName = "skill1";
		mAnimation->Choice(AnimationName, Animation_Skill01_Speed, false);
		Player_Skill_01();
	}
	else if (mState & PLAYER_STATE_SKILL_02)
	{
		AnimationName = "skill2";
		mAnimation->Choice(AnimationName, Animation_Skill02_Speed, false);
		AnimationSpeed = 2;
		Player_Skill_02();
	}
	mAnimation->Play();
	AnimationSpeed = 1;
}

void Player::PlayerState_Base()
{
	IsAttack	= false;
	/// <summary>
	/// �÷��̾� ���ݻ��°� �ƴҶ� ���ɴϴ�
	/// </summary>
	Speed = MaxSpeed;
	if (mState & PLAYER_STATE_JUMP)
	{
		mAnimation->Choice("evade", Animation_Jump_Speed, true);
		Player_Jump();
	}
	else
	{
		Player_Move_Check();
	}
}

bool Player::PlayerEndFrameCheck()
{
	AttackEndFrame	= mAnimation->GetEndFrame();
	NowFrame		= mAnimation->GetNowFrame();
	if (NowFrame > AttackEndFrame)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Player::PlayerHitTimeCheck()
{
	if (IsHit == false) { return; }
	HitTime += GetDeltaTime();
	if (HitTime >= 0.5f)
	{
		IsHit = false;
		HitTime = 0;
	}
}

void Player::Player_Attack_01()
{
	WeaponTR->SetPosition(0.0f, -0.02f, 0.1f);
	WeaponTR->SetRotate(8.0f, 19.0f, -10.0f);

	if (mAnimation->EventCheck() == true)
	{
		IsAttackTime = true;
	}
	else
	{
		IsAttackTime = false;
	}

	//��ġ�� ��¦�ø��� ������� ����
	if (PlayerEndFrameCheck() == true)
	{
		if (AttackKeyDownCount > 1)
		{
			mState = PLAYER_STATE_ATTACK_02;
			AttackKeyDownCount = 0;
		}
		else
		{
			Player_Move_Check();
		}
	}
}

void Player::Player_Attack_02()
{
	//��ġ�� �ڷ� ������ ũ�� ������� ����
	WeaponTR->SetPosition(0.0f, -0.02f, 0.1f);
	WeaponTR->SetRotate(186.0f, 31.0f, 0.0f);

	if (mAnimation->EventCheck() == true)
	{
		IsAttackTime = true;
	}
	else
	{
		IsAttackTime = false;
	}

	if (PlayerEndFrameCheck() == true)
	{
		Player_Move_Check();
	}
}

void Player::Player_Skill_01()
{
	WeaponTR->SetPosition(0.0f, -0.02f, 0.1f);
	WeaponTR->SetRotate(186.0f, 31.0f, 0.0f);

	if (mAnimation->EventCheck() == true)
	{
		IsAttackTime = true;
	}
	else
	{
		IsAttackTime = false;
	}


	if (PlayerEndFrameCheck())
	{
		Player_Move_Check();
	}
}

void Player::Player_Skill_02()
{
	WeaponTR->SetPosition(0.0f, -0.02f, 0.1f);
	WeaponTR->SetRotate(186.0f, 31.0f, 0.0f);

	if (mAnimation->EventCheck() == true)
	{
		IsAttackTime = true;
	}
	else
	{
		IsAttackTime = false;
	}

	if (PlayerEndFrameCheck())
	{
		Player_Move_Check();
	}
}

void Player::Player_Skill_03()
{
	WeaponTR->SetPosition(0.0f, -0.02f, 0.1f);
	WeaponTR->SetRotate(8.0f, 19.0f, -10.0f);

	if (mAnimation->EventCheck() == true)
	{
		IsAttackTime = true;
	}
	else
	{
		IsAttackTime = false;
	}


	if (PlayerEndFrameCheck())
	{
		Player_Move_Check();
	}
}

void Player::Player_Jump()
{
	WeaponTR->SetPosition(0.0f, -0.02f, 0.1f);
	WeaponTR->SetRotate(-22.0f, 33.0f, 2.0f);

	if (PlayerEndFrameCheck() == true)
	{
		Player_Move_Check();
	}
}

void Player::Player_Hit(int HitPower)
{
	if (IsHit == true){return;}

	IsHit = true;
	HP -= HitPower;
	if (HP <= 0)
	{
		//�׾�����
		HP = 0;
	}
	else
	{
		MessageManager::GetGM()->SEND_Message(TARGET_UI, MESSAGE_UI_HP_NOW, &HP);
	}
}

bool Player::Player_Move_Check()
{
	if (DirPos == BasePos)
	{
		WeaponTR->SetPosition(0.0f, -0.02f, 0.1f);
		WeaponTR->SetRotate(186.0f, 31.0f, 0.0f);

		mState = PLAYER_STATE_IDLE;
		mAnimation->Choice("idle", Animation_Idle_Speed, true);
		IsMove = false;
		return false;
	}
	else
	{
		WeaponTR->SetPosition(0.0f, -0.02f, 0.1f);
		WeaponTR->SetRotate(0.0f, 21.0f, -5.0f);

		mState = PLAYER_STATE_MOVE; 
		mAnimation->Choice("move", Animation_Move_Speed, true);
		IsMove = true;
		return true;
	}
	AttackKeyDownCount = 0;
}

void Player::PlayerAttackColliderUpdate()
{
	Vector3 Look = mTransform->GetLocalPosition_Look();
	Look *= 2;
	Look.y = 1;
	Look.z *= -1 * AttackDir;
	AttackColliderObject->GetTransform()->SetPosition(mTransform->GetPosition() + Look);
}

void Player::PlayerGroundCheck()
{
	Vector3 RayStartPoint;
	Vector3 position = mTransform->GetPosition();
	for (int i = 0; i < 5; i++)
	{
		switch (i)
		{
		case 0: //Front
			RayStartPoint = position + mCameraTR->GetLocalPosition_Look();
			break;
		case 1: //Back
			RayStartPoint = position + (mCameraTR->GetLocalPosition_Look() * -1);
			break;
		case 2: //Right
			RayStartPoint = position + mCameraTR->GetLocalPosition_Right();
			break; 
		case 3: //Left
			RayStartPoint = position + (mCameraTR->GetLocalPosition_Right() * -1);
			break;
		case 4: //Center
			RayStartPoint = position;
			break;
		}
		RayStartPoint.y = position.y + 2;

		//Ray �� ����
		RayCastHit[i];
		RayCastHit[i].Origin		= RayStartPoint;
		RayCastHit[i].Direction		= {0,-1,0};
		RayCastHit[i].MaxDistance	= 10;
		bool isHit = RayCast(&RayCastHit[i]);
	
		switch (i)
		{
		case 0:
			IsFrontRayCheck = isHit;
			break;
		case 1:
			IsBackRayCheck = isHit;
			break;
		case 2:
			IsRightRayCheck = isHit;
			break;
		case 3:
			IsLeftRayCheck = isHit;
			break;
		case 4:
			mTransform->SetPosition_Y(RayCastHit[i].Hit.HitPoint.y);
			break;
		}
	}
}
