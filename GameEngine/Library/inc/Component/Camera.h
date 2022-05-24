#pragma once
#include <DirectXMath.h>
#include <vector>
#include "Component.h"
#include "EaterEngineDLL.h"

/// <summary>
/// ī�޶� ���۳�Ʈ ���Ʈ������ �������� ��Ʈ������ �������ش�
/// </summary>
 
class Transform;
class CameraData;
class DebugBoxData;

class Camera : public Component
{
public:
	EATER_ENGINEDLL Camera();
	virtual ~Camera();

	virtual void Awake();
	virtual void Update();

	//����ī�޶� ����ī�޶�� �����Ų��
	EATER_ENGINEDLL void ChoiceMainCam();
	EATER_ENGINEDLL void ChoiceCameraAnimation(std::string Name);
	//EATER_ENGINEDLL void LookAt(GameObject* obj);
	//EATER_ENGINEDLL void LookAt(Vector3 Pos);
	// Camera Position
	DirectX::SimpleMath::Vector3 GetPos();
	//���� ī�޶��� �����
	DirectX::SimpleMath::Matrix GetView();
	//�������� ����� �����´�
	DirectX::SimpleMath::Matrix GetProj();
	//ī�޶��� ������
	CameraData* GetCameraData();

public:
	//OnResize ���� ����� �Լ�
	void SetSize(int Change_Width, int Change_Height);
	void CreateProj(int winsizeX,int WinSizeY);
	EATER_ENGINEDLL void SetCulling(bool cull);

private:
	Transform* tranform;

	//�������� ��� �����ϱ�
	void CreateView();

	//��������
	DirectX::SimpleMath::Matrix mProj;
	DirectX::SimpleMath::Matrix mOrthoProj;

	//��
	DirectX::SimpleMath::Matrix mView;

	//��������
	DirectX::BoundingFrustum mFrustum;

	//ī�޶� ����Ʈ���� ���� �ε���
	int MyIndex;

	//ī�޶󸮽�Ʈ�� ���ڽ��� �о�ִ´�
	void PushCamList();
	//ī�޶� ����Ʈ

	CameraData* mCameraData;
public:
	static std::vector<Camera*> CamList;
	//����ī�޶�
	static Camera* g_MainCam;
};