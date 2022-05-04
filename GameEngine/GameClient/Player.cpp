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
PLAYER_STATE Player::mState;

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
	
	//AttackColliderObject = FindGameObjectTag("AttackCollider");
	//AttackCollider = AttackColliderObject->GetComponent<Collider>();
}

void Player::Start()
{
	mCameraTR = GetMainCamera()->GetTransform();

	GameObject* Hand = gameobject->GetChildBone("hand.l");
	GameObject* WeponObejct = FindGameObjectTag("Weapon");
	
	Hand->GetTransform()->SetChild(WeponObejct->GetTransform());
	WeponObejct->GetTransform()->SetParent(Hand->GetTransform());
	WeponObejct->GetTransform()->Rotation = { -90,0,90 };
}

void Player::Update()
{
	//������ �����Ҷ� �÷��̾� ���¸� �⺻���·� ��ȯ
	mState = PLAYER_STATE::IDLE;

	//�÷��̾� �� üũ
	PlayerGroundCheck();

	//�÷��̾� Ű�Է�
	PlayerKeyinput();

	//���� �浹ü�� ��ġ�� ����
	PlayerAttackColliderUpdate();

	//�÷��̾� ���� ������Ʈ
	PlayerStateUpdate();
}

Transform* Player::GetPlayerTransform()
{
	return mTransform;
}

PLAYER_STATE Player::GetState()
{
	return mState;
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

	//�̹������ӿ� �̵��ؾ��ϴ� ����
	DirPos.Normalize();

	//���Ⱚ�� �ִٸ� ȸ���� �̵�
	if (DirPos != Vector3(0, 0, 0))
	{
		Vector3 MyPos = DirPos + mTransform->Position;
		mTransform->SetTranlate(DirPos * Speed * GetDeltaTime());
	}

	DirRot.Normalize();
	mTransform->Slow_Y_Rotation(DirRot + mTransform->Position, 450);
	
	//�̵����Ϸ������Ƿ� ���Ⱚ�� �ʱ�ȭ
	DirPos = { 0,0,0 };
}

void Player::PlayerStateUpdate()
{
	//����Ű�� ������ ���º�ȯ
	if (GetKeyDown(VK_SPACE))
	{
		mState = PLAYER_STATE::ATTACK;
	}
}

void Player::PlayerAttackColliderUpdate()
{
	Vector3 Look = mTransform->GetLocalPosition_Look();
	Look *= 2;
	Look.y = 1;
	Look.z *= -1;
	//AttackColliderObject->GetTransform()->Position = mTransform->Position + Look;
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
