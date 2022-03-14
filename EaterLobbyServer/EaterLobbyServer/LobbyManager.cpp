#include "LobbyManager.h"
#include "PortIPDefine.h"
#include "DHNetWorkAPI.h"
#include "DHDB.h"
#include "GameStructDefine.h"

LobbyManager::LobbyManager()
{
	// 네트워크 생성
	m_Lobby_Server = new DHNetWorkAPI();
	m_Lobby_Server->Initialize(DHNetWork_Name::DHNet, DHDEBUG_DETAIL);
	m_Lobby_Server->Accept(LOBBY_SERVER_PORT, 10, 10);

	// 클라이언트로 메세지를 보내기 위한 패킷정의
	_S2C_Msg = new S2C_Packet();

	// DB 연결
	m_DB = new DHDB();
	m_DB->ConnectDB(SERVER_CONNECT_IP, "CDH", "ehxk2Rnfwoa!", "GAME", DATABASE_PORT);

	// DB에서 초기 시작 데이터를 조회한다.
	Inquire_DB();

	// 임시 데이터 추가
	m_Player.insert({ 0, new Player_Info{ m_Base_Character[EATER_CHARACTER_01], m_Base_Player[EATER_PLAYER_01] } });
	m_Player.insert({ 1, new Player_Info{ m_Base_Character[EATER_CHARACTER_02], m_Base_Player[EATER_PLAYER_02] } });
	// 초기 데이터를 flat 버퍼용 데이터로 변환
	Convert_FlatData();
}

LobbyManager::~LobbyManager()
{

}

void LobbyManager::Start()
{
	while (true)
	{
		// 주기적으로 받은 패킷이 있으면 확인한다.
		if (m_Lobby_Server->Recv(Msg_Vec))
		{
			// 클라이언트로부터 받은 데이터들로 로직을 처리..
			for (auto& Msg_Packet : Msg_Vec)
			{
				// 클라이언트의 소켓 번호
				SOCKET _Recv_Socket_Num = Msg_Packet.Socket;
				// 클라이언트가 보낸 메세지
				C2S_Packet* C2S_Msg = static_cast<C2S_Packet*>(Msg_Packet.Packet);

				// 잘못 된 데이터가 들어온 경우.
				if (C2S_Msg == NULL)
				{
					continue;
				}

				// 게임서버가 새로 준비가 되었음을 알린다.
				if (C2S_Msg->Packet_Type == C2S_CONNECT_PORT_REQ)
				{
					bool Is_Already_In = false;
					// 새로 들어온 게임서버에게 기본적인 게임 데이터를 전송
					if (Send_BaseData(_Recv_Socket_Num))
					{
						// 게임서버에서 접속할 때 사용한 Port 번호
						unsigned short* Recv_Port = (unsigned short*)C2S_Msg->Packet_Buffer;

						// 만약 포트번호가 같은 서버가 접속했다? => 기존 서버가 연결을 끊고 다른 Socket 번호로 접속함.
						for (auto& _Game_Server : m_Game_Server_List)
						{
							if (_Game_Server.first == *Recv_Port)
							{
								_Game_Server.second = _Recv_Socket_Num;
								Is_Already_In = true;
								break;
							}
						}

						if (!Is_Already_In)
						{
							// 접속한 게임 서버 리스트에 등록.
							m_Game_Server_List.insert({ *Recv_Port, _Recv_Socket_Num });
						}

					}
					// 임시로 매칭 데이터도 보내준다. 추후 수정
					Send_MatchData(_Recv_Socket_Num);
				}

				// 사용한데이터 해제.
				delete Msg_Packet.Packet;
				Msg_Packet.Packet = nullptr;
			}
			Msg_Vec.clear();
		}

		Sleep(0);
	}
}

Eater::StartData::Vec3 LobbyManager::ConvertEaterVec3(DirectX::SimpleMath::Vector3& _Vec3)
{
	return Eater::StartData::Vec3(_Vec3.x, _Vec3.y, _Vec3.z);
}

