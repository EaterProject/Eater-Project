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
#include "MaterialManager.h"
#include "EaterSound.h"
#include "DebugManager.h"

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
#include "Image.h"
#include "Slider.h"

#include "Profiler/Profiler.h"

int GameEngine::WinSizeWidth	= 0;
int GameEngine::WinSizeHeight	= 0;
Eater::Delegate<int, int> GameEngine::ResizeFunction;
Eater::Delegate<RenderOption*> GameEngine::RenderOptionFunction;

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
	MaterialManager::Release();

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
	DebugManager::Initialize();
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
	DebugCamera = InstanceCamera("DebugCamera");
	DebugCamera->AddComponent<CameraDebugKeyInput>();
	DebugCamera->SetDontDestroy(true);
	DebugCamera->transform->Position = {0,10,-25};

	//�𷺼� ����Ʈ ����
	DirectionLight = Instance_Light("DirectionLight", LIGHT_TYPE::DIRECTION_LIGHT);
	DirectionLight->SetDontDestroy(true);
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
	mSoundManager->Update();
	//mNavigationManager->Update();

	// ��� ������Ʈ�� �Ͼ �� ������ ����..
	GlobalDataManager::Update(mTimeManager->DeltaTime());

	//���۳�Ʈ ������Ʈ ��

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

