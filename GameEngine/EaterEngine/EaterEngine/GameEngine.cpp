#include "GameEngine.h"

//매니저들
#include "BaseManager.h"
#include "KeyinputManager.h"
#include "LoadManager.h"
#include "ObjectManager.h"
#include "SceneManager.h"
#include "DebugManager.h"
#include "GraphicEngineManager.h"
#include "TimeManager.h"
#include "MaterialManager.h"
#include "LightManager.h"
#include "PhysManager.h"
#include "NetworkManager.h"

#include "ParserData.h"
#include "EngineData.h"
//오브젝트
#include "GameObject.h"

//컨퍼넌트
#include "Transform.h"
#include "Camera.h"
#include "MeshFilter.h"
#include "SkinningFilter.h"
#include "Light.h"
#include "Collider.h"
#include "Material.h"
#include "Terrain.h"
#include "ParticleSystem.h"
#include "CameraDebugKeyInput.h"

int GameEngine::WinSizeWidth	= 0;
int GameEngine::WinSizeHeight	= 0;
GameEngine::GameEngine()
{
	InitializeCriticalSection(&g_CS);

	mDebugManager = nullptr;
	mLoadManager = nullptr;
	mObjectManager = nullptr;
	mSceneManager = nullptr;
	mKeyManager = nullptr;
	mPhysManager = nullptr;
	mLightManager = nullptr;
	mMaterialManager = nullptr;
	mTimeManager = nullptr;
	mGraphicManager = nullptr;
	mNetworkManager = nullptr;

	//기본 윈도우 사이즈 설정
	//WinSizeWidth	= 1920;
	//WinSizeHeight	= 1080;

	//윈도우 핸들
	mHwnd = NULL;

	ConsoleDebug = true;

	mRenderOption = new RenderOption();

	mRenderOption->DebugOption = DEBUG_ENGINE;
	mRenderOption->RenderingOption = RENDER_DEBUG | RENDER_SHADOW | RENDER_SSAO | RENDER_IBL;
	mRenderOption->PostProcessOption = RENDER_BLOOM | RENDER_HDR | RENDER_FXAA;
}

GameEngine::~GameEngine()
{

}

///게임 엔진 관련
void GameEngine::Initialize(HWND Hwnd, bool mConsoleDebug)
{
	//클라쪽에서 넘겨준 데이터 받기
	mHwnd = Hwnd;

	//매니저들 생성
	mKeyManager			= new KeyinputManager();
	mLoadManager		= new LoadManager();
	mObjectManager		= new ObjectManager();
	mSceneManager		= new SceneManager();
	mDebugManager		= new DebugManager();
	mGraphicManager		= new GraphicEngineManager();
	mTimeManager		= new TimeManager();
	mMaterialManager	= new MaterialManager();
	mLightManager		= new LightManager();
	mPhysManager		= new PhysManager();
	mNetworkManager		= new NetworkManager();

	//매니저들 초기화
	BaseManager::Initialize();
	mGraphicManager->Initialize(Hwnd, WinSizeWidth, WinSizeHeight);
	mKeyManager->Initialize(mHwnd);
	mDebugManager->Initialize(mKeyManager,mConsoleDebug);
	mObjectManager->Initialize();
	mSceneManager->Initialize(mObjectManager);
	mLoadManager->Initialize(mGraphicManager, mMaterialManager, &g_CS);
	mTimeManager->Initialize();
	mMaterialManager->Initialize();
	mPhysManager->Initialize();
	mNetworkManager->Initialize();

	Component::SetManager(mTimeManager, mKeyManager);

	// 최초 Render Setting..
	mGraphicManager->RenderSetting(mRenderOption);
}

void GameEngine::Start()
{
	//매니저 초기화 후 추가 작업들
	mLoadManager->Start();

	//카메라처음 생성 키인풋을 받을수있도록 컨퍼넌트 붙임
	GameObject* obj = InstanceCamera();
	obj->AddComponent<CameraDebugKeyInput>();
	obj->SetDontDestroy(true);
	obj->SetTag("MainCamera");

	//디렉션 라이트 생성
	obj = InstanceLight();
	obj->transform->Position.y += 10.0f;
	obj->SetDontDestroy(true);
}

