#include "GameManager.h"
#include "DHNetWorkAPI.h"
#include "DHTimer.h"
#include "PortIPDefine.h"
#include "GameProtocol/WorldData_generated.h"
#include "GameProtocol/StartData_generated.h"
#include "PhysXManager.h"
#include "Player.h"
#include "Enemy.h"
#include "PhysData.h"

GameManager::GameManager()
{
	InitializeCriticalSection(&g_DataIn_CS);
	InitializeCriticalSection(&g_ListIn_CS);

	// 데이터 전송을 위한 패킷 생성(재활용 할 예정)
	_S2C_Msg_Loop = new S2C_Packet;
	_S2C_Msg_Main = new S2C_Packet;

	// Flatbuffer Builder
	Loop_Builder = new flatbuffers::FlatBufferBuilder;
	Main_Builder = new flatbuffers::FlatBufferBuilder;

	// PhysX Manager
	m_PhysXManager = new PhysXManager();
	m_PhysXManager->Initialize();

	// 타이머 생성 서버 프리퀀시 20.
	m_SequenceTimer = new DHTimer;
	m_SequenceTimer->SetFrame(SERVER_FREQUENCY);

	// 물리 처리를 하기 위한 타이머 (서버와 클라이언트가 동일해야 한다)
	m_PhysicsTimer = new DHTimer;
	m_PhysicsTimer->SetFrame(60);

	// 게임클라이언트를 받기 위한 게임서버 생성
	m_Game_Server = new DHNetWorkAPI();
	m_Game_Server->Initialize(DHNetWork_Name::DHNet, DHDEBUG_NONE);
  
	unsigned short Connect_Port = GAME_SERVER_PORT_1;
	bool Accept_Result = m_Game_Server->Accept(Connect_Port, 2, 8);
	while (!Accept_Result)
	{
		Connect_Port++;
		assert(Connect_Port <= GAME_SERVER_PORT_5);
		Accept_Result = m_Game_Server->Accept(Connect_Port, 2, 8);
	}

	// 로비 서버와 통신(클라이언트 입장)
	m_Lobby_Client = new DHNetWorkAPI();
	m_Lobby_Client->Initialize(DHNetWork_Name::DHNet);
	while (!m_Lobby_Client->Connect(LOBBY_SERVER_PORT, LOCAL_CONNECT_IP)) { Sleep(0); };

	// 현재 어떤 포트로 서버를 열었는지 로비서버에게 알려준다.
	C2S_Packet _Send_Packet;
	_Send_Packet.Packet_Type = C2S_CONNECT_PORT_REQ;
	_Send_Packet.Packet_Size = sizeof(Connect_Port);
	memcpy_s(_Send_Packet.Packet_Buffer, sizeof(Connect_Port), &Connect_Port, sizeof(Connect_Port));

	m_Lobby_Client->Send(&_Send_Packet);

	g_Constant_Send_Thread = new std::thread(std::bind(&GameManager::S2C_Loop, this));
	g_Lobby_Logic_Thread = new std::thread(std::bind(&GameManager::Lobby_Loop, this));
	g_PhysX_World_Thread = new std::thread(std::bind(&GameManager::WorldUpdate_Loop, this));
}

GameManager::~GameManager()
{
	DeleteCriticalSection(&g_DataIn_CS);
	DeleteCriticalSection(&g_ListIn_CS);
}

void GameManager::S2C_Loop()
{
	// 게임서버는 계속 재사용되므로 이 로직은 계속 실행되어야 한다.
	while (true)
	{
		// 게임이 시작되었으면
		while (Is_Game_Playing.load())
		{
			// 타이머를 한프레임에 한번씩 업데이트해줘야한다.
			m_SequenceTimer->Update();
			// 1초에 SERVER_FREQUENCY 에 정의된 프레임 수 만큼 실행
			if (m_SequenceTimer->Ready_Frame())
			{
				// 현재 World 전송.
				S2C_World_Update();
			}
			Sleep(0);
		}
		// 현재 게임서버에 연결되어 플레이중인 클라이언트가 존재하지 않는다면..
		Sleep(0);
	}
}

