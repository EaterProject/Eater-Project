#include "CameraDebugKeyInput.h"
#include "Transform.h"
#include "GameObject.h"
#include "KeyinputManager.h"
#include "TimeManager.h"
#include "Camera.h"
#include "EaterEngineAPI.h"
CameraDebugKeyInput::CameraDebugKeyInput()
{
	mTransform = nullptr;
	speed = 10;
	mLastMousePosX = 0;
	mLastMousePosY = 0;
}

CameraDebugKeyInput::~CameraDebugKeyInput()
{

}

void CameraDebugKeyInput::Awake()
{
	mTransform = gameobject->GetTransform();
}
void CameraDebugKeyInput::Start()
{
	Cam = gameobject->GetComponent<Camera>();
}

void CameraDebugKeyInput::Update()
{
	float delta = mTimeManager->DeltaTime();

	if (Cam->g_MainCam != Cam) { return; }

	if (mKeyInputManger->GetKeyDown(VK_RBUTTON))
	{
		//���콺 �����ʹ�ư�� ���������� ��ġ���� ����
		//mKeyInputManger->SetMouseCenter();
		mLastMousePosX = GetMousePosX();
		mLastMousePosY = GetMousePosY();
		//���콺 �������
		ShowCursor(false);
	}

	if (mKeyInputManger->GetKeyUp(VK_RBUTTON))
	{
		mLastMousePosX = GetMousePosX();
		mLastMousePosY = GetMousePosY();
		//���콺 ��Ÿ����
		ShowCursor(true);
	}

	if (mKeyInputManger->GetKey(VK_RBUTTON))
	{
		if (mKeyInputManger->GetKey(VK_LSHIFT))
		{
			speed = MaxSpeed;
		}
		else
		{
			speed = 10;
		}

		if (mKeyInputManger->GetKey('D'))
		{
			mTransform->SetLocalPosition((speed * delta), 0, 0);
		}

		if (mKeyInputManger->GetKey('A'))
		{
			mTransform->SetLocalPosition(-(speed * delta), 0, 0);
		}

		if (mKeyInputManger->GetKey('W'))
		{
			mTransform->SetLocalPosition(0, 0, (speed * delta));
		}

		if (mKeyInputManger->GetKey('S'))
		{
			mTransform->SetLocalPosition(0, 0, -(speed * delta));
		}

		if (mKeyInputManger->GetKey('Q'))
		{
			mTransform->SetLocalPosition(0, (speed * delta), 0);
		}

		if (mKeyInputManger->GetKey('E'))
		{
			mTransform->SetLocalPosition(0, -(speed * delta), 0);
		}

		float RotSpeed = 5;

		if (mKeyInputManger->GetKey(VK_RBUTTON) == true)
		{

			float x = (float)mKeyInputManger->GetMousePos()->x;
			float y = (float)mKeyInputManger->GetMousePos()->y;

			float dx = DirectX::XMConvertToRadians(static_cast<float>(x - mLastMousePosX));
			float dy = DirectX::XMConvertToRadians(static_cast<float>(y - mLastMousePosY));

			mTransform->SetRotate(-dy * RotSpeed, -dx * RotSpeed, 0);
			
			SetCursorPos(mLastMousePosX, mLastMousePosY);
		}
	}
}


