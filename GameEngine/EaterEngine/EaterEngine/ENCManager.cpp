#include "ENCManager.h"
#include "DebugManager.h"
#include "UnitNet.h"
#include "GameObject.h"
#include "MeshFilter.h"
#include "KeyinputManager.h"
#include "Rigidbody.h"
ENCManager::ENCManager()
{
	
}

ENCManager::~ENCManager()
{


}

void ENCManager::Initialize()
{
	//���� ��û
	C2S_KEEP_ALIVE_CHECK_REQ();
	DebugManager::Print("������...");
	Sleep(100);

	//���� �Ϸ�
	C2S_KEEP_ALIVE_CHECK_RES();
	DebugManager::Print("Ŭ���̾�Ʈ1 ���� �Ϸ�");
	DebugManager::Print("Ŭ���̾�Ʈ2 ���� �Ϸ�");

	//������ �����Ҷ� �ʿ��� �����͸� �޴´�
	S2C_START_GAME();
	Sleep(100);
}

void ENCManager::Awake()
{
	///GetComponent �� ���ִ� �κ�

	
}


void ENCManager::SetUp()
{
	///���۳�Ʈ�� �ʱ�ȭ �Ҷ� ����ȴ�

	//S2C_START_GAME ���� ���� �����ͷ� Player�� �ʱ�ȭ ��Ų��

	if (Client01 != nullptr)
	{
		MeshFilter* MF = Obj_Client01->GetComponent<MeshFilter>();
		Rigidbody* Rig = Obj_Client01->GetComponent<Rigidbody>();
		MF->SetMeshName("Sphere");
		Rig->CreateSphereCollider(1);
		//Rig->SetRestitution(0);
		Rig->SetFreezeRotation(true, true, true);
	}

	if (Client02 != nullptr)
	{
		MeshFilter* MF = Obj_Client02->GetComponent<MeshFilter>();
		Rigidbody* Rig = Obj_Client02->GetComponent<Rigidbody>();
		MF->SetMeshName("box");
		Rig->CreateBoxCollider(1);
		//Rig->SetRestitution(0);
		Rig->SetFreezeRotation(true, true, true);
	}
}

void ENCManager::Start()
{
	///SetUp���� �־��ذ��� ������ ���۳�Ʈ�� ó���ϴºκ�



}

void ENCManager::Update()
{
	///������ ���� �Ǿ����� ���θ� ��� ����
	//C2S_KEEP_ALIVE_CHECK_REQ();
	//C2S_KEEP_ALIVE_CHECK_RES();
}


void ENCManager::AddPlayer(GameObject* mClient01, GameObject* mClient02)
{
	//��ü�� �޴´�
	Client01 = mClient01->GetComponent<UnitNet>();
	Client02 = mClient02->GetComponent<UnitNet>();
	Obj_Client01 = mClient01;
	Obj_Client02 = mClient02;

	//�Ŵ����� ����ϱ����� ������ �־���
	Client01->mManger = this;
	Client02->mManger = this;

	//������ ��ü �ޱ�
	Client01->MyPlayer = true;
	Client02->MyPlayer = false;
}

void ENCManager::SetMovePos(Vector3 Pos)
{
	DebugManager::Print("Client�� �����δ�");
}

void ENCManager::C2S_KEEP_ALIVE_CHECK_REQ()
{
	///�÷��̾ ����Ǿ����� ����
	DebugManager::Print("Client-->-->-->-->--LINK-->-->-->-->Server");
}

void ENCManager::C2S_KEEP_ALIVE_CHECK_RES()
{
	///�÷��̾ ����Ǿ����� ����
	DebugManager::Print("Client--<--<--<--<--LINK--<--<--<--<Server");
}

void ENCManager::S2C_START_GAME()
{
	///���ӽ��ۿ� �ʿ��� �����͸� �޴´�
	DebugManager::Print("Client--<--<--<--<--GameStartData--<--<--<--<Server");

	//���������̿��ϴ� ��ü���� �ƴ��� �Ǵ�
}

void ENCManager::C2S_PLAYER_MOVE_REQ(Vector3 Pos, Vector3 Direction, float Speed)
{
	///������ �����͸� �ѱ��
	DebugManager::Print("Client-->-->-->-->--Move-->-->-->-->Server");
}

void ENCManager::C2S_PLAYER_MOVE_RES()
{
	///�������� �����͸� �޴´�

	DebugManager::Print("Client--<--<--<--<--Move--<--<--<--<Server");
	bool istest = true;

	//�̵����� ������ ��ġ���� �ʴ´ٸ�
	if (istest != true)
	{
		//Client01�� ���� ��ü���
		if (Client01->MyPlayer == true) 
		{
			//Client01�� ��ġ���� �������ش�
		}
		else
		{
			//Client02�� ��ġ���� �������ش�
		}
	}
}
