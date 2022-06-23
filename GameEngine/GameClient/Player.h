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
	void PlayerKeyinput();				//플레이어 키인풋
	void PlayerAttackColliderUpdate();	//플레이어 공격 Collider
	void PlayerGroundCheck();			//플레이어 땅체크
	void PlayerState_Attack();			//플레이어 공격 상태 일때
	void PlayerState_Base();			//플레이어 기본 상태 일때
	bool PlayerEndFrameCheck();			//플레이어 현재 애니메이션이 끝났는지 체크
	void PlayerHitTimeCheck();
private:
	void Player_Attack_01();			//기본공격 1
	void Player_Attack_02();			//기본공격 2
	void Player_Skill_01();				//스킬 1
	void Player_Skill_02();				//스킬 2
	void Player_Skill_03();				//스킬 3
	void Player_Jump();					//점프
	void Player_Hit(int HitPower);
	bool Player_Move_Check();			//기본 상태 체크
private:
	Vector3 DirPos;						//방향
	Vector3 DirRot;						//회전
	Vector3 PastDirRot;					//과거의 방향
	Vector3 BasePos;					//(0,0,0)인 벡터
	Vector3 WeaponOffsetRot;			//무기 충돌체 초기 오프셋
	Vector3 AttackStartRot;				//공격을 시작했을때의 방향
	bool Attack_Rot;					//공격회전을 한번만 구하기위해

	//컨퍼넌트
	static Transform*		mTransform;
	AnimationController*	mAnimation;
	MeshFilter*				mMeshFilter;
	Transform*				mCameraTR;
	Transform*				WeaponTR;
	Collider*				mCollider;
	Collider*				AttackCollider;

	//오브젝트
	GameObject*				WeaponObject;
	GameObject*				AttackColliderObject;
private:
	///State관련
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
	///Animation 관련
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
	///Raycast 관련
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
	float AttackDir				= 2.0f; //공격 거리
	float AttackRange			= 1.5f;
	const float Attack01_Dir	= 0.75f;
	const float Attack02_Dir	= 0.75f;
	const float Skill01_Dir		= 0.75f;
	const float Skill02_Dir		= 0.75f;
	const float Skill03_Dir		= 0.75f;
};