void GameEngine::Update()
{
	//매니저들 업데이트 (컨퍼넌트 업데이트후 변경된 사항을 각각의 게임오브젝트 OneMeshData에 전달)
	//타임매니저는 먼저실행되어야함
	mNetworkManager->Update();
	mTimeManager->Update();
	mKeyManager->Update();
	mSceneManager->Update();
	mDebugManager->Update();
	mObjectManager->PlayUpdate();
	mPhysManager->Update(mTimeManager->DeltaTime());

	// 모든 업데이트가 일어난 후 데이터 세팅..
	BaseManager::UpdateGlobalData(mTimeManager->DeltaTime());

	//컨퍼넌트 업데이트 끝

	// 현재 랜더링 옵션 설정..
	RenderOptionCheck();

	//랜더큐 넘겨줌
	EnterCriticalSection(&g_CS);
	mGraphicManager->Render();
	LeaveCriticalSection(&g_CS);

	//랜더링이 끝나고 오브젝트 Delete
	mObjectManager->DeleteObject();
}

void GameEngine::Finish()
{
	delete mRenderOption;
	delete mKeyManager;
	delete mLoadManager;

	mDebugManager->Delete();
	mSceneManager->Delete();

	BaseManager::Reset();
}

void GameEngine::OnResize(int Change_Width, int Change_Height)
{
	//윈도우 크기 재설정
	WinSizeWidth	= Change_Width;
	WinSizeHeight	= Change_Height;

	// 창 최소화시는 제외
	if (Change_Width == 0 || Change_Height == 0) return;
	if (mGraphicManager == nullptr) return;

	DebugManager::Print(std::to_string(WinSizeWidth).c_str());
	DebugManager::Print(std::to_string(WinSizeHeight).c_str());

	//카메라의 변화할 사이즈를 넣어준다
	Camera::g_MainCam->SetSize(WinSizeWidth, WinSizeHeight);

	//그래픽쪽에 랜더타겟을 변경해야하기때문에 
	mGraphicManager->OnReSize(WinSizeWidth, WinSizeHeight);
	Camera::g_MainCam->CreateProj(WinSizeWidth, WinSizeHeight);


	std::string Width = std::to_string(WinSizeWidth);
	std::string Height = std::to_string(WinSizeHeight);;
	std::string temp = "윈도우 사이즈 변경:" + Width + "," + Height;
}

///오브젝트 생성 삭제
GameObject* GameEngine::Instance(std::string ObjName)
{
	DebugManager::Line("(Mesh)");
	//오브젝트 생성
	GameObject* temp = CreateInstance();
	temp->Name = ObjName;
	temp->SetTag("Mesh");
	DebugManager::Print(DebugManager::MSG_TYPE::MSG_CREATE, "GameObject", ObjName, false);

	//Transform 은 기본으로 넣어준다
	Transform* Tr = temp->AddComponent<Transform>();
	temp->transform = Tr;

	return temp;
}

GameObject* GameEngine::InstanceTerrain(std::string ObjName)
{
	DebugManager::Line("(Terrain)");
	GameObject* temp = CreateInstance();
	temp->Name = ObjName;
	temp->SetTag("Terrain");
	DebugManager::Print(DebugManager::MSG_TYPE::MSG_CREATE, "Terrain", ObjName, false);

	//Transform
	Transform* Tr = temp->AddComponent<Transform>();
	temp->transform = Tr;

	temp->AddComponent<MeshFilter>();
	temp->AddComponent<Collider>();
	temp->AddComponent<Terrain>();

	return temp;
}

GameObject* GameEngine::InstanceParticle(std::string ObjName /*= "Particle"*/)
{
	DebugManager::Line("(Particle)");
	GameObject* temp = CreateInstance();
	temp->Name = ObjName;
	temp->SetTag("Particle");
	DebugManager::Print(DebugManager::MSG_TYPE::MSG_CREATE, "Particle", ObjName, false);
	
	//Transform
	Transform* Tr = temp->AddComponent<Transform>();
	temp->transform = Tr;

	temp->AddComponent<MeshFilter>();
	temp->AddComponent<ParticleSystem>();

	return temp;
}