void GameManager::Lobby_Loop()
{
	// 로비와의 통신은 연결되어 있는한 계속 해야한다.
	while (true)
	{
		if (m_Lobby_Client->Recv(Msg_Vec_Lobby))
		{
			// 로비서버로부터 받은 데이터들로 로직을 처리..
			for (auto& Msg_Packet : Msg_Vec_Lobby)
			{
				// 서버로 부터 온 메세지
				S2C_Packet* S2C_Msg = static_cast<S2C_Packet*>(Msg_Packet.Packet);

				// World 초기 데이터가 도착.
				if (S2C_Msg->Packet_Type == S2C_CONNECT_PORT_RES)
				{
					S2C_Set_BaseData(S2C_Msg);
				}

				// 매칭 완료된 유저의 정보가 도착.
				if (S2C_Msg->Packet_Type == S2C_MATCHING_SUCCESS)
				{
					S2C_Set_MatchData(S2C_Msg);
					// 로비 정보가 모두 셋팅됨.
					Is_Lobby_SetUp.exchange(true);
				}

				// 사용한데이터 해제.
				delete Msg_Packet.Packet;
				Msg_Packet.Packet = nullptr;
			}
			Msg_Vec_Lobby.clear();
		}

		Sleep(0);
	}
}

void GameManager::ServerRecv_Loop()
{
	while (!Is_Game_End.load())
	{
		while (Is_Lobby_SetUp.load())
		{
			// 주기적으로 받은 패킷이 있으면 확인한다.
			if (m_Game_Server->Recv(Msg_Vec))
			{
				// 클라이언트로부터 받은 데이터들로 로직을 처리..
				for (auto& Msg_Packet : Msg_Vec)
				{
					// 클라이언트의 소켓 번호
					SOCKET _Recv_Socket_Num = Msg_Packet.Socket;
					// 클라이언트가 보낸 메세지
					C2S_Packet* C2S_Msg = static_cast<C2S_Packet*>(Msg_Packet.Packet);

					// 클라이언트가 강제종료한 경우..
					if (C2S_Msg == NULL)
					{
						continue;
					}

					// 클라이언트가 준비되었다는 메세지가 들어옴.
					if (C2S_Msg->Packet_Type == C2S_LOADING_COMPLETE_REQ)
					{
						unsigned int* _User_Key = (unsigned int*)C2S_Msg->Packet_Buffer;

						C2S_Player_Loading_Complete(_Recv_Socket_Num, *_User_Key);
						// 게임에 시작하기위한 인원이 모두 들어왔으면 시작한다.
						if (Game_Max_User_Count == Connect_Player_List.size())
						{
							// 게임 시작.
							S2C_Start_Game();
							Is_Game_Playing.exchange(true);

							// 사용한데이터 해제.
							delete Msg_Packet.Packet;
							Msg_Packet.Packet = nullptr;
							continue;
						}
					}

					// 게임이 시작되지 않았다면 Player의 이동에 대한 처리는 하지않아도 된다.
					if (Is_Game_Playing.load() == false) { continue; }

					// 플레이어 이동에 대한 처리..
					if (C2S_Msg->Packet_Type == C2S_PLAYER_MOVE)
					{
						if (C2S_Player_Move(_Recv_Socket_Num, C2S_Msg) == false)
						{
							// 로직실패..
						}
					}

					// 사용한데이터 해제.
					delete Msg_Packet.Packet;
					Msg_Packet.Packet = nullptr;
				}
				Msg_Vec.clear();
			}
			Sleep(0);
		}
		Sleep(0);
	}
}

void GameManager::WorldUpdate_Loop()
{
	// 게임이 종료되었는가?
	while (!Is_Game_End.load())
	{
		// 게임이 시작되었으면
		while (Is_Game_Playing.load())
		{
			// 타이머를 한프레임에 한번씩 업데이트해줘야한다.
			m_PhysicsTimer->Update();

			// 정의된 시간이 지났다면..
			if (m_PhysicsTimer->Ready_Frame())
			{
				// 현재의 World 를 업데이트한다.
				PhysX_World_Update();
			}

			Sleep(0);
		}
	}
}

