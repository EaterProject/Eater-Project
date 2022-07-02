#include "CinematicCamera.h"
#include "EngineData.h"
#include "GameObject.h"
#include "Transform.h"
#include "Camera.h"
#include "TimeManager.h"
#include "LoadManager.h"

EATER_ENGINEDLL CinematicCamera::CinematicCamera()
	:isAnimation(false), NowAnimationFrame(0), NowFrameIndex(0), Cam_Animation(nullptr), PrevCamera(nullptr)
{
	// ī�޶� ������Ʈ���� ���� ������Ʈ �ؾ���..
	DefaultUpdate_Order = FUNCTION_ORDER_FIRST;
}

CinematicCamera::~CinematicCamera()
{

}

void CinematicCamera::Awake()
{
	tranform = gameobject->transform;
	NowCamera = gameobject->GetComponent<Camera>();
}

void CinematicCamera::Update()
{
	if (isAnimation == true)
	{
		// ���� ������� �ִϸ��̼��� ���ų� ���� �ִϸ��̼��� �ִٸ� �ִϸ��̼� ����..
		if (Cam_Animation == nullptr)
		{
			Cam_Animation = AnimationList.front();
			AnimationList.pop();
		}
		else
		{
			NowAnimationFrame += mTimeManager->DeltaTime();
			if (NowAnimationFrame >= Cam_Animation->OneFrame)
			{
				NowAnimationFrame = 0;
			}
		}

		if (NowFrameIndex >= Cam_Animation->Position.size())
		{
			// ������ �ִϸ��̼��� ���..
			if (AnimationList.empty())
			{
				isAnimation = false;

				// ���� ������ ī�޶�� ����..
				PrevCamera->ChoiceMainCam();
			}

			// ���� �ִϸ��̼� ����..
			Cam_Animation = nullptr;

			NowFrameIndex = 0;
		}
		else
		{
			tranform->SetPosition(Cam_Animation->Position[NowFrameIndex]);
			tranform->SetRotate(Cam_Animation->Rotation[NowFrameIndex]);
			NowFrameIndex++;
		}
	}
}

void CinematicCamera::PushCameraAnimation(std::string Name)
{
	CameraAnimation* ani = LoadManager::GetCamAnimation(Name);

	if (ani == nullptr) return;

	// �ִϸ��̼� ť�� ����..
	AnimationList.push(ani);

	isAnimation = true;
	NowCamera->ChoiceMainCam();
}

bool CinematicCamera::ChoiceCameraAnimationEnd()
{
	return (isAnimation == true) ? false : true;
}

void CinematicCamera::ChoiceMainCam()
{
	if (NowCamera == nullptr) return;
	
	NowCamera->ChoiceMainCam();
}

void CinematicCamera::SetChangeCamera(Camera* cam)
{
	PrevCamera = cam;
}
