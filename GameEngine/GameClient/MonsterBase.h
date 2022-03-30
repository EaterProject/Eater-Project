#pragma once
#include "Component.h"
class MonsterBase : public Component
{
public:
	MonsterBase();
	virtual ~MonsterBase();
public:
	bool isLife = false;
	void Awake();
protected:
	void Reset();//현재 몬스터의 상태를 초기화 시킨다
	void Create(float HP, float AttackPower, float AttackTime);	//초기값 설정
	void ErrorCheck();
	void SetPosition(float x, float y, float z);

	enum class STATE
	{
		ATTACK,
		MOVE,
		DEAD,
		CREATE,
	};

	float HP;				//체력
	float AttackPower;		//공격력
	float AttackDelayTime;	//공격 주기
	STATE State;
private:
	//변하지않는 초기값
	float Start_HP;
	float Start_AttackPower;
	float Start_AttackDelayTime;
	STATE Start_State;
};

