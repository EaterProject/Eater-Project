#include "GlobalDataManager.h"
#include "EngineData.h"

GlobalData* GlobalDataManager::g_GlobalData;

void GlobalDataManager::Initialize()
{
	if (g_GlobalData == nullptr)
	{
		g_GlobalData = new GlobalData();
	}
}

void GlobalDataManager::Release()
{
	if (g_GlobalData != nullptr)
	{
		delete g_GlobalData;
	}
}

void GlobalDataManager::Update(float dTime)
{
	g_GlobalData->Time = dTime;
}
