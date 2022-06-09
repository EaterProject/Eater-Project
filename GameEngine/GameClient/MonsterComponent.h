#pragma once
#include "Component.h"
#include <string>
class PhysRayCast;
class MeshFilter;
class Transform;
class AnimationController;
class Collider;
class Rigidbody;
class MonsterComponent : public Component
{
public:
	MonsterComponent();
	virtual ~MonsterComponent();
public:
	virtual void Awake() override;
	virtual void SetUp() override;
	virtual void Update() override;
	virtual void OnTriggerStay(GameObject* Obj) override;

	virtual void Move();
	virtual void Attack();
	virtual void Idle();
	virtual void Dead();
	virtual void Chase();
	virtual void Hit();
	virtual void Debug();

	void PlayerDistanceCheck();
	void SetSearchPoint(int Index, Vector3 Point);
	bool GetStopPoint(const Vector3& Pos);
	void SetMovePoint(float x, float y, float z);
protected:
	MeshFilter*				mMeshFilter	= nullptr;
	Transform*				mTransform	= nullptr;
	AnimationController*	mAnimation	= nullptr;
	Collider*				mColider	= nullptr;
	Transform*				mPlayerTR	= nullptr;
	Rigidbody*				mRigidbody	= nullptr;
protected:
	Vector3 SearchPoint[5];
	Vector3 MovePoint;		//�̵��ؾ��ϴ� ����
	Vector3 DirPoint;		//�̵������� ���⺤��
	Vector3 ReturnPoint;	//�߰��� ���ư����ϴ� ��ġ
protected:
	int		MonsterState	= 0;
	bool	MonsterFront_Z	= false;
	float	PlayerDistance  = 10;
protected:
	//ó�� �ѹ��� �����ϱ����� ������
	bool IdleStart		= false;	//Idle	 ���� ���� ����
	bool AttackStart	= false;	//Attack ���� ���� ����
	bool MoveStart		= false;	//Move   ���� ���� ����
	bool ChaseStart		= false;	//Chase  ���� ���� ����
	bool HitStart		= false;    //Hit	 ���� ���� ����
protected:
	///�ִϸ��̼� �̸� ������
	std::string ModelName;
	std::string AnimationName;

	std::string Animation_Idel		= "idle";
	std::string Animation_Attack	= "attack";
	std::string Animation_Move		= "move";
	std::string Animation_Die		= "die";
	std::string Animation_hit		= "hit";
protected:
	///Idle ���� ������
	float		IdleTime			= 0;		//���� ���ð�
	float		Idle_MaxTime		= 0;		//�ִ밪�� �ּҰ��� ������ ���� ���ð�
	const int	Idle_MaxTime_Max	= 5;		//���ð��� �ִ밪
	const int	Idle_MaxTime_Min	= 2;		//���ð��� �ּҰ�
protected:
	///Attack ���� ����

protected:
	///Hit ���� ����
	float HitTime				= 0.0f;			//������ �¾����� �ð� ��¿�
	float HitMaxTime			= 1.0f;			//������ �¾����� �ִ� �����ð�
protected:
	///Chase ���� ������
	float ChaseTime				= 0.0f;			//�߰��ϴ� ���� �ð�
	float ChaseEndTime			= 5.0f;			//�߰���  �����ϴ� �ð� 
protected:
	float AttackRange			= 2.5f;			//���� �Ÿ�
	float ChaseRange			= 5.5f;			//�߰� �Ÿ�
protected:
	float	HP					= 100;			//���� ü��
	float	Speed				= 0.75f;		//�̵� �ӵ�
	float	AttackTime			= 0;			//���� ���� �ð�
	int		PointNumber			= -1;			
	const float IdleSpeed		= 0.75f;
	const float ChaseSpeed		= 1.5f;
private:
	PhysRayCast* mRay = new PhysRayCast();
};


