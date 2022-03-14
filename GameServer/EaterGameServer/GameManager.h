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

// ���� �� Object
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
	// ���Ӽ����� ����� �÷��̾� ����
private:
	// ����� ������ ����
	std::map<SOCKET, unsigned int> Connect_Player_List;

private:
	// ����̵Ǹ� �����Ұǰ�?
	int Game_Max_User_Count = 0;
	/// �ӽ� ������ (���� ���° ���ΰ�?)
	int8_t m_Day = 1;

	// �κ� �������� �޾ƿ� �⺻ ���� ��������
	std::map<unsigned int, Player*> m_Player;						// ���� ���� key �� / �ش� �÷��̾� ����
	std::map<unsigned int, std::vector<Enemy*>>	m_Enemy_Data;		// ��¥ / �� ����
	std::map<unsigned int, std::vector<Object*>> m_Object_Data;		// ��¥ / Object ����

	// Physics �Ŵ���
	PhysXManager* m_PhysXManager = nullptr;

private:
	CRITICAL_SECTION g_DataIn_CS;
	CRITICAL_SECTION g_ListIn_CS;

	DHNetWorkAPI* m_Game_Server = nullptr;
	DHNetWorkAPI* m_Lobby_Client = nullptr;

	// Timer
	DHTimer* m_SequenceTimer = nullptr;
	DHTimer* m_PhysicsTimer = nullptr;

	// Loop Thread ��
	S2C_Packet* _S2C_Msg_Loop = nullptr;
	flatbuffers::FlatBufferBuilder* Loop_Builder = nullptr;
	// Main Logic  ��
	S2C_Packet* _S2C_Msg_Main = nullptr;
	flatbuffers::FlatBufferBuilder* Main_Builder = nullptr;

	// ������ ���Ῡ�� (����� ���� ���Ӽ����� �ʱ�ȭ�ϰ� �ٽ� ��밡���� ���·� �κ񼭹����� �˷���� �Ѵ�.)
	std::atomic<bool> Is_Game_End = false;
	// �κ񼭹��� ���� �ʱ� ���� �����Ϳ� ��Ī�� ������ ������ �� �޾Ҵ°�?
	std::atomic<bool> Is_Lobby_SetUp = false;
	// ���� ������ �������ΰ�? (2���̼� ������ ������ ���۵ǰ�, �Ѹ��� �������� ������ �����ϵ��� �ϱ����ؼ� ������ �÷��׷� �����Ͽ���.)
	std::atomic<bool> Is_Game_Playing = false;

	// Ŭ���̾�Ʈ���� �� �޼����� �޴� ����.
	std::vector<Network_Message> Msg_Vec;
	// �κ񿡼� �� �޼����� �޴� ����
	std::vector<Network_Message> Msg_Vec_Lobby;
	// �÷��̾ �����ߴ��� �˻��ϰ�, �ش������� �����ϱ� ���� ����
	std::vector<SOCKET> Out_User;

	// ���� �ֱ�� ������� �ϴ� ��Ŷ�� ���� ������.
	std::thread* g_Constant_Send_Thread = nullptr;
	// �κ� �����κ��� �޴� ��Ŷ�� ���� ������.
	std::thread* g_Lobby_Logic_Thread = nullptr;
	// ������ ������ ó���ϱ� ���� Thread
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

	// N �� �߿� �������� m ���� �̾� ��ȯ (�ߺ�����)
	std::vector<int> GetRandomCount(int _Total, int _GetCount);
	// PhysX �� ���� ������Ʈ
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

