#pragma once

/// <summary>
/// 게임엔진에서 받은 데이터를 데이터 타입에 따라 데이터를변경하고 오브젝트에 전달해주는 매니저
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
		unsigned short					m_Sequence;			// 시퀀스 번호.
		DirectX::SimpleMath::Vector3	m_Position;			// 현재 프레임에서의 위치 (방향벡터 계산후의 값)
		DirectX::SimpleMath::Vector3	m_Mov_Vector;		// 이동할 방향
		double							m_dtime;			// dtime 값
	};

	ClientNetworkManager();
	~ClientNetworkManager();
	void Update();
private:
	///RECV 
	virtual void RECV(void* Data, int type);
	void RECV_START_GAME(void* Data);				//게임이 시작되었을때 받는 데이터
	void RECV_LOADING_COMPLETE(void* Data);			//로딩이 시작되었을때 받는 데이터
	void RECV_WORLD_UPDATE(void* Data);				//프레임 받는 월드 데이터
private:
	///생성
	void CreatePlayer(Vector3 Pos,int PlayerIndex,float Speed,int PlayerType);
	void CreateEnemy(int index ,Vector3 Pos);
	void CreateManaStone(int index, Vector3 Pos);
private:
	///삭제
	void DeleteManaStone(int index);
	void DeleteMonster(int index);
private:
	///업데이트
	void UpdatePlayer(const Eater::GameData::WorldData* Data);
	void UpdateEnemy(const Eater::GameData::WorldData* Data);
	void UpdateMana(const Eater::GameData::WorldData* Data);
private:
	///오브젝트 리스트들
	std::map<int,Player*>		UserObjList;		//유저 오브젝트 리스트
	std::map<int,MonsterA*>		MonsterList;		//몬스터 오브젝트 리스트
	std::map<int,ManaStone*>	ManaList;			//마나석 오브젝트 리스트
private:
	///데이터 변수들
	int ClientNumber;				//현재 나의 클라이언트 넘버
	
	double Current_Time;			//현재 시간
	double Prev_Time;				//이전 시간
	double Passed_Time;				//계산된 시간
	double m_Passed_Client_Time;	//서버에서 데이터를 받고 지난시간

	bool isCreateEnemy;				//적유닛 생성여부
	bool isCreateMana;				//마나석 생성여부
	bool isStartGame;				//게임 시작 여부
};

