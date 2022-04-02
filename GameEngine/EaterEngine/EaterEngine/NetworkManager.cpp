#include "NetworkManager.h"

//���� ���� �ʿ��� ���
#include "DHNetWorkAPI.h"
#include "SharedDataStruct.h"
#include "WorldData_generated.h"
#include "GameClientGameServerPacketDefine.h"
#include "PortIPDefine.h"

//������ ���
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
	//�����Ҷ� ����� ��Ŷ�� ��������
	delete Send_Packet;
	delete mBuilder;
	
	Send_Packet = nullptr;
	mBuilder	= nullptr;
}

void NetworkManager::Initialize()
{
	// Recv ������ �޾ƿ���
	std::vector<Network_Message> Msg_Vec;
	
	//��Ʈ��ũ ���� ����
	mDHNetWork = new DHNetWorkAPI();
	mDHNetWork->Initialize(DHNetWork_Name::DHNet);

	//���� ����
	mBuilder = new flatbuffers::FlatBufferBuilder();

	//������Ŷ ������ ����
	Send_Packet = new C2S_Packet();
}

void NetworkManager::Update()
{
	//�����κ��� ���¸� ������Ʈ �ް� üũ�Ѵ�.
	NETWORK_RECV();
}

void NetworkManager::SetClientNetworkManager(NetworkManagerComponent* mManager)
{
	//Ŭ���̾�Ʈ�� �Ŵ��� ���
	mClientNetworkManager = mManager;
	mClientNetworkManager->_Builder = mBuilder;
}

void NetworkManager::NETWORK_RECV()
{
	//�������� �����͸� �޴´�	
	if (mDHNetWork->Recv(Msg_Vec))
	{
		for (auto& S2C_Msg : Msg_Vec)
		{
			Recv_Packet = static_cast<S2C_Packet*>(S2C_Msg.Packet);
			uint8_t* Recv_Data_Ptr = (unsigned char*)Recv_Packet->Packet_Buffer;

			//Ŭ���̾�Ʈ���� ��Ʈ��ũ �Ŵ����� �޾�����
			if (mClientNetworkManager != nullptr)
			{
				mClientNetworkManager->RECV(Recv_Packet->Packet_Buffer,Recv_Packet->Packet_Type);
			}

			//���� �������� ��Ŷ�� ����
			delete S2C_Msg.Packet;
			S2C_Msg.Packet = nullptr;
		}
		Msg_Vec.clear();
	}
}

void NetworkManager::NETWORK_SEND(flatbuffers::FlatBufferBuilder* Builder, int Type)
{
	//������ �����͸� ��Ŷ���� ī��
	Send_Packet->Packet_Type = Type;
	Send_Packet->Packet_Size = Builder->GetSize();

	memcpy_s
	(
		Send_Packet->Packet_Buffer,
		Builder->GetSize(),
		Builder->GetBufferPointer(),
		Builder->GetSize()
	);

	//����
	mDHNetWork->Send(Send_Packet);

	Builder->Clear();
}

void NetworkManager::C2S_LOADING_COMPLETE(unsigned int Number)
{
	///Ŭ���̾�Ʈ �ε� �Ϸ�
	Send_Packet->Packet_Type = C2S_LOADING_COMPLETE_REQ;
	Send_Packet->Packet_Size = sizeof(Number);
	memcpy_s(Send_Packet->Packet_Buffer, sizeof(Number), &Number, sizeof(Number));
	mDHNetWork->Send(Send_Packet);
}

void NetworkManager::C2S_CONNECT(int ServerPort,std::string  Local_Connect_IP)
{
	/// ������ ���� ��û�� �Ѵ�
	PROFILE_LOG(PROFILE_OUTPUT::CONSOLE, "(��Ʈ��ũ ����)");

	while (!mDHNetWork->Connect(ServerPort, Local_Connect_IP))
	{
		PROFILE_LOG(PROFILE_OUTPUT::CONSOLE, "CONNECT �����û��...");
	};

	///����Ϸ�
	PROFILE_LOG(PROFILE_OUTPUT::CONSOLE, "CONNECT ����Ϸ�...");
}
