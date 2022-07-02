#pragma once
#include "State.h"

class AnimationController;
class Transform;
class Boss_Idle :public State
{
public:
	Boss_Idle();
	virtual ~Boss_Idle();

	// StateInterface��(��) ���� ��ӵ�
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

	float	AttackRange		= 3.5f;		//���� ���� ����
	float	FightRange		= 30.0f;	//������ �ο� �� �ִ� �Ÿ�
	float	PlayerDistance	= 0.0f;		//
};

