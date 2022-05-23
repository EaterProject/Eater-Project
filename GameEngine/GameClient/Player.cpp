#include "Player.h"
#include "AnimationController.h"
#include "Transform.h"
#include "MeshFilter.h"
#include "GameObject.h"
#include "EaterEngineAPI.h"
#include "Camera.h"
#include "Collider.h"
#include "PhysData.h"


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
	WeaponObject = FindGameObjectTag("Weapon");
	WeaponObject->GetComponent<MeshFilter>()->SetModelName("player_weapon2");

	//�浹 ���� ��������
	AttackColliderObject = FindGameObjectTag("PlayerCollider");
	AttackCollider = AttackColliderObject->GetComponent<Collider>();
}

void Player::SetUp()
{
	//Player ���۳�Ʈ�� �־��ֱ�
	mCameraTR = GetMainCamera()->GetTransform();
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
	GameObject* WeponObejct = FindGameObjectTag("Weapon");
	//
	Hand->ChoiceChild(WeponObejct);
	WeponObejct->ChoiceParent(Hand);
	WeaponTR = WeaponObject->GetTransform();
}

void Player::Update()
{
	//�÷��̾� �� üũ
	PlayerGroundCheck();

	//�÷��̾� Ű�Է�
	PlayerKeyinput();

	//���� �浹ü�� ��ġ�� ����
	PlayerAttackColliderUpdate();

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
	 	AttackKeyDownCount++;
	}
	else if (GetKeyDown('E'))
	{
		AttackKeyDownCount++;
		mState |= PLAYER_STATE_SKILL_02;
	}
	else if (GetKeyDown('Q'))
	{
		//mState |= PLAYER_STATE_SKILL_03;
	}
	else if (GetKeyDown(VK_SPACE))
	{
		mState |= PLAYER_STATE_JUMP;
	}

	//�̹������ӿ� �̵��ؾ��ϴ� ����
	DirPos.Normalize();

	

	DirRot.Normalize();
	if (IsAttack == false)
	{
		mTransform->Slow_Y_Rotation(DirRot + mTransform->Position, 450);

		//�������� �ƴϰ� ȸ������ �������
		if (DirPos != Vector3(0, 0, 0))
		{
			Vector3 MyPos = DirPos + mTransform->Position;
			mTransform->SetTranlate(DirPos * Speed * GetDeltaTime());
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
		mTransform->Slow_Y_Rotation(AttackStartRot + mTransform->Position, 450);
	}
}

void Player::PlayerState_Attack()
{
	IsAttack = true;
	IsMove = false;
	/// <summary>
	/// �÷��̾� ���ݻ����϶� ���ɴϴ�
	/// </summary>
	Speed = 0;
	if (mState & PLAYER_STATE_ATTACK_01)
	{
		AnimationName = "attack1";
		Player_Attack_01();
	}
	else if (mState & PLAYER_STATE_ATTACK_02)
	{
		AnimationName = "attack2";
		Player_Attack_02();
	}
	else if (mState & PLAYER_STATE_SKILL_01)
	{
		AnimationName = "skill1";
		Player_Skill_01();
	}
	else if (mState & PLAYER_STATE_SKILL_02)
	{
		AnimationName = "skill2";
		AnimationSpeed = 2;
		Player_Skill_02();
	}
	//else
	//{
	//	AnimationName = "skill3";
	//	Player_Skill_03();
	//}
	mAnimation->Choice(AnimationName, AnimationSpeed, false);
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
		mAnimation->Choice("evade", 1, true);
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

void Player::Player_Attack_01()
{
	WeaponTR->Position = { 0,-0.02f,0.1f };
	WeaponTR->Rotation = { 8,19,-10 };
	if (mAnimation->EventCheck() == true)
	{
		DebugDrawCircle(1, mTransform->Position, mTransform->Rotation,Vector3(0,1,0));
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
	WeaponTR->Position = { 0,-0.02f,0.1f };
	WeaponTR->Rotation = { 186,31,0 };
	if (PlayerEndFrameCheck() == true)
	{
		Player_Move_Check();
	}
}

void Player::Player_Skill_01()
{
	WeaponTR->Position = { 0,-0.02f,0.1f };
	WeaponTR->Rotation = { 186,31,0 };
	if (PlayerEndFrameCheck())
	{
		Player_Move_Check();
	}
}

void Player::Player_Skill_02()
{
	WeaponTR->Position = { 0,-0.02f,0.1f };
	WeaponTR->Rotation = { 186,31,0 };
	if (PlayerEndFrameCheck())
	{
		Player_Move_Check();
	}
}

void Player::Player_Skill_03()
{
	WeaponTR->Position = { 0,-0.02f,0.1f };
	WeaponTR->Rotation = { 8,19,-10 };
	if (PlayerEndFrameCheck())
	{
		Player_Move_Check();
	}
}

void Player::Player_Jump()
{
	WeaponTR->Position = { 0,-0.02f,0.1f };
	WeaponTR->Rotation = { -22,33,2 };
	if (PlayerEndFrameCheck() == true)
	{
		Player_Move_Check();
	}
}

bool Player::Player_Move_Check()
{
	if (DirPos == BasePos)
	{
		WeaponTR->Position = { 0,-0.02f,0.1f };
		WeaponTR->Rotation = { 186,31,0 };
		mState = PLAYER_STATE_IDLE;
		mAnimation->Choice("idle", 1, true);
		IsMove = false;
		return false;
	}
	else
	{
		WeaponTR->Position = { 0,-0.02f,0.1f };
		WeaponTR->Rotation = { 0,21,-5 };
		mState = PLAYER_STATE_MOVE; 
		mAnimation->Choice("move", 1, true);
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
	AttackColliderObject->GetTransform()->Position = mTransform->Position + Look;
}

void Player::PlayerGroundCheck()
{
	Vector3 RayStartPoint;
	for (int i = 0; i < 5; i++)
	{
		switch (i)
		{
		case 0: //Front
			RayStartPoint = mTransform->Position + mCameraTR->GetLocalPosition_Look();
			break;
		case 1: //Back
			RayStartPoint = mTransform->Position + (mCameraTR->GetLocalPosition_Look() * -1);
			break;
		case 2: //Right
			RayStartPoint = mTransform->Position + mCameraTR->GetLocalPosition_Right();
			break; 
		case 3: //Left
			RayStartPoint = mTransform->Position + (mCameraTR->GetLocalPosition_Right() * -1);
			break;
		case 4: //Center
			RayStartPoint = mTransform->Position;
			break;
		}
		RayStartPoint.y = mTransform->Position.y + 2;

		//Ray �� ����
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
			mTransform->Position.y = RayCastHit[i].Hit.HitPoint.y;
			break;
		}
	}
}
