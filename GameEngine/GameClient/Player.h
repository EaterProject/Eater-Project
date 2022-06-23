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

#define PLAYER_STATE_IDLE		0x00000001
#define PLAYER_STATE_ATTACK_01	0x00000010
#define PLAYER_STATE_ATTACK_02	0x00000100
#define PLAYER_STATE_SKILL_01	0x00001000
#define PLAYER_STATE_SKILL_02	0x00010000
#define PLAYER_STATE_SKILL_03	0x00100000
#define PLAYER_STATE_JUMP		0x01000000
#define PLAYER_STATE_MOVE		0x10000000

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

	static Transform* GetPlayerTransform();
	static bool GetAttackState();
	void Healing(float HealingPower);
private:
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
private:
	Vector3 DirPos;						//����
	Vector3 DirRot;						//ȸ��
	Vector3 PastDirRot;					//������ ����
	Vector3 BasePos;					//(0,0,0)�� ����
	Vector3 WeaponOffsetRot;			//���� �浹ü �ʱ� ������
	Vector3 AttackStartRot;				//������ ������������ ����
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
private:
	///State����
	unsigned int mState = 0x00000000;
	int AttackKeyDownCount	= 0;
	int ChangeCount			= 0;
	int HP					= 100;
	float Speed				= 10;
	const float MaxSpeed	= 10;
	bool IsAttack			= false;
	bool IsMove				= false;
	bool IsHit				= false;
	bool IsCreate			= false;
	static bool IsAttackTime;
	int	ComboCount;
	float		ComboTime	= 0.0f;
	float		HitTime		= 0.0f;
	MeshFilterSetting		mColor;
private:
	///Animation ����
	std::string AnimationName;
	int AnimationSpeed	= 5;
	int AttackEndFrame	= 0;
	int NowFrame		= 0;

	float Animation_Idle_Speed		= 1.5f;
	float Animation_Move_Speed		= 1.0f;
	float Animation_Attack01_Speed	= 1.5f;
	float Animation_Attack02_Speed	= 1.5f;
	float Animation_Skill01_Speed	= 1.5f;
	float Animation_Skill02_Speed	= 1.5f;
	float Animation_Jump_Speed		= 1.5f;
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
	float AttackDir				= 2.0f; //���� �Ÿ�
	float AttackRange			= 1.5f;
	const float Attack01_Dir	= 0.75f;
	const float Attack02_Dir	= 0.75f;
	const float Skill01_Dir		= 0.75f;
	const float Skill02_Dir		= 0.75f;
	const float Skill03_Dir		= 0.75f;
};