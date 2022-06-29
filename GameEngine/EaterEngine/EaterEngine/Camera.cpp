#include "Camera.h"
#include "GameObject.h"
#include "EngineData.h"
#include "Transform.h"
#include "GameEngine.h"
#include "EngineData.h"
#include "EaterEngineAPI.h"
#include "DebugManager.h"
#include "GlobalDataManager.h"
#include "Profiler/Profiler.h"
#include "LoadManager.h"

#include "Profiler/Profiler.h"

using namespace DirectX;
std::vector<Camera*> Camera::CamList;
Camera* Camera::g_MainCam = nullptr;
const Matrix  Camera::g_TexSpace = Matrix(0.5f, 0.0f, 0.0f, 0.0f,
										  0.0f, -0.5f, 0.0f, 0.0f,
										  0.0f, 0.0f, 1.0f, 0.0f,
										  0.5f, 0.5f, 0.0f, 1.0f);

Camera::Camera()
{
	mCameraData = new CameraData();
	PushCamList();
}

Camera::~Camera()
{
	GlobalData* globalData = GlobalDataManager::g_GlobalData;
	
	// 현재 카메라 제거..
	CamList[MyIndex] = nullptr;
	CamList.erase(std::next(CamList.begin(), MyIndex));

	// Global Camera Data 제거..
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

	if (g_MainCam == this)
	{
		Camera* otherCamera = *CamList.begin();

		if (otherCamera)
		{
			g_MainCam = otherCamera;
			globalData->MainCamera_Data = otherCamera->GetCameraData();
		}
	}
}

void Camera::Awake()
{
	//오브젝트의 컨퍼넌트 가져오기
	tranform = gameobject->transform;
	CreateProj(GameEngine::WinSizeWidth, GameEngine::WinSizeHeight);
	gameobject->OneMeshData->Object_Data->ObjType = OBJECT_TYPE::CAMERA;
}

void Camera::Update()
{
	if (isAnimation == true)
	{
		NowAnimationFrame += GetDeltaTime();
		if (NowAnimationFrame >= mAnimation->OneFrame)
		{
			NowAnimationFrame -= 0;
		}


		if (NowFrameIndex >= mAnimation->Position.size())
		{
			isAnimation = false;
			NowFrameIndex = 0;
			//MainCamera->ChoiceMainCam();
		}
		else
		{
			tranform->SetPosition(mAnimation->Position[NowFrameIndex]);
			tranform->SetRotate(mAnimation->Rotation[NowFrameIndex]);
			NowFrameIndex++;
		}
	}


	if (g_MainCam == nullptr)
	{
		PROFILE_LOG(PROFILE_OUTPUT::LOG_FILE, "[ Engine ][ Camera ][ Update ] 지정된 메인 카메라가 없습니다.");
	}

	CreateView();

	if (this != g_MainCam)
	{
		Transform* TR = gameobject->GetTransform();
		DebugManager::DebugDrawBox(TR->GetScale(), TR->GetRotation(), TR->GetPosition(), Vector3(0, 1, 0));
		DebugManager::DebugDrawLine(TR->GetPosition(), TR->GetLocalPosition_Look(), Vector3(1,1,0));
	}
}

DirectX::SimpleMath::Matrix Camera::GetProj()
{
	if (g_MainCam == nullptr)
	{
		PROFILE_LOG(PROFILE_OUTPUT::LOG_FILE, "[ Engine ][ Camera ][ GetProj ] 지정된 메인 카메라가 없습니다.");
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
		PROFILE_LOG(PROFILE_OUTPUT::LOG_FILE, "[ Engine ][ Camera ][ GetPos ] 지정된 메인 카메라가 없습니다.");
		return Vector3();
	}
	else
	{
		return tranform->GetPosition();
	}
}

void Camera::SetSize(int Change_Width, int Change_Height)
{
	float Ratio = (float)(Change_Width / Change_Height);
}

void Camera::ChoiceMainCam()
{
	//이전 메인카메라의 태그를 그냥 카메라로 변경
	g_MainCam->gameobject->SetTag("Camera");
	g_MainCam = nullptr;

	// Main Camera 지정..
	g_MainCam = this;

	CreateProj(GameEngine::WinSizeWidth, GameEngine::WinSizeHeight);

	// Main Camera Data 변경..
	GlobalDataManager::g_GlobalData->MainCamera_Data = g_MainCam->mCameraData;

	//바뀐 카메라의 태그를 메인카메라로 변경
	g_MainCam->gameobject->SetTag("MainCamera");
}

void Camera::ChoiceCameraAnimation(std::string Name)
{
	mAnimation = LoadManager::GetCamAnimation(Name);
	isAnimation = true;
	MainCamera = g_MainCam;
	ChoiceMainCam();
}

bool Camera::ChoiceCameraAnimationEnd()
{
	return (isAnimation == true) ? false : true ;
}

DirectX::SimpleMath::Vector2 Camera::WorldToScreen(float x, float y, float z)
{
	Vector4 world_pos = { x, y, z, 1.0f };
	Vector4 screen_pos = Vector4::Transform(world_pos, mCameraData->CamViewProj);

	if (screen_pos.z < 0.0f)
	{
		return Vector2(-1.0f, -1.0f);
	}

	return Vector2((screen_pos.x / screen_pos.w) * 0.5f + 0.5f, (-screen_pos.y / screen_pos.w) * 0.5f + 0.5f);
}

