#pragma once
#include "MonsterComponent.h"
class MonsterB  :public MonsterComponent
{
public:
	MonsterB();
	virtual ~MonsterB();
public:
	void Awake() override;
	void SetUp() override;
	void Start() override;
	void Update() override;
	void Debug() override;
	void OnTriggerStay(GameObject* Obj) override;
private:
};

