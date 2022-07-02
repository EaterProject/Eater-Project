#pragma once
#include "State.h"
class BossWeapon;
class PhysRayCast;
class Boss_ChaseAttack :public State
{
public:
	Boss_ChaseAttack();
	~Boss_ChaseAttack();


	// State을(를) 통해 상속됨
	virtual int Play() override;
	virtual void SetObject(GameObject* Object) override;
private:
	int Phase_1();
	int Phase_2();

	void SkillStart();
	void SkillPlay();
	void SkillEnd();

	void CreateSkillPoint();
private:
	BossWeapon* Weapon[5] = { nullptr, };
	PhysRayCast* mRay;

	Vector3 SkillPoint[10];
	bool	IsShooting = false;
	bool	StartOnePlay[3] = { false };
	int		BossPhase = 0;
	int		mStateNumber = 7;
	int		SkillNumber  = 0;
	int		WeaponIndex  = 0;

	float	ChaseSkillPlayTime		= 0.0f;
	float	ChaseSkillPlayTimeMax	= 3.0f;

};

