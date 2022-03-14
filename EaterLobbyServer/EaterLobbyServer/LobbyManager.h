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
	int					Identifier = 0;		// 식별자
	unsigned short		User_State = 0;		// 현재 유저의 상태 
	std::string			User_ID;			// 아이디
};

class LobbyManager
{
private:
	// 현재 로비서버의 네트워크
	DHNetWorkAPI* m_Lobby_Server = nullptr;
	// 게임 서버들을 관리할 리스트 (port 번호와 socket 번호를 저장한다)
	std::map<int, SOCKET> m_Game_Server_List;
	// 로그인 서버와 통신 (서버 입장)
	SOCKET m_Login_Server = INVALID_SOCKET;
	// 초기 데이터 설정값 조회용 DB
	DHDB* m_DB = nullptr;
	// 클라이언트 리스트 (로그인 서버에서 접속가능한 클라이언트 정보를 받아오고, 이를 통해 클라이언트 소켓을 같이 저장)
	std::map<SOCKET, ClientInfo> m_Game_Client_List;	// 유저의 식별자와 접속한 Socket을 같이 저장해둔다.
	// 로그인 서버로 부터 받아온 클라이언트에 대한 정보 식별자와 아이디를 맵핑하여 저장한다.
	std::map<int, std::string> m_Client_Name_Table;
	// 유저의 key 값과 해당하는 플레이어의 게임 플레이 데이터를 같이 저장한다.
	std::map<int, Player_Info*> m_Player;
private:
	// 마나석들의 데이터
	ManaData* m_World_Mana = nullptr;
	// 적들의 데이터
	EnemyData* m_World_Enemy = nullptr;
	// 플레이어 데이터 type / data
	std::map<unsigned short, CharacterData*> m_Base_Character;
	// 캐릭터 데이터	type / data
	std::map<unsigned short, PlayerData*> m_Base_Player;
	// 게임 초기 셋팅값에 대한 flatbuffer 데이터
	flatbuffers::FlatBufferBuilder m_BaseData_Builder;
	// 클라이언트에서 온 메세지를 받는 벡터.
	std::vector<Network_Message> Msg_Vec;
	// 데이터를 전소하기 위한 패킷
	S2C_Packet* _S2C_Msg = nullptr;

public:
	LobbyManager();
	~LobbyManager();

	void Start();

private:
	// Eater 벡터로 변환
	Eater::StartData::Vec3 ConvertEaterVec3(DirectX::SimpleMath::Vector3& _Vec3);
	// DB 조회
	void Inquire_DB();
	// 초기 Start Data를 Flatbuffer용 데이터로 변환
	void Convert_FlatData();
	// 해당하는 서버로 매칭 된 유저들의 정보를 보내줌
	BOOL Send_MatchData(SOCKET _Target);
	// 해당하는 서버로 초기 데이터를 보내준다.
	BOOL Send_BaseData(SOCKET _Target);

};

