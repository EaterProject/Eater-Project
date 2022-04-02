#pragma once
#include "ClientBaseManager.h"
#include "MonsterBase.h"
#include "MemoryPool.h"

class MonsterManager : public ClientBaseManager 
{
public:
	MonsterManager() = default;
	~MonsterManager() = default;

	virtual void Initialize() override;
private:
	MemoryPool MonsterList;
};

