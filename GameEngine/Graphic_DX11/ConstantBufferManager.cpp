#include "ConstantBufferManager.h"

GlobalData* ConstantBufferManager::g_GlobalData = nullptr;
RenderOption* ConstantBufferManager::g_RenderOption = nullptr;

CB_Light ConstantBufferManager::LIGHT_BUFFER;
CB_LightSub ConstantBufferManager::LIGHT_SUB_BUFFER;

void ConstantBufferManager::Update()
{
	const CameraData* cam = g_GlobalData->MainCamera_Data;
	const Matrix& viewprojtex = cam->CamViewProjTex;

	// Light Buffer
	LIGHT_BUFFER.gDirLightCount = (UINT)g_GlobalData->DirectionLightList.size();
	LIGHT_BUFFER.gPointLightCount = (UINT)g_GlobalData->PointLightList.size();
	LIGHT_BUFFER.gSpotLightCount = (UINT)g_GlobalData->SpotLightList.size();

	for (UINT d = 0; d < LIGHT_BUFFER.gDirLightCount; d++)
	{
		if (d >= DIRECTION_LIGHT_COUNT) break;

		LIGHT_BUFFER.gDirLights[d] = *g_GlobalData->DirectionLightList[d];
	}
	for (UINT p = 0; p < LIGHT_BUFFER.gPointLightCount; p++)
	{
		if (p >= POINT_LIGHT_COUNT) break;

		LIGHT_BUFFER.gPointLights[p] = *g_GlobalData->PointLightList[p];
	}
	for (UINT s = 0; s < LIGHT_BUFFER.gSpotLightCount; s++)
	{
		if (s >= SPOT_LIGHT_COUNT) break;

		LIGHT_BUFFER.gSpotLights[s] = *g_GlobalData->SpotLightList[s];
	}

	// Light Sub Buffer
	LIGHT_SUB_BUFFER.gEyePosW = cam->CamPos;
	LIGHT_SUB_BUFFER.gViewProjTex = viewprojtex;
	LIGHT_SUB_BUFFER.gIBLFactor = g_RenderOption->SkyLight_Factor;
}
