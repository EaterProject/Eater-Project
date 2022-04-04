#include "NetworkManager.h"

//서버 연결 필요한 헤더
#include "DHNetWorkAPI.h"
#include "SharedDataStruct.h"
#include "WorldData_generated.h"
#include "GameClientGameServerPacketDefine.h"
#include "PortIPDefine.h"

//엔진쪽 헤더
#include "GameObject.h"
#include "NetworkComponent.h"
#include "NetworkManagerComponent.h"
#include "Profiler/Profiler.h"

std::vector<Network_Message> Msg_Vec;
NetworkManager::NetworkManager()
{
	Recv_Packet = nullptr;
	Send_Packet = nullptr;
	mDHNetWork = nullptr;
	mBuilder	= nullptr;
	StartNework = false;
	mClientNetworkManager = nullptr;
}

NetworkManager::~NetworkManager()
{
	//종료할때 사용한 페킷은 삭제주자
	delete Send_Packet;
	delete mBuilder;
	
	Send_Packet = nullptr;
	mBuilder	= nullptr;
}

void NetworkManager::Initialize()
{
	// Recv 데이터 받아오기
	std::vector<Network_Message> Msg_Vec;
	
	//네트워크 엔진 생성
	mDHNetWork = new DHNetWorkAPI();
	mDHNetWork->Initialize(DHNetWork_Name::DHNet);

	//빌더 생성
	mBuilder = new flatbuffers::FlatBufferBuilder();

	//보낼페킷 데이터 생성
	Send_Packet = new C2S_Packet();
}

void NetworkManager::Update()
{
	//서버로부터 상태를 업데이트 받고 체크한다.
	NETWORK_RECV();
}

void NetworkManager::SetClientNetworkManager(NetworkManagerComponent* mManager)
{
	//클라이언트쪽 매니저 등록
	mClientNetworkManager = mManager;
	mClientNetworkManager->_Builder = mBuilder;
}

void NetworkManager::NETWORK_RECV()
{
	//서버에서 데이터를 받는다	
	if (mDHNetWork->Recv(Msg_Vec))
	{
		for (auto& S2C_Msg : Msg_Vec)
		{
			Recv_Packet = static_cast<S2C_Packet*>(S2C_Msg.Packet);
			uint8_t* Recv_Data_Ptr = (unsigned char*)Recv_Packet->Packet_Buffer;

			//클라이언트에서 네트워크 매니저를 받았을때
			if (mClientNetworkManager != nullptr)
			{
				mClientNetworkManager->RECV(Recv_Packet->Packet_Buffer,Recv_Packet->Packet_Type);
			}

			//받은 데이터의 페킷은 삭제
			delete S2C_Msg.Packet;
			S2C_Msg.Packet = nullptr;
		}
		Msg_Vec.clear();
	}
}

void NetworkManager::NETWORK_SEND(flatbuffers::FlatBufferBuilder* Builder, int Type)
{
	//빌더의 데이터를 패킷으로 카피
	Send_Packet->Packet_Type = Type;
	Send_Packet->Packet_Size = Builder->GetSize();

	memcpy_s
	(
		Send_Packet->Packet_Buffer,
		Builder->GetSize(),
		Builder->GetBufferPointer(),
		Builder->GetSize()
	);

	//전송
	mDHNetWork->Send(Send_Packet);

	Builder->Clear();
}

void NetworkManager::C2S_LOADING_COMPLETE(unsigned int Number)
{
	///클라이언트 로딩 완료
	Send_Packet->Packet_Type = C2S_LOADING_COMPLETE_REQ;
	Send_Packet->Packet_Size = sizeof(Number);
	memcpy_s(Send_Packet->Packet_Buffer, sizeof(Number), &Number, sizeof(Number));
	mDHNetWork->Send(Send_Packet);
}

void NetworkManager::C2S_CONNECT(int ServerPort,std::string  Local_Connect_IP)
{
	/// 서버에 연결 요청을 한다
	PROFILE_LOG(PROFILE_OUTPUT::CONSOLE, "(네트워크 연결)");

	while (!mDHNetWork->Connect(ServerPort, Local_Connect_IP))
	{
		PROFILE_LOG(PROFILE_OUTPUT::CONSOLE, "CONNECT 연결요청중...");
	};

	///연결완료
	PROFILE_LOG(PROFILE_OUTPUT::CONSOLE, "CONNECT 연결완료...");
}
