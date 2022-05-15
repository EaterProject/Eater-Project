#pragma once
#include "MonsterComponent.h"
class ManaStone;
class MonsterA :public MonsterComponent
{
public:
	MonsterA();
	virtual ~MonsterA();
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

