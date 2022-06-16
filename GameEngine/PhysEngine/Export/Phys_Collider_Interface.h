#pragma once
#include "PhysEngineDLL.h"
#include "BaseData.h"
class Phys_Collider_Interface
{
public:
	Phys_Collider_Interface()	= default;
	~Phys_Collider_Interface()	= default;

	virtual void Initialize()	= 0;
	virtual void Release()		= 0;
	
	PHYS_ENGINEDLL virtual void SetTrigger(bool mTrigger);
	bool IsTrigger;
};

