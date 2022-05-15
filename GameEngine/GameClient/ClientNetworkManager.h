#pragma once

/// <summary>
/// ���ӿ������� ���� �����͸� ������ Ÿ�Կ� ���� �����͸������ϰ� ������Ʈ�� �������ִ� �Ŵ���
/// </summary>

#include "NetworkManagerComponent.h"
#include <map>
#include <queue>
namespace flatbuffers
{
	class FlatBufferBuilder;
	class Vec3;
}
namespace Eater
{
	namespace PlayerData
	{
		struct Player;
	}
	namespace GameData
	{
		struct WorldData;
	}
}

class NetwrokPlayer;
class NetworkComponent;
class Player;
class MonsterA;
class ManaStone;
class ClientNetworkManager : public NetworkManagerComponent
{
public:
	struct FrameData
	{
		unsigned short					m_Sequence;			// ������ ��ȣ.
		DirectX::SimpleMath::Vector3	m_Position;			// ���� �����ӿ����� ��ġ (���⺤�� ������� ��)
		DirectX::SimpleMath::Vector3	m_Mov_Vector;		// �̵��� ����
		double							m_dtime;			// dtime ��
	};

	ClientNetworkManager();
	~ClientNetworkManager();
	void Update();
private:
	///RECV 
	virtual void RECV(void* Data, int type);
	void RECV_START_GAME(void* Data);				//������ ���۵Ǿ����� �޴� ������
	void RECV_LOADING_COMPLETE(void* Data);			//�ε��� ���۵Ǿ����� �޴� ������
	void RECV_WORLD_UPDATE(void* Data);				//������ �޴� ���� ������
private:
	///����
	void CreatePlayer(Vector3 Pos,int PlayerIndex,float Speed,int PlayerType);
	void CreateEnemy(int index ,Vector3 Pos);
	void CreateManaStone(int index, Vector3 Pos);
private:
	///����
	void DeleteManaStone(int index);
	void DeleteMonster(int index);
private:
	///������Ʈ
	void UpdatePlayer(const Eater::GameData::WorldData* Data);
	void UpdateEnemy(const Eater::GameData::WorldData* Data);
	void UpdateMana(const Eater::GameData::WorldData* Data);
private:
	///������Ʈ ����Ʈ��
	std::map<int,Player*>		UserObjList;		//���� ������Ʈ ����Ʈ
	std::map<int,MonsterA*>		MonsterList;		//���� ������Ʈ ����Ʈ
	std::map<int,ManaStone*>	ManaList;			//������ ������Ʈ ����Ʈ
private:
	///������ ������
	int ClientNumber;				//���� ���� Ŭ���̾�Ʈ �ѹ�
	
	double Current_Time;			//���� �ð�
	double Prev_Time;				//���� �ð�
	double Passed_Time;				//���� �ð�
	double m_Passed_Client_Time;	//�������� �����͸� �ް� �����ð�

	bool isCreateEnemy;				//������ ��������
	bool isCreateMana;				//������ ��������
	bool isStartGame;				//���� ���� ����
};

