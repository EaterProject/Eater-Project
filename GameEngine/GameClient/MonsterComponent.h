#pragma once
#include "Component.h"
#include "ClientTypeOption.h"
#include "MeshFilterSetting.h"
#include <string>
#include <functional>
class PhysRayCast;
class MeshFilter;
class Transform;
class AnimationController;
class Collider;
class Rigidbody;
class ParticleController;
class MonsterComponent : public Component
{
public:
	MonsterComponent();
	virtual ~MonsterComponent();
public:
	virtual void Awake() override;
	virtual void SetUp() override;
	virtual void Start() override;
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
	Vector3 MovePoint;		//이동해야하는 지점
	Vector3 DirPoint;		//이동지점의 방향벡터
	Vector3 ReturnPoint;	//추격후 돌아가야하는 위치
protected:
	int		MonsterState	= 0;
	bool	MonsterFront_Z	= false;
	float	PlayerDistance  = 10;
protected:
	std::string ANIMATION_NAME[6];
	std::string SOUND_NAME[6];
	bool		STATE[6];
	float		ANIMATION_TIME[6];

	//처음 한번만 실행하기위한 변수들
	bool HitStart		= false;    //Hit	 상태 시작 변수
	bool HitFXStart		= false;	//Hit FX 상태 시작 변수
	bool IsAttack		= false;
	bool DissolveStart	= false;
protected:
	///애니메이션 이름 변수들
	std::string ModelName;
	std::string AnimationName;

	bool IsUI_ON = false;
	bool IsUI_OFF = false;
protected:
	///Move 상태 변수들
	float MoveSoundTime				= 0;
	float MoveSoundTimeMax			= 0.5;
protected:
	///Idle 상태 변수들
	float		IdleTime			= 0;		//현재 대기시간(건들면 안됨)
	float		Idle_MaxTime		= 0;		//최대값과 최소값을 설정한 최종 대기시간
	const int	Idle_MaxTime_Max	= 5;		//대기시간의 최대값
	const int	Idle_MaxTime_Min	= 2;		//대기시간의 최소값
protected:
	///Attack 상태 변수

protected:
	///Hit 상태 변수
	float HitTime				= 0.0f;			//공격을 맞았을때 시간 재는용(건들면 안됨)
	float HitMaxTime			= 2.0f;			//공격을 맞았을때 최대 무적시간
protected:
	///Chase 상태 변수들
	float ChaseTime				= 0.0f;			//추격하는 현재 시간(건들면 안됨)
	float ChaseEndTime			= 10.0f;		//추격을  중지하는 시간 
protected:
	///사정거리
	float AttackRange			= 2.5f;			//공격 거리
	float ChaseRange			= 5.5f;			//추격 거리
protected:
	float	Max_HP				= 100;			//현재 체력
	float	HP					= 100;			//현재 체력
	float	Speed				= 1.5f;			//이동 속도
	float	AttackTime			= 0;			//현재 공격 시간
	int		Damage				= 30;			//데미지
	float	RotationSpeed		= 200;			//몬스터 회전하는 속도
	int		PointNumber			= -1;			//몬스터 이동 포인터 인덱스
	int		ComboCount			= 6;			//현재 콤보 카운터
	float	MonsterScale		= 1.0f;			
	int		MonsterType			= 0;
	int		PureManaCount		= 0;			//퓨어마나 획득 개수
protected:
	void GetRandomColor();
	void SetMonsterColor();
	int		MonsterColor		= 0;			//건들면 안됨
protected:
	MeshFilterSetting	mMF_Setting;
	ParticleController* mParticleController;
	ParticleController* mHitController;
	std::function<void()> HitFunction;

};


