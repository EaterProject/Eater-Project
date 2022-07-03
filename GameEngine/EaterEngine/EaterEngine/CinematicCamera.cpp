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
	// 카메라 업데이트보다 먼저 업데이트 해야함..
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
		// 현재 재생중인 애니메이션이 없거나 다음 애니메이션이 있다면 애니메이션 설정..
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
			// 마지막 애니메이션일 경우..
			if (AnimationList.empty())
			{
				isAnimation = false;

				// 이전 상태의 카메라로 변경..
				if (PrevCamera)
				{
					PrevCamera->ChoiceMainCam();
				}
			}

			// 현재 애니메이션 비우기..
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

	// 애니메이션 큐에 삽입..
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
