#pragma once

/*
	2021/12/23 15:58 - CDH
	
	< �������� >
		1. ������ ������ ������ ������ ���� ���� ���̺�(���̵� , ���Ϲ�ȣ) �� �����Ѵ�.
		2. ���� ������ ���ؼ� ģ�� ����� DB���� ��ȸ�Ͽ� ���� ����Ѵ�.
		3. �ش� ������ �����ٸ� ���Ϲ�ȣ�� invalid �������� �ٲ۴�.
		4. �����ð����� �ش� ���̺� ������ �����Ѵ�. (���� ����)
*/


#include <atomic>
#include <iostream>
#include "PortIPDefine.h"
#include "SharedDataStruct.h"
#include "LauncherLoginPacketDefine.h"
#include "LoginLauncher_generated.h"

class DHNetWorkAPI;
class DHDB;
class DHTimer;

struct User_Data
{
	// Socket��ȣ�� INVAILD �� ���� Offline �̶�� ���̴�.
	std::atomic<int>			Socket_Num = INVALID_SOCKET;
	std::atomic<unsigned int>	User_State = USER_OFFLINE;
	std::set<std::string> Friend_List;
	std::set<std::string> Friend_Request_List;
};

class LoginManager
{
private:
	CRITICAL_SECTION g_CS;

private:
	/// ��Ʈ��ũ ���� �� ���� ����
	// ���� ��ó�� ������ ��Ʈ��ũ (������ ����)
	DHNetWorkAPI* m_Connect_Launcher = nullptr;
	// ���� �ֱ�� ������ ó���ϱ� ���� ������
	std::thread* m_Keep_Alive_Thread = nullptr;

	/// DB ����
	DHDB* m_DB = nullptr;

	/// Timer
	DHTimer* m_Timer = nullptr;

	/// ������ �ʿ��� ����
	// Recv ���� �����
	std::vector<Network_Message> Launcher_Msg_Vec;
	std::vector<Network_Message> Lobby_Msg_Vec;
	// Send/Recv �� ����ü ����
	// Launcher
	flatbuffers::FlatBufferBuilder Launcher_Builder;
	C2S_Packet* Launcher_Recv_Packet = new C2S_Packet();
	S2C_Packet Launcher_Send_Packet;
	// SendUserState
	flatbuffers::FlatBufferBuilder KeepAlive_Builder;
	C2S_Packet* KeepAlive_Recv_Packet = new C2S_Packet();
	S2C_Packet KeepAlive_Send_Packet;

	/// ���� ��������
	// ����� Ŭ���̾�Ʈ�� �ľ��ϱ� ����. (���Ͽ� �ش��ϴ� �α��ε� ���̵��� �����صд�)
	std::map<std::string, User_Data*> Logged_In_User_Data;

private:
	void SendUserState();
	void LoginCertify(SOCKET _Socket_Num, C2S_Packet* _C2S_Msg);
	void CreateUser(SOCKET _Socket_Num, C2S_Packet* _C2S_Msg);
	void AddFriend(SOCKET _Socket_Num, C2S_Packet* _C2S_Msg);
	void AcceptFriend(SOCKET _Socket_Num, C2S_Packet* _C2S_Msg);
	void StartGame(SOCKET _Socket_Num, C2S_Packet* _C2S_Msg);
	User_Data* LoginUserCheck(SOCKET _Socket_Num, std::string& _Name);

public:
	LoginManager();
	~LoginManager();

	void LauncherSideLogic();
};

