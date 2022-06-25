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

CameraManager::CameraManager()
{



}

CameraManager::~CameraManager()
{



}


void CameraManager::Awake()
{
	//카메라 오브젝트 생성
	GameObject* mPlayerCamera = Instance_Camera("Camera");
	mPlayerCamera->SetTag("PlayerCamera");
	mPlayerCamera->AddComponent<PlayerCamera>();
	mPlayer = mPlayerCamera->GetComponent<Camera>();

	//시네마틱 오브젝트 생성
	mCinematic = gameobject->GetComponent<Camera>();

	GameObject* mDebugCamera = GetDebugCamera();
	mDebug = mDebugCamera->GetComponent<Camera>();
}

void CameraManager::Start()
{
	//타이틀 씬을 위한 카메라 각도 조정
	Transform* mTransform = gameobject->GetTransform();
	mTransform->SetPosition(-2.410f, 2.453f, 19.626f);
	mTransform->SetRotate(1.570f, 53.320f, 0.00f);
	mCinematic->ChoiceMainCam();
}


void CameraManager::Update()
{
	if (IsCinematic_Play == true)
	{
		if (mCinematic->ChoiceCameraAnimationEnd() == true)
		{
			switch (CinematicType)
			{
			case MESSAGE_CAMERA_CINEMATIC_GAME_START:
				MessageManager::GetGM()->SEND_Message(TARGET_GLOBAL, MESSAGE_GLOBAL_GAMESTART);
				break;
			case MESSAGE_CAMERA_CINEMATIC_GAME_END:
				break;
			case MESSAGE_CAMERA_CINEMATIC_BOSS_START:
				break;
			case MESSAGE_CAMERA_CINEMATIC_BOSS_END:
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
		break;
	case MESSAGE_CAMERA_CHANGE_DEBUG:
		mDebug->ChoiceMainCam();
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

	mCinematic->ChoiceCameraAnimation(Name);
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

	mCinematic->ChoiceCameraAnimation(Name);
}
