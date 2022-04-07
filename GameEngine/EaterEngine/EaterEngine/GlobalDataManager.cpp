#include "GlobalDataManager.h"
#include "EngineData.h"

GlobalData* GlobalDataManager::g_GlobalData;

void GlobalDataManager::Initialize()
{
	if (g_GlobalData == nullptr)
	{
		g_GlobalData = new GlobalData();

		// 기본 Texture 행렬 삽입..
		g_GlobalData->TexSpace = Matrix(0.5f, 0.0f, 0.0f, 0.0f,
										0.0f, -0.5f, 0.0f, 0.0f,
										0.0f, 0.0f, 1.0f, 0.0f,
										0.5f, 0.5f, 0.0f, 1.0f);
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
