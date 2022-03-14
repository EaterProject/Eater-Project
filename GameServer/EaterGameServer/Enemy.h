#pragma once

#include "Object.h"

class Enemy : public Object
{
private:
	BaseData m_Prev_Data;

public:
	Enemy();
	virtual ~Enemy();

	void BackUp_PrevData();
	void Create();
	virtual void Update();
};

