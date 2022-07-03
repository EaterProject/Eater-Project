#include "PlayerCamera.h"
#include "Camera.h"
#include "GameObject.h"
#include "Transform.h"
#include "EaterEngineAPI.h"
#include "windows.h"

PlayerCamera::PlayerCamera()
{
	MainCam = nullptr;
	Userobject = nullptr;
	mTransform = nullptr;
	mUserTransform = nullptr;

	PastX = 0;
	PastY = 0;
	NowX = 0;
	NowY = 0;
	X_Radian = 0;
	Y_Radian = 0;

	CamOffSet = { 0,1.75f,0 };
	MouseCursor = false;
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
	mTransform->SetPosition(0.0f, 0.0f, -2.0f);

	//MouseCursor = GetTogle(VK_F10);
	//ShowMouseCursor(MouseCursor);
	//MouseCursorClip(MouseCursor);
	//MainCam->ChoiceCameraAnimation("StartCam");
	//SetMousePosCenter();
	//GetWindowSize(&PastX, &PastY);
	//PastX = PastX / 2;
	//PastY = PastY / 2;
	//NowX = PastX;
	//NowY = PastY;
	//SetCursorPos(960, 540);
	//SetMousePosCenter();
	
}

void PlayerCamera::Update()
{
	float DTime = GetDeltaTime();

	// ������..
	if (GetKeyDown(VK_TAB))
	{
		if (MouseCursor == true)
		{
			MouseCursor = false;
			ShowMouseCursor(true);
		}
		else
		{
			MouseCursor = true;
			ShowMouseCursor(false);
		}
	}

	if (MouseCursor == false) { return; }

	POINT Pos;
	::GetCursorPos(&Pos);

	NowX = Pos.x;
	NowY = Pos.y;

	//���콺 ��ġ�� �����´�
	float MosSpeed = 15;

	float Dir = 4.25f;
	float MosControl = 250;

	X_Radian += -((NowX - 960)* MosSpeed) * DTime;
	Y_Radian +=  ((NowY - 540) * MosSpeed) * DTime;


	if (Y_Radian >= 120) { Y_Radian = 120; }
	if (Y_Radian <= -80) { Y_Radian = -80; }

	//���� ���������� X Y�� ���Ѵ�
	float Horizontal_X = (float)-sin(X_Radian / MosControl) * Dir;
	float Horizontal_Z = (float)cos(X_Radian / MosControl) * Dir;

	//���� ���������� Y �� ������ �Ÿ��� X,Z ��ǥ�� ratio �� ���ϱ����� cos(��Ÿ)�� ���Ѵ�.
	float Vertical_Y = (float)sin(Y_Radian / MosControl) * Dir;
	float X_Z_Ratio = (float)cos(Y_Radian / MosControl);

	//Ÿ���� ��ġ�� �����´�
	if (Userobject != nullptr)
	{
		Vector3 TargetPos = Userobject->GetTransform()->GetPosition();

		//���� ī�޶��� ��ġ�� ���
		float X = (Horizontal_X * X_Z_Ratio);
		float Y = Vertical_Y;
		float Z = Horizontal_Z * X_Z_Ratio;

		//ī�޶��� ��ġ��
		mTransform->SetPosition(X + TargetPos.x + CamOffSet.x, Y + TargetPos.y + CamOffSet.y, Z + TargetPos.z + CamOffSet.z);
		
		//ī�޶��� ȸ����
		Vector3 rotation = mTransform->GetRotation();
		mTransform->SetRotate(-(Y_Radian / MosControl) * 180 / 3.141592f, (X_Radian / MosControl) * 180 / 3.141592f - 180.0f, rotation.z);
	}

	//���� ������ ����� ������
	SetCursorPos(960, 540);
}

void PlayerCamera::Debug()
{
	//DebugPrint("X : %.2f", mTransform->GetRotation().x);
	//DebugPrint("Y : %.2f", mTransform->GetRotation().y);
}

void PlayerCamera::SetMouseFix(bool Active)
{
	MouseCursor = Active;

	if (MouseCursor)
	{
		SetCursorPos(960, 540);
	}

	ShowMouseCursor(Active ^ true);
}

