#pragma once
#include "MonsterComponent.h"
class MonsterA :public MonsterComponent
{
public:
	MonsterA();
	virtual ~MonsterA();
public:
	void Awake()	override;
	void SetUp()	override;
	void Start()	override;
	void Update()	override;
	void Debug()	override;
private:
};

