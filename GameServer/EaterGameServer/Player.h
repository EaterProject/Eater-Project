#pragma once

#include "Object.h"

class Player : public Object
{
private:
	BaseData m_Prev_Data;

public:
	Player();
	virtual ~Player();

	void BackUp_PrevData();
	void Create();
	virtual void Update();
};