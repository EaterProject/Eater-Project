#pragma once
#include "State.h"
class Boss_BaseAttack :public State
{
public:
	Boss_BaseAttack();
	~Boss_BaseAttack();

	
	// State을(를) 통해 상속됨
	virtual void SetObject(GameObject* Object) override;
	virtual int Play() override;

private:
	int Phase_1();
	int Phase_2();


	bool isAttack	 = false;
	bool RightAttack = false;
	float AttackRange = 3.5f;

	int mStateNumber = 0;
};

