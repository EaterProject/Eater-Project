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
	
	//PastX = GetMousePosX();
	//PastY = GetMousePosY();
	mTransform->Position = { 0, 0 ,-2 };
	
	//MouseCursor = GetTogle(VK_F10);
	//ShowMouseCursor(MouseCursor);
	//MouseCursorClip(MouseCursor);
	//MainCam->ChoiceCameraAnimation("StartCam");
	SetMousePosCenter();
	PastX = 960;
	PastY = 530;
	NowX = 960;
	NowY = 530;
}

void PlayerCamera::StartUpdate()
{
	if (MainCam->ChoiceCameraAnimationEnd() == false) { return; }
	if (GetKeyDown(VK_ESCAPE))
	{
		if (MouseCursor == true)
		{
			MouseCursor = false;
			ShowMouseCursor(true);
		}
		else
		{
			SetMousePosCenter();
			MouseCursor = true;
			ShowMouseCursor(false);
			PastX = 960;
			PastY = 530;
		}
	}
	
	if (MouseCursor == false) { return; }
	
	//���콺 ��ġ�� �����´�
	float MosSpeed = 50;
	
	NowX = GetMousePosX();
	NowY = GetMousePosY();
	
	//���콺�� �̵����� ����Ѵ� 
	int x = (NowX - PastX);
	int y = (NowY - PastY);
	
	X_Radian += -x;
	Y_Radian +=  y;
	
	if (Y_Radian >= 120) { Y_Radian = 120; }
	if (Y_Radian <= -80) { Y_Radian = -80; }
	
	
	float Dir = 4.25f;
	float MosControl = 250;
	
	//���� ���������� X Y�� ���Ѵ�
	float Horizontal_X = (float)-sin(X_Radian / MosControl) * Dir;
	float Horizontal_Z = (float)cos(X_Radian / MosControl) * Dir;
	
	//���� ���������� Y �� ������ �Ÿ��� X,Z ��ǥ�� ratio �� ���ϱ����� cos(��Ÿ)�� ���Ѵ�.
	float Vertical_Y	= (float)sin(Y_Radian / MosControl) * Dir;
	float X_Z_Ratio		= (float)cos(Y_Radian / MosControl);
	
	//Ÿ���� ��ġ�� �����´�
	if (Userobject != nullptr)
	{
		Vector3 TargetPos = Userobject->GetTransform()->Position;
	
		//���� ī�޶��� ��ġ�� ���
		float X = Horizontal_X * X_Z_Ratio;
		float Y = Vertical_Y;
		float Z = Horizontal_Z * X_Z_Ratio;
	
		//ī�޶��� ��ġ��
		mTransform->Position.x = (X + TargetPos.x + CamOffSet.x);
		mTransform->Position.y = (Y + TargetPos.y + CamOffSet.y);
		mTransform->Position.z = (Z + TargetPos.z + CamOffSet.z);
	
		//ī�޶��� ȸ����
		mTransform->Rotation.x = -(Y_Radian / MosControl) * 180 / 3.141592f;
		mTransform->Rotation.y = (X_Radian / MosControl) * 180 / 3.141592f - 180.0f;
	}
	SetMousePosCenter();
}

