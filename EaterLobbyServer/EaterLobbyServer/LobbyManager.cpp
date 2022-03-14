#include "LobbyManager.h"
#include "PortIPDefine.h"
#include "DHNetWorkAPI.h"
#include "DHDB.h"
#include "GameStructDefine.h"

LobbyManager::LobbyManager()
{
	// ��Ʈ��ũ ����
	m_Lobby_Server = new DHNetWorkAPI();
	m_Lobby_Server->Initialize(DHNetWork_Name::DHNet, DHDEBUG_DETAIL);
	m_Lobby_Server->Accept(LOBBY_SERVER_PORT, 10, 10);

	// Ŭ���̾�Ʈ�� �޼����� ������ ���� ��Ŷ����
	_S2C_Msg = new S2C_Packet();

	// DB ����
	m_DB = new DHDB();
	m_DB->ConnectDB(SERVER_CONNECT_IP, "CDH", "ehxk2Rnfwoa!", "GAME", DATABASE_PORT);

	// DB���� �ʱ� ���� �����͸� ��ȸ�Ѵ�.
	Inquire_DB();

	// �ӽ� ������ �߰�
	m_Player.insert({ 0, new Player_Info{ m_Base_Character[EATER_CHARACTER_01], m_Base_Player[EATER_PLAYER_01] } });
	m_Player.insert({ 1, new Player_Info{ m_Base_Character[EATER_CHARACTER_02], m_Base_Player[EATER_PLAYER_02] } });
	// �ʱ� �����͸� flat ���ۿ� �����ͷ� ��ȯ
	Convert_FlatData();
}

LobbyManager::~LobbyManager()
{

}

