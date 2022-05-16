#pragma once
#include "FactoryBase.h"

class TextureBuffer;
class SkyLightBuffer;

interface IBakingFactory : public FactoryBase
{
public:
	friend class FactoryManager;

public:
	virtual void PreBakeShadowMap(std::string fileName) abstract;
	virtual void PreBakeEnvironmentMap(SkyLightBuffer* resource) abstract;
	virtual void BakeConvertCubeMap(TextureBuffer* resource, float angle, bool save_file, TextureBuffer* pResource) abstract;
};