GameObject* GameEngine::InstanceCamera(std::string ObjName)
{
	DebugManager::Line("(Camera)");
	GameObject* temp = CreateInstance();
	temp->Name = "Camera";
	Transform* Tr = temp->AddComponent<Transform>();
	temp->transform = Tr;
	Tr->Position = { 0,5,-25 };

	temp->AddComponent<Camera>();
	
	return temp;
}

GameObject* GameEngine::InstanceLight(std::string ObjName, LIGHT_TYPE type)
{
	DebugManager::Line("(Light)");
	GameObject* temp = CreateInstance();
	temp->Name = "Light";

	Transform* Tr = temp->AddComponent<Transform>();
	temp->transform = Tr;

	temp->AddComponent<Light>()->SetType(type);

	return temp;
}

Material* GameEngine::InstanceMaterial(std::string matName /*= "Material"*/)
{
	DebugManager::Line("(Material)");

	Material* temp = CreateMaterial();

	return temp;
}

GameObject* GameEngine::FindGameObjectTag(std::string& TagName)
{
	return mObjectManager->FindGameObjectTag(TagName);
}

GameObject* GameEngine::FindGameObjectName(std::string& ObjName)
{
	return mObjectManager->FindGameObjectString(ObjName);
}

void GameEngine::Destroy(GameObject* obj)
{
	mObjectManager->PushDeleteObject(obj);
}

void GameEngine::DestroyAll()
{
	mObjectManager->AllDeleteObject();
}

/// 스크린 관련 함수들
void GameEngine::PushScene(Scene* mScene, std::string name)
{
	mSceneManager->PushScene(mScene, name);
}

void GameEngine::ChoiceScene(std::string name)
{
	DebugManager::Print(DebugManager::MSG_TYPE::MSG_SYSTEM, "현재 씬:", name, false);

	//씬 선택이 되면 씬자체의 Awack와 Start 함수 실행 그리고나서 컨퍼넌트의 Awack와 Start 도 실행 
	mSceneManager->ChoiceScene(name);
	mSceneManager->SceneStart();
}

///로드 관련 함수들
void GameEngine::LoadTerrain(std::string mMeshName, std::string mMaskName, UINT parsingMode)
{
	mLoadManager->LoadTerrain(mMeshName, mMaskName, parsingMode);
}
void GameEngine::Load(std::string& Path, UINT MODE)
{
	mLoadManager->Load(Path, MODE);
}
int GameEngine::LoadMeshCount()
{
	return mLoadManager->GetMeshCount();
}
int GameEngine::LoadTextureCount()
{
	return mLoadManager->GetTextureCount();
}
int GameEngine::LoadAnimationCount()
{
	return mLoadManager->GetAnimationCount();
}
ModelData* GameEngine::GetLoadMeshData(std::string& Path)
{
	return mLoadManager->GetMesh(Path);
}
void GameEngine::LoadEnvironment(std::string mPath)
{
	EnterCriticalSection(&g_CS);
	mGraphicManager->LoadEnvironment(mPath);
	LeaveCriticalSection(&g_CS);
}

void GameEngine::SetEnvironment(bool enable)
{
	mGraphicManager->SetEnvironment(enable);
}
///키인풋 함수들
bool GameEngine::GetKeyDown(byte number)
{
	//키를 한번 눌렀을때  true
	return mKeyManager->GetKeyDown(number);
}

bool GameEngine::GetKeyUp(byte number)
{
	//키를 한번땠을때 true
	return mKeyManager->GetKeyUp(number);
}

bool GameEngine::GetKey(byte number)
{
	//키를누르고있을때 프레임마다 true
	return mKeyManager->GetKey(number);
}

bool GameEngine::GetTogle(byte number)
{
	//한번 키를 누르면 on 다시누르면 off
	return mKeyManager->GetKeyToggle(number);
}

