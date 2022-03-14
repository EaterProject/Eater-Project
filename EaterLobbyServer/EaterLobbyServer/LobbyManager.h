#pragma once

#include "GameServerLobbyPacketDefine.h"
#include "SharedDataStruct.h"
#include "StartData_generated.h"
#include "SimpleMath.h"

class DHNetWorkAPI;
class DHTimer;
class DHDB;
struct S2C_Packet;
struct ManaData;
struct EnemyData;
struct CharacterData;
struct PlayerData;
struct Player_Info;

struct ClientInfo
{
	int					Identifier = 0;		// �ĺ���
	unsigned short		User_State = 0;		// ���� ������ ���� 
	std::string			User_ID;			// ���̵�
};

class LobbyManager
{
private:
	// ���� �κ񼭹��� ��Ʈ��ũ
	DHNetWorkAPI* m_Lobby_Server = nullptr;
	// ���� �������� ������ ����Ʈ (port ��ȣ�� socket ��ȣ�� �����Ѵ�)
	std::map<int, SOCKET> m_Game_Server_List;
	// �α��� ������ ��� (���� ����)
	SOCKET m_Login_Server = INVALID_SOCKET;
	// �ʱ� ������ ������ ��ȸ�� DB
	DHDB* m_DB = nullptr;
	// Ŭ���̾�Ʈ ����Ʈ (�α��� �������� ���Ӱ����� Ŭ���̾�Ʈ ������ �޾ƿ���, �̸� ���� Ŭ���̾�Ʈ ������ ���� ����)
	std::map<SOCKET, ClientInfo> m_Game_Client_List;	// ������ �ĺ��ڿ� ������ Socket�� ���� �����صд�.
	// �α��� ������ ���� �޾ƿ� Ŭ���̾�Ʈ�� ���� ���� �ĺ��ڿ� ���̵� �����Ͽ� �����Ѵ�.
	std::map<int, std::string> m_Client_Name_Table;
	// ������ key ���� �ش��ϴ� �÷��̾��� ���� �÷��� �����͸� ���� �����Ѵ�.
	std::map<int, Player_Info*> m_Player;
private:
	// ���������� ������
	ManaData* m_World_Mana = nullptr;
	// ������ ������
	EnemyData* m_World_Enemy = nullptr;
	// �÷��̾� ������ type / data
	std::map<unsigned short, CharacterData*> m_Base_Character;
	// ĳ���� ������	type / data
	std::map<unsigned short, PlayerData*> m_Base_Player;
	// ���� �ʱ� ���ð��� ���� flatbuffer ������
	flatbuffers::FlatBufferBuilder m_BaseData_Builder;
	// Ŭ���̾�Ʈ���� �� �޼����� �޴� ����.
	std::vector<Network_Message> Msg_Vec;
	// �����͸� �����ϱ� ���� ��Ŷ
	S2C_Packet* _S2C_Msg = nullptr;

public:
	LobbyManager();
	~LobbyManager();

	void Start();

private:
	// Eater ���ͷ� ��ȯ
	Eater::StartData::Vec3 ConvertEaterVec3(DirectX::SimpleMath::Vector3& _Vec3);
	// DB ��ȸ
	void Inquire_DB();
	// �ʱ� Start Data�� Flatbuffer�� �����ͷ� ��ȯ
	void Convert_FlatData();
	// �ش��ϴ� ������ ��Ī �� �������� ������ ������
	BOOL Send_MatchData(SOCKET _Target);
	// �ش��ϴ� ������ �ʱ� �����͸� �����ش�.
	BOOL Send_BaseData(SOCKET _Target);

};

