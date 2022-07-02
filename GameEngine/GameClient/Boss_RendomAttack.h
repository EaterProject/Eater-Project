#pragma once
#include "State.h"
class BossWeapon;
class PhysRayCast;
class Boss_RendomAttack :public State
{
public:
	Boss_RendomAttack();
	~Boss_RendomAttack();
	
	virtual void SetObject(GameObject* Object) override;
	virtual int Play() override;
private:
	int Phase_1();
	int Phase_2();
	void CreateSkillPoint();

	//스킬을 4단계로 분리
	void SkillStart();
	void SkillPlay();
	void SkillEnd();
	void SkillReset();
private:
	bool	StartOnePlay[4] = {false};
	bool	IsShooting = false;

	int		BossPhase		= 0;
	int		SkillNumber		= 0;
	int		WeaponIndex		= 0;
	int		mStateNumber	= 10;
	float	SkillRange		= 10.0f;

	float	RendomSkillPlayTime		= 0.0f;
	float	RendomSkillPlayTimeMax	= 5.0f;
private:
	Vector3 StartPoint;
	Vector3	SkillPoint[5];
private:
	BossWeapon*		Weapon[5] = { nullptr, };
	PhysRayCast*	mRay;
};