float GameEngine::GetMousePosX()
{
	//마우스 위치 X좌표
	return (float)mKeyManager->GetMousePos()->x;
}

float GameEngine::GetMousePosY()
{
	//마우스 위치 y좌표
	return (float)mKeyManager->GetMousePos()->y;
}

void GameEngine::SetMousePos(float x, float y)
{

}

void GameEngine::SetMousePosCenter()
{

}

void GameEngine::ShowMouseCursor(bool Show)
{

}

void GameEngine::MouseCursorClip(bool Clip)
{

}

GameObject* GameEngine::GetMainCamera()
{
	return Camera::g_MainCam->gameobject;
}

float GameEngine::GetdeltaTime()
{
	return mTimeManager->DeltaTime();
}

void GameEngine::SetNetworkManager(NetworkManagerComponent* Manager)
{
	mNetworkManager->SetClientNetworkManager(Manager);
}

void GameEngine::NETWORK_SEND(flatbuffers::FlatBufferBuilder* Builder, int Type)
{
	mNetworkManager->NETWORK_SEND(Builder, Type);
}

void GameEngine::NETWORK_LOADING_COMPLETE(unsigned int Number)
{
	mNetworkManager->C2S_LOADING_COMPLETE(Number);
}

void GameEngine::NETWORK_CONNECT(int ServerPort, std::string  Local_Connect_IP)
{
	mNetworkManager->C2S_CONNECT(ServerPort, Local_Connect_IP);
}

void GameEngine::EditorSetting()
{
	mRenderOption->DebugOption = DEBUG_EDITOR;
}

void GameEngine::DebugDrawLine(Vector3 start, Vector3 end, Vector4 color)
{
	mGraphicManager->DebugDrawLine(start, end, color);
}

void GameEngine::DebugDrawLine(Vector3 start, Vector3 dir, float distance, Vector4 color)
{
	mGraphicManager->DebugDrawLine(start, dir, distance, color);
}

GameObject* GameEngine::CreateInstance()
{
	// 새로운 Instance Object 생성..
	GameObject* newObject = new GameObject();

	// Object Mesh Data Renderer측에 추가..
	mGraphicManager->AddMeshData(newObject->OneMeshData);

	return newObject;
}

Material* GameEngine::CreateMaterial()
{
	// 새로운 Material 생성..
	Material* newMaterial = new Material();

	//

	return newMaterial;
}

void GameEngine::CreateObject()
{
	GameObject* light = Instance();
	light->AddComponent<Light>()->SetType(LIGHT_TYPE::DIRECTION_LIGHT);
}

void GameEngine::RenderOptionCheck()
{
	if (mKeyManager->GetKeyUp(VK_F1))
	{
		// Debug On/Off
		mRenderOption->RenderingOption ^= RENDER_DEBUG;
	}
	if (mKeyManager->GetKeyUp(VK_F2))
	{
		// Shadow On/Off
		mRenderOption->RenderingOption ^= RENDER_SHADOW;
	}
	if (mKeyManager->GetKeyUp(VK_F3))
	{
		// SSAO On/Off
		mRenderOption->RenderingOption ^= RENDER_SSAO;
	}
	if (mKeyManager->GetKeyUp(VK_F4))
	{
		// IBL On/Off
		mRenderOption->RenderingOption ^= RENDER_IBL;
	}
	if (mKeyManager->GetKeyUp(VK_F5))
	{
		// Fog On/Off
		mRenderOption->PostProcessOption ^= RENDER_FOG;
	}
	if (mKeyManager->GetKeyUp(VK_F6))
	{
		// Bloom On/Off
		mRenderOption->PostProcessOption ^= RENDER_BLOOM;
	}
	if (mKeyManager->GetKeyUp(VK_F7))
	{
		// HDR On/Off
		mRenderOption->PostProcessOption ^= RENDER_HDR;
	}
	if (mKeyManager->GetKeyUp(VK_F8))
	{
		// FXAA On/Off
		mRenderOption->PostProcessOption ^= RENDER_FXAA;
	}
}
