#include "BaseManager.h"
#include "LightManager.h"
#include "EngineData.h"

std::vector<Light*> LightManager::g_DirectionLightList;
std::vector<Light*> LightManager::g_PointLightList;
std::vector<Light*> LightManager::g_SpotLightList;

LightManager::LightManager()
{
}

LightManager::~LightManager()
{

}

void LightManager::PushLight(Light* light)
{
	// Light Type에 따른 Global Data 삽입..
	switch (light->GetType())
	{
	case LIGHT_TYPE::DIRECTION_LIGHT:
	{
		light->SetIndex((UINT)g_DirectionLightList.size());

		// 새로운 Light 추가..
		g_DirectionLightList.push_back(light);
		Global->mDirectionLights.push_back(light->GetDirectionLight());
	}
	break;
	case LIGHT_TYPE::POINT_LIGHT:
	{
		light->SetIndex((UINT)g_PointLightList.size());

		// 새로운 Light 추가..
		g_PointLightList.push_back(light);
		Global->mPointLights.push_back(light->GetPointLight());
	}
	break;
	case LIGHT_TYPE::SPOT_LIGHT:
	{
		light->SetIndex((UINT)g_SpotLightList.size());

		// 새로운 Light 추가..
		g_SpotLightList.push_back(light);
		Global->mSpotLights.push_back(light->GetSpotLight());
	}
	break;
	default:
		break;
	}
}

void LightManager::DeleteLight(Light* light)
{
	std::vector<Light*>* lightList = nullptr;
	UINT index = light->m_LightIndex;

	// Light Data 제거..
	switch (light->GetType())
	{
	case LIGHT_TYPE::DIRECTION_LIGHT:
		Global->mDirectionLights.erase(std::next(Global->mDirectionLights.begin(), index));
		g_DirectionLightList.erase(std::next(g_DirectionLightList.begin(), index));
		lightList = &g_DirectionLightList;
		break;
	case LIGHT_TYPE::POINT_LIGHT:
		Global->mPointLights.erase(std::next(Global->mPointLights.begin(), index));
		g_PointLightList.erase(std::next(g_PointLightList.begin(), index));
		lightList = &g_PointLightList;
		break;
	case LIGHT_TYPE::SPOT_LIGHT:
		Global->mSpotLights.erase(std::next(Global->mSpotLights.begin(), index));
		g_SpotLightList.erase(std::next(g_SpotLightList.begin(), index));
		lightList = &g_SpotLightList;
		break;
	default:
		return;
	}

	// Light Index 재설정..
	for (int index = 0; index < lightList->size(); index++)
	{
		(*lightList)[index]->SetIndex(index);
	}
}