DirectX::SimpleMath::Vector2 Camera::WorldToScreen(DirectX::SimpleMath::Vector3 world)
{
	Vector4 world_pos = { world.x, world.y, world.z, 1.0f };
	Vector4 screen_pos = Vector4::Transform(world_pos, mCameraData->CamViewProj);

	if (screen_pos.z < 0.0f)
	{
		return Vector2(-1.0f, -1.0f);
	}

	return Vector2((screen_pos.x / screen_pos.w) * 0.5f + 0.5f, (-screen_pos.y / screen_pos.w) * 0.5f + 0.5f);
}

DirectX::SimpleMath::Matrix Camera::GetView()
{
	if (g_MainCam == nullptr)
	{
		PROFILE_LOG(PROFILE_OUTPUT::LOG_FILE, "[ Engine ][ Camera ][ GetView ] 지정된 메인 카메라가 없습니다.");
		return Matrix();
	}
	else
	{
		return mView;
	}
}

void Camera::CreateProj(int winsizeX, int WinSizeY)
{
	if (winsizeX == 0 || WinSizeY == 0) return;

	///모르면 MSDN 참고 
	///XMMatrixPerspectiveFovLH 함수 검색하면됨
	
	static XMMATRIX gTexSpace = XMMATRIX(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f);

	//시야각
	float mFovY	= 0.3f * 3.1415926535f;
	//뷰 공간 X:Y 가로세로비율
	float mAspect = (float)winsizeX / (float)WinSizeY;
	//근접평면 까지의 거리 0보다 커야됨
	float mNearZ	= 0.1f;
	//먼 평면 까지의 거리 0보다 커야됨
	float mFarZ	= 4000.0f;

	float mNearWindowHeight = 2.0f * mNearZ * tanf(0.5f * mFovY);
	float mFarWindowHeight = 2.0f * mFarZ * tanf(0.5f * mFovY);
	
	//원근 투영
	mProj = DirectX::XMMatrixPerspectiveFovLH(mFovY, mAspect, mNearZ, mFarZ);

	//직교 투영
	mOrthoProj = DirectX::XMMatrixOrthographicOffCenterLH(0.0f, winsizeX, WinSizeY, 0.0f, 0.0f, 1.0f);

	// Camera Data 재설정..
	mCameraData->CamProj = mProj;
	mCameraData->CamProjTex = mProj * gTexSpace;
	mCameraData->CamOrthoProj = mOrthoProj;

	// Camera Frustum 설정..
	BoundingFrustum::CreateFromMatrix(mCameraData->OriginFrustum, mProj);
}

void Camera::SetCulling(bool cull)
{
	mCameraData->Culling = cull;
}

void Camera::CreateView()
{
	Vector3 R = tranform->GetLocalPosition_Right();
	Vector3 U = tranform->GetLocalPosition_UP();
	Vector3 L = tranform->GetLocalPosition_Look();
	Vector3 P = tranform->GetPosition();

	mView._11 = R.x;	mView._12 = U.x;	mView._13 = L.x; mView._14 = 0;
	mView._21 = R.y;	mView._22 = U.y;	mView._23 = L.y; mView._24 = 0;
	mView._31 = R.z;	mView._32 = U.z;	mView._33 = L.z; mView._34 = 0;

	L = XMVector3Normalize(L);
	U = XMVector3Normalize(XMVector3Cross(L, R));
	R = XMVector3Cross(U, L);

	float x = DirectX::XMVectorGetX(DirectX::XMVector3Dot(P, R));
	float y = DirectX::XMVectorGetX(DirectX::XMVector3Dot(P, U));
	float z = DirectX::XMVectorGetX(DirectX::XMVector3Dot(P, L));

	mView._41 = -x;		mView._42 = -y;		mView._43 = -z;	  mView._44 = 1;

	// Camera Data Update..
	mCameraData->CamView = mView;
	mCameraData->CamInvView = mView.Invert();
	mCameraData->CamViewProj = mView * mProj;
	mCameraData->CamViewProjTex = mCameraData->CamViewProj * g_TexSpace;
	mCameraData->CamPos = tranform->GetPosition();
	mCameraData->OriginFrustum.Transform(mCameraData->BoundFrustum, mCameraData->CamInvView);
}

void Camera::PushCamList()
{
	//만약 메인카메라로 지정한놈이없다면 처음생성한놈이 메인카메라다
	if (g_MainCam == nullptr)
	{
		g_MainCam = this;

		// Main Camera Data 변경..
		GlobalDataManager::g_GlobalData->MainCamera_Data = g_MainCam->mCameraData;
	}

	//카메라 리스트로 넣느다
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

	//여기로 왔다는것은 리스트가 꽉찼다는 것
	CamList.push_back(this);

	//자기의 인덱스 저장
	MyIndex = count;

	// Global Camera List 연동..
	GlobalDataManager::g_GlobalData->CameraList.push_back(this->mCameraData);
}


