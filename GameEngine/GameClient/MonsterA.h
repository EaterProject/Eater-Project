#pragma once
#include "ClientComponent.h"
class MeshFilter;
class Transform;
class GameObject;
class AnimationController;
class Collider;
class Rigidbody;
class ManaStone;

class MonsterA :public ClientComponent
{
public:
	MonsterA();
	virtual ~MonsterA();
public:
	ManaStone* Mana;
public:
	void Awake();
	void SetUp();
	void Update();
	void SetMovePoint(float x, float y, float z);
private:
	MeshFilter*				mMeshFilter;
	Transform*				mTransform;
	AnimationController*	mAnimation;
	Collider*				mColider;
	Rigidbody*				mRigidbody;

	Vector3 MovePoint;	//�̵��ؾ��ϴ� ����
	Vector3 DirPoint;	//�̵������� ���⺤��
private:
	float HP			= 100;	//���� ü��
	float Speed			= 1;	//�̵� �ӵ�
	float AttackTime	= 0;	//���� ���� �ð�

	const float PushPower		= 100;
	const float AttackMaxTime	= 10;	//�����ֱ�
	const float MaxHP			= 100;	//�ִ�ü��
private:
	bool GetStopPoint();
};