void LobbyManager::Start()
{
	while (true)
	{
		// �ֱ������� ���� ��Ŷ�� ������ Ȯ���Ѵ�.
		if (m_Lobby_Server->Recv(Msg_Vec))
		{
			// Ŭ���̾�Ʈ�κ��� ���� �����͵�� ������ ó��..
			for (auto& Msg_Packet : Msg_Vec)
			{
				// Ŭ���̾�Ʈ�� ���� ��ȣ
				SOCKET _Recv_Socket_Num = Msg_Packet.Socket;
				// Ŭ���̾�Ʈ�� ���� �޼���
				C2S_Packet* C2S_Msg = static_cast<C2S_Packet*>(Msg_Packet.Packet);

				// �߸� �� �����Ͱ� ���� ���.
				if (C2S_Msg == NULL)
				{
					continue;
				}

				// ���Ӽ����� ���� �غ� �Ǿ����� �˸���.
				if (C2S_Msg->Packet_Type == C2S_CONNECT_PORT_REQ)
				{
					bool Is_Already_In = false;
					// ���� ���� ���Ӽ������� �⺻���� ���� �����͸� ����
					if (Send_BaseData(_Recv_Socket_Num))
					{
						// ���Ӽ������� ������ �� ����� Port ��ȣ
						unsigned short* Recv_Port = (unsigned short*)C2S_Msg->Packet_Buffer;

						// ���� ��Ʈ��ȣ�� ���� ������ �����ߴ�? => ���� ������ ������ ���� �ٸ� Socket ��ȣ�� ������.
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
							// ������ ���� ���� ����Ʈ�� ���.
							m_Game_Server_List.insert({ *Recv_Port, _Recv_Socket_Num });
						}

					}
					// �ӽ÷� ��Ī �����͵� �����ش�. ���� ����
					Send_MatchData(_Recv_Socket_Num);
				}

				// ����ѵ����� ����.
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
	// ������ ���� ��ȸ
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

	// �� ���� ��ȸ
	_Result_Vec.clear();
	m_DB->QueryDB("SELECT * FROM DAY_1_POSITION_ENEMY", &Field_Count, _Result_Vec);
	m_World_Enemy = new EnemyData();

	_Result_Vec_Index = 0;
	for (int i = 0; i < (_Result_Vec.size() / Field_Count); i++)
	{
		_Result_Vec_Index = i * Field_Count;
		m_World_Enemy->Enemy_Position_Day_1.insert({ stoi(_Result_Vec[_Result_Vec_Index]) , { stof(_Result_Vec[_Result_Vec_Index + 1]), stof(_Result_Vec[_Result_Vec_Index + 2]), stof(_Result_Vec[_Result_Vec_Index + 3]) } });
	}

	// �������� �� ���� ���� ��ȸ
	_Result_Vec.clear();
	m_DB->QueryDB("SELECT * FROM CREATE_ATTRIBUTE", &Field_Count, _Result_Vec);

	_Result_Vec_Index = 0;
	for (int i = 0; i < (_Result_Vec.size() / Field_Count); i++)
	{
		_Result_Vec_Index = i * Field_Count;
		// ������
		int _Day = stoi(_Result_Vec[_Result_Vec_Index]);
		// ������ / �� ������
		m_World_Enemy->Enemy_Spawn_Count.insert({ _Day, stoi(_Result_Vec[_Result_Vec_Index + 1]) });
		// ������ / ������ ����
		m_World_Mana->Mana_Spawn_Count.insert({ _Day, stoi(_Result_Vec[_Result_Vec_Index + 2]) });
	}

	// �÷��̾� ������ ��ȸ �� ����
	_Result_Vec.clear();
	m_DB->QueryDB("SELECT * FROM PLAYER_ATTRIBUTE", &Field_Count, _Result_Vec);

	_Result_Vec_Index = 0;
	for (int i = 0; i < (_Result_Vec.size() / Field_Count); i++)
	{
		_Result_Vec_Index = i * Field_Count;
		// �÷��̾� ��ȣ (1P,2P ..)
		auto _Player_Index = stoi(_Result_Vec[_Result_Vec_Index]);
		// �÷��̾� ����
		PlayerData* _Player_Data = new PlayerData;
		_Player_Data->Type = _Player_Index;
		_Player_Data->Position = { stof(_Result_Vec[_Result_Vec_Index + 1]), stof(_Result_Vec[_Result_Vec_Index + 2]), stof(_Result_Vec[_Result_Vec_Index + 3]) };
		// key / data
		m_Base_Player.insert({ _Player_Index, _Player_Data });
	}

	// ĳ���� ���� ��ȸ �� ����
	_Result_Vec.clear();
	m_DB->QueryDB("SELECT * FROM CHARACTER_ATTRIBUTE", &Field_Count, _Result_Vec);

	_Result_Vec_Index = 0;
	for (int i = 0; i < (_Result_Vec.size() / Field_Count); i++)
	{
		_Result_Vec_Index = i * Field_Count;
		// ĳ���� ��ȣ
		auto _Character_Index = stoi(_Result_Vec[_Result_Vec_Index]);
		// ĳ������ ����
		CharacterData* _Character_Data = new CharacterData;
		_Character_Data->Type = _Character_Index;
		_Character_Data->Speed = stof(_Result_Vec[_Result_Vec_Index + 1]);
		// key / data
		m_Base_Character.insert({ _Character_Index, _Character_Data });
	}
}

void LobbyManager::Convert_FlatData()
{
	// �ʱ� ���� �����͸� ����� ���� Vector ( �÷��̾��� ������ ���� )
	std::vector<flatbuffers::Offset<Eater::StartData::ManaPosition>>	_Mana_Pos_List;
	std::vector<flatbuffers::Offset<Eater::StartData::EnemyPosition>>	_Enemy_Pos_List;
	std::vector<flatbuffers::Offset<Eater::StartData::GameData>>		_Game_Data_List;

	// �������� �������� �ִ´�. 
	for (auto _Mana_Data : m_World_Mana->Mana_Position_Day_1)
	{
		auto _Mana_Pos = ConvertEaterVec3(_Mana_Data.second);
		
		auto _Data = Eater::StartData::CreateManaPosition(m_BaseData_Builder, _Mana_Data.first, &_Mana_Pos);

		_Mana_Pos_List.push_back(_Data);
	}
	//... ���� Day2,3,4 ...

	// ���� �������� �ִ´�.
	for (auto _Enemy_Data : m_World_Enemy->Enemy_Position_Day_1)
	{
		auto _Enemy_Pos = ConvertEaterVec3(_Enemy_Data.second);
		
		// ���� �� ���� type �� �������������Ƿ�, 0���� �ھƵ�.
		auto _Data = Eater::StartData::CreateEnemyPosition(m_BaseData_Builder, _Enemy_Data.first, 0, &_Enemy_Pos);

		_Enemy_Pos_List.push_back(_Data);
	}
	//... ���� Day2,3,4 ...
	
	// ���� ������ ���� (�ӽ÷� ù���� 10�� ����..)
	auto _Game_Data = Eater::StartData::CreateGameDataDirect(m_BaseData_Builder, 1, 10, &_Mana_Pos_List, &_Enemy_Pos_List);
	//... ���� Day2,3,4 ...

	// �ʱ� �������� �߰�.
	_Game_Data_List.push_back(_Game_Data);

	auto _Final_BaseData = Eater::StartData::CreateBaseDataDirect(m_BaseData_Builder, &_Game_Data_List);

	m_BaseData_Builder.Finish(_Final_BaseData);
}

BOOL LobbyManager::Send_MatchData(SOCKET _Target)
{
	flatbuffers::FlatBufferBuilder m_Builder;

	std::vector<flatbuffers::Offset<Eater::StartData::UserData>> _User_Data_List;
	
	// ��Ī�� �����鿡 ���ؼ� �����͸� �־� �����ش�.
	for (int i = 0; i < 2; i++)
	{
		// �ӽ÷� Key�� ����
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

	// �ش� ���Ͽ��� ����
	return m_Lobby_Server->Send(_S2C_Msg, SEND_TYPE_TARGET, _Target);
}

BOOL LobbyManager::Send_BaseData(SOCKET _Target)
{
	_S2C_Msg->Packet_Type = S2C_CONNECT_PORT_RES;
	_S2C_Msg->Packet_Size = m_BaseData_Builder.GetSize();
	memcpy_s(_S2C_Msg->Packet_Buffer, m_BaseData_Builder.GetSize(), m_BaseData_Builder.GetBufferPointer(), m_BaseData_Builder.GetSize());

	// �ش� ���Ͽ��� ����
	return m_Lobby_Server->Send(_S2C_Msg, SEND_TYPE_TARGET, _Target);
}
