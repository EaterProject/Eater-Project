#include "GameEngine.h"

//�Ŵ�����
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
//������Ʈ
#include "GameObject.h"

//���۳�Ʈ
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
	

	//�⺻ ������ ������ ����
	//WinSizeWidth	= 1920;
	//WinSizeHeight	= 1080;

	//������ �ڵ�
	mHwnd = NULL;

	ConsoleDebug = true;

	// Render Option �ʱ� ����..
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

///���� ���� ����
void GameEngine::Initialize(HWND Hwnd, bool mConsoleDebug)
{
	//Ŭ���ʿ��� �Ѱ��� ������ �ޱ�
	mHwnd = Hwnd;

	//�Ŵ����� ����
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

	//�Ŵ����� �ʱ�ȭ
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
	//�Ŵ��� �ʱ�ȭ �� �߰� �۾���
	mLoadManager->Start();

	//ī�޶�ó�� ���� Ű��ǲ�� �������ֵ��� ���۳�Ʈ ����
	DebugCame = InstanceCamera("DebugCamera");
	DebugCame->AddComponent<CameraDebugKeyInput>();
	DebugCame->SetDontDestroy(true);
	DebugCame->transform->Position = {0,10,-25};

	//�𷺼� ����Ʈ ����
	GameObject* obj = InstanceLight("DirectionLight", LIGHT_TYPE::DIRECTION_LIGHT);
	obj->SetDontDestroy(true);
}

void GameEngine::Update()
{
	//�Ŵ����� ������Ʈ (���۳�Ʈ ������Ʈ�� ����� ������ ������ ���ӿ�����Ʈ OneMeshData�� ����)
	//Ÿ�ӸŴ����� ��������Ǿ����
	//mNetworkManager->Update();
	mTimeManager->Update();
	mKeyManager->Update();
	mPhysManager->Update(mTimeManager->DeltaTime());
	mSceneManager->Update();
	mObjectManager->PlayUpdate();
	//mNavigationManager->Update();

	// ��� ������Ʈ�� �Ͼ �� ������ ����..
	GlobalDataManager::Update(mTimeManager->DeltaTime());

	//���۳�Ʈ ������Ʈ ��

	// ���� ������ �ɼ� ����..
	RenderOptionCheck();

	//����ť �Ѱ���
	EnterCriticalSection(&g_CS);
	mGraphicManager->Render();
	LeaveCriticalSection(&g_CS);

	//�������� ������ ������Ʈ Delete
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
	//������ ũ�� �缳��
	WinSizeWidth	= Change_Width;
	WinSizeHeight	= Change_Height;

	// â �ּ�ȭ�ô� ����
	if (Change_Width == 0 || Change_Height == 0) return;
	if (mGraphicManager == nullptr) return;

	PROFILE_LOG(PROFILE_OUTPUT::CONSOLE, "[ Engine ][ Resize ][ OnResize ] %d / %d", WinSizeWidth, WinSizeHeight);

	//ī�޶��� ��ȭ�� ����� �־��ش�
	Camera::g_MainCam->SetSize(WinSizeWidth, WinSizeHeight);

	//�׷����ʿ� ����Ÿ���� �����ؾ��ϱ⶧���� 
	mGraphicManager->OnReSize(WinSizeWidth, WinSizeHeight);
	Camera::g_MainCam->CreateProj(WinSizeWidth, WinSizeHeight);

	//Resize �Լ�����Ʈ ����
	ResizeFunction(Change_Width, Change_Height);

	std::string Width = std::to_string(WinSizeWidth);
	std::string Height = std::to_string(WinSizeHeight);;
	std::string temp = "������ ������ ����:" + Width + "," + Height;
}

///������Ʈ ���� ����
GameObject* GameEngine::Instance(std::string ObjName)
{
	PROFILE_LOG(PROFILE_OUTPUT::CONSOLE, "[ Engine ][ Create ][ GameObject ] %s", ObjName.c_str());

	//������Ʈ ����
	GameObject* temp = CreateInstance();
	temp->Name = ObjName;

	//Transform �� �⺻���� �־��ش�
	Transform* Tr = temp->AddComponent<Transform>();
	temp->transform = Tr;

	//������Ʈ�� �����Ҷ� AWAKE���°� �ƴҋ�
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
	
	// Light �� �ʱⰪ..
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

/// ��ũ�� ���� �Լ���
void GameEngine::PushScene(Scene* mScene, std::string name)
{
	mSceneManager->PushScene(mScene, name);
}

void GameEngine::ChoiceScene(std::string name)
{
	PROFILE_LOG(PROFILE_OUTPUT::CONSOLE, "[ Engine ][ Choice ][ Scene ] %s", name.c_str());

	//�� ������ �Ǹ� ����ü�� Awack�� Start �Լ� ���� �׸����� ���۳�Ʈ�� Awack�� Start �� ���� 
	mSceneManager->ChoiceScene(name);
	mSceneManager->SceneStart();
}

///�ε� ���� �Լ���
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

///Ű��ǲ �Լ���
bool GameEngine::GetKeyDown(byte number)
{
	//Ű�� �ѹ� ��������  true
	return mKeyManager->GetKeyDown(number);
}

bool GameEngine::GetKeyUp(byte number)
{
	//Ű�� �ѹ������� true
	return mKeyManager->GetKeyUp(number);
}

bool GameEngine::GetKey(byte number)
{
	//Ű�������������� �����Ӹ��� true
	return mKeyManager->GetKey(number);
}

bool GameEngine::GetTogle(byte number)
{
	//�ѹ� Ű�� ������ on �ٽô����� off
	return mKeyManager->GetKeyToggle(number);
}

int GameEngine::GetMousePosX()
{
	//���콺 ��ġ X��ǥ
	return mKeyManager->GetMousePos()->x;
}

int GameEngine::GetMousePosY()
{
	//���콺 ��ġ y��ǥ
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
	// ���� Window Focus ���¿� ���� ���º�ȭ�� ���⿡��..
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
	// ���ο� Instance Object ����..
	GameObject* newObject = new GameObject();

	// Object Mesh Data Renderer���� �߰�..
	mGraphicManager->PushInstance(newObject->OneMeshData);

	return newObject;
}

Material* GameEngine::CreateMaterial()
{
	// ���ο� Material ����..
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
