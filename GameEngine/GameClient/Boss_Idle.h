#pragma once
#include "State.h"

class AnimationController;
class Transform;
class Boss_Idle :public State
{
public:
	Boss_Idle();
	virtual ~Boss_Idle();

	// StateInterface을(를) 통해 상속됨
	virtual void SetMessage(void* Message) override;
	virtual void SetObject(GameObject* Object) override;
	virtual int Play() override;
private:
	int Phase_1();
	int Phase_2();

	void TargetDistance();
private:
	bool IsUI_ON = false;

	int BossPhase = 0;

	float	AttackRange		= 3.5f;		//근접 공격 범위
	float	FightRange		= 30.0f;	//보스와 싸울 수 있는 거리
	float	PlayerDistance	= 0.0f;		//
};

