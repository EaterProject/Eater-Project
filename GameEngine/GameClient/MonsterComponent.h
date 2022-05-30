#pragma once
#include "Component.h"
class PhysRayCast;
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
	virtual void Dead()		= 0;
	virtual void Debug()	= 0;

	void SetMovePoint(float x, float y, float z);
	bool GetStopPoint();
protected:
	MeshFilter*				mMeshFilter;
	Transform*				mTransform;
	AnimationController*	mAnimation;
	Collider*				mColider;
	Transform*				mPlayerTR;
protected:
	Vector3 MovePoint;		//�̵��ؾ��ϴ� ����
	Vector3 DirPoint;		//�̵������� ���⺤��
	Vector3 ReturnPoint;	//�߰��� ���ư����ϴ� ��ġ
protected:
	int		State = 0;
protected:
	///Idle ���� ������
	bool		IdleStart			= false; //Idle ���� ���� ����
	float		IdleTime			= 0; //���� ���ð�
	float		Idle_MaxTime		= 0; //�ִ밪�� �ּҰ��� ������ ���� ���ð�
	const int	Idle_MaxTime_Max	= 5; //���ð��� �ִ밪
	const int	Idle_MaxTime_Min	= 2; //���ð��� �ּҰ�
protected:
	///Attack ���� ������
	bool	AttackStart		= false;
	bool	MoveStart		= false;
protected:
	///Chase ���� ������
	float		ChaseTime		= 0.0f; //�߰��ϴ� ���� �ð�
	const float ChaseEndTime	= 5.0f;	//�߰���  �����ϴ� �ð� 
protected:
	float AttackRange	= 2.5f;			//���� �Ÿ�
	float ChaseRange	= 5.5f;			//�߰ݰŸ�
protected:
	float	HP			= 100;			//���� ü��
	float	Speed		= 0.75f;		//�̵� �ӵ�
	float	AttackTime	= 0;			//���� ���� �ð�
	int		PointNumber = -1;			
	const float IdleSpeed	= 0.75f;
	const float ChaseSpeed	= 1.5f;
	PhysRayCast* mRay;
};


