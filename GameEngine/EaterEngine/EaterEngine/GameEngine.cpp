#include "GameEngine.h"

//매니저들
#include "KeyinputManager.h"
#include "LoadManager.h"
#include "ObjectManager.h"
#include "SceneManager.h"
#include "GraphicEngineManager.h"
#include "TimeManager.h"
#include "LightManager.h"
#include "PhysManager.h"
#include "NetworkManager.h"
#include "GlobalDataManager.h"
#include "NavigationManager.h"
#include "EaterSound.h"

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
#include "RectTransform.h"

#include "Profiler/Profiler.h"

int GameEngine::WinSizeWidth	= 0;
int GameEngine::WinSizeHeight	= 0;
Eater::Delegate<int, int> GameEngine::ResizeFunction;

GameEngine::GameEngine()
{
	InitializeCriticalSection(&g_CS);

	mLoadManager = nullptr;
	mObjectManager = nullptr;
	mSceneManager = nullptr;
	mKeyManager = nullptr;
	mPhysManager = nullptr;
	mLightManager = nullptr;
	mTimeManager = nullptr;
	mGraphicManager = nullptr;
	mNetworkManager = nullptr;
	mSoundManager = nullptr;
	

	//기본 윈도우 사이즈 설정
	//WinSizeWidth	= 1920;
	//WinSizeHeight	= 1080;

	//윈도우 핸들
	mHwnd = NULL;

	ConsoleDebug = true;

	// Render Option 초기 설정..
	mRenderOption = new RenderOption();

	mRenderOption->RenderingOption		= RENDER_OPTION::RENDER_SHADOW | RENDER_OPTION::RENDER_SSAO | RENDER_OPTION::RENDER_IBL | RENDER_OPTION::RENDER_FOG;
	mRenderOption->PostProcessOption	= POSTPROCESS_OPTION::RENDER_BLOOM | POSTPROCESS_OPTION::RENDER_HDR | POSTPROCESS_OPTION::RENDER_FXAA;
}

GameEngine::~GameEngine()
{
	mLoadManager = nullptr;
	mObjectManager = nullptr;
	mSceneManager = nullptr;
	mKeyManager = nullptr;
	mPhysManager = nullptr;
	mLightManager = nullptr;
	mTimeManager = nullptr;
	mGraphicManager = nullptr;
	mNetworkManager = nullptr;
	mSoundManager = nullptr;
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
	mGraphicManager		= new GraphicEngineManager();
	mTimeManager		= new TimeManager();
	mLightManager		= new LightManager();
	mPhysManager		= new PhysManager();
	mSoundManager		= EaterSound::GetInstance();
	//mNavigationManager	= new NavigationManager();
	//mNetworkManager		= new NetworkManager();

	//매니저들 초기화
	GlobalDataManager::Initialize();
	mGraphicManager->Initialize(Hwnd, WinSizeWidth, WinSizeHeight, mRenderOption);
	mKeyManager->Initialize(mHwnd);
	mObjectManager->Initialize();
	mSceneManager->Initialize(mObjectManager);
	mLoadManager->Initialize(mGraphicManager, &g_CS);
	mTimeManager->Initialize();
	mPhysManager->Initialize();

	//mNavigationManager->Initialize();
	//mNetworkManager->Initialize();

	Component::SetManager(mTimeManager, mKeyManager);
}

void GameEngine::Start()
{
	//매니저 초기화 후 추가 작업들
	mLoadManager->Start();

	//카메라처음 생성 키인풋을 받을수있도록 컨퍼넌트 붙임
	DebugCame = InstanceCamera("DebugCamera");
	DebugCame->AddComponent<CameraDebugKeyInput>();
	DebugCame->SetDontDestroy(true);
	DebugCame->transform->Position = {0,10,-25};

	//디렉션 라이트 생성
	GameObject* obj = InstanceLight("DirectionLight", LIGHT_TYPE::DIRECTION_LIGHT);
	obj->SetDontDestroy(true);
}