void GameManager::S2C_Start_Game()
{
	// 플레이어 리스트 자료를 만들기 위한 Vector ( 플레이어의 정보를 담음 )
	std::vector<flatbuffers::Offset<Eater::GameData::ClientStat>> _Client_Data;

	for (auto _Player : Connect_Player_List)
	{
		unsigned short _Player_Index = _Player.second;

		auto _Player_Position = Eater::GameData::Vec3(m_Player[_Player_Index]->m_Position.x, m_Player[_Player_Index]->m_Position.y, m_Player[_Player_Index]->m_Position.z);

		auto _Data = Eater::GameData::CreateClientStat(*Main_Builder, m_Player[_Player_Index]->m_Index, m_Player[_Player_Index]->m_Speed, m_Player[_Player_Index]->m_Type, &_Player_Position);

		_Client_Data.push_back(_Data);
	}

	// 클라이언트 유저에 따라 해당 플레이어 정보와 선택한 캐릭터에 맞는 정보를 보내준다.
	auto _Client_Start_Data = Eater::GameData::CreateStartInfoDirect(*Main_Builder, &_Client_Data);

	Main_Builder->Finish(_Client_Start_Data);

	_S2C_Msg_Main->Packet_Type = S2C_START_GAME;
	_S2C_Msg_Main->Packet_Size = Main_Builder->GetSize();
	memcpy_s(_S2C_Msg_Main->Packet_Buffer, Main_Builder->GetSize(), Main_Builder->GetBufferPointer(), Main_Builder->GetSize());

	m_Game_Server->Send(_S2C_Msg_Main);

	printf_s("[Eater Game Server] [S2C_START_GAME] 패킷 전송완료\n");

	Main_Builder->Clear();
}

