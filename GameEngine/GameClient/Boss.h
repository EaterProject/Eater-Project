#pragma once
#include "Component.h"
#include "ClientTypeOption.h"
#include <string>

class AnimationController;
class MeshFilter;
class Transform;
class Collider;
class Rigidbody;
class PhysRayCast;
class Boss :public Component
{
public:
	Boss();
	virtual ~Boss();

	void Awake() override;
	void SetUp() override;
	void Update() override;
	void Debug() override;
	void OnTriggerStay(GameObject* Obj) override;
private:
	AnimationController* mAnimation = nullptr;
	MeshFilter*		mMeshFilter		= nullptr;
	MeshFilter*		mSkinFilter		= nullptr;
	Transform*		mTransform		= nullptr;
	Collider*		mColider		= nullptr;
	Transform*		mPlayerTR		= nullptr;
	Rigidbody*		mRigidbody		= nullptr;
private:
	std::string ANIMATION_NAME[15];	//�ִϸ��̼� �̸�
	float ANIMATION_TIME[15];		//�ִϸ��̼� �ð�
	bool  STATE_START[15];			//���� ���� ����
private:
	//���¿� ���� �Լ���
	void Boss_Idle();
	void Boss_DEAD();
	void Boss_Groggy_Start();
	void Boss_Groggy_Play();
	void Boss_Groggy_End();
	void Boss_Teleport_Ready();
	void Boss_Teleport_Start();
	void Boss_Create();
	void Boss_Closer_Attack_L();
	void Boss_Closer_Attack_R();
	void Boss_Chase_Attack_Ready();
	void Boss_Chase_Attack_Play();
	void Boss_Rendom_Attack_Ready();
	void Boss_Rendom_Attack_Play();
	void Boss_Hit();
private:
	void	SetState(BOSS_STATE State);	//���¸� ��ȯ
	bool	FirstState();				//���� ���°� ó������ üũ
	float	PlayerDistanceCheck();		//�÷��̾��� �Ÿ��� üũ
	void	CreateSkillPoint();			//��ų ��� ��ġ�� ����
private:
	bool	IsHit			= false;
	bool	IsSkill			= false;

	int		mState			= 0;		//������ ����
	float	AttackRange		= 3.5f;		//���� ���� ����
	float	FightRange		= 15.0f;	//������ �ο� �� �ִ� �Ÿ�
	float	SkillRange		= 10.0f;	//������ ��ų �Ÿ�
	float	PlayerDistance	= 0.0f;		//�÷��̾��� �Ÿ�
	float	HP				= 100;		//���� ü��
	float	RendomSkillReadyTime		= 0.0f;
	float	RendomSkillReadyTimeMax		= 10.0f;
	float	RendomSkillPlayTime			= 0.0f;
	float	RendomSkillPlayTimeMax		= 10.0f;

	Vector3	SkillPoint[5];
	GameObject* Weapon[5];
	PhysRayCast* mRay;
};