void LobbyManager::Inquire_DB()
{
	// 마나석 정보 조회
	std::vector<std::string> _Result_Vec;
	int Field_Count = 0;
	m_DB->QueryDB("SELECT * FROM DAY_1_POSITION_MANA", &Field_Count, _Result_Vec);
	m_World_Mana = new ManaData();

	int _Result_Vec_Index = 0;
	for (int i = 0; i < (_Result_Vec.size() / Field_Count); i++)
	{
		_Result_Vec_Index = i * Field_Count;
		m_World_Mana->Mana_Position_Day_1.insert({ stoi(_Result_Vec[_Result_Vec_Index]) , { stof(_Result_Vec[_Result_Vec_Index + 1]), stof(_Result_Vec[_Result_Vec_Index + 2]), stof(_Result_Vec[_Result_Vec_Index + 3]) } });
	}

	// 적 정보 조회
	_Result_Vec.clear();
	m_DB->QueryDB("SELECT * FROM DAY_1_POSITION_ENEMY", &Field_Count, _Result_Vec);
	m_World_Enemy = new EnemyData();

	_Result_Vec_Index = 0;
	for (int i = 0; i < (_Result_Vec.size() / Field_Count); i++)
	{
		_Result_Vec_Index = i * Field_Count;
		m_World_Enemy->Enemy_Position_Day_1.insert({ stoi(_Result_Vec[_Result_Vec_Index]) , { stof(_Result_Vec[_Result_Vec_Index + 1]), stof(_Result_Vec[_Result_Vec_Index + 2]), stof(_Result_Vec[_Result_Vec_Index + 3]) } });
	}

	// 마나석과 적 생성 개수 조회
	_Result_Vec.clear();
	m_DB->QueryDB("SELECT * FROM CREATE_ATTRIBUTE", &Field_Count, _Result_Vec);

	_Result_Vec_Index = 0;
	for (int i = 0; i < (_Result_Vec.size() / Field_Count); i++)
	{
		_Result_Vec_Index = i * Field_Count;
		// 생성일
		int _Day = stoi(_Result_Vec[_Result_Vec_Index]);
		// 생성일 / 적 마릿수
		m_World_Enemy->Enemy_Spawn_Count.insert({ _Day, stoi(_Result_Vec[_Result_Vec_Index + 1]) });
		// 생성일 / 마나석 개수
		m_World_Mana->Mana_Spawn_Count.insert({ _Day, stoi(_Result_Vec[_Result_Vec_Index + 2]) });
	}

	// 플레이어 데이터 조회 및 저장
	_Result_Vec.clear();
	m_DB->QueryDB("SELECT * FROM PLAYER_ATTRIBUTE", &Field_Count, _Result_Vec);

	_Result_Vec_Index = 0;
	for (int i = 0; i < (_Result_Vec.size() / Field_Count); i++)
	{
		_Result_Vec_Index = i * Field_Count;
		// 플레이어 번호 (1P,2P ..)
		auto _Player_Index = stoi(_Result_Vec[_Result_Vec_Index]);
		// 플레이어 정보
		PlayerData* _Player_Data = new PlayerData;
		_Player_Data->Type = _Player_Index;
		_Player_Data->Position = { stof(_Result_Vec[_Result_Vec_Index + 1]), stof(_Result_Vec[_Result_Vec_Index + 2]), stof(_Result_Vec[_Result_Vec_Index + 3]) };
		// key / data
		m_Base_Player.insert({ _Player_Index, _Player_Data });
	}

	// 캐릭터 정보 조회 및 저장
	_Result_Vec.clear();
	m_DB->QueryDB("SELECT * FROM CHARACTER_ATTRIBUTE", &Field_Count, _Result_Vec);

	_Result_Vec_Index = 0;
	for (int i = 0; i < (_Result_Vec.size() / Field_Count); i++)
	{
		_Result_Vec_Index = i * Field_Count;
		// 캐릭터 번호
		auto _Character_Index = stoi(_Result_Vec[_Result_Vec_Index]);
		// 캐릭터의 정보
		CharacterData* _Character_Data = new CharacterData;
		_Character_Data->Type = _Character_Index;
		_Character_Data->Speed = stof(_Result_Vec[_Result_Vec_Index + 1]);
		// key / data
		m_Base_Character.insert({ _Character_Index, _Character_Data });
	}
}

