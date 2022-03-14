#pragma once

#include <atomic>
#include <map>
#include "SharedDataStruct.h"
#include "GameClientGameServerPacketDefine.h"
#include "GameServerLobbyPacketDefine.h"
#include "SimpleMath.h"

class DHNetWorkAPI;
class DHTimer;
struct S2C_Packet;

// PhysX Manager
class PhysXManager;

// 게임 내 Object
class Player;
class Object;
class Enemy;

namespace flatbuffers
{
	class FlatBufferBuilder;
}

namespace Eater
{
	namespace GameData
	{
		struct Vec3;
		struct ClientMove;
	}

	namespace StartData
	{
		struct Vec3;
	}
}

class GameManager
{
	// 게임서버에 연결된 플레이어 정보
private:
	// 연결된 유저들 정보
	std::map<SOCKET, unsigned int> Connect_Player_List;

private:
	// 몇명이되면 시작할건가?
	int Game_Max_User_Count = 0;
	/// 임시 데이터 (현재 몇번째 날인가?)
	int8_t m_Day = 1;

	// 로비 서버에서 받아온 기본 게임 설정정보
	std::map<unsigned int, Player*> m_Player;						// 유저 정보 key 값 / 해당 플레이어 정보
	std::map<unsigned int, std::vector<Enemy*>>	m_Enemy_Data;		// 날짜 / 적 정보
	std::map<unsigned int, std::vector<Object*>> m_Object_Data;		// 날짜 / Object 정보

	// Physics 매니저
	PhysXManager* m_PhysXManager = nullptr;

private:
	CRITICAL_SECTION g_DataIn_CS;
	CRITICAL_SECTION g_ListIn_CS;

	DHNetWorkAPI* m_Game_Server = nullptr;
	DHNetWorkAPI* m_Lobby_Client = nullptr;

	// Timer
	DHTimer* m_SequenceTimer = nullptr;
	DHTimer* m_PhysicsTimer = nullptr;

	// Loop Thread 용
	S2C_Packet* _S2C_Msg_Loop = nullptr;
	flatbuffers::FlatBufferBuilder* Loop_Builder = nullptr;
	// Main Logic  용
	S2C_Packet* _S2C_Msg_Main = nullptr;
	flatbuffers::FlatBufferBuilder* Main_Builder = nullptr;

	// 게임의 종료여부 (종료시 현재 게임서버를 초기화하고 다시 사용가능한 상태로 로비서버에게 알려줘야 한다.)
	std::atomic<bool> Is_Game_End = false;
	// 로비서버로 부터 초기 시작 데이터와 매칭된 유저의 정보를 다 받았는가?
	std::atomic<bool> Is_Lobby_SetUp = false;
	// 현재 게임이 진행중인가? (2명이서 진행한 게임이 시작되고, 한명이 나가더라도 진행은 가능하도록 하기위해서 별도의 플래그로 지정하였다.)
	std::atomic<bool> Is_Game_Playing = false;

	// 클라이언트에서 온 메세지를 받는 벡터.
	std::vector<Network_Message> Msg_Vec;
	// 로비에서 온 메세지를 받는 벡터
	std::vector<Network_Message> Msg_Vec_Lobby;
	// 플레이어가 종료했는지 검사하고, 해당유저를 삭제하기 위한 벡터
	std::vector<SOCKET> Out_User;

	// 일정 주기로 보내줘야 하는 패킷에 대한 쓰레드.
	std::thread* g_Constant_Send_Thread = nullptr;
	// 로비 서버로부터 받는 패킷에 대한 쓰레드.
	std::thread* g_Lobby_Logic_Thread = nullptr;
	// 서버의 물리를 처리하기 위한 Thread
	std::thread* g_PhysX_World_Thread = nullptr;

public:
	GameManager();
	~GameManager();

	void S2C_Loop();
	void Lobby_Loop();
	void ServerRecv_Loop();
	void WorldUpdate_Loop();

private:
	// Eater Vec3 -> SimpleMath Vec3
	DirectX::SimpleMath::Vector3 ConvertSimpleVec3(const Eater::GameData::Vec3* _Vec3);
	DirectX::SimpleMath::Vector3 ConvertSimpleVec3(const Eater::StartData::Vec3* _Vec3);
	// SimpleMath Vec3 -> Eater Vec3
	Eater::GameData::Vec3 ConvertEaterVec3(DirectX::SimpleMath::Vector3& _Vec3);
	// Bool Input -> Mov Vector3
	DirectX::SimpleMath::Vector3 ConvertMovVec3(const Eater::GameData::ClientMove* _Input);

	// N 개 중에 랜덤으로 m 개를 뽑아 반환 (중복없이)
	std::vector<int> GetRandomCount(int _Total, int _GetCount);
	// PhysX 의 월드 업데이트
	void PhysX_World_Update();

	// S2C Function
public:
	void	S2C_Start_Game();
	void	S2C_World_Update();
	void	S2C_Set_BaseData(S2C_Packet* _Packet);
	void	S2C_Set_MatchData(S2C_Packet* _Packet);

	// C2S Function
public:
	BOOL	C2S_Player_Move(SOCKET _Socket_Num, C2S_Packet* _Packet);
	BOOL	C2S_Keep_Alive();
	BOOL	C2S_Player_Loading_Complete(SOCKET _Sokcet_Num, unsigned int _User_Key);
};

