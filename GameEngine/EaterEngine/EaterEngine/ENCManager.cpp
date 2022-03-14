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
	//연결 요청
	C2S_KEEP_ALIVE_CHECK_REQ();
	DebugManager::Print("연결중...");
	Sleep(100);

	//연결 완료
	C2S_KEEP_ALIVE_CHECK_RES();
	DebugManager::Print("클라이언트1 연결 완료");
	DebugManager::Print("클라이언트2 연결 완료");

	//게임이 시작할때 필요한 데이터를 받는다
	S2C_START_GAME();
	Sleep(100);
}

void ENCManager::Awake()
{
	///GetComponent 를 해주는 부분

	
}


void ENCManager::SetUp()
{
	///컨퍼넌트를 초기화 할때 실행된다

	//S2C_START_GAME 에서 받은 데이터로 Player를 초기화 시킨다

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
	///SetUp에서 넣어준값을 각자의 컨퍼넌트가 처리하는부분



}

void ENCManager::Update()
{
	///서버에 연결 되었는지 여부를 계속 보냄
	//C2S_KEEP_ALIVE_CHECK_REQ();
	//C2S_KEEP_ALIVE_CHECK_RES();
}


void ENCManager::AddPlayer(GameObject* mClient01, GameObject* mClient02)
{
	//객체를 받는다
	Client01 = mClient01->GetComponent<UnitNet>();
	Client02 = mClient02->GetComponent<UnitNet>();
	Obj_Client01 = mClient01;
	Obj_Client02 = mClient02;

	//매니저와 통신하기위해 포인터 넣어줌
	Client01->mManger = this;
	Client02->mManger = this;

	//움직일 객체 받기
	Client01->MyPlayer = true;
	Client02->MyPlayer = false;
}

void ENCManager::SetMovePos(Vector3 Pos)
{
	DebugManager::Print("Client가 움직인다");
}

void ENCManager::C2S_KEEP_ALIVE_CHECK_REQ()
{
	///플레이어가 연결되었는지 보냄
	DebugManager::Print("Client-->-->-->-->--LINK-->-->-->-->Server");
}

void ENCManager::C2S_KEEP_ALIVE_CHECK_RES()
{
	///플레이어가 연결되었는지 받음
	DebugManager::Print("Client--<--<--<--<--LINK--<--<--<--<Server");
}

void ENCManager::S2C_START_GAME()
{
	///게임시작에 필요한 데이터를 받는다
	DebugManager::Print("Client--<--<--<--<--GameStartData--<--<--<--<Server");

	//내가움직이여하는 객체인지 아닌지 판단
}

void ENCManager::C2S_PLAYER_MOVE_REQ(Vector3 Pos, Vector3 Direction, float Speed)
{
	///서버로 데이터를 넘긴다
	DebugManager::Print("Client-->-->-->-->--Move-->-->-->-->Server");
}

void ENCManager::C2S_PLAYER_MOVE_RES()
{
	///서버에서 데이터를 받는다

	DebugManager::Print("Client--<--<--<--<--Move--<--<--<--<Server");
	bool istest = true;

	//이동값이 서버와 일치하지 않는다면
	if (istest != true)
	{
		//Client01이 나의 객체라면
		if (Client01->MyPlayer == true) 
		{
			//Client01의 위치값을 변경해준다
		}
		else
		{
			//Client02의 위치값을 변경해준다
		}
	}
}
