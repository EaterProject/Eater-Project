#pragma once
#include "Component.h"

/// <summary>
/// ������� Ű��ǲ���޾� �����̴� ī�޶�
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

	//���� Ʈ������
	Transform*	mTransform;
	//����ĳ������ Ʈ������
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