GameObject* GameEngine::InstanceParticle(std::string ObjName, std::string FileName)
{
	PROFILE_LOG(PROFILE_OUTPUT::CONSOLE, "[ Engine ][ Create ][ Particle ] %s", ObjName.c_str());
	
	GameObject* temp = CreateInstance();
	temp->Name = ObjName;
	
	//Transform
	Transform* Tr = temp->AddComponent<Transform>();
	temp->transform = Tr;
	MeshFilter*		mMeshFilter = temp->AddComponent<MeshFilter>();
	ParticleSystem* mParticle	= temp->AddComponent<ParticleSystem>();

	if (FileName != "Default")
	{
		LoadParticleData* Data = LoadManager::GetParticle(FileName);
		mParticle->SetMeshName("Quad");
		mParticle->SetDiffuseName(Data->TextrueName);
		switch (Data->RenderType)
		{
		case 0 :
			mParticle->SetRenderType(PARTICLE_RENDER_OPTION::BILLBOARD);
			break;
		case 1:
			mParticle->SetRenderType(PARTICLE_RENDER_OPTION::VERTICAL_BILLBOARD);
			break;
		case 2:
			mParticle->SetRenderType(PARTICLE_RENDER_OPTION::HORIZONTAL_BILLBOARD);
			break;
		}

		switch (Data->ColorType)
		{
		case 0:
			mParticle->SetLifeTimeColor(Data->LifeColor_Min, Data->LifeColor_Max, NONE);
			break;
		case 1:
			mParticle->SetLifeTimeColor(Data->LifeColor_Min, Data->LifeColor_Max, UP);
			break;
		case 2:
			mParticle->SetLifeTimeColor(Data->LifeColor_Min, Data->LifeColor_Max, DOWN);
			break;
		case 3:
			mParticle->SetLifeTimeColor(Data->LifeColor_Min, Data->LifeColor_Max, UPDOWN);
			break;
		}

		switch (Data->ScaleType)
		{
		case 0:
			mParticle->SetLifeTimeScale(Data->LifeScale.x, Data->LifeScale.y, NONE);
			break;
		case 1:
			mParticle->SetLifeTimeScale(Data->LifeScale.x, Data->LifeScale.y, UP);
			break;
		case 2:
			mParticle->SetLifeTimeScale(Data->LifeScale.x, Data->LifeScale.y, DOWN);
			break;
		case 3:
			mParticle->SetLifeTimeScale(Data->LifeScale.x, Data->LifeScale.y, UPDOWN);
			break;
		}

		mParticle->SetStrength(Data->Strength);
		mParticle->SetMaxParticles(Data->MaxParticle);
		mParticle->SetDelayTime(Data->DelayTime);
		mParticle->SetRateOverTime(Data->RateOverTime);
		mParticle->SetShapeRadius(Data->Radius.x, Data->Radius.y, Data->Radius.z);
		mParticle->SetTextureTiling(Data->Tiling.x, Data->Tiling.y);
		mParticle->SetStartForce(Data->StartForce_Min, Data->StartForce_Max);
		mParticle->SetStartColor(Data->StartColor_Min, Data->StartColor_Max);
		mParticle->SetStartLifeTime(Data->StartLifeTime.x, Data->StartLifeTime.y);
		mParticle->SetStartScale(Data->StartScale.x, Data->StartScale.y);
		mParticle->SetStartRotation(Data->StartRotation.x, Data->StartRotation.y);
		mParticle->SetLifeTimeForce(Data->LifeForce_Min, Data->LifeForce_Max);
		mParticle->SetLifeTimeRotation(Data->LifeRotation.x, Data->LifeRotation.y);
		mParticle->SetPlayTime(1, true);
		mParticle->Play();
	}

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

GameObject* GameEngine::Instance_Light(std::string ObjName, LIGHT_TYPE type)
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
		Tr->Rotation.x = -30.0f;
		Tr->Rotation.y = 10.0f;
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

GameObject* GameEngine::Instance_UI(std::string ObjName /*= "UI"*/)
{
	GameObject* Obj = CreateInstance();
	Obj->AddComponent<RectTransform>();

	return Obj;
}

GameObject* GameEngine::Instance_Image(std::string ObjName /*= "Image"*/)
{
	GameObject* Obj = Instance_UI();
	Obj->AddComponent<Image>();

	return Obj;
}

GameObject* GameEngine::Instance_Slider(std::string ObjName /*= "Slider"*/)
{
	GameObject* Obj = CreateInstance();
	Slider* slider  = Obj->AddComponent<Slider>();

	GameObject* Back_Img = Instance_Image();
	slider->SetBackGroundImage(Back_Img->GetComponent<Image>());

	GameObject* Fill_Img = Instance_Image();
	slider->SetFillImage(Fill_Img->GetComponent<Image>());

	return Obj;
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

	if (environment == nullptr) return;

	mGraphicManager->SetSkyCube(environment);
}

void GameEngine::SetSkyLight(std::string& Path, UINT index)
{
	SkyLightBuffer* skyLight = mLoadManager->GetSkyLight(Path);

	if (skyLight == nullptr) return;

	if (skyLight->Prefilter == nullptr) return;
	if (skyLight->Irradiance == nullptr) return;

	mGraphicManager->SetSkyLight(skyLight, index);
}

void GameEngine::AddOccluder(std::string mMeshName)
{
	MeshBuffer* mesh = mLoadManager->GetMeshBuffer(mMeshName);

	if (mesh == nullptr) return;

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

void GameEngine::Sound_Play_BGM(std::string& SoundName)
{
	mSoundManager->SoundPlay(Sound_Category::BGM, SoundName);
}

void GameEngine::Sound_Pause_BGM(bool Pause)
{
	mSoundManager->PauseSound(Sound_Category::BGM, Pause);
}

void GameEngine::Sound_VolumeUP_BGM()
{
	mSoundManager->VolumeUp(Sound_Category::BGM);
}

void GameEngine::Sound_VolumeDown_BGM()
{
	mSoundManager->VolumeDown(Sound_Category::BGM);
}

void GameEngine::Sound_FrequencyUp_BGM()
{
	mSoundManager->FrequencyUp(Sound_Category::BGM);
}

void GameEngine::Sound_FrequencyDown_BGM()
{
	mSoundManager->FrequencyDown(Sound_Category::BGM);
}

void GameEngine::Sound_PitchUp_BGM()
{
	mSoundManager->PitchUp(Sound_Category::BGM);
}

void GameEngine::Sound_PitchDown_BGM()
{
	mSoundManager->PitchDown(Sound_Category::BGM);
}

void GameEngine::Sound_Play_SFX(std::string& SoundName)
{
	mSoundManager->SoundPlay(Sound_Category::SFX, SoundName);
}

void GameEngine::Sound_Pause_SFX(bool Pause)
{
	mSoundManager->PauseSound(Sound_Category::SFX, Pause);
}

void GameEngine::Sound_VolumeUP_SFX()
{
	mSoundManager->VolumeUp(Sound_Category::SFX);
}

void GameEngine::Sound_VolumeDown_SFX()
{
	mSoundManager->VolumeDown(Sound_Category::SFX);
}

void GameEngine::Sound_FrequencyUp_SFX()
{
	mSoundManager->FrequencyUp(Sound_Category::SFX);
}

void GameEngine::Sound_FrequencyDown_SFX()
{
	mSoundManager->FrequencyDown(Sound_Category::SFX);
}

void GameEngine::Sound_PitchUp_SFX()
{
	mSoundManager->PitchUp(Sound_Category::SFX);
}

void GameEngine::Sound_PitchDown_SFX()
{
	mSoundManager->PitchDown(Sound_Category::SFX);
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

GameObject* GameEngine::GetDebugCamera()
{
	return DebugCamera;
}

GameObject* GameEngine::GetDirectionLight()
{
	return DirectionLight;
}

float GameEngine::GetdeltaTime()
{
	return mTimeManager->DeltaTime();
}

void GameEngine::SetSlowTime(float mTime)
{
	mTimeManager->SlowTime(mTime);
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

	// RenderOption Function List ����..
	RenderOptionFunction(mRenderOption);
}

GameObject* GameEngine::CreateInstance()
{
	// ���ο� Instance Object ����..
	GameObject* newObject = new GameObject();

	// Object Mesh Data Renderer���� �߰�..
	mGraphicManager->PushInstance(newObject->OneMeshData);

	return newObject;
}

void GameEngine::CreateObject()
{
	GameObject* light = Instance();
	light->AddComponent<Light>()->SetType(LIGHT_TYPE::DIRECTION_LIGHT);
}
