#pragma once
#include "Component.h"
#include "ClientTypeOption.h"
#include "MeshFilterSetting.h"
#include <string>

class State;

class AnimationController;
class MeshFilter;
class Transform;
class Collider;
class Rigidbody;
class PhysRayCast;
class BossWeapon;
class BossFriend;
class MaterialPropertyBlock;
class ParticleController;
class BossStateManager;
class Boss :public Component
{
public:
	Boss();
	virtual ~Boss();

	void Awake() override;
	void SetUp() override;
	void Start() override;

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
	std::string ANIMATION_NAME[18];	//�ִϸ��̼� �̸�
	float ANIMATION_TIME[18];		//�ִϸ��̼� �ð�
	bool  STATE_START[18];			//���� ���� ����
private:
	//���¿� ���� �Լ���
	//void Boss_Idle();
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
	void Boss_Chase_Attack_End();

	void Boss_Rendom_Attack_Start();
	void Boss_Rendom_Attack_Play();
	void Boss_Rendom_Attack_End();
	void Boss_Rendom_Attack_Reset();

	void Boss_Hit();
private:
	void	SetState(BOSS_STATE State);	//���¸� ��ȯ
	bool	FirstState();				//���� ���°� ó������ üũ
	float	PlayerDistanceCheck();		//�÷��̾��� �Ÿ��� üũ
	void	CreateSkillPoint();			//��ų ��� ��ġ�� ����
	void	GroundCheck();
private:
	bool	IsHit			= false;
	bool	IsSkill			= false;
	bool	IsRight			= false;
	bool	IsShooting		= false;
	bool	IsUI_ON			= false;

	int		mState			=  0;		//������ ����
	int		FriendIndex		= -1;		//������ ��ġ �ε���
	int		WeaponIndex		=  0;		//������ ���� �ε���
	int		ColorIndex		=  0;		//������ �÷� �ε���
	int		HP				= 3000;		//���� ü��
	int		HP_MAX			= 3000;		//������ �ִ� ü��
	int		ComboCount		= 30;		//������ �⺻ �޺� ī����

	float	AttackRange		= 3.5f;		//���� ���� ����
	float	FightRange		= 30.0f;	//������ �ο� �� �ִ� �Ÿ�
	float	SkillRange		= 10.0f;	//������ ��ų �Ÿ�
	float	PlayerDistance	= 0.0f;		//�÷��̾��� �Ÿ�
	float	PositionY		= 6.0f;		//���� Y�� ������

	float	RendomSkillReadyTime		= 0.0f;
	float	RendomSkillReadyTimeMax		= 5.0f;
	float	RendomSkillPlayTime			= 0.0f;
	float	RendomSkillPlayTimeMax		= 5.0f;
	float	ChaseSkillPlayTime			= 0.0f;
	float	ChaseSkillPlayTimeMax		= 5.0f;

	Vector3 StartPoint;
	Vector3	SkillPoint[5];
	PhysRayCast* mRay;

	BossWeapon* Weapon[5];
	BossFriend* Friend;
	MeshFilter* ChildeMeshFilter[7];

	MeshFilterSetting	mMF_Setting;
	ParticleController* mParticle = nullptr;


	State* mBossIdle;
	State* mBossRendomAttack;
	State* mBossChaseAttack;
	State* mBossBaseAttack;
};