void GameEngine::Update()
{
	//매니저들 업데이트 (컨퍼넌트 업데이트후 변경된 사항을 각각의 게임오브젝트 OneMeshData에 전달)
	//타임매니저는 먼저실행되어야함
	//mNetworkManager->Update();
	mTimeManager->Update();
	mKeyManager->Update();
	mPhysManager->Update(mTimeManager->DeltaTime());
	mSceneManager->Update();
	mObjectManager->PlayUpdate();
	//mNavigationManager->Update();

	// 모든 업데이트가 일어난 후 데이터 세팅..
	GlobalDataManager::Update(mTimeManager->DeltaTime());

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

	
	mSceneManager->Delete();
	mLoadManager->Release();

	GlobalDataManager::Release();
}

void GameEngine::OnResize(int Change_Width, int Change_Height)
{
	//윈도우 크기 재설정
	WinSizeWidth	= Change_Width;
	WinSizeHeight	= Change_Height;

	// 창 최소화시는 제외
	if (Change_Width == 0 || Change_Height == 0) return;
	if (mGraphicManager == nullptr) return;

	PROFILE_LOG(PROFILE_OUTPUT::CONSOLE, "[ Engine ][ Resize ][ OnResize ] %d / %d", WinSizeWidth, WinSizeHeight);

	//카메라의 변화할 사이즈를 넣어준다
	Camera::g_MainCam->SetSize(WinSizeWidth, WinSizeHeight);

	//그래픽쪽에 랜더타겟을 변경해야하기때문에 
	mGraphicManager->OnReSize(WinSizeWidth, WinSizeHeight);
	Camera::g_MainCam->CreateProj(WinSizeWidth, WinSizeHeight);

	//Resize 함수리스트 실행
	ResizeFunction(Change_Width, Change_Height);

	std::string Width = std::to_string(WinSizeWidth);
	std::string Height = std::to_string(WinSizeHeight);;
	std::string temp = "윈도우 사이즈 변경:" + Width + "," + Height;
}

///오브젝트 생성 삭제
GameObject* GameEngine::Instance(std::string ObjName)
{
	PROFILE_LOG(PROFILE_OUTPUT::CONSOLE, "[ Engine ][ Create ][ GameObject ] %s", ObjName.c_str());

	//오브젝트 생성
	GameObject* temp = CreateInstance();
	temp->Name = ObjName;

	//Transform 은 기본으로 넣어준다
	Transform* Tr = temp->AddComponent<Transform>();
	temp->transform = Tr;

	//오브젝트를 생성할때 AWAKE상태가 아닐떄
	int State = ObjectManager::GetFunctionState();
	if (State != (int)FUNCTION_STATE::FUNCTION_AWAKE)
	{
		int num = 0;
	}

	return temp;
}

GameObject* GameEngine::InstanceTerrain(std::string ObjName)
{
	PROFILE_LOG(PROFILE_OUTPUT::CONSOLE, "[ Engine ][ Create ][ Terrain ] %s", ObjName.c_str());

	GameObject* temp = CreateInstance();
	temp->Name = ObjName;

	//Transform
	Transform* Tr = temp->AddComponent<Transform>();
	temp->transform = Tr;

	temp->AddComponent<MeshFilter>();
	//temp->AddComponent<Collider>();
	temp->AddComponent<Terrain>();

	return temp;
}

GameObject* GameEngine::InstanceParticle(std::string ObjName)
{
	PROFILE_LOG(PROFILE_OUTPUT::CONSOLE, "[ Engine ][ Create ][ Particle ] %s", ObjName.c_str());
	
	GameObject* temp = CreateInstance();
	temp->Name = ObjName;
	
	//Transform
	Transform* Tr = temp->AddComponent<Transform>();
	temp->transform = Tr;

	temp->AddComponent<MeshFilter>();
	temp->AddComponent<ParticleSystem>();

	return temp;
}

GameObject* GameEngine::InstanceCamera(std::string ObjName)
{
	PROFILE_LOG(PROFILE_OUTPUT::CONSOLE, "[ Engine ][ Create ][ Camera ] %s", ObjName.c_str());
	
	GameObject* Obj	= CreateInstance();
	Obj->transform = Obj->AddComponent<Transform>();
	Obj->AddComponent<Camera>();
	Obj->Name = ObjName;
	Obj->transform->Position.z = -40.0f;
	Obj->transform->Position.y = 5.0f;
	return Obj;
}

