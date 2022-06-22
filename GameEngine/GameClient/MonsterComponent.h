#pragma once
#include "Component.h"
#include "ClientTypeOption.h"
#include <string>
#include <functional>
class PhysRayCast;
class MeshFilter;
class Transform;
class AnimationController;
class Collider;
class Rigidbody;
class MaterialPropertyBlock;
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
	void SetMonsterState(MONSTER_STATE State);
	void SetLimLightColor();
	void UpdateColor();
	void SetState(MONSTER_STATE mState);
	bool FirstState();
protected:
	MeshFilter*				mMeshFilter	= nullptr;
	MeshFilter*				mSkinFilter = nullptr;
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
	std::string ANIMATION_NAME[6];
	std::string SOUND_NAME[6];
	bool		STATE[6];
	float		ANIMATION_TIME[6];

	//ó�� �ѹ��� �����ϱ����� ������
	bool HitStart		= false;    //Hit	 ���� ���� ����
	bool HitFXStart		= false;	//Hit FX ���� ���� ����
protected:
	///�ִϸ��̼� �̸� ������
	std::string ModelName;
	std::string AnimationName;

	std::string Sound_Hit			= "Monster_Hit";
	std::string Sound_move;
	std::string Sound_Attack;

protected:
	///Move ���� ������
	float MoveSoundTime				= 0;
	float MoveSoundTimeMax			= 0.5;
protected:
	///Idle ���� ������
	float		IdleTime			= 0;		//���� ���ð�(�ǵ�� �ȵ�)
	float		Idle_MaxTime		= 0;		//�ִ밪�� �ּҰ��� ������ ���� ���ð�
	const int	Idle_MaxTime_Max	= 5;		//���ð��� �ִ밪
	const int	Idle_MaxTime_Min	= 2;		//���ð��� �ּҰ�
protected:
	///Attack ���� ����

protected:
	///Hit ���� ����
	float HitTime				= 0.0f;			//������ �¾����� �ð� ��¿�(�ǵ�� �ȵ�)
	float HitMaxTime			= 2.0f;			//������ �¾����� �ִ� �����ð�
protected:
	///Chase ���� ������
	float ChaseTime				= 0.0f;			//�߰��ϴ� ���� �ð�(�ǵ�� �ȵ�)
	float ChaseEndTime			= 10.0f;		//�߰���  �����ϴ� �ð� 
protected:
	///�����Ÿ�
	float AttackRange			= 2.5f;			//���� �Ÿ�
	float ChaseRange			= 5.5f;			//�߰� �Ÿ�
protected:
	float	HP					= 100;			//���� ü��
	float	Speed				= 1.5f;			//�̵� �ӵ�
	float	AttackTime			= 0;			//���� ���� �ð�
	float	RotationSpeed		= 200;			//���� ȸ���ϴ� �ӵ�
	int		PointNumber			= -1;			//���� �̵� ������ �ε���
	int		ComboCount			= 6;			//���� �޺� ī����
protected:
	Vector3 NowLimLightColor	= { 1,0,0 };	//�ǵ�� �ȵ�
	float	NowHitMonsterScale_F = 0.25f;		//�¾����� Ŀ���� �����ϰ� �Ҽ���
	float	NowHitMonsterScale   = 1.0f;		//�¾����� Ŀ���� �����ϰ� �Ҽ���
	float	NowLimLightFactor	= 2.0f;			//�ǵ�� �ȵ�
	float	NowLimLightWidth	= 0.9f;			//�ǵ�� �ȵ�
	bool	NowUpdateColor		= false;		//�ǵ�� �ȵ�
	MONSTER_COLOR	ComboColor  = MONSTER_COLOR::RED;
protected:
	MaterialPropertyBlock* MPB  = nullptr;
	std::function<void()> HitFunction;

};


