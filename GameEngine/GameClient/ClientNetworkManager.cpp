#include "ClientNetworkManager.h"
#include "NetworkComponent.h"
#include "GameClientGameServerPacketDefine.h"
#include "MainHeader.h"
#include "WorldData_generated.h"

//���۳�Ʈ
#include "GameObject.h"
#include "MeshFilter.h"
#include "Transform.h"
#include "Player.h"
#include "AnimationController.h"
#include "ManaStone.h"
#include "Monster.h"

//����ü
#include "PlayerData.h"

ClientNetworkManager::ClientNetworkManager()
{
	ClientNumber	= -1;
	Current_Time	= 0;
	Passed_Time		= 0;
	Prev_Time		= 0;
	ClientNumber	= -1;
	m_Passed_Client_Time = 0;

	isCreateEnemy	= false;
	isCreateMana	= false;
	isStartGame		= false;
}

ClientNetworkManager::~ClientNetworkManager()
{
	

	
}

void ClientNetworkManager::Update()
{
	m_Passed_Client_Time += GetDeltaTime();
}

void ClientNetworkManager::RECV(void* Data, int type)
{
	switch (type)
	{
	case S2C_LOADING_COMPLETE_RES:  //���� ���� �� ����
		RECV_LOADING_COMPLETE(Data);
		break;
	case S2C_START_GAME:			//���� ���� �� ����
		RECV_START_GAME(Data);
		break;
	case S2C_WORLD_UPDATE:			//���� ������Ʈ
		RECV_WORLD_UPDATE(Data);
		break;
	}
}

void ClientNetworkManager::RECV_START_GAME(void* Data)
{
	if (isStartGame == true) { return; }
	//������ ��ȯ
	uint8_t* Recv_Data_Ptr = (unsigned char*)Data;
	const Eater::GameData::StartInfo* _Recv_Data = flatbuffers::GetRoot<Eater::GameData::StartInfo>(Recv_Data_Ptr);

	//���� �����͸� �޴´�
	auto _Players_Data = _Recv_Data->users();
	
	//�ʱⰪ�� �޾Ƽ� �÷��̾� ������Ʈ ����
	int MaxSize = (int)_Players_Data->size();
	for (int i = 0; i < MaxSize; i++)
	{
		auto Player_Data	= _Players_Data->Get(i);
		auto _Player_Index	= Player_Data->playerindex();

		int PlayerType	= Player_Data->character_type();
		float Speed		= Player_Data->world_speed();

		float Pos_X = Player_Data->world_position()->x();
		float Pos_Y = Player_Data->world_position()->y();
		float Pos_Z = Player_Data->world_position()->z();

		//������Ʈ�� �����Ѵ�
		CreatePlayer(Vector3(Pos_X,Pos_Y,Pos_Z), _Player_Index, Speed, PlayerType);
	}
	isStartGame = true;
}

void ClientNetworkManager::RECV_LOADING_COMPLETE(void* Data)
{
	//Ŭ���̾�Ʈ �ѹ����޴´�
	unsigned short* Recv_Data_Ptr = (unsigned short*)Data;
	ClientNumber = *Recv_Data_Ptr;
}

void ClientNetworkManager::RECV_WORLD_UPDATE(void* Data)
{
	///���� ������ ������Ʈ

	uint8_t* Recv_Data_Ptr = (unsigned char*)Data;

	//���� �����͸� �д´�
	const Eater::GameData::WorldData* _Recv_Data
		= flatbuffers::GetRoot<Eater::GameData::WorldData>(Recv_Data_Ptr);

	//�ð��� ���
	Current_Time = GetDeltaTime();			//���� �������� �ð�
	Passed_Time = Current_Time - Prev_Time; //���� �ð����� �����ð��� �A��  = �ɸ��ð�
	Prev_Time = Current_Time;				//���Žð��� ���� �ð����� ������Ʈ

	UpdatePlayer(_Recv_Data);	//�÷��̾� ���¸� ������Ʈ
	UpdateEnemy(_Recv_Data);	//�� ���� ���¸� ������Ʈ
	UpdateMana(_Recv_Data);		//������	���¸� ������Ʈ
}

/// ����
void ClientNetworkManager::CreatePlayer(Vector3 Pos, int PlayerIndex, float Speed, int PlayerType)
{
	//�÷��̾� ��ü�� ������ش�
	GameObject* obj = Instance("Player");
	MeshFilter* mMeshFilter = obj->AddComponent<MeshFilter>();
	Transform*	mTransform	= obj->GetTransform();
	Player*		mPlayer		= obj->AddComponent<Player>();
	AnimationController* AC = obj->AddComponent<AnimationController>();

	//���� �����͸� �־���
	mTransform->Position	= Pos;
	mPlayer->SetClientNumber(ClientNumber, PlayerIndex);
	//����� �� �ֵ��� ������ �Ѱ��ش�
	mPlayer->_Builder		= _Builder;
	mPlayer->Speed			= Speed;

	//������ �����͸� ����Ʈ�� ��Ƶд�
	UserObjList.insert({ PlayerIndex ,mPlayer });
}

