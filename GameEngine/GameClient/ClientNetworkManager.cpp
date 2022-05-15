#include "ClientNetworkManager.h"
#include "NetworkComponent.h"
#include "GameClientGameServerPacketDefine.h"
#include "MainHeader.h"
#include "WorldData_generated.h"

//컨퍼넌트
#include "GameObject.h"
#include "MeshFilter.h"
#include "Transform.h"
#include "Player.h"
#include "AnimationController.h"
#include "ManaStone.h"
#include "Monster.h"

//구조체
#include "PlayerData.h"

ClientNetworkManager::ClientNetworkManager()
{
	ClientNumber	= -1;
	Current_Time	= 0;
	Passed_Time		= 0;
	Prev_Time		= 0;
	ClientNumber	= -1;
	m_Passed_Client_Time = 0;

	isCreateEnemy	= false;
	isCreateMana	= false;
	isStartGame		= false;
}

ClientNetworkManager::~ClientNetworkManager()
{
	

	
}

void ClientNetworkManager::Update()
{
	m_Passed_Client_Time += GetDeltaTime();
}

void ClientNetworkManager::RECV(void* Data, int type)
{
	switch (type)
	{
	case S2C_LOADING_COMPLETE_RES:  //게임 시작 전 정보
		RECV_LOADING_COMPLETE(Data);
		break;
	case S2C_START_GAME:			//게임 시작 후 정보
		RECV_START_GAME(Data);
		break;
	case S2C_WORLD_UPDATE:			//게임 업데이트
		RECV_WORLD_UPDATE(Data);
		break;
	}
}

void ClientNetworkManager::RECV_START_GAME(void* Data)
{
	if (isStartGame == true) { return; }
	//데이터 변환
	uint8_t* Recv_Data_Ptr = (unsigned char*)Data;
	const Eater::GameData::StartInfo* _Recv_Data = flatbuffers::GetRoot<Eater::GameData::StartInfo>(Recv_Data_Ptr);

	//유저 데이터를 받는다
	auto _Players_Data = _Recv_Data->users();
	
	//초기값을 받아서 플레이어 오브젝트 생성
	int MaxSize = (int)_Players_Data->size();
	for (int i = 0; i < MaxSize; i++)
	{
		auto Player_Data	= _Players_Data->Get(i);
		auto _Player_Index	= Player_Data->playerindex();

		int PlayerType	= Player_Data->character_type();
		float Speed		= Player_Data->world_speed();

		float Pos_X = Player_Data->world_position()->x();
		float Pos_Y = Player_Data->world_position()->y();
		float Pos_Z = Player_Data->world_position()->z();

		//오브젝트를 생성한다
		CreatePlayer(Vector3(Pos_X,Pos_Y,Pos_Z), _Player_Index, Speed, PlayerType);
	}
	isStartGame = true;
}

void ClientNetworkManager::RECV_LOADING_COMPLETE(void* Data)
{
	//클라이언트 넘버를받는다
	unsigned short* Recv_Data_Ptr = (unsigned short*)Data;
	ClientNumber = *Recv_Data_Ptr;
}

void ClientNetworkManager::RECV_WORLD_UPDATE(void* Data)
{
	///월드 데이터 업데이트

	uint8_t* Recv_Data_Ptr = (unsigned char*)Data;

	//월드 데이터를 읽는다
	const Eater::GameData::WorldData* _Recv_Data
		= flatbuffers::GetRoot<Eater::GameData::WorldData>(Recv_Data_Ptr);

	//시간을 계산
	Current_Time = GetDeltaTime();			//현재 한프레임 시간
	Passed_Time = Current_Time - Prev_Time; //현재 시간에서 이전시간을 뺸다  = 걸린시간
	Prev_Time = Current_Time;				//과거시간을 현재 시간으로 업데이트

	UpdatePlayer(_Recv_Data);	//플레이어 상태를 업데이트
	UpdateEnemy(_Recv_Data);	//적 유닛 상태를 업데이트
	UpdateMana(_Recv_Data);		//마나석	상태를 업데이트
}

/// 생성
void ClientNetworkManager::CreatePlayer(Vector3 Pos, int PlayerIndex, float Speed, int PlayerType)
{
	//플레이어 객체를 만들어준다
	GameObject* obj = Instance("Player");
	MeshFilter* mMeshFilter = obj->AddComponent<MeshFilter>();
	Transform*	mTransform	= obj->GetTransform();
	Player*		mPlayer		= obj->AddComponent<Player>();
	AnimationController* AC = obj->AddComponent<AnimationController>();

	//시작 데이터를 넣어줌
	mTransform->Position	= Pos;
	mPlayer->SetClientNumber(ClientNumber, PlayerIndex);
	//통신할 수 있도록 빌더를 넘겨준다
	mPlayer->_Builder		= _Builder;
	mPlayer->Speed			= Speed;

	//생성한 데이터를 리스트에 담아둔다
	UserObjList.insert({ PlayerIndex ,mPlayer });
}

