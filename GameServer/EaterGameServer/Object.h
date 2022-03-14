#pragma once

#include "Rigidbody.h"
#include "BaseData.h"

class Object : public Rigidbody, public BaseData
{
public:
	Object();
	virtual ~Object();

public:
	virtual void Update();
	virtual void Create();
};

