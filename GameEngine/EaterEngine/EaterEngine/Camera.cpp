#include "Camera.h"
#include "GameObject.h"
#include "EngineData.h"
#include "Transform.h"
#include "GameEngine.h"
#include "EngineData.h"
#include "MainHeader.h"
#include "MainHeader.h"
#include "DebugManager.h"
#include "GlobalDataManager.h"
#include "Profiler/Profiler.h"

using namespace DirectX;
std::vector<Camera*> Camera::CamList;
Camera* Camera::g_MainCam = nullptr;

Camera::Camera()
{
	mCameraData = new CameraData();
	PushCamList();
}

Camera::~Camera()
{
	GlobalData* globalData = GlobalDataManager::g_GlobalData;
	
	if (g_MainCam == this)
	{
		g_MainCam = nullptr;
		globalData->MainCamera_Data = nullptr;
	}

	CamList[MyIndex] = nullptr;

	// Global Camera Data ����..
	for (int i = 0; i < globalData->CameraList.size(); i++)
	{
		if (globalData->CameraList[i] == mCameraData)
		{
			globalData->CameraList.erase(std::next(globalData->CameraList.begin(), i));
			break;
		}
	}

	delete mCameraData;
	mCameraData = nullptr;
}

void Camera::Awake()
{
	//������Ʈ�� ���۳�Ʈ ��������
	tranform = gameobject->transform;
	CreateProj(GameEngine::WinSizeWidth, GameEngine::WinSizeHeight);
	gameobject->OneMeshData->Object_Data->ObjType = OBJECT_TYPE::CAMERA;
}

void Camera::Update()
{
	if (g_MainCam == nullptr)
	{
		PROFILE_LOG(PROFILE_OUTPUT::CONSOLE, "[ Engine ][ Camera ][ Update ] ������ ���� ī�޶� �����ϴ�.");
	}

	CreateView();

	if (this != g_MainCam)
	{
		Transform* TR = gameobject->GetTransform();
		DebugManager::DebugDrawBox(TR->Scale, TR->Rotation, TR->Position, Vector3(0, 1, 0));
		DebugManager::DebugDrawLine(TR->Position, TR->GetLocalPosition_Look(), Vector3(1,1,0));
	}
}

DirectX::SimpleMath::Matrix Camera::GetProj()
{
	if (g_MainCam == nullptr)
	{
		PROFILE_LOG(PROFILE_OUTPUT::CONSOLE, "[ Engine ][ Camera ][ GetProj ] ������ ���� ī�޶� �����ϴ�.");
		return Matrix();
	}
	else
	{
		return mProj;
	}
}

CameraData* Camera::GetCameraData()
{
	return mCameraData;
}

DirectX::SimpleMath::Vector3 Camera::GetPos()
{
	if (g_MainCam == nullptr)
	{
		PROFILE_LOG(PROFILE_OUTPUT::CONSOLE, "[ Engine ][ Camera ][ GetPos ] ������ ���� ī�޶� �����ϴ�.");
		return Vector3();
	}
	else
	{
		return tranform->Position;
	}
}

void Camera::SetSize(int Change_Width, int Change_Height)
{
	float Ratio = (float)(Change_Width / Change_Height);
}

void Camera::ChoiceMainCam()
{
	//���� ����ī�޶��� �±׸� �׳� ī�޶�� ����
	g_MainCam->gameobject->SetTag("Camera");
	g_MainCam = nullptr;

	// Main Camera ����..
	g_MainCam = this;

	// Main Camera Data ����..
	GlobalDataManager::g_GlobalData->MainCamera_Data = g_MainCam->mCameraData;

	//�ٲ� ī�޶��� �±׸� ����ī�޶�� ����
	g_MainCam->gameobject->SetTag("MainCamera");
}

DirectX::SimpleMath::Matrix Camera::GetView()
{
	if (g_MainCam == nullptr)
	{
		PROFILE_LOG(PROFILE_OUTPUT::CONSOLE, "[ Engine ][ Camera ][ GetView ] ������ ���� ī�޶� �����ϴ�.");
		return Matrix();
	}
	else
	{
		return mView;
	}
}

