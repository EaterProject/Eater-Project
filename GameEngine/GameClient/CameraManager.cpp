#include "CameraManager.h"
#include "Camera.h"
#include "EaterEngineAPI.h"
#include "MessageManager.h"

//Component
#include "Camera.h"
#include "CameraDebugKeyInput.h"
#include "GameObject.h"
#include "PlayerCamera.h"
#include "Transform.h"
#include "CinematicCamera.h"

CameraManager::CameraManager()
{



}

CameraManager::~CameraManager()
{



}


void CameraManager::Awake()
{
	//카메라 오브젝트 생성
	GameObject* playerCamera = Instance_Camera("Camera");
	playerCamera->SetTag("PlayerCamera");
	mPlayerCamera = playerCamera->AddComponent<PlayerCamera>();
	mPlayer = playerCamera->GetComponent<Camera>();

	//시네마틱 오브젝트 생성
	mCinematicCamera = gameobject->AddComponent<CinematicCamera>();

	GameObject* debugCamera = GetDebugCamera();
	mDebug = debugCamera->GetComponent<Camera>();
}

void CameraManager::Start()
{
	//타이틀 씬을 위한 카메라 각도 조정
	Transform* mTransform = gameobject->GetTransform();
	mTransform->SetPosition(-2.410f, 2.453f, 19.626f);
	mTransform->SetRotate(1.570f, 53.320f, 0.00f);
	mCinematicCamera->ChoiceMainCam();
}

void CameraManager::Update()
{
	if (IsCinematic_Play == true)
	{
		if (mCinematicCamera->ChoiceCameraAnimationEnd() == true)
		{
			switch (CinematicType)
			{
			case MESSAGE_CAMERA_CINEMATIC_GAME_START:
				//MessageManager::GetGM()->SEND_Message(TARGET_GLOBAL, MESSAGE_GLOBAL_GAMESTART);
				break;
			case MESSAGE_CAMERA_CINEMATIC_GAME_END:
				break;
			case MESSAGE_CAMERA_CINEMATIC_BOSS_START:
				break;
			case MESSAGE_CAMERA_CINEMATIC_BOSS_END:
				break;
			case MESSAGE_CAMERA_CINEMATIC_TITLE:
				break;
			}

			IsCinematic_Play = false;	
		}
	}
}

void CameraManager::SetPlayerCamera(GameObject* Obj)
{
	if (mPlayer == nullptr)
	{
		mPlayer = Obj->GetComponent<Camera>();
	}
	mPlayer->ChoiceMainCam();
}

void CameraManager::Change(int Type)
{
	switch (Type)
	{
	case MESSAGE_CAMERA_CHANGE_PLAYER:
		mPlayer->ChoiceMainCam();
		mCinematicCamera->SetChangeCamera(mPlayer);
		break;
	case MESSAGE_CAMERA_CHANGE_DEBUG:
		mDebug->ChoiceMainCam();
		mCinematicCamera->SetChangeCamera(mDebug);
		break;
	}
}

void CameraManager::SetCinematic(int Type,std::string& Name)
{
	IsCinematic_Play = true;
	CinematicType = Type;
	
	GameObject* Object = nullptr;
	Object = GetMainCamera();
	mMain = Object->GetComponent<Camera>();

	//디버깅 카메라
	Object = GetDebugCamera();
	mDebug = Object->GetComponent<Camera>();

	mCinematicCamera->PushCameraAnimation(Name);
}

void CameraManager::SetCinematic(int Type, std::string&& Name)
{
	IsCinematic_Play = true;
	CinematicType = Type;

	GameObject* Object = nullptr;
	Object = GetMainCamera();
	mMain = Object->GetComponent<Camera>();

	//디버깅 카메라
	Object = GetDebugCamera();
	mDebug = Object->GetComponent<Camera>();

	mCinematicCamera->PushCameraAnimation(Name);
}

void CameraManager::SetCinematic(int Type)
{
	IsCinematic_Play = true;
	CinematicType = Type;

	GameObject* Object = nullptr;
	Object = GetMainCamera();
	mMain = Object->GetComponent<Camera>();

	//디버깅 카메라
	Object = GetDebugCamera();
	mDebug = Object->GetComponent<Camera>();

	//시네마틱 타입에 따른 카메라 애니메이션 설정..
	switch (CinematicType)
	{
	case MESSAGE_CAMERA_CINEMATIC_GAME_START:
		GameStartCinematic();
		break;
	case MESSAGE_CAMERA_CINEMATIC_GAME_END:
		GameEndCinematic();
		break;
	case MESSAGE_CAMERA_CINEMATIC_BOSS_START:
		BossStartCinematic();
		break;
	case MESSAGE_CAMERA_CINEMATIC_BOSS_END:
		BossEndCinematic();
		break;
	case MESSAGE_CAMERA_CINEMATIC_TITLE:
		TitleCinematic();
		break;
	}
}

void CameraManager::SetMouseFix(bool Active)
{
	mPlayerCamera->SetMouseFix(Active);
}

void CameraManager::GameStartCinematic()
{
	mCinematicCamera->PushCameraAnimation("06_Camera_Start");
}

void CameraManager::GameEndCinematic()
{
	mCinematicCamera->PushCameraAnimation("05_Camera_Ending");
}

void CameraManager::BossStartCinematic()
{
	mCinematicCamera->PushCameraAnimation("00_Camera_Boss_Spawn");
	mCinematicCamera->PushCameraAnimation("01_Camera_Boss_Spawn1");
	mCinematicCamera->PushCameraAnimation("02_Camera_Boss_Spawn2");
	mCinematicCamera->PushCameraAnimation("03_Camera_Boss_Spawn3");
	mCinematicCamera->PushCameraAnimation("04_Camera_Boss_Spawn4");
}

void CameraManager::BossEndCinematic()
{
	mCinematicCamera->PushCameraAnimation("");
}

void CameraManager::TitleCinematic()
{
	//mCinematic->PushCameraAnimation("07_Camera_Title");
	mCinematicCamera->PushCameraAnimation("TitleCam");
}