GameObject* GameEngine::InstanceLight(std::string ObjName, LIGHT_TYPE type)
{
	PROFILE_LOG(PROFILE_OUTPUT::CONSOLE, "[ Engine ][ Create ][ Light ] %s", ObjName.c_str());
	
	GameObject* temp = CreateInstance();
	temp->Name = ObjName;
	
	Transform* Tr = temp->AddComponent<Transform>();
	temp->transform = Tr;

	Light* light = temp->AddComponent<Light>();
	light->SetType(type);
	
	// Light 별 초기값..
	switch (type)
	{
	case DIRECTION_LIGHT:
		light->SetPower(1);
		break;
	case POINT_LIGHT:
		light->SetColor(1.0f, 1.0f, 0.0f);
		light->SetPower(100.0f);
		break;
	case SPOT_LIGHT:
		Tr->Rotation.x = -90.0f;
		light->SetColor(1.0f, 1.0f, 0.0f);
		light->SetPower(100.0f);
		light->SetAttenuate(0.5f);
		light->SetAngle(25.0f);
		break;
	default:
		break;
	}

	return temp;
}

GameObject* GameEngine::InstanceUI(std::string ObjName /*= "UI"*/)
{
	GameObject* Obj = CreateInstance();
	Obj->AddComponent<RectTransform>();

	return Obj;
}

Material* GameEngine::InstanceMaterial(std::string matName /*= "Material"*/)
{
	PROFILE_LOG(PROFILE_OUTPUT::CONSOLE, "[ Engine ][ Create ][ Material ] %s", matName.c_str());

	Material* temp = CreateMaterial();

	return temp;
}

GameObject* GameEngine::FindGameObjectTag(std::string& TagName)
{
	return mObjectManager->FindGameObjectTag(TagName);
}

void GameEngine::FindGameObjectTags(std::string& TagName, std::vector<GameObject*>* ObjectList)
{
	mObjectManager->FindGameObjectTags(TagName, ObjectList);
}

int GameEngine::FindTagNumber(std::string& TagName)
{
	return ObjectManager::FindTag(TagName);
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
	PROFILE_LOG(PROFILE_OUTPUT::CONSOLE, "[ Engine ][ Choice ][ Scene ] %s", name.c_str());

	//씬 선택이 되면 씬자체의 Awack와 Start 함수 실행 그리고나서 컨퍼넌트의 Awack와 Start 도 실행 
	mSceneManager->ChoiceScene(name);
	mSceneManager->SceneStart();
}

