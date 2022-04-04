#include "EngineData.h"
#include "BaseManager.h"
#include "Camera.h"
#include "Light.h"

GlobalData* BaseManager::Global = nullptr;

void BaseManager::Initialize()
{
	Global = new GlobalData();
	Global->TexSpace = Matrix(	0.5f, 0.0f, 0.0f, 0.0f,
								0.0f, -0.5f, 0.0f, 0.0f,
								0.0f, 0.0f, 1.0f, 0.0f,
								0.5f, 0.5f, 0.0f, 1.0f	);
}

void BaseManager::Reset()
{
	if (Global != nullptr)
	{
		delete Global;
		Global = nullptr;
	}
}

void BaseManager::UpdateGlobalData(float dTime)
{
	//카메라 데이터
	CameraData* camData = Camera::g_MainCam->GetCameraData();

	// 메인 카메라가 바뀌었을 경우 Data 변경
	if (Global->Camera_Data != camData)
	{
		Global->Camera_Data = camData;
	}

	// 시간
	Global->Time = dTime;
}

GlobalData* BaseManager::GetGlobalData()
{
	return Global;
}
