#pragma once

#ifdef DHNETWORK_EXPORTS
#define NETWORK_DLL __declspec(dllexport)
#else
#define NETWORK_DLL __declspec(dllimport)

#ifdef _DEBUG
#pragma comment(lib,"DHNetWork_x64d")
#else
#pragma comment(lib,"DHNetWork_x64r")
#endif

#endif

#include "DHNetWorkAPIDefine.h"
// 현재 네트워크를 생성했을 때, 서버인지 클라이언트인지?
#define TYPE_NONSET				0b0000
#define TYPE_DHSERVER			0b0001
#define TYPE_DHCLIENT			0b0010

class NetWorkBase;

class NETWORK_DLL DHNetWork : public DHNetworkAPIBase
{

private:
	/// 클라이언트와 서버셋팅.
	NetWorkBase* m_NetWork = nullptr;
	unsigned short g_Debug_Option = DHDEBUG_NONE;
	int Current_Type = TYPE_NONSET;

private:
	void PrintTypeErrMessage();

public:
	DHNetWork();
	virtual ~DHNetWork();

	/// 초기화
	virtual BOOL	Initialize(unsigned short _Debug_Option = DHDEBUG_NONE) override;
	/// 서버일 경우 Accept 호출
	virtual BOOL	Accept(unsigned short _Port, unsigned short _Max_User_Count, unsigned short _Work_Thread_Count) override;
	/// 클라일 경우 Connect 호출
	virtual BOOL	Connect(unsigned short _Port, std::string _IP) override;
	/// 공통 Function
	virtual BOOL	Recv(std::vector<Network_Message>& _Message_Vec) override;
	virtual BOOL	Send(Packet_Header* _Packet, int _SendType = SEND_TYPE_BROADCAST, SOCKET _Socket = INVALID_SOCKET) override;
	virtual BOOL	Disconnect(SOCKET _Socket) override;
	virtual BOOL	End() override;
};