void GameManager::S2C_World_Update()
{
	// 플레이어 리스트 자료를 만들기 위한 Vector ( 플레이어의 정보를 담음 )
	std::vector<flatbuffers::Offset<Eater::GameData::UserData>> _User_Data;
	std::vector<flatbuffers::Offset<Eater::GameData::EnemyData>> _Enemy_Data;
	std::vector<flatbuffers::Offset<Eater::GameData::ObjectData>> _Object_Data;

	// User들의 현재 데이터를 보내준다.
	for (auto _Player : Connect_Player_List)
	{
		unsigned short _Player_Index = _Player.second;

		// 한프레임 전의 데이터를 기록해두기 위해 현재 데이터를 따로 기록해둔다.
		if (m_Player[_Player_Index]->m_Position == m_Player[_Player_Index]->m_PhysData->WorldPosition)
		{
			// 만약 상태변화가 없다면 Idle 상태로 현재는 이동 말고는 없으니 이전과 포지션이 같다면 IDLE 이다.
			m_Player[_Player_Index]->m_Animation = ANIMATION_IDLE;
		}

		EnterCriticalSection(&g_DataIn_CS);

		unsigned short _Animation = m_Player[_Player_Index]->m_Animation;

		unsigned short _Sequence = m_Player[_Player_Index]->m_Processed_Sequence;

		auto _SimpleMath_Pos = m_Player[_Player_Index]->m_Position;

		auto _SimpleMath_Mov_Vec = m_Player[_Player_Index]->m_Mov_Vector;

		LeaveCriticalSection(&g_DataIn_CS);

		auto _Position = ConvertEaterVec3(_SimpleMath_Pos);

		auto _Mov_Vec = ConvertEaterVec3(_SimpleMath_Mov_Vec);

		auto _Data = Eater::GameData::CreateUserData(*Loop_Builder, m_Player[_Player_Index]->m_Index, _Animation, &_Position, &_Mov_Vec, _Sequence);

		_User_Data.push_back(_Data);

		// 이전 프레임 데이터 빽업
		m_Player[_Player_Index]->BackUp_PrevData();
	}

	for (auto _Current_Mana : m_Object_Data[m_Day])
	{
		// 마나석 인덱스
		auto _Obj_Index = _Current_Mana->m_Index;
		/// 임시 타입 지정
		auto _Obj_Type = EATER_OBJECT_MANA;
		// 마나석 위치
		auto _Obj_Pos = ConvertEaterVec3(_Current_Mana->m_Position);

		auto _Obj = Eater::GameData::CreateObjectData(*Loop_Builder, _Obj_Index, _Obj_Type, &_Obj_Pos);

		_Object_Data.push_back(_Obj);
	}

	for (auto _Current_Enemy : m_Enemy_Data[m_Day])
	{
		// 적 인덱스
		auto _Enemy_Index = _Current_Enemy->m_Index;
		/// 임시 타입 지정
		auto _Enemy_Type = EATER_ENEMY_NORMAL;
		// 마나석 위치
		auto _Enemy_Pos = ConvertEaterVec3(_Current_Enemy->m_Position);

		auto _Enemy = Eater::GameData::CreateEnemyData(*Loop_Builder, _Enemy_Index, _Enemy_Type, &_Enemy_Pos);

		_Enemy_Data.push_back(_Enemy);
	}

	// 현재 서버에서 구동되는 World에 대한 스냅샷 데이터..
	auto _World_Data = Eater::GameData::CreateWorldDataDirect(*Loop_Builder, &_User_Data, &_Enemy_Data, &_Object_Data);
	// 데이터 직렬화 완료.
	Loop_Builder->Finish(_World_Data);

	// 패킷 헤더를 붙여 보내준다.
	_S2C_Msg_Loop->Packet_Type = S2C_WORLD_UPDATE;
	_S2C_Msg_Loop->Packet_Size = Loop_Builder->GetSize();
	memcpy_s(_S2C_Msg_Loop->Packet_Buffer, Loop_Builder->GetSize(), Loop_Builder->GetBufferPointer(), Loop_Builder->GetSize());

	for (auto _Player_Socket : Connect_Player_List)
	{
		if (m_Game_Server->Send(_S2C_Msg_Loop, SEND_TYPE_TARGET, _Player_Socket.first) == false)
		{
			if (_Player_Socket.second == EATER_PLAYER_01)
			{
				printf_s("[Eater Game Server] 1번 플레이어 접속 종료\n");
			}
			if (_Player_Socket.second == EATER_PLAYER_02)
			{
				printf_s("[Eater Game Server] 2번 플레이어 접속 종료\n");
			}
			if (_Player_Socket.second == EATER_PLAYER_03)
			{
				printf_s("[Eater Game Server] 3번 플레이어 접속 종료\n");
			}
			if (_Player_Socket.second == EATER_PLAYER_04)
			{
				printf_s("[Eater Game Server] 4번 플레이어 접속 종료\n");
			}
			// 유저가 나갔으므로 삭제할 유저 리스트에 추가함.
			Out_User.push_back(_Player_Socket.first);
			break;
		}
	}

	// 나간 유저가 있다면 연결 유저 리스트에서 제거.
	if (!Out_User.empty())
	{
		EnterCriticalSection(&g_ListIn_CS);

		for (auto _User : Out_User)
		{

			Connect_Player_List.erase(_User);

		}

		LeaveCriticalSection(&g_ListIn_CS);

		Out_User.clear();
	}


	// 사용한 빌더 초기화.
	Loop_Builder->Clear();
}

