#pragma once

#ifdef DHNETWORKAPI_EXPORTS
#define DHNETWORKAPI_DLL __declspec(dllexport)
#else
#define DHNETWORKAPI_DLL __declspec(dllimport)

#ifdef _DEBUG
#pragma comment(lib,"DHNetworkAPI_x64d")
#else
#pragma comment(lib,"DHNetworkAPI_x64r")
#endif

#endif

#include <WinSock2.h>
#include <string>
#include <vector>

struct Packet_Header;
struct Network_Message;
class DHNetworkAPIBase;

enum class DHNetWork_Name
{
	DHNet,
};

class DHNETWORKAPI_DLL DHNetWorkAPI
{
public:
	DHNetWorkAPI();
	~DHNetWorkAPI();

	/// ���� ���õ� ��Ʈ��ũ�� ���� ������.
	DHNetworkAPIBase* Set_NetWork = nullptr;

	/// �⺻���� ��ɵ�.. 
	BOOL	Initialize(DHNetWork_Name _Using_NetWork_Name, unsigned short _Debug_Option = 0);
	BOOL	Accept(unsigned short _Port, unsigned short _Max_User_Count, unsigned short _Work_Thread_Count);
	BOOL	Connect(unsigned short _Port, std::string _IP);
	BOOL	Recv(std::vector<Network_Message>& _Message_Vec);
	BOOL	Send(Packet_Header* _Packet, int _SendType = 0, SOCKET _Socket = -1);
	BOOL	Disconnect(SOCKET _Socket);
	BOOL	End();

	/// ��Ÿ �߰� ����� ������ ����..
};