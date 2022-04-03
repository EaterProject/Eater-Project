#pragma once

#include "EaterEngineDLL.h"

#include "Component.h"

class ENCManager;
class Rigidbody;
class Transform;
class NetworkManager;
class UnitNet :public Component
{
public:
	EATER_ENGINEDLL UnitNet();
	~UnitNet();

	virtual void Awake();
	virtual void Update();


	//움직일 수 있는 객체인지 체크
	EATER_ENGINEDLL bool NET_PLATER_CHECK();
	//객체를 움직임
	EATER_ENGINEDLL void NET_MOVE_POS(Vector3 Pos, Vector3 Direction,float Speed);
private:
	bool MyPlayer;	//내가 움직일 수 있는 객체인지 여부
	void PushData(Vector3 Pos);
private:
	friend NetworkManager;
	
	NetworkManager* Manager;
	Transform* mTransform;
	Rigidbody* mRigidbody;
	int Number = -1;

	///서버에서 받을 데이터들
	Vector3 Position;	//위치
	Vector3 Direction;	//방향
	float	Speed;		//속도
	int		HP;			//체력
	int		Mana;		//마나
};

