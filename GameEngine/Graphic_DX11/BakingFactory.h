#pragma once
#include "BakingFactoryBase.h"

class BakingFactory : public IBakingFactory
{
public:
	BakingFactory();
	~BakingFactory();

public:
	void Create(int width, int height) override;
	void Start() override;
	void Release() override;

public:
	void PreBakeShadowMap(std::string fileName) override;
	void BakeLookUpTable3D(std::string fileName) override;
	void BakeSkyLightMap(TextureBuffer* resource, bool hdri, SkyLightBuffer* pResource) override;
	void BakeConvertCubeMap(TextureBuffer* resource, float angle, float threshold, bool hdri, TextureBuffer* pResource);

public:
	void SaveConvertCubeMap(TextureBuffer* resource, std::string SaveName) override;

private:
	void PreBakeBRDFMap();
};

