#pragma once
#include "MonsterComponent.h"
class Boss :public MonsterComponent
{
public:
	Boss();
	virtual ~Boss();

	void Awake() override;
	void SetUp() override;
	void Update() override;
	void Debug() override;
	void OnTriggerStay(GameObject* Obj) override;

public:
	

};

