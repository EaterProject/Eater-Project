#include "PlayerCamera.h"
#include "Camera.h"
#include "GameObject.h"
#include "Transform.h"
#include "EaterEngineAPI.h"

PlayerCamera::PlayerCamera()
{
	MainCam			= nullptr;
	Userobject		= nullptr;
	mTransform		= nullptr;
	mUserTransform	= nullptr;

	PastX	= 0;
	PastY	= 0;
	NowX = 0;
	NowY = 0;
	X_Radian = 0;
	Y_Radian = 0;

	CamOffSet = { 0,1.75f,0 };
	MouseCursor = true;
}

PlayerCamera::~PlayerCamera()
{


}

void PlayerCamera::SetUp()
{
	Userobject = FindGameObjectTag("Player");
	MainCam = gameobject->GetComponent<Camera>();
	mTransform = gameobject->GetTransform();

	PastX = GetMousePosX();
	PastY = GetMousePosY();
	mTransform->Position = { 0, 0 ,-2 };

	//MouseCursor = GetTogle(VK_F10);
	ShowMouseCursor(MouseCursor);
	MouseCursorClip(MouseCursor);
	//SetMousePosCenter();
	//MainCam->ChoiceCameraAnimation("StartCam");
}

void PlayerCamera::StartUpdate()
{
	if (MainCam->ChoiceCameraAnimationEnd() == false) { return; }

	//마우스 위치를 가져온다
	float MosSpeed = 50;
	NowX = -GetMousePosX();
	NowY = GetMousePosY();
	
	//마우스의 이동량을 계산한다 
	float x = (NowX - PastX);
	float y = (NowY - PastY);
	
	X_Radian += x;
	Y_Radian += y;
	
	if (Y_Radian >= 120) { Y_Radian = 120; }
	if (Y_Radian <= -80) { Y_Radian = -80; }
	
	
	float Dir = 4.25f;
	float MosControl = 250;
	
	//수평 방향으로의 X Y를 구한다
	float Horizontal_X = (float)-sin(X_Radian / MosControl) * Dir;
	float Horizontal_Z = (float)cos(X_Radian / MosControl) * Dir;
	
	//수직 방향으로의 Y 축 까지의 거리와 X,Z 좌표의 ratio 를 구하기위한 cos(세타)를 구한다.
	float Vertical_Y	= (float)sin(Y_Radian / MosControl) * Dir;
	float X_Z_Ratio		= (float)cos(Y_Radian / MosControl);
	
	//타겟의 위치를 가져온다
	Vector3 TargetPos = Userobject->GetTransform()->Position;
	
	//최종 카메라의 위치를 계산
	float X = Horizontal_X * X_Z_Ratio;
	float Y = Vertical_Y;
	float Z = Horizontal_Z * X_Z_Ratio;
	
	//카메라의 위치값
	mTransform->Position.x = (X + TargetPos.x + CamOffSet.x);
	mTransform->Position.y = (Y + TargetPos.y + CamOffSet.y);
	mTransform->Position.z = (Z + TargetPos.z + CamOffSet.z);
	
	//카메라의 회전값
	mTransform->Rotation.x = -(Y_Radian / MosControl) * 180 / 3.141592f;
	mTransform->Rotation.y = (X_Radian / MosControl) * 180 / 3.141592f - 180.0f;
	
	
	//키를 눌렀을때
	if (GetKeyDown(VK_F10) == true)
	{
		MouseCursor = GetTogle(VK_F10);
		ShowMouseCursor(MouseCursor);
		MouseCursorClip(MouseCursor);
	}
	
	//이전 프레임 마우스위치를 저장
	PastX = NowX;
	PastY = NowY;
}

