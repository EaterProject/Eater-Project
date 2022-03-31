#include "Player.h"
#include "AnimationController.h"
#include "Transform.h"
#include "MeshFilter.h"
#include "GameObject.h"
#include "MainHeader.h"
#include "Camera.h"
#include "Rigidbody.h"
#include "Collider.h"


#include "GameClientGameServerPacketDefine.h"
#include "PlayerData.h"
#include "WorldData_generated.h"
#include "PlayerCamera.h"

Player::Player()
{
	mAnimation	= nullptr;
	mTransform	= nullptr;
	mMeshFilter = nullptr;
	Speed = 5;
}

Player::~Player()
{
}

void Player::Awake()
{
	//컨퍼넌트 가져오기
	mTransform	= gameobject->GetTransform();
	mMeshFilter = gameobject->GetComponent<MeshFilter>();
	mAnimation	= gameobject->GetComponent<AnimationController>();
	
	AttackColliderObject = FindGameObjectTag("AttackCollider");
	AttackCollider = AttackColliderObject->GetComponent<Collider>();
	
}

void Player::SetUp()
{
	mCameraTR = GetMainCamera()->GetTransform();

	//AttackCollider->SetTrigger(true);
	//AttackRigidbody->SetGrvity(false);
	//AttackCollider->CreatePhys();
}

void Player::Update()
{
	Vector3 Look = mTransform->GetLocalPosition_Look();
	Look *= 2;
	Look.y = 1;
	Look.z *= -1;

	AttackColliderObject->GetTransform()->Position = mTransform->Position + Look;
}

void Player::StartUpdate()
{
	PlayerKeyinput();
	//mAnimation->Play(1, true);
}


void Player::PlayerKeyinput()
{
	if (mCameraTR == nullptr) { return; }
	if (GetKey('D'))
	{
		DirPos += mCameraTR->GetLocalPosition_Right();
		//mAnimation->Choice("Run");
	}

	if (GetKey('A'))
	{
		DirPos += -mCameraTR->GetLocalPosition_Right();
		//mAnimation->Choice("Run");
	}

	if (GetKey('W'))
	{
		Vector3 Pos = mCameraTR->GetLocalPosition_Look();
		//mAnimation->Choice("Run");
		Pos.y = 0;
		DirPos += Pos;
	}

	if (GetKey('S'))
	{
		Vector3 Pos = mCameraTR->GetLocalPosition_Look();
		//mAnimation->Choice("Run");
		Pos.y = 0;
		DirPos += -Pos;
	}

	DirPos.Normalize();
	if (DirPos != Vector3(0, 0, 0))
	{
		DirRot = DirPos;
		mTransform->SetTranlate(DirPos * Speed * GetDeltaTime());
		mTransform->Slow_Y_Rotation(DirRot, 450);
	}
	
	DirPos = { 0,0,0 };
}