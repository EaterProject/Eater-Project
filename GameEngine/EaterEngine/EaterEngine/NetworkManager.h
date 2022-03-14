#pragma once
#include <vector>
#include <string>

namespace Eater
{
	namespace PlayerData 
	{
		struct Player;
	}
}

namespace flatbuffers
{
	class FlatBufferBuilder;
}


struct S2C_Packet;
struct C2S_Packet;
class DHNetWorkAPI;
class GameObject;
class NetworkManagerComponent;
class NetworkComponent;

class NetworkManager
{
public:
	NetworkManager();
	~NetworkManager();
	void Initialize();
	void Update();
public:
	///클라이언트와 소통할 매니저 받기
	void SetClientNetworkManager(NetworkManagerComponent* mManager);
public:
	///Sand
	void NETWORK_SEND(flatbuffers::FlatBufferBuilder* Builder, int Type);
	//클라에서 연결될떄까지 대기
	void C2S_CONNECT(int ServerPort, std::string  Local_Connect_IP);
	//연결이 완료되고 로딩이 완료되었다면 
	void C2S_LOADING_COMPLETE(unsigned int Number);
private:
	///Recv
	void NETWORK_RECV();
private:
	S2C_Packet*		Recv_Packet;	//받을 데이터
	C2S_Packet*		Send_Packet;	//보내줄 데이터
	DHNetWorkAPI*	mDHNetWork;		//통신을 위한 네트워크 Engine
private:
	NetworkManagerComponent* mClientNetworkManager;	//클라에서 받은 네트워크 매니저
	flatbuffers::FlatBufferBuilder* mBuilder;		//클라쪽으로 보내줄 데이터
	bool StartNework;
};