void ClientNetworkManager::CreateEnemy(int index, Vector3 Pos)
{
	//적 몬스터를 생성한다

	GameObject* obj = Instance();
	obj->AddComponent<MeshFilter>();
	obj->AddComponent<AnimationController>();

	Transform*	mTransform	= obj->GetTransform();
	Monster*	mMonster	= obj->AddComponent<Monster>();

	mTransform->Position = Pos;
	MonsterList.insert({index,mMonster});
}

void ClientNetworkManager::CreateManaStone(int index, Vector3 Pos)
{
	//마나석을 생성한다

	GameObject* obj = Instance();
	obj->AddComponent<MeshFilter>();
	obj->AddComponent<AnimationController>();

	Transform* mTransform = obj->GetTransform();
	ManaStone* mManaStone = obj->AddComponent<ManaStone>();

	mTransform->Position = Pos;
	ManaList.insert({index,mManaStone });
}

///삭제
void ClientNetworkManager::DeleteManaStone(int index)
{
	///마나석 삭제

	//없으면 통과
	if (ManaList[index] == nullptr) { return; }

	//오브젝트 가져오기
	GameObject* Obj = ManaList[index]->gameobject;
	ManaList[index] = nullptr;
	
	//삭제
	Destroy(Obj);
}

void ClientNetworkManager::DeleteMonster(int index)
{
	///몬스터 삭제

	//없으면 통과
	if (MonsterList[index] == nullptr) { return; }

	//오브젝트 가져오기
	GameObject* Obj = MonsterList[index]->gameobject;
	MonsterList[index] = nullptr;

	//삭제
	Destroy(Obj);
}

///업데이트
void ClientNetworkManager::UpdatePlayer(const Eater::GameData::WorldData* Data)
{
	//유저 데이터를 가져온다
	auto _Players_Data = Data->users();

	//플레이어 데이터 읽기
	for (int i = 0; i < (int)_Players_Data->size(); i++)
	{
		auto Player_Data = _Players_Data->Get(i);

		// 유저 번호.
		auto	_Player_Index = Player_Data->playerindex();		// Player index

		PlayerData UserData;
		//유저 위치
		UserData.Position.x = Player_Data->position()->x();
		UserData.Position.y = Player_Data->position()->y();
		UserData.Position.z = Player_Data->position()->z();

		UserData.SequenceNumber = (int)Player_Data->sequence();		// 시퀀스 번호
		//유저 애니메이션 타입
		UserData.AnimationType = (int)Player_Data->animation();
		//오브젝트 넘버
		UserData.ObjectNumber = _Player_Index;
		UserData.Passed_Time = m_Passed_Client_Time;

		//플레이어 객체에게 네트워크에서 받은 데이터를 넘겨준다
		UserObjList[_Player_Index]->SetNetworkPlayerData(UserData);
	}
}

void ClientNetworkManager::UpdateEnemy(const Eater::GameData::WorldData* Data)
{
	//적 유닛 데이터를 받아온다
	auto _Enemy_Data = Data->enemies();
	int Size = (int)_Enemy_Data->size();

	for (int i = 0; i < Size; i++)
	{
		auto _Enemy = _Enemy_Data->Get(i);

		int index = _Enemy->index();
		float x = _Enemy->position()->x();
		float y = _Enemy->position()->y();
		float z = _Enemy->position()->z();
		
		//함수가 처음 시작되었을때는 생성 그다음부터는 업데이트
		if (isCreateEnemy == false)
		{
			//생성
			CreateEnemy(index, Vector3(x, y, z));
		}
		else
		{
			//업데이트
			//MonsterList[index]->MonsterUpdate(Vector3(x,y,z));
		}
	}
	isCreateEnemy = true;
}

void ClientNetworkManager::UpdateMana(const Eater::GameData::WorldData* Data)
{
	//마나석 데이터를 받아온다
	auto _Object_Data = Data->objects();
	int Size = (int)_Object_Data->size();

	
	for (int i = 0; i < (int)_Object_Data->size(); i++)
	{
		auto _Object = _Object_Data->Get(i);

		int index = _Object->index();
		float x = _Object->position()->x();
		float y = _Object->position()->y();
		float z = _Object->position()->z();

		//함수가 처음 시작되었을때는 생성 그다음부터는 업데이트
		if (isCreateMana == false)
		{
			//생성
			CreateManaStone(index, Vector3(x, y, z));
		}
		else
		{
			//업데이트
			//ManaList[index]->ManaStoneUpdate(Vector3(x,y,z));
		}
	}
	isCreateMana = true;
}


