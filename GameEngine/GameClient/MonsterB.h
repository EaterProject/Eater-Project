#pragma once
#include "MonsterComponent.h"
class ManaStone;
class MonsterB  :public MonsterComponent
{
public:
	MonsterB();
	virtual ~MonsterB();
public:
	void Create(ManaStone* mMana, int mCreatePointIndex);
public:
	void Awake() override;
	void SetUp() override;
	void Update() override;

	void Move()		override;
	void Attack()	override;
	void Idle()		override;
	void Dead()	    override;
	void Debug()	override;
private:
	ManaStone* Mana;
	int PointIndex = 0;
};