void LobbyManager::Convert_FlatData()
{
	// 초기 시작 데이터를 만들기 위한 Vector ( 플레이어의 정보를 담음 )
	std::vector<flatbuffers::Offset<Eater::StartData::ManaPosition>>	_Mana_Pos_List;
	std::vector<flatbuffers::Offset<Eater::StartData::EnemyPosition>>	_Enemy_Pos_List;
	std::vector<flatbuffers::Offset<Eater::StartData::GameData>>		_Game_Data_List;

	// 마나석의 포지션을 넣는다. 
	for (auto _Mana_Data : m_World_Mana->Mana_Position_Day_1)
	{
		auto _Mana_Pos = ConvertEaterVec3(_Mana_Data.second);
		
		auto _Data = Eater::StartData::CreateManaPosition(m_BaseData_Builder, _Mana_Data.first, &_Mana_Pos);

		_Mana_Pos_List.push_back(_Data);
	}
	//... 추후 Day2,3,4 ...

	// 적의 포지션을 넣는다.
	for (auto _Enemy_Data : m_World_Enemy->Enemy_Position_Day_1)
	{
		auto _Enemy_Pos = ConvertEaterVec3(_Enemy_Data.second);
		
		// 현재 적 종류 type 은 존재하지않으므로, 0으로 박아둠.
		auto _Data = Eater::StartData::CreateEnemyPosition(m_BaseData_Builder, _Enemy_Data.first, 0, &_Enemy_Pos);

		_Enemy_Pos_List.push_back(_Data);
	}
	//... 추후 Day2,3,4 ...
	
	// 게임 데이터 생성 (임시로 첫날에 10개 생성..)
	auto _Game_Data = Eater::StartData::CreateGameDataDirect(m_BaseData_Builder, 1, 10, &_Mana_Pos_List, &_Enemy_Pos_List);
	//... 추후 Day2,3,4 ...

	// 초기 게임정보 추가.
	_Game_Data_List.push_back(_Game_Data);

	auto _Final_BaseData = Eater::StartData::CreateBaseDataDirect(m_BaseData_Builder, &_Game_Data_List);

	m_BaseData_Builder.Finish(_Final_BaseData);
}

BOOL LobbyManager::Send_MatchData(SOCKET _Target)
{
	flatbuffers::FlatBufferBuilder m_Builder;

	std::vector<flatbuffers::Offset<Eater::StartData::UserData>> _User_Data_List;
	
	// 매칭된 유저들에 대해서 데이터를 넣어 보내준다.
	for (int i = 0; i < 2; i++)
	{
		// 임시로 Key값 설정
		auto _User_Key = i;
		auto _Char_Type = m_Player[i]->Character->Type;
		auto _Speed = m_Player[i]->Character->Speed;
		auto _Player_Index = m_Player[i]->Player->Type;
		auto _Start_Pos = ConvertEaterVec3(m_Player[i]->Player->Position);

		auto _User_Data = Eater::StartData::CreateUserData(m_Builder, i, _Char_Type, _Player_Index, &_Start_Pos, _Speed);
		_User_Data_List.push_back(_User_Data);
	}

	auto _MatchData = Eater::StartData::CreateMatchDataDirect(m_Builder, &_User_Data_List);

	m_Builder.Finish(_MatchData);

	_S2C_Msg->Packet_Type = S2C_MATCHING_SUCCESS;
	_S2C_Msg->Packet_Size = m_Builder.GetSize();
	memcpy_s(_S2C_Msg->Packet_Buffer, m_Builder.GetSize(), m_Builder.GetBufferPointer(), m_Builder.GetSize());

	// 해당 소켓에게 전송
	return m_Lobby_Server->Send(_S2C_Msg, SEND_TYPE_TARGET, _Target);
}

BOOL LobbyManager::Send_BaseData(SOCKET _Target)
{
	_S2C_Msg->Packet_Type = S2C_CONNECT_PORT_RES;
	_S2C_Msg->Packet_Size = m_BaseData_Builder.GetSize();
	memcpy_s(_S2C_Msg->Packet_Buffer, m_BaseData_Builder.GetSize(), m_BaseData_Builder.GetBufferPointer(), m_BaseData_Builder.GetSize());

	// 해당 소켓에게 전송
	return m_Lobby_Server->Send(_S2C_Msg, SEND_TYPE_TARGET, _Target);
}
