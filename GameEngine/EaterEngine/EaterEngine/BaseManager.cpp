#include "EngineData.h"
#include "BaseManager.h"
#include "Camera.h"
#include "Light.h"

GlobalData* BaseManager::Global = nullptr;

void BaseManager::Initialize()
{
	Global = new GlobalData();

	Global->mTexSpace = Matrix(	0.5f, 0.0f, 0.0f, 0.0f,
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
	Matrix camInvView;
	camInvView._11 = camView._11;	camInvView._12 = camView._12;	camInvView._13 = camView._13;
	camInvView._21 = camView._21;	camInvView._22 = camView._22;	camInvView._23 = camView._23;
	camInvView._31 = camView._31;	camInvView._32 = camView._32;	camInvView._33 = camView._33;
	
	Global->mCamView = camView;
	Global->mCamProj = camProj;
	Global->mCamInvView = camInvView.Invert();
	Global->mCamPos = Camera::g_MainCam->GetPos();

	Global->mCamVP = camView * camProj;

	// 시간
	Global->mTime = dTime;
}

GlobalData* BaseManager::GetGlobalData()
{
	return Global;
}