void GameManager::S2C_Set_BaseData(S2C_Packet* _Packet)
{
	// 데이터 캐스팅.
	const uint8_t* Recv_Data_Ptr = (unsigned char*)_Packet->Packet_Buffer;

	const Eater::StartData::BaseData* Recv_Client_Move_Data = flatbuffers::GetRoot<Eater::StartData::BaseData>(Recv_Data_Ptr);

	auto _World_Data = Recv_Client_Move_Data->world_data();

	for (int i = 0; i < _World_Data->size(); i++)
	{
		// day
		auto _Day = _World_Data->Get(i)->day();

		// 적과 Object map 데이터 추가
		m_Object_Data.insert({ _Day,  std::vector<Object*>() });
		m_Enemy_Data.insert({ _Day,  std::vector<Enemy*>() });

		// manaspawn count
		auto _ManaSpawn_Count = _World_Data->Get(i)->manacount();

		// manaspawn
		auto _ManaSpawn = _World_Data->Get(i)->manaspawn();

		int _ManaSpawn_Size = _ManaSpawn->size();

		for (auto _Index : GetRandomCount(_ManaSpawn_Size, _ManaSpawn_Count))
		{
			auto _ManaPosition = _ManaSpawn->Get(_Index);

			// Mana Data
			auto _Mana_Data = new Object();

			_Mana_Data->m_Position = ConvertSimpleVec3(_ManaPosition->position());
			_Mana_Data->m_Index = _Index;
			_Mana_Data->m_PhysData->WorldPosition = _Mana_Data->m_Position;
			_Mana_Data->Create();

			m_Object_Data[_Day].push_back(_Mana_Data);
		}

		// enemyspawn
		auto _EnemySpawn = _World_Data->Get(i)->enemyspawn();

		for (int j = 0; j < _EnemySpawn->size(); j++)
		{
			// Enemy Data
			auto _Enemy_Data = new Enemy();

			auto _EnemyPosition = _EnemySpawn->Get(j);
			_Enemy_Data->m_Position = ConvertSimpleVec3(_EnemyPosition->position());
			_Enemy_Data->m_PhysData->WorldPosition = _Enemy_Data->m_Position;
			_Enemy_Data->Create();

			m_Enemy_Data[_Day].push_back(_Enemy_Data);
		}
	}
}

void GameManager::S2C_Set_MatchData(S2C_Packet* _Packet)
{
	// 데이터 캐스팅.
	const uint8_t* Recv_Data_Ptr = (unsigned char*)_Packet->Packet_Buffer;

	const Eater::StartData::MatchData* Recv_Client_Move_Data = flatbuffers::GetRoot<Eater::StartData::MatchData>(Recv_Data_Ptr);

	auto _User_List = Recv_Client_Move_Data->users();

	Game_Max_User_Count = _User_List->size();

	// 로비서버로 부터 받아온 플레이어 데이터.
	for (int i = 0; i < _User_List->size(); i++)
	{
		auto _User_Data = _User_List->Get(i);

		auto _Player_Index = _User_Data->player_index();
		auto _Key = _User_Data->key();
		auto _Char_Type = _User_Data->character_type();
		auto _Speed = _User_Data->speed();
		auto _Pos = ConvertSimpleVec3(_User_Data->position());

		Player* _Player = new Player();
		_Player->m_Index = _Player_Index;
		_Player->m_Type = _Char_Type;
		_Player->m_Speed = _Speed;
		_Player->m_PhysData->WorldPosition = _Pos;
		_Player->m_Position = _Pos;
		_Player->Create();
		m_Player.insert({ _Key , _Player });

		// 서버에서 이전 프레임을 백업 해놓기 위한 데이터.
		_Player->BackUp_PrevData();
	}
}

DirectX::SimpleMath::Vector3 GameManager::ConvertSimpleVec3(const Eater::GameData::Vec3* _Vec3)
{
	return DirectX::SimpleMath::Vector3(_Vec3->x(), _Vec3->y(), _Vec3->z());
}

DirectX::SimpleMath::Vector3 GameManager::ConvertSimpleVec3(const Eater::StartData::Vec3* _Vec3)
{
	return DirectX::SimpleMath::Vector3(_Vec3->x(), _Vec3->y(), _Vec3->z());
}

Eater::GameData::Vec3 GameManager::ConvertEaterVec3(DirectX::SimpleMath::Vector3& _Vec3)
{
	return Eater::GameData::Vec3(_Vec3.x, _Vec3.y, _Vec3.z);
}

DirectX::SimpleMath::Vector3 GameManager::ConvertMovVec3(const Eater::GameData::ClientMove* _Input)
{
	DirectX::SimpleMath::Vector3 _Mov_Vec;

	if (_Input->forward())
	{
		_Mov_Vec += {0, 0, -1};
	}

	if (_Input->back())
	{
		_Mov_Vec += {0, 0, 1};
	}

	if (_Input->right())
	{
		_Mov_Vec += {1, 0, 0};
	}

	if (_Input->left())
	{
		_Mov_Vec += {-1, 0, 0};
	}

	if (_Input->dash())
	{

	}

	if (_Input->skill1())
	{

	}

	if (_Input->skill2())
	{

	}

	return _Mov_Vec;
}

