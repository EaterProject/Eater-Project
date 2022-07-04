#pragma once
#include "Component.h"
#include "ClientTypeOption.h"
#include "MeshFilterSetting.h"
#include <string>

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

	void Set_Boss_Active(bool Active);

private:
	AnimationController* mAnimation = nullptr;
	MeshFilter*		mMeshFilter		= nullptr;
	MeshFilter*		mSkinFilter		= nullptr;
	Transform*		mTransform		= nullptr;
	Collider*		mColider		= nullptr;
	Transform*		mPlayerTR		= nullptr;
	Rigidbody*		mRigidbody		= nullptr;
private:
	std::string ANIMATION_NAME[25];	//�ִϸ��̼� �̸�
	float ANIMATION_TIME[25];		//�ִϸ��̼� �ð�
	bool  STATE_START[25];			//���� ���� ����
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

	void Boss_Chase_Attack_Ready(int Phase);
	void Boss_Chase_Attack_Play(int Phase);
	void Boss_Chase_Attack_End(int Phase);

	void Boss_Rendom_Attack_Ready(int Phase);
	void Boss_Rendom_Attack_Play(int Phase);
	void Boss_Rendom_Attack_End(int Phase);
	void Boss_Rendom_Attack_Reset(int Phase);
	void Boss_Rendom_Attack_AddSkill();
	void Boss_Rendom_Attack_Big();

	void Boss_Guidedmissile_Attack_Start();
	void Boss_Guidedmissile_Attack_Play();
	void Boss_Guidedmissile_Attack_End();


	void Boss_Hit();
	void Phase_UP_Start();
	void Phase_UP_End();
private:
	void	SetState(BOSS_STATE State);	//���¸� ��ȯ
	bool	FirstState();				//���� ���°� ó������ üũ
	float	PlayerDistanceCheck();		//�÷��̾��� �Ÿ��� üũ
	void	CreateSkillPoint();			//��ų ��� ��ġ�� ����
	void	CreateSkillPoint_Chase();
	void	GroundCheck();
	void	SkillCheck();
	void	PushPlayer();
	void	BossColorUpdate();

private:

	bool	IsHit			= false;
	bool	IsSkill			= false;
	bool	IsRight			= false;
	bool	IsShooting		= false;
	bool	IsBossFriend	= false;
	bool	IsUpdateColor	= false;
	bool	IsAttack		= false;
	bool	IsStart			= false;

	bool    IsCredit		= false;

	int		mState			= 0;		//������ ����
	int		FriendIndex		= -1;		//������ ��ġ �ε���
	int		BossPhase		= 0;
	int		ColorType		= 0;

	float	AttackRange		= 3.5f;		//���� ���� ����
	float	FightRange		= 30.0f;	//������ �ο� �� �ִ� �Ÿ�
	float	SkillRange		= 10.0f;	//������ ��ų �Ÿ�
	float	PlayerDistance	= 0.0f;		//�÷��̾��� �Ÿ�
	int		HP				= 3000;		//���� ü��
	float	PositionY		= 6.0f;		//���� Y�� ������

private:
	//��ų�� ��Ÿ��
	 
	//���ǰ��� ��Ÿ��
	float	RendomSkillPlayTime			= 0.0f;
	float	RendomSkillPlayTimeMax		= 1.0f;
	
	float SkillTime[6]		= { 0.0f, };
	float SkillTimeMax[6]	= { 0.0f, };


	//�� ��ų ��Ÿ�� 
	const int Rendom_Time	= 0;
	const int Chase_Time	= 1;
	const int Teleport_Time = 2;
	const int Base_Time		= 3;
	const int Groggy_Time	= 4;
	const int Missile_Time	= 5;
private:
	Vector3 StartPoint;
	Vector3	SkillPoint[5];
	Vector3 SkillPoint_01[5];
	PhysRayCast* mRay;

	BossWeapon* Weapon[5];
	BossWeapon* BigWeapon;
	BossFriend* Friend;
	MeshFilter* ChildeMeshFilter[7];

	MeshFilterSetting mMF_Setting;

	ParticleController* mBaseAttackParticle;
	ParticleController* mPushParticle;
	ParticleController* mCountAttackParticle;
	ParticleController* mBossPhaseParticle[5];
};