///로드 관련 함수들
void GameEngine::LoadTerrain(std::string mMeshName, std::string mMaskName1, std::string mMaskName2, UINT parsingMode)
{
	mLoadManager->LoadTerrain(mMeshName, mMaskName1, mMaskName2, parsingMode);
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
int GameEngine::LoadBufferCount()
{
	return mLoadManager->GetBufferCount();
}
int GameEngine::LoadMaterialCount()
{
	return mLoadManager->GetMaterialCount();
}
ModelData* GameEngine::GetLoadMeshData(std::string& Path)
{
	return mLoadManager->GetModelData(Path);
}

void GameEngine::BakeSkyLightMap(std::string& Path, bool hdri)
{
	mLoadManager->BakeSkyLightMap(Path, hdri);
}

void GameEngine::BakeAnimation()
{
	mLoadManager->BakeAnimation();
}

void GameEngine::BakeConvertSkyLightMap(std::string& Path, float angle, float threshold, bool hdri)
{
	mLoadManager->BakeConvertSkyLightMap(Path, angle, threshold, hdri);
}

void GameEngine::BakeConvertSkyCubeMap(std::string& Path, float angle, float threshold, bool hdri)
{
	mLoadManager->BakeConvertSkyCubeMap(Path, angle, threshold, hdri);
}

void GameEngine::SaveConvertSkyLightMap(std::string& Path, std::string& SaveName)
{
	mLoadManager->SaveConvertSkyLightMap(Path, SaveName);
}

void GameEngine::SaveConvertSkyCubeMap(std::string& Path, std::string& SaveName)
{
	mLoadManager->SaveConvertSkyCubeMap(Path, SaveName);
}

void GameEngine::SetSkyCube(std::string& Path)
{
	TextureBuffer* environment = mLoadManager->GetTexture(Path);

	mGraphicManager->SetSkyCube(environment);
}

void GameEngine::SetSkyLight(std::string& Path)
{
	SkyLightBuffer* skyLight = mLoadManager->GetSkyLight(Path);

	mGraphicManager->SetSkyLight(skyLight);
}

void GameEngine::AddOccluder(std::string mMeshName)
{
	MeshBuffer* mesh = mLoadManager->GetMeshBuffer(mMeshName);

	mGraphicManager->AddOccluder(mesh);
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

int GameEngine::GetMousePosX()
{
	//마우스 위치 X좌표
	return mKeyManager->GetMousePos()->x;
}

int GameEngine::GetMousePosY()
{
	//마우스 위치 y좌표
	return mKeyManager->GetMousePos()->y;
}

void GameEngine::SetMousePos(float x, float y)
{
	mKeyManager->SetMousePos(x, y);
}

void GameEngine::SetMousePosCenter()
{
	mKeyManager->SetMouseCenter();
}

void GameEngine::ShowMouseCursor(bool Show)
{
	mKeyManager->ShowMouse(Show);
}

void GameEngine::MouseCursorClip(bool Clip)
{

}

void GameEngine::SetFocus(bool focus)
{
	// 현재 Window Focus 상태에 따라 상태변화는 여기에서..
	mKeyManager->SetFocus(focus);
}

void* GameEngine::Picking(int x, int y)
{
	return mGraphicManager->PickingRender(x, y);
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

RenderOption* GameEngine::GetRenderOptionData()
{
	return mRenderOption;
}

void GameEngine::RenderSetting()
{
	mGraphicManager->RenderSetting();
}

GameObject* GameEngine::CreateInstance()
{
	// 새로운 Instance Object 생성..
	GameObject* newObject = new GameObject();

	// Object Mesh Data Renderer측에 추가..
	mGraphicManager->PushInstance(newObject->OneMeshData);

	return newObject;
}

Material* GameEngine::CreateMaterial()
{
	// 새로운 Material 생성..
	Material* newMaterial = new Material();
	return newMaterial;
}

void GameEngine::CreateObject()
{
	GameObject* light = Instance();
	light->AddComponent<Light>()->SetType(LIGHT_TYPE::DIRECTION_LIGHT);
}

void GameEngine::RenderOptionCheck()
{
	bool change = false;

	if (mKeyManager->GetKeyUp(VK_F1))
	{
		// Debug On/Off
		mRenderOption->DebugOption ^= DEBUG_OPTION::DEBUG_MODE;
		change = true;
	}
	if (mKeyManager->GetKeyUp(VK_F2))
	{
		// Shadow On/Off
		mRenderOption->DebugOption ^= DEBUG_OPTION::DEBUG_RENDERTARGET;
		change = true;
	}
	if (mKeyManager->GetKeyUp(VK_F3))
	{
		// SSAO On/Off
		mRenderOption->RenderingOption ^= RENDER_OPTION::RENDER_SSAO;
		change = true;
	}
	if (mKeyManager->GetKeyUp(VK_F4))
	{
		// IBL On/Off
		mRenderOption->RenderingOption ^= RENDER_OPTION::RENDER_IBL;
		change = true;
	}
	if (mKeyManager->GetKeyUp(VK_F5))
	{
		// Fog On/Off
		mRenderOption->RenderingOption ^= RENDER_OPTION::RENDER_FOG;
		change = true;
	}
	if (mKeyManager->GetKeyUp(VK_F6))
	{
		// Bloom On/Off
		mRenderOption->PostProcessOption ^= POSTPROCESS_OPTION::RENDER_BLOOM;
		change = true;
	}
	if (mKeyManager->GetKeyUp(VK_F7))
	{
		// HDR On/Off
		mRenderOption->PostProcessOption ^= POSTPROCESS_OPTION::RENDER_HDR;
		change = true;
	}
	if (mKeyManager->GetKeyUp(VK_F8))
	{
		// FXAA On/Off
		mRenderOption->PostProcessOption ^= POSTPROCESS_OPTION::RENDER_FXAA;
		change = true;
	}

	if (change)
	{
		mGraphicManager->RenderSetting();
	}
}
