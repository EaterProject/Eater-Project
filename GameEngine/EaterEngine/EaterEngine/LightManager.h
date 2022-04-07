#pragma once
#include <vector>
#include "Light.h"

class LightManager
{
public:
	LightManager();
	~LightManager();

public:
	static void PushLight(Light* light);
	static void DeleteLight(Light* light);

private:
	static std::vector<Light*> g_DirectionLightList;
	static std::vector<Light*> g_PointLightList;
	static std::vector<Light*> g_SpotLightList;
};

