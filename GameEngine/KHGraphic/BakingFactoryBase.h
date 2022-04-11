#pragma once
#include "FactoryBase.h"

interface IBakingFactory : public FactoryBase
{
public:
	friend class FactoryManager;

public:
	virtual void PreBakeShadowMap(std::string path) abstract;
	virtual void PreBakeIBLMap() abstract;
};