void ClientNetworkManager::CreateEnemy(int index, Vector3 Pos)
{
	//�� ���͸� �����Ѵ�

	GameObject* obj = Instance();
	obj->AddComponent<MeshFilter>();
	obj->AddComponent<AnimationController>();

	Transform*	mTransform	= obj->GetTransform();
	Monster*	mMonster	= obj->AddComponent<Monster>();

	mTransform->Position = Pos;
	MonsterList.insert({index,mMonster});
}

void ClientNetworkManager::CreateManaStone(int index, Vector3 Pos)
{
	//�������� �����Ѵ�

	GameObject* obj = Instance();
	obj->AddComponent<MeshFilter>();
	obj->AddComponent<AnimationController>();

	Transform* mTransform = obj->GetTransform();
	ManaStone* mManaStone = obj->AddComponent<ManaStone>();

	mTransform->Position = Pos;
	ManaList.insert({index,mManaStone });
}

///����
void ClientNetworkManager::DeleteManaStone(int index)
{
	///������ ����

	//������ ���
	if (ManaList[index] == nullptr) { return; }

	//������Ʈ ��������
	GameObject* Obj = ManaList[index]->gameobject;
	ManaList[index] = nullptr;
	
	//����
	Destroy(Obj);
}

void ClientNetworkManager::DeleteMonster(int index)
{
	///���� ����

	//������ ���
	if (MonsterList[index] == nullptr) { return; }

	//������Ʈ ��������
	GameObject* Obj = MonsterList[index]->gameobject;
	MonsterList[index] = nullptr;

	//����
	Destroy(Obj);
}

///������Ʈ
void ClientNetworkManager::UpdatePlayer(const Eater::GameData::WorldData* Data)
{
	//���� �����͸� �����´�
	auto _Players_Data = Data->users();

	//�÷��̾� ������ �б�
	for (int i = 0; i < (int)_Players_Data->size(); i++)
	{
		auto Player_Data = _Players_Data->Get(i);

		// ���� ��ȣ.
		auto	_Player_Index = Player_Data->playerindex();		// Player index

		PlayerData UserData;
		//���� ��ġ
		UserData.Position.x = Player_Data->position()->x();
		UserData.Position.y = Player_Data->position()->y();
		UserData.Position.z = Player_Data->position()->z();

		UserData.SequenceNumber = (int)Player_Data->sequence();		// ������ ��ȣ
		//���� �ִϸ��̼� Ÿ��
		UserData.AnimationType = (int)Player_Data->animation();
		//������Ʈ �ѹ�
		UserData.ObjectNumber = _Player_Index;
		UserData.Passed_Time = m_Passed_Client_Time;

		//�÷��̾� ��ü���� ��Ʈ��ũ���� ���� �����͸� �Ѱ��ش�
		UserObjList[_Player_Index]->SetNetworkPlayerData(UserData);
	}
}

void ClientNetworkManager::UpdateEnemy(const Eater::GameData::WorldData* Data)
{
	//�� ���� �����͸� �޾ƿ´�
	auto _Enemy_Data = Data->enemies();
	int Size = (int)_Enemy_Data->size();

	for (int i = 0; i < Size; i++)
	{
		auto _Enemy = _Enemy_Data->Get(i);

		int index = _Enemy->index();
		float x = _Enemy->position()->x();
		float y = _Enemy->position()->y();
		float z = _Enemy->position()->z();
		
		//�Լ��� ó�� ���۵Ǿ������� ���� �״������ʹ� ������Ʈ
		if (isCreateEnemy == false)
		{
			//����
			CreateEnemy(index, Vector3(x, y, z));
		}
		else
		{
			//������Ʈ
			//MonsterList[index]->MonsterUpdate(Vector3(x,y,z));
		}
	}
	isCreateEnemy = true;
}

void ClientNetworkManager::UpdateMana(const Eater::GameData::WorldData* Data)
{
	//������ �����͸� �޾ƿ´�
	auto _Object_Data = Data->objects();
	int Size = (int)_Object_Data->size();

	
	for (int i = 0; i < (int)_Object_Data->size(); i++)
	{
		auto _Object = _Object_Data->Get(i);

		int index = _Object->index();
		float x = _Object->position()->x();
		float y = _Object->position()->y();
		float z = _Object->position()->z();

		//�Լ��� ó�� ���۵Ǿ������� ���� �״������ʹ� ������Ʈ
		if (isCreateMana == false)
		{
			//����
			CreateManaStone(index, Vector3(x, y, z));
		}
		else
		{
			//������Ʈ
			//ManaList[index]->ManaStoneUpdate(Vector3(x,y,z));
		}
	}
	isCreateMana = true;
}


