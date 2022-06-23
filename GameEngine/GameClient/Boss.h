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
	std::string ANIMATION_NAME[16];	//애니메이션 이름
	float ANIMATION_TIME[16];		//애니메이션 시간
	bool  STATE_START[16];			//상태 시작 여부
private:
	//상태에 대한 함수들
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
	void Boss_Rendom_Attack_End();
	void Boss_Hit();
private:
	void	SetState(BOSS_STATE State);	//상태를 변환
	bool	FirstState();				//현재 상태가 처음인지 체크
	float	PlayerDistanceCheck();		//플레이어의 거리를 체크
	void	CreateSkillPoint();			//스킬 쏘는 위치를 생성
	void	GroundCheck();
private:
	bool	IsHit			= false;
	bool	IsSkill			= false;
	bool	IsRight			= false;
	bool	IsShooting		= false;

	int		mState			= 0;		//보스의 상태
	int		FriendIndex		= -1;		//보스의 위치 인덱스

	float	AttackRange		= 3.5f;		//근접 공격 범위
	float	FightRange		= 30.0f;	//보스와 싸울 수 있는 거리
	float	SkillRange		= 10.0f;	//보스의 스킬 거리
	float	PlayerDistance	= 0.0f;		//플레이어의 거리
	float	HP				= 100;		//보스 체력
	float	PositionY		= 6.0f;		//보스 Y축 오프셋

	float	RendomSkillReadyTime		= 0.0f;
	float	RendomSkillReadyTimeMax		= 5.0f;
	float	RendomSkillPlayTime			= 0.0f;
	float	RendomSkillPlayTimeMax		= 5.0f;

	Vector3 StartPoint;
	Vector3	SkillPoint[5];
	PhysRayCast* mRay;

	BossWeapon* Weapon[5];
	BossFriend* Friend;
	MeshFilter* ChildeMeshFilter[7];

	MeshFilterSetting mMF_Setting;
};

