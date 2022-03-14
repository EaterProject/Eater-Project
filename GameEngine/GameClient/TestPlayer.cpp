#include "TestPlayer.h"
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

TestPlayer::TestPlayer()
{
	mAnimation	= nullptr;
	mTransform	= nullptr;
	mMeshFilter = nullptr;
	Speed = 5;
}

TestPlayer::~TestPlayer()
{
}

void TestPlayer::Awake()
{
	//���۳�Ʈ ��������
	mTransform = gameobject->GetTransform();
	mMeshFilter = gameobject->GetComponent<MeshFilter>();
	//mAnimation = gameobject->GetComponent<AnimationController>();
	mRigidbody = gameobject->GetComponent<Rigidbody>();
	mCollider  = gameobject->GetComponent<Collider>();
}

void TestPlayer::SetUp()
{
	//�Ž�, �ִϸ��̼� ���� �Է�
	mMeshFilter->SetMeshName("box");
	//mMeshFilter->SetAnimationName("MOdNA");
	//mAnimation->Choice("Idle");

	GameObject* MainCam = GetMainCamera();
	MainCam->GetComponent<PlayerCamera>()->Userobject = gameobject;
	mCameraTR = MainCam->GetTransform();
	mTransform->Rotation = { 0,0,0 };
	mCollider->SetBoxCollider(0.25f);
	mRigidbody->SetPosition(5, 10, 0);
	mRigidbody->SetFreezeRotation(true, true, true);
	
	//������ ����
	mTransform->Scale = { 0.25f,0.25f ,0.25f };
}

void TestPlayer::Start()
{
	//MeshObject		= gameobject->GetChildBone(0);
	//GameObject* obj	=  gameobject->GetChildMesh(0);
	//
	////���� �� ����
	//obj->GetTransform()->Position = { 0,-9.5f,0 };
	//obj->GetTransform()->Rotation = { -90,0,90 };
}

void TestPlayer::Update()
{
	//�÷��̾ Ű�Է��� �������� üũ �Ѵ�
	if (GetKeyDown(VK_SPACE))
	{
		mRigidbody->SetPosition(5, 0, 0);
	}

	if (mRigidbody->GetTriggerEnter() == true)
	{
		DebugPrint("�浹����");
	}

	if (mRigidbody->GetTriggerStay() == true)
	{
		DebugPrint("�浹��");
	}

	if (mRigidbody->GetTriggerExit() == true)
	{
		DebugPrint("�浹��");
	}
}

void TestPlayer::StartUpdate()
{
	PlayerKeyinput();
	//mAnimation->Play(1, true);
}

void TestPlayer::PlayerKeyinput()
{
	if (mCameraTR == nullptr) { return; }
	if (GetKey(VK_RIGHT))
	{
		DirPos += mCameraTR->GetLocalPosition_Right();
		//mAnimation->Choice("Run");
	}

	if (GetKey(VK_LEFT))
	{
		DirPos += -mCameraTR->GetLocalPosition_Right();
		//mAnimation->Choice("Run");
	}

	if (GetKey(VK_UP))
	{
		Vector3 Pos = mCameraTR->GetLocalPosition_Look();
		//mAnimation->Choice("Run");
		Pos.y = 0;
		DirPos += Pos;
	}

	if (GetKey(VK_DOWN))
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
	}
	else
	{
		mRigidbody->SetVelocity(DirPos.x, 0, DirPos.z);
	}
	

	//���� �������� ĳ���͸� ȸ�������ش�
	Vector3 Dir;
	Dir.x = DirPos.x;
	Dir.y = DirPos.y;
	Dir.z = DirPos.z;
	//MeshObject->GetTransform()->Slow_Y_Rotation(Dir, 300);
	//MeshObject->GetTransform()->SetRotate(0, 0, 1);
	DirPos*= Speed;
	mRigidbody->SetVelocity(DirPos.x, 0, DirPos.z);
	DirPos = { 0,0,0 };
}