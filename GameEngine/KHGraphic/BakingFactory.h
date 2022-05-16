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
	void PreBakeEnvironmentMap(SkyLightBuffer* resource) override;
	void BakeConvertCubeMap(TextureBuffer* resource, float angle, bool save_file, TextureBuffer* pResource);

private:
	void PreBakeBRDFMap();
};

