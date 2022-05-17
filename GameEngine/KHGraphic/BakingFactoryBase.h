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
	virtual void PreBakeEnvironmentMap(TextureBuffer* resource, bool hdri, SkyLightBuffer* pResource) abstract;
	virtual void BakeConvertCubeMap(TextureBuffer* resource, float angle, bool save_file, bool hdri, TextureBuffer* pResource) abstract;
};