void Camera::CreateProj(int winsizeX, int WinSizeY, bool ViewPoint)
{
	///�𸣸� MSDN ���� 
	///XMMatrixPerspectiveFovLH �Լ� �˻��ϸ��
	
	static XMMATRIX gTexSpace = XMMATRIX(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f);

	//�þ߰�
	float mFovY	= 0.25f * 3.1415926535f;
	//�� ���� X:Y ���μ��κ���
	float mAspect = (float)winsizeX / (float)WinSizeY;
	//������� ������ �Ÿ� 0���� Ŀ�ߵ�
	float mNearZ	= 0.1f;
	//�� ��� ������ �Ÿ� 0���� Ŀ�ߵ�
	float mFarZ	= 4000.0f;

	float mNearWindowHeight = 2.0f * mNearZ * tanf(0.5f * mFovY);
	float mFarWindowHeight = 2.0f * mFarZ * tanf(0.5f * mFovY);
	
	if (ViewPoint == false)
	{
		//���� ����
		mProj = DirectX::XMMatrixPerspectiveFovLH(mFovY, mAspect, mNearZ, mFarZ);
	}
	else
	{
		//���� ����
		mProj = DirectX::XMMatrixOrthographicLH(mFovY, mAspect, mNearZ, mFarZ);
	}

	// Camera Data �缳��..
	mCameraData->CamProj = mProj;
	BoundingFrustum::CreateFromMatrix(mCameraData->OriginFrustum, mProj);
}

void Camera::SetCulling(bool cull)
{
	mCameraData->Culling = cull;
}

void Camera::CreateView()
{
	DirectX::XMFLOAT3 r_ = gameobject->GetTransform()->GetLocalPosition_Right();
	DirectX::XMFLOAT3 u_ = gameobject->GetTransform()->GetLocalPosition_UP();
	DirectX::XMFLOAT3 l_ = gameobject->GetTransform()->GetLocalPosition_Look();


	DirectX::XMVECTOR R = XMLoadFloat3(&r_);
	DirectX::XMVECTOR U = XMLoadFloat3(&u_);
	DirectX::XMVECTOR L = XMLoadFloat3(&l_);
	DirectX::XMVECTOR P = XMLoadFloat3(&tranform->Position);

	L = XMVector3Normalize(L);
	U = XMVector3Normalize(XMVector3Cross(L, R));
	R = XMVector3Cross(U, L);

	float x = DirectX::XMVectorGetX(DirectX::XMVector3Dot(P, R));
	float y = DirectX::XMVectorGetX(DirectX::XMVector3Dot(P, U));
	float z = DirectX::XMVectorGetX(DirectX::XMVector3Dot(P, L));

	mView._11 = r_.x;	mView._12 = u_.x;	mView._13 = l_.x; mView._14 = 0;
	mView._21 = r_.y;	mView._22 = u_.y;	mView._23 = l_.y; mView._24 = 0;
	mView._31 = r_.z;	mView._32 = u_.z;	mView._33 = l_.z; mView._34 = 0;
	mView._41 = -x;		mView._42 = -y;		mView._43 = -z;	  mView._44 = 1;

	// Camera Data Update..
	mCameraData->CamView = mView;
	mCameraData->CamInvView = mView.Invert();
	mCameraData->CamViewProj = mView * mProj;
	mCameraData->CamPos = tranform->Position;
	mCameraData->CamLook = l_;
	mCameraData->CamLook.Normalize();
	mCameraData->OriginFrustum.Transform(mCameraData->BoundFrustum, mCameraData->CamInvView);
}

void Camera::PushCamList()
{
	//���� ����ī�޶�� �����ѳ��̾��ٸ� ó�������ѳ��� ����ī�޶��
	if (g_MainCam == nullptr)
	{
		g_MainCam = this;

		// Main Camera Data ����..
		GlobalDataManager::g_GlobalData->MainCamera_Data = g_MainCam->mCameraData;
	}

	//ī�޶� ����Ʈ�� �ִ���
	int count = (int)CamList.size();
	for (int i = 0; i < count; i++)
	{
		if (CamList[i] == nullptr)
		{
			CamList[i] = this;
			CamList[i]->MyIndex = i;
			return;
		}
	}

	//����� �Դٴ°��� ����Ʈ�� ��á�ٴ� ��
	CamList.push_back(this);

	//�ڱ��� �ε��� ����
	MyIndex = count;

	// Global Camera List ����..
	GlobalDataManager::g_GlobalData->CameraList.push_back(this->mCameraData);
}


