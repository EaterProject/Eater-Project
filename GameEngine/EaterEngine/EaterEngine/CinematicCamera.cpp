#include "CinematicCamera.h"
#include "EngineData.h"
#include "GameObject.h"
#include "Transform.h"
#include "Camera.h"
#include "TimeManager.h"
#include "LoadManager.h"

EATER_ENGINEDLL CinematicCamera::CinematicCamera()
	:isAnimation(false), NowTickTime(0.0f), NowFrameRatio(0.0f), PrevFrameIndex(0), NextFrameIndex(0), Cam_Animation(nullptr), PrevCamera(nullptr)
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
			NowTickTime += mTimeManager->DeltaTime();
			
			if (NowTickTime >= Cam_Animation->TickFrame)
			{
				NextFrameIndex += (int)(NowTickTime * Cam_Animation->TickFrameRatio);
				PrevFrameIndex = NextFrameIndex - 1;

				if (PrevFrameIndex < 0)
				{
					PrevFrameIndex = 0;
				}

				NowTickTime = 0.0f;
			}
		}

		if (NextFrameIndex >= Cam_Animation->Position.size())
		{
			// ������ �ִϸ��̼��� ���..
			if (AnimationList.empty())
			{
				isAnimation = false;

				// ���� ������ ī�޶�� ����..
				if (PrevCamera)
				{
					PrevCamera->ChoiceMainCam();
				}
			}

			// ���� �ִϸ��̼� ����..
			Cam_Animation = nullptr;

			NextFrameIndex = 0;
			PrevFrameIndex = 0;
		}
		else
		{
			NowFrameRatio = NowTickTime * Cam_Animation->TickFrameRatio;

			NowPostion = Vector3::Lerp(Cam_Animation->Position[PrevFrameIndex], Cam_Animation->Position[NextFrameIndex], NowFrameRatio);
			NowRotation = Vector3::Lerp(Cam_Animation->Rotation[PrevFrameIndex], Cam_Animation->Rotation[NextFrameIndex], NowFrameRatio);

			tranform->SetPosition(NowPostion);
			tranform->SetRotate(NowRotation);
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
