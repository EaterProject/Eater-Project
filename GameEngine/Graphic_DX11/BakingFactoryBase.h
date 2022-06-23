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
	virtual void BakeSkyLightMap(TextureBuffer* resource, bool hdri, SkyLightBuffer* pResource) abstract;
	virtual void BakeConvertCubeMap(TextureBuffer* resource, float angle, float threshold, bool hdri, TextureBuffer* pResource) abstract;
	
public:
	virtual void SaveConvertCubeMap(TextureBuffer* resource, std::string SaveName) abstract;
	virtual void SaveSpriteToVolumeTexture(TextureBuffer* resource, std::string saveName, UINT pixelSize, TextureBuffer* pResource) abstract;
	virtual void SaveSpriteToVolumeTexture(std::string fileName, std::string saveName, UINT pixelSize) abstract;
};