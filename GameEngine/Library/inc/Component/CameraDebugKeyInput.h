#pragma once

#include "Component.h"
class Transform;
class Camera;
class CameraDebugKeyInput : public Component
{
public: 
	CameraDebugKeyInput();
	~CameraDebugKeyInput();

	void Update() override;
	void Start() override;
	void Awake() override;

private:
	Transform* mTransform;
	float mStartMousePosX;
	float mStartMousePosY;

	float mLastMousePosX;
	float mLastMousePosY;
	Camera* Cam;

	float speed				= 10;
	const float MaxSpeed	= 50;
	int Count = 0;
};

