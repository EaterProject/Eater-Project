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
	void Awake()	override;
	void SetUp()	override;
	void Update()	override;
	void OnTriggerStay(GameObject* Obj);

	void Move()		override;
	void Attack()	override;
	void Idle()		override;
	void Dead()		override;
	void Chase();
	void Debug();
private:
	ManaStone* Mana;
	GameObject* mPlayer;
	int PointIndex = 0;
	float Pos_Y;
};

