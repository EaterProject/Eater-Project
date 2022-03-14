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


	//������ �� �ִ� ��ü���� üũ
	EATER_ENGINEDLL bool NET_PLATER_CHECK();
	//��ü�� ������
	EATER_ENGINEDLL void NET_MOVE_POS(Vector3 Pos, Vector3 Direction,float Speed);
private:
	bool MyPlayer;	//���� ������ �� �ִ� ��ü���� ����
	void PushData(Vector3 Pos);
private:
	friend NetworkManager;
	
	NetworkManager* Manager;
	Transform* mTransform;
	Rigidbody* mRigidbody;
	int Number = -1;

	///�������� ���� �����͵�
	Vector3 Position;	//��ġ
	Vector3 Direction;	//����
	float	Speed;		//�ӵ�
	int		HP;			//ü��
	int		Mana;		//����
};

