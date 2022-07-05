#pragma once
#include "NetworkComponent.h"
#include "MeshFilterSetting.h"
#include <queue>
#include "PlayerData.h"

class AnimationController;
class Transform;
class MeshFilter;
class Camera;
class Rigidbody;
class FrameData;
class Collider;
class PhysRayCast;
class ParticleController;

#define PLAYER_STATE_IDLE		0x00000001
#define PLAYER_STATE_ATTACK_01	0x00000010
#define PLAYER_STATE_ATTACK_02	0x00000100
#define PLAYER_STATE_SKILL_01	0x00001000
#define PLAYER_STATE_SKILL_02	0x00010000
#define PLAYER_STATE_SKILL_03	0x00100000
#define PLAYER_STATE_JUMP		0x01000000
#define PLAYER_STATE_MOVE		0x10000000
#define PLAYER_STATE_DEAD		0x11111111

class Player :public Component
{
public:
	Player();
	virtual ~Player();
	void Awake()	 override;
	void SetUp()	 override;
	void Start()	 override;
	void Update() override;
	
	void SetMessageRECV(int Type, void* Data);
	void SetKeyState(bool Active);

	static Transform* GetPlayerTransform();
	static bool GetNoHitState();
	static bool GetAttackState();
	static int	GetPlayerColor();
	static int	GetPlayerCombo();
	static float GetPlayerPower();
	static float GetPlayerComboPower();
	void Healing(float HealingPower);
	void SetNoHit(bool Active);

private:
	void PlayerCoolTimeCheck();			//�÷��̾� ��ų ��Ÿ�� üũ
	void PlayerKeyinput();				//�÷��̾� Ű��ǲ
	void PlayerAttackColliderUpdate();	//�÷��̾� ���� Collider
	void PlayerGroundCheck();			//�÷��̾� ��üũ
	void PlayerState_Attack();			//�÷��̾� ���� ���� �϶�
	void PlayerState_Base();			//�÷��̾� �⺻ ���� �϶�
	bool PlayerEndFrameCheck();			//�÷��̾� ���� �ִϸ��̼��� �������� üũ
	void PlayerHitTimeCheck();
private:
	void Player_Attack_01();			//�⺻���� 1
	void Player_Attack_02();			//�⺻���� 2
	void Player_Skill_01();				//��ų 1
	void Player_Skill_02();				//��ų 2
	void Player_Skill_03();				//��ų 3
	void Player_Jump();					//����
	void Player_Hit(int HitPower);
	bool Player_Move_Check();			//�⺻ ���� üũ
	void Player_Push();
	void Player_Dead();
private:
	void ChangeSkyLight(int index);
	void SetMeshFilterList();

	void Get_PureMana(int count);
	void Get_CoreMana(int count);
	void Upgrade_Change_Emagin();
	void Upgrade_Max_HP();
	void Upgrade_Attack_Speed();
	void Upgrade_Move_Speed();
private:
	/// ��ġ �ִ밪 ����
	const int ChangeEmagin_PureMana_Count = 10;
	const int HP_CoreMana_Count = 1;
	const int AttackSpeed_PureMana_Count = 15;
	const int MoveSpeed_PureMana_Count = 5;

	const int Max_Mana_Value = 99;

	const int Max_ChangeEmagin_Value = 15;
	const int Max_HP_Value = 5500;
	const float Max_AttackSpeed_Animation = 2.1f;
	const float Max_Move_Speed_Value = 10.0f;
	const float Max_MoveSpeed_Animation = 1.35f;

	/// ��ų ��Ÿ��
	const float Skill_E_CoolTime = 1.0f;
	const float Skill_MR_CoolTime = 1.0f;
	const float Skill_SPC_CoolTime = 1.0f;

	float Skill_E_CoolTime_Percent;
	float Skill_MR_CoolTime_Percent;
	float Skill_SPC_CoolTime_Percent;

