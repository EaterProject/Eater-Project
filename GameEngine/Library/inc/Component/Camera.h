#pragma once
#include <DirectXMath.h>
#include <vector>
#include "Component.h"
#include "EaterEngineDLL.h"

/// <summary>
/// 카메라 컨퍼넌트 뷰매트릭스와 프로젝션 매트릭스를 생성해준다
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

	//현재카메라를 메인카메라로 변경시킨다
	EATER_ENGINEDLL void ChoiceMainCam();
	EATER_ENGINEDLL void ChoiceCameraAnimation(std::string Name);
	//EATER_ENGINEDLL void LookAt(GameObject* obj);
	//EATER_ENGINEDLL void LookAt(Vector3 Pos);
	// Camera Position
	DirectX::SimpleMath::Vector3 GetPos();
	//현재 카메라의 뷰행렬
	DirectX::SimpleMath::Matrix GetView();
	//프로젝션 행렬을 가져온다
	DirectX::SimpleMath::Matrix GetProj();
	//카메라의 데이터
	CameraData* GetCameraData();

public:
	//OnResize 에서 실행될 함수
	void SetSize(int Change_Width, int Change_Height);
	void CreateProj(int winsizeX,int WinSizeY);
	EATER_ENGINEDLL void SetCulling(bool cull);

private:
	Transform* tranform;

	//프로젝션 행렬 생성하기
	void CreateView();

	//프로젝션
	DirectX::SimpleMath::Matrix mProj;
	DirectX::SimpleMath::Matrix mOrthoProj;

	//뷰
	DirectX::SimpleMath::Matrix mView;

	//프러스텀
	DirectX::BoundingFrustum mFrustum;

	//카메라 리스트에서 나의 인덱스
	int MyIndex;

	//카메라리스트에 나자신을 밀어넣는다
	void PushCamList();
	//카메라 리스트

	CameraData* mCameraData;
public:
	static std::vector<Camera*> CamList;
	//메인카메라
	static Camera* g_MainCam;
};