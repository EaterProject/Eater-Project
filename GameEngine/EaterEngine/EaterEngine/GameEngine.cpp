#include "GameEngine.h"

//�Ŵ�����
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

	//�⺻ ������ ������ ����
	//WinSizeWidth	= 1920;
	//WinSizeHeight	= 1080;

	//������ �ڵ�
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
	mDebugManager		= new DebugManager();
	mGraphicManager		= new GraphicEngineManager();
	mTimeManager		= new TimeManager();
	mMaterialManager	= new MaterialManager();
	mLightManager		= new LightManager();
	mPhysManager		= new PhysManager();
	mNetworkManager		= new NetworkManager();

	//�Ŵ����� �ʱ�ȭ
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

	// ���� Render Setting..
	mGraphicManager->RenderSetting(mRenderOption);
}

void GameEngine::Start()
{
	//�Ŵ��� �ʱ�ȭ �� �߰� �۾���
	mLoadManager->Start();

	//ī�޶�ó�� ���� Ű��ǲ�� �������ֵ��� ���۳�Ʈ ����
	GameObject* obj = InstanceCamera();
	obj->AddComponent<CameraDebugKeyInput>();
	obj->SetDontDestroy(true);
	obj->SetTag("MainCamera");

	//�𷺼� ����Ʈ ����
	obj = InstanceLight();
	obj->transform->Position.y += 10.0f;
	obj->SetDontDestroy(true);
}

void GameEngine::Update()
{
	//�Ŵ����� ������Ʈ (���۳�Ʈ ������Ʈ�� ����� ������ ������ ���ӿ�����Ʈ OneMeshData�� ����)
	//Ÿ�ӸŴ����� ��������Ǿ����
	mNetworkManager->Update();
	mTimeManager->Update();
	mKeyManager->Update();
	mSceneManager->Update();
	mDebugManager->Update();
	mObjectManager->PlayUpdate();
	mPhysManager->Update(mTimeManager->DeltaTime());

	// ��� ������Ʈ�� �Ͼ �� ������ ����..
	BaseManager::UpdateGlobalData(mTimeManager->DeltaTime());

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

	mDebugManager->Delete();
	mSceneManager->Delete();

	BaseManager::Reset();
}

void GameEngine::OnResize(int Change_Width, int Change_Height)
{
	//������ ũ�� �缳��
	WinSizeWidth	= Change_Width;
	WinSizeHeight	= Change_Height;

	// â �ּ�ȭ�ô� ����
	if (Change_Width == 0 || Change_Height == 0) return;
	if (mGraphicManager == nullptr) return;

	DebugManager::Print(std::to_string(WinSizeWidth).c_str());
	DebugManager::Print(std::to_string(WinSizeHeight).c_str());

	//ī�޶��� ��ȭ�� ����� �־��ش�
	Camera::g_MainCam->SetSize(WinSizeWidth, WinSizeHeight);

	//�׷����ʿ� ����Ÿ���� �����ؾ��ϱ⶧���� 
	mGraphicManager->OnReSize(WinSizeWidth, WinSizeHeight);
	Camera::g_MainCam->CreateProj(WinSizeWidth, WinSizeHeight);


	std::string Width = std::to_string(WinSizeWidth);
	std::string Height = std::to_string(WinSizeHeight);;
	std::string temp = "������ ������ ����:" + Width + "," + Height;
}

///������Ʈ ���� ����
GameObject* GameEngine::Instance(std::string ObjName)
{
	DebugManager::Line("(Mesh)");
	//������Ʈ ����
	GameObject* temp = CreateInstance();
	temp->Name = ObjName;
	temp->SetTag("Mesh");
	DebugManager::Print(DebugManager::MSG_TYPE::MSG_CREATE, "GameObject", ObjName, false);

	//Transform �� �⺻���� �־��ش�
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

/// ��ũ�� ���� �Լ���
void GameEngine::PushScene(Scene* mScene, std::string name)
{
	mSceneManager->PushScene(mScene, name);
}

void GameEngine::ChoiceScene(std::string name)
{
	DebugManager::Print(DebugManager::MSG_TYPE::MSG_SYSTEM, "���� ��:", name, false);

	//�� ������ �Ǹ� ����ü�� Awack�� Start �Լ� ���� �׸����� ���۳�Ʈ�� Awack�� Start �� ���� 
	mSceneManager->ChoiceScene(name);
	mSceneManager->SceneStart();
}

///�ε� ���� �Լ���
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

float GameEngine::GetMousePosX()
{
	//���콺 ��ġ X��ǥ
	return (float)mKeyManager->GetMousePos()->x;
}

float GameEngine::GetMousePosY()
{
	//���콺 ��ġ y��ǥ
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
	// ���ο� Instance Object ����..
	GameObject* newObject = new GameObject();

	// Object Mesh Data Renderer���� �߰�..
	mGraphicManager->AddMeshData(newObject->OneMeshData);

	return newObject;
}

Material* GameEngine::CreateMaterial()
{
	// ���ο� Material ����..
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
