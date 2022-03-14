#include "Transform.h"
#include "MainHeader.h"
#include "AI.h"
#include "GameObject.h"
#include "Rigidbody.h"
#include "MeshFilter.h"
#include "AnimationController.h"
#include "PhysData.h"
#include "Camera.h"


AI::AI()
{
	mTransform				= nullptr;
	mRigidbody				= nullptr;
	mMeshFilter				= nullptr;
	mAnimationController	= nullptr;

	Speed = 2;
	isJump = false;
}

AI::~AI()
{

}

void AI::Awake()
{
	mTransform	= gameobject->GetComponent<Transform>();
	mMeshFilter = gameobject->GetComponent<MeshFilter>();
	mRigidbody = gameobject->GetComponent<Rigidbody>();
	mAnimationController = gameobject->GetComponent<AnimationController>();
	
}

void AI::SetUp()
{
	mTransform->Position = { 0,5,0 };
	mTransform->Rotation	= { 0,0,0 };
	mTransform->Scale		= { 0.25f,0.25f,0.25f };

	

	mMeshFilter->SetMeshName("MOdNA");
	mMeshFilter->SetAnimationName("MOdNA");
	
	mAnimationController->Choice("Idle");
	//mRigidbody->CreateBoxCollider(0.35f, 0.5f, 0.35f);
	//mRigidbody->SetCenterPoint(0, 9.75f, 0);
	//mRigidbody->SetRestitution(0);
	//
	////mRigidbody->SetRestitution(0);
	//mRigidbody->SetFreezeRotation(true, true, true);
}

void AI::Update()
{
	KeyInputMove();
	mObject = gameobject->GetChildBone(0);
	mObject->GetTransform()->Rotation = { -90,0,Keyinput_Angle};
	mAnimationController->Play(1, true);
}

void AI::KeyInputMove()
{
	if (GetKey(VK_UP))
	{
		Keyinput_Right = 1* Speed;
		Keyinput_Angle = 180;
	}
	else if (GetKey(VK_DOWN))
	{
		Keyinput_Right = -1* Speed;
		Keyinput_Angle = 0;
	}
	else
	{
		Keyinput_Right = 0;
	}


	if (GetKey(VK_RIGHT))
	{
		keyinput_Up = 1* Speed;
		Keyinput_Angle = -90;
	}
	else if (GetKey(VK_LEFT))
	{
		keyinput_Up = -1* Speed;
		Keyinput_Angle = 90;
	}
	else
	{
		keyinput_Up = 0;
	}


	if (GetKeyDown(VK_SPACE))
	{
		mRigidbody->SetAddForce(0, 500, 0);
	}

	//아무것도 누르지않았을 경우
	if (keyinput_Up == 0 && Keyinput_Right == 0)
	{
		mAnimationController->Choice("Idle");
	}
	else
	{
		mAnimationController->Choice("Run");
	}

	mRigidbody->SetVelocity(keyinput_Up,0, Keyinput_Right);
	mTransform->Rotation = {0,Keyinput_Angle,0};
}





