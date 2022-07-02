#pragma once
#include <queue>
#include "Component.h"
#include "EaterEngineDLL.h"

class Camera;
class Transform;
class CameraAnimation;

class CinematicCamera : public Component
{
public:
	EATER_ENGINEDLL CinematicCamera();
	~CinematicCamera();

public:
	void Awake() override;
	void Update() override;

public:
	EATER_ENGINEDLL void PushCameraAnimation(std::string Name);
	EATER_ENGINEDLL bool ChoiceCameraAnimationEnd();
	EATER_ENGINEDLL void ChoiceMainCam();
	EATER_ENGINEDLL void SetChangeCamera(Camera* cam);

private:
	Transform* tranform;
	Camera* NowCamera;

	//카메라 애니메이션 데이터
	bool isAnimation;
	float NowAnimationFrame = 0;
	int	NowFrameIndex;

	std::queue<CameraAnimation*> AnimationList;
	CameraAnimation* Cam_Animation;
	Camera* PrevCamera;
};

