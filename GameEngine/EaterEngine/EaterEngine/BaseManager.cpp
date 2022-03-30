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
	Matrix camView = Camera::g_MainCam->GetView();
	Matrix camProj = Camera::g_MainCam->GetProj();

	Global->Camera_Data->CamView = camView;
	Global->Camera_Data->CamProj = camProj;
	Global->Camera_Data->CamViewProj = camView * camProj;
	Global->Camera_Data->CamInvView = camView.Invert();
	Global->Camera_Data->CamPos = Camera::g_MainCam->GetPos();

	Global->Camera_Data->BoundFrustum = Camera::g_MainCam->GetFrustum();

	// 시간
	Global->Time = dTime;
}

GlobalData* BaseManager::GetGlobalData()
{
	return Global;
}
