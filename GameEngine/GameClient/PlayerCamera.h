#pragma once
#include "Component.h"

/// <summary>
/// 사용자의 키인풋을받아 움직이는 카메라
/// </summary>
class Camera;
class GameObject;
class Transform;
class PlayerCamera : public Component
{
public:
	PlayerCamera();
	~PlayerCamera();

	void SetUp();
	void StartUpdate();

private:
	GameObject* Userobject;
	Camera*		MainCam;

	//나의 트렌스폼
	Transform*	mTransform;
	//유저캐릭터의 트랜스폼
	Transform*	mUserTransform;

	Vector3 CamOffSet;

	float X_Radian;
	float Y_Radian;
	int NowX;
	int NowY;
	int PastX;
	int PastY;

	bool MouseCursor;
	
};