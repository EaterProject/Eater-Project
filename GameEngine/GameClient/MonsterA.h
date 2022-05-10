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

	Vector3 MovePoint;	//이동해야하는 지점
	Vector3 DirPoint;	//이동지점의 방향벡터
private:
	float HP			= 100;	//현재 체력
	float Speed			= 1;	//이동 속도
	float AttackTime	= 0;	//현재 공격 시간

	const float PushPower		= 100;
	const float AttackMaxTime	= 10;	//공격주기
	const float MaxHP			= 100;	//최대체력
private:
	bool GetStopPoint();
};

