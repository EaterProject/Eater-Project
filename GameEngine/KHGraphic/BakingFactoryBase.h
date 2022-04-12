#pragma once
#include "FactoryBase.h"

class TextureBuffer;
class EnvironmentBuffer;

interface IBakingFactory : public FactoryBase
{
public:
	friend class FactoryManager;

public:
	virtual void PreBakeShadowMap(std::string fileName) abstract;
	virtual void PreBakeEnvironmentMap(EnvironmentBuffer* tex) abstract;
};