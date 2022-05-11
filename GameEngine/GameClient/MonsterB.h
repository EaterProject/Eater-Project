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
	void Awake();
	void SetUp();
	void Update();

	void Move();
	void Attack();
	void Idle();
	void Debug();
private:
	ManaStone* Mana;
	int PointIndex = 0;
};

