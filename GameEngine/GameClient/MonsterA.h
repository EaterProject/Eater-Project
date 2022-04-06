#pragma once
#include "ClientComponent.h"
class MeshFilter;
class Transform;
class GameObject;
class AnimationController;
class Collider;
class Rigidbody;

class MonsterA :public ClientComponent
{
public:
	MonsterA();
	virtual ~MonsterA();
public:
	void Awake();
	void SetUp();
	void Update();
	void OnTriggerStay(GameObject* Obj);
	void OnTriggerEnter(GameObject* Obj);
	void ReSet();
	void SetMovePoint(float x, float y, float z);
private:
	MeshFilter*				mMeshFilter;
	Transform*				mTransform;
	AnimationController*	mAnimation;
	Collider*				mColider;
	Rigidbody*				mRigidbody;
private:
	Vector3 MovePoint;	//�̵��ؾ��ϴ� ����
	Vector3 DirPoint;	//�̵������� ���⺤��

	int BulletTag;
	float HP			= 100;	//���� ü��
	float AttackTime	= 0;	//���� ���� �ð�
	int	  NowTag		= 0;

	const float AttackMaxTime	= 10;	//�����ֱ�
	const float MaxHP			= 100;	//�ִ�ü��
};

