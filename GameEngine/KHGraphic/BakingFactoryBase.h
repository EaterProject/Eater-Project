#pragma once
#include "FactoryBase.h"

interface IBakingFactory : public FactoryBase
{
public:
	friend class FactoryManager;

public:
	virtual void PreBakeIBLMap() abstract;
};