std::vector<int> GameManager::GetRandomCount(int _Total, int _GetCount)
{
	// 만약 총 개수 보다 뽑을 개수가 많다면 안됨..
	if (_GetCount > _Total)
	{
		assert(false);
		return std::vector<int>();
	}

	std::vector<bool> _Num_List;
	std::vector<int> _Result;

	_Num_List.resize(_Total);

	while (_Result.size() != _GetCount)
	{
		while (true)
		{
			int Random_index = rand() % _Total;
			if (_Num_List[Random_index] == false)
			{
				_Num_List[Random_index] = true;
				_Result.push_back(Random_index);
				break;
			}
		}
	}

	return _Result;
}

void GameManager::PhysX_World_Update()
{
	// 정의된 dtime 을 기준으로하여 물리 연산 업데이트.
	m_PhysXManager->Update(PHYSX_UPDATE_TIME);

	// Player Position Update
	EnterCriticalSection(&g_DataIn_CS);
	for (auto _Player : m_Player)
	{
		_Player.second->Update();
	}

	for (auto _Enemy : m_Enemy_Data[m_Day])
	{
		_Enemy->Update();
	}
	LeaveCriticalSection(&g_DataIn_CS);
}

BOOL GameManager::C2S_Player_Move(SOCKET _Socket_Num, C2S_Packet* _Packet)
{
	// 데이터 캐스팅.
	const uint8_t* Recv_Data_Ptr = (unsigned char*)_Packet->Packet_Buffer;

	const Eater::GameData::ClientMove* Recv_Client_Move_Data = flatbuffers::GetRoot<Eater::GameData::ClientMove>(Recv_Data_Ptr);

	// 현재 플레이어와 패킷의 시퀀스 번호.
	unsigned short Player_Index = Connect_Player_List[_Socket_Num];

	// 방향 벡터
	DirectX::SimpleMath::Vector3 Mov_Vector = ConvertMovVec3(Recv_Client_Move_Data);
	Mov_Vector *= m_Player[Player_Index]->m_Speed;

	m_Player[Player_Index]->m_Mov_Vector += Mov_Vector;

	EnterCriticalSection(&g_DataIn_CS);

	// 이전프레임의 이동 벡터가 만약 처리되지 않았을 경우에는 누적해준다.

	m_Player[Player_Index]->m_Sequence = Recv_Client_Move_Data->sequence();
	m_Player[Player_Index]->m_PhysData->SetVelocity(m_Player[Player_Index]->m_Mov_Vector.x, m_Player[Player_Index]->m_Mov_Vector.y, m_Player[Player_Index]->m_Mov_Vector.z);

	LeaveCriticalSection(&g_DataIn_CS);

	return true;
}

BOOL GameManager::C2S_Keep_Alive()
{
	return true;
}

BOOL GameManager::C2S_Player_Loading_Complete(SOCKET _Sokcet_Num, unsigned int _User_Key)
{
	// 현재 플레이어가 몇번유저인지 클라이언트에게 알려줌.
	unsigned short _Player_Index = m_Player[_User_Key]->m_Index;

	if (_Player_Index < EATER_PLAYER_01 || _Player_Index > EATER_PLAYER_04)
	{
		printf_s("[Eater Game Server][Key : %d] 허가되지 않은 유저입니다.\n", _User_Key);
		assert(false);
		return false;
	}

	EnterCriticalSection(&g_ListIn_CS);

	// 연결된 유저로 등록
	Connect_Player_List.insert({ _Sokcet_Num, _User_Key });

	LeaveCriticalSection(&g_ListIn_CS);

	_S2C_Msg_Main->Packet_Type = S2C_LOADING_COMPLETE_RES;
	_S2C_Msg_Main->Packet_Size = sizeof(_Player_Index);
	memcpy_s(_S2C_Msg_Main->Packet_Buffer, sizeof(_Player_Index), &_Player_Index, sizeof(_Player_Index));

	m_Game_Server->Send(_S2C_Msg_Main, SEND_TYPE_TARGET, _Sokcet_Num);

	printf_s("[Eater Game Server] 플레이어 접속\n");
	return true;
}