	/// 1ȸ ��ȭ ��ġ
	const int Upgrade_ChangeEmagin = 1;
	const int Upgrade_HP = 500;
	const float Upgrade_AttackSpeed_1 = 0.12f;
	const float Upgrade_AttackSpeed_2 = 0.15f;
	const float Upgrade_MoveSpeed_Value = 0.5f;
	const float Upgrade_MoveSpeed_Animation = 0.1f;

private:
	std::string ANIMATION_NAME[8]	= { "", };
	float		ANIMATION_SPEED[8]	= { 0.0f };

	float Now_Attack_Speed = 1.5f;
	float MoveSoundTime = 0.0f;

	Vector3 DirPos;						//����
	Vector3 DirRot;						//ȸ��
	Vector3 PastDirRot;					//������ ����
	Vector3 PastDirPos;
	Vector3 BasePos;					//(0,0,0)�� ����
	Vector3 WeaponOffsetRot;			//���� �浹ü �ʱ� ������
	Vector3 AttackStartRot;				//������ ������������ ����
	Vector3 PushPoint;
	Vector3	PushNomal;
	bool Attack_Rot;					//����ȸ���� �ѹ��� ���ϱ�����

	//���۳�Ʈ
	static Transform*		mTransform;
	AnimationController*	mAnimation;
	MeshFilter*				mMeshFilter;
	Transform*				mCameraTR;
	Transform*				WeaponTR;
	Collider*				mCollider;
	Collider*				AttackCollider;

	//������Ʈ
	GameObject*				WeaponObject;
	GameObject*				AttackColliderObject;

	std::vector<MeshFilter*> ChildMeshFilter;

private:
	///State����
	unsigned int mState = 0x00000000;
	int AttackKeyDownCount	= 0;

	static int ChangeCount;
	static int MaxChangeCount;

	int HP					= 3000;
	int HP_Max				= 3000;

	float Speed				= 7.5f;
	float SpeedMax			= 7.5f;

	bool IsAttack			= false;
	bool IsMove				= false;
	bool IsHit				= false;
	bool IsCreate			= false;
	bool IsKeyUpdate		= false;
	bool IsPush				= false;

	bool IsGetPureMana		= false;
	bool IsGetCoreMana		= false;

	static bool		IsNoHit;
	static bool		IsAttackTime;
	static int		ComboCount;
	static float	PlayerPower;
	static float	PlayerComboPower;

	float		Test = 0.0f;
	float		ComboTime	= 0.0f;
	float		HitTime		= 0.0f;
	MeshFilterSetting		mPlayerColor;
	MeshFilterSetting		mWeaponColor;
private:
	///Mana ����
	int PureMana = 0;
	int CoreMana = 0;

private:
	/// ��ų ��Ÿ�� ����
	float E_CoolTime = 0.0f;
	float MR_CoolTime = 0.0f;
	float SPC_CoolTime = 0.0f;

private:
	///Animation ����
	int AnimationSpeed	= 5;
	int AttackEndFrame	= 0;
	int NowFrame		= 0;
private:
	///Raycast ����
	PhysRayCast* RayCastHit;
	const int Ray_Front		= 0;
	const int Ray_Back		= 1;
	const int Ray_Right		= 2;
	const int Ray_Left		= 3;
	const int Ray_Center	= 4;
	bool IsFrontRayCheck	= false;
	bool IsBackRayCheck		= false;
	bool IsRightRayCheck	= false;
	bool IsLeftRayCheck		= false;
	int HitCount			= 0;
private:
	float AttackDir				= 1.0f; //���� �Ÿ�
	float AttackRange			= 1.5f;
	const float Attack01_Dir	= 0.75f;
	const float Attack02_Dir	= 0.75f;
	const float Skill01_Dir		= 0.75f;
	const float Skill02_Dir		= 0.75f;
	const float Skill03_Dir		= 0.75f;

	ParticleController* mAttackParticle = nullptr;
	ParticleController* mHealParticle = nullptr;
}; 