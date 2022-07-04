#include "LightManager.h"
#include "EngineData.h"
#include "GlobalDataManager.h"

std::vector<Light*> LightManager::g_DirectionLightList;
std::vector<Light*> LightManager::g_PointLightList;
std::vector<Light*> LightManager::g_SpotLightList;

LightManager::LightManager()
{
}

LightManager::~LightManager()
{

}

void LightManager::ActiveLight(Light* light, bool Active)
{
	bool Check = false;

	if (Active)
	{
		Check = FindLight(light);

		if (Check == true) return;

		PushLight(light);
	}
	else
	{
		Check = FindLight(light);

		if (Check == false) return;

		DeleteLight(light);
	}
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
		GlobalDataManager::g_GlobalData->DirectionLightList.push_back(light->GetDirectionLight());
	}
	break;
	case LIGHT_TYPE::POINT_LIGHT:
	{
		light->SetIndex((UINT)g_PointLightList.size());

		// 새로운 Light 추가..
		g_PointLightList.push_back(light);
		GlobalDataManager::g_GlobalData->PointLightList.push_back(light->GetPointLight());
	}
	break;
	case LIGHT_TYPE::SPOT_LIGHT:
	{
		light->SetIndex((UINT)g_SpotLightList.size());

		// 새로운 Light 추가..
		g_SpotLightList.push_back(light);
		GlobalDataManager::g_GlobalData->SpotLightList.push_back(light->GetSpotLight());
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
	GlobalData* globalData = GlobalDataManager::g_GlobalData;

	// Light Data 제거..
	switch (light->GetType())
	{
	case LIGHT_TYPE::DIRECTION_LIGHT:
		globalData->DirectionLightList.erase(std::next(globalData->DirectionLightList.begin(), index));
		g_DirectionLightList.erase(std::next(g_DirectionLightList.begin(), index));
		lightList = &g_DirectionLightList;
		break;
	case LIGHT_TYPE::POINT_LIGHT:
		globalData->PointLightList.erase(std::next(globalData->PointLightList.begin(), index));
		g_PointLightList.erase(std::next(g_PointLightList.begin(), index));
		lightList = &g_PointLightList;
		break;
	case LIGHT_TYPE::SPOT_LIGHT:
		globalData->SpotLightList.erase(std::next(globalData->SpotLightList.begin(), index));
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

bool LightManager::FindLight(Light* light)
{
	switch (light->GetType())
	{
	case LIGHT_TYPE::DIRECTION_LIGHT:
	{
		if (g_DirectionLightList[light->m_LightIndex] == light)
		{
			return true;
		}
	}
	return false;
	case LIGHT_TYPE::POINT_LIGHT:
	{
		if (g_PointLightList[light->m_LightIndex] == light)
		{
			return true;
		}
	}
	return false;
	case LIGHT_TYPE::SPOT_LIGHT:
	{
		if (g_SpotLightList[light->m_LightIndex] == light)
		{
			return true;
		}
	}
	return false;
	default:
		return false;
	}

	return false;
}
