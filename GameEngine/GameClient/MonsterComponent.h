#pragma once
#include "Component.h"

class MeshFilter;
class Transform;
class AnimationController;
class Collider;
class Rigidbody;
class MonsterComponent : public Component
{
public:
	MonsterComponent() = default;
	virtual ~MonsterComponent() = default;

public:
	virtual void Move()		= 0;
	virtual void Attack()	= 0;
	virtual void Idle()		= 0;
	virtual void Debug()	= 0;

	void SetMovePoint(float x, float y, float z);
	bool GetStopPoint();
protected:
	MeshFilter*				mMeshFilter;
	Transform*				mTransform;
	AnimationController*	mAnimation;
	Collider*				mColider;
	Rigidbody*				mRigidbody;
protected:
	Vector3 MovePoint;	//이동해야하는 지점
	Vector3 DirPoint;	//이동지점의 방향벡터
protected:
	int		State = 0;
protected:
	///Idle 상태 변수들
	bool		IdleStart			= false; //Idle 상태 시작 변수
	float		IdleTime			= 0; //현재 대기시간
	float		Idle_MaxTime		= 0; //최대값과 최소값을 설정한 최종 대기시간
	const int	Idle_MaxTime_Max	= 5; //대기시간의 최대값
	const int	Idle_MaxTime_Min	= 2; //대기시간의 최소값
protected:
	///Attack 상태 변수들
	bool	AttackStart		= false;
	bool	MoveStart		= false;
protected:
	float	HP			= 100;		//현재 체력
	float	Speed		= 0.75f;	//이동 속도
	float	AttackTime	= 0;		//현재 공격 시간
	int		PointNumber = -1;
};


