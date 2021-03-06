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
#include "MaterialManager.h"
#include "EaterSound.h"
#include "DebugManager.h"

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
#include "Image.h"
#include "Slider.h"
#include "ImageFont.h"
#include "Button.h"
#include "ParticleController.h"

#include "Profiler/Profiler.h"

CRITICAL_SECTION GameEngine::g_CS;

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
	

	//기본 윈도우 사이즈 설정
	//WinSizeWidth	= 1920;
	//WinSizeHeight	= 1080;

	//윈도우 핸들
	mHwnd = NULL;

	ConsoleDebug = true;

	// Render Option 초기 설정..
	mRenderOption = new RenderOption();

	mRenderOption->RenderingOption		= RENDER_OPTION::RENDER_SHADOW | RENDER_OPTION::RENDER_SSAO | RENDER_OPTION::RENDER_IBL | RENDER_OPTION::RENDER_FOG;
	mRenderOption->PostProcessOption	= POSTPROCESS_OPTION::POSTPROCESS_BLOOM | POSTPROCESS_OPTION::POSTPROCESS_HDR | POSTPROCESS_OPTION::POSTPROCESS_COLORGRADING | POSTPROCESS_OPTION::POSTPROCESS_FXAA;
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
	//매니저 초기화 후 추가 작업들
	mLoadManager->Start();

	//카메라처음 생성 키인풋을 받을수있도록 컨퍼넌트 붙임
	DebugCamera = Instance_Camera("DebugCamera");
	DebugCamera->AddComponent<CameraDebugKeyInput>();
	DebugCamera->SetDontDestroy(true);
	DebugCamera->transform->SetPosition(0.0f, 10.0f, -25.0f);

	//디렉션 라이트 생성
	DirectionLight = Instance_Light("DirectionLight", LIGHT_TYPE::DIRECTION_LIGHT);
	DirectionLight->SetDontDestroy(true);
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
	mSoundManager->Update();
	//mNavigationManager->Update();

	// 모든 업데이트가 일어난 후 데이터 세팅..
	GlobalDataManager::Update(mTimeManager->DeltaTime());

	//컨퍼넌트 업데이트 끝

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

	PROFILE_LOG(PROFILE_OUTPUT::VS_CODE, "[ Engine ][ Resize ][ OnResize ] %d / %d", WinSizeWidth, WinSizeHeight);

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
	PROFILE_LOG(PROFILE_OUTPUT::VS_CODE, "[ Engine ][ Create ][ GameObject ] %s", ObjName.c_str());

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

GameObject* GameEngine::Instance_Terrain(std::string ObjName)
{
	PROFILE_LOG(PROFILE_OUTPUT::VS_CODE, "[ Engine ][ Create ][ Terrain ] %s", ObjName.c_str());

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

GameObject* GameEngine::Instance_Particle(std::string ObjName, std::string FileName)
{
	PROFILE_LOG(PROFILE_OUTPUT::VS_CODE, "[ Engine ][ Create ][ Particle ] %s", ObjName.c_str());
	
	GameObject* temp = CreateInstance();
	temp->Name = ObjName;
	
	//Transform
	Transform* Tr = temp->AddComponent<Transform>();
	temp->transform = Tr;
	MeshFilter*		mMeshFilter = temp->AddComponent<MeshFilter>();
	ParticleSystem* mBaseAttackParticle	= temp->AddComponent<ParticleSystem>();

	if (FileName != "Default")
	{
		LoadParticleData* Data = LoadManager::GetParticle(FileName);
		mBaseAttackParticle->SetMeshName("Quad");
		mBaseAttackParticle->SetDiffuseName(Data->TextrueName);
		switch (Data->RenderType)
		{
		case 0 :
			mBaseAttackParticle->SetRenderType(PARTICLE_RENDER_OPTION::BILLBOARD);
			break;
		case 1:
			mBaseAttackParticle->SetRenderType(PARTICLE_RENDER_OPTION::VERTICAL_BILLBOARD);
			break;
		case 2:
			mBaseAttackParticle->SetRenderType(PARTICLE_RENDER_OPTION::HORIZONTAL_BILLBOARD);
			break;
		}

		switch (Data->ColorType)
		{
		case 0:
			mBaseAttackParticle->SetLifeTimeColor(Data->LifeColor_Min, Data->LifeColor_Max, NONE);
			break;
		case 1:
			mBaseAttackParticle->SetLifeTimeColor(Data->LifeColor_Min, Data->LifeColor_Max, UP);
			break;
		case 2:
			mBaseAttackParticle->SetLifeTimeColor(Data->LifeColor_Min, Data->LifeColor_Max, DOWN);
			break;
		case 3:
			mBaseAttackParticle->SetLifeTimeColor(Data->LifeColor_Min, Data->LifeColor_Max, UPDOWN);
			break;
		}

		switch (Data->ScaleType)
		{
		case 0:
			mBaseAttackParticle->SetLifeTimeScale(Data->LifeScale.x, Data->LifeScale.y, NONE);
			break;
		case 1:
			mBaseAttackParticle->SetLifeTimeScale(Data->LifeScale.x, Data->LifeScale.y, UP);
			break;
		case 2:
			mBaseAttackParticle->SetLifeTimeScale(Data->LifeScale.x, Data->LifeScale.y, DOWN);
			break;
		case 3:
			mBaseAttackParticle->SetLifeTimeScale(Data->LifeScale.x, Data->LifeScale.y, UPDOWN);
			break;
		}

		mBaseAttackParticle->SetStrength(Data->Strength);
		mBaseAttackParticle->SetMaxParticles(Data->MaxParticle);
		mBaseAttackParticle->SetDelayTime(Data->DelayTime);
		mBaseAttackParticle->SetRateOverTime(Data->RateOverTime);
		mBaseAttackParticle->SetShapeRadius(Data->Radius.x, Data->Radius.y, Data->Radius.z);
		mBaseAttackParticle->SetTextureTiling(Data->Tiling.x, Data->Tiling.y);
		mBaseAttackParticle->SetStartForce(Data->StartForce_Min, Data->StartForce_Max);
		mBaseAttackParticle->SetStartColor(Data->StartColor_Min, Data->StartColor_Max);
		mBaseAttackParticle->SetStartLifeTime(Data->StartLifeTime.x, Data->StartLifeTime.y);
		mBaseAttackParticle->SetStartScale(Data->StartScale.x, Data->StartScale.y);
		mBaseAttackParticle->SetStartRotation(Data->StartRotation.x, Data->StartRotation.y);
		mBaseAttackParticle->SetLifeTimeForce(Data->LifeForce_Min, Data->LifeForce_Max);
		mBaseAttackParticle->SetLifeTimeRotation(Data->LifeRotation.x, Data->LifeRotation.y);
		mBaseAttackParticle->SetPlayTime(Data->PlayTime);
		mBaseAttackParticle->SetLoop(Data->Loop);
	}

	return temp;
}

GameObject* GameEngine::Instance_ParticleController(std::string ObjName /*= "ParticleController"*/)
{
	GameObject* Obj = CreateInstance();

	Transform* Tr = Obj->AddComponent<Transform>();
	Obj->transform = Tr;

	Obj->AddComponent<ParticleController>();

	return Obj;
}

GameObject* GameEngine::Instance_Camera(std::string ObjName)
{
	PROFILE_LOG(PROFILE_OUTPUT::VS_CODE, "[ Engine ][ Create ][ Camera ] %s", ObjName.c_str());
	
	GameObject* Obj	= CreateInstance();
	Obj->transform = Obj->AddComponent<Transform>();
	Obj->AddComponent<Camera>();
	Obj->Name = ObjName;
	Obj->transform->SetPosition(0.0f, 5.0f, -40.0f);
	return Obj;
}

GameObject* GameEngine::Instance_Light(std::string ObjName, LIGHT_TYPE type)
{
	PROFILE_LOG(PROFILE_OUTPUT::VS_CODE, "[ Engine ][ Create ][ Light ] %s", ObjName.c_str());
	
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
		Tr->SetRotate(-30.0f, 10.0f, 0.0f);
		light->SetPower(9.0f);
		break;
	case POINT_LIGHT:
		light->SetColor(1.0f, 1.0f, 0.0f);
		light->SetPower(100.0f);
		break;
	case SPOT_LIGHT:
		Tr->SetRotate(-90.0f, 0.0f, 0.0f);
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

GameObject* GameEngine::Instance_ImageFont(std::string ObjName)
{
	GameObject* Obj = CreateInstance();
	Obj->AddComponent<ImageFont>();
	return Obj;
}

GameObject* GameEngine::Instance_Button(std::string ObjName)
{
	GameObject* Obj = Instance_UI();
	Obj->AddComponent<Image>();
	Obj->AddComponent<Button>();
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

Material* GameEngine::FindMaterialName(std::string MaterialName)
{
	return mLoadManager->GetMaterial(MaterialName);
}

TextureBuffer* GameEngine::GetTexture(std::string& TextureName)
{
	return mLoadManager->GetTexture(TextureName);
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
	PROFILE_LOG(PROFILE_OUTPUT::VS_CODE, "[ Engine ][ Choice ][ Scene ] %s", name.c_str());

	//씬 선택이 되면 씬자체의 Awack와 Start 함수 실행 그리고나서 컨퍼넌트의 Awack와 Start 도 실행 
	mSceneManager->ChoiceScene(name);
	mSceneManager->SceneStart();
}

void GameEngine::SetFullScreenBlur(bool enable, UINT blur_count)
{
	mGraphicManager->SetFullScreenBlur(enable, blur_count);
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

void GameEngine::BakeConvertSkyLightMap(std::string& Path, float angle, float threshold, bool hdri, UINT index)
{
	mLoadManager->BakeConvertSkyLightMap(Path, angle, threshold, hdri, index);
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

void GameEngine::SaveSpriteToVolumeTexture_LUT(std::string& fileName, std::string& saveName, UINT pixelSize)
{
	mLoadManager->SaveSpriteToVolumeTexture_LUT(fileName, saveName, pixelSize);
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

void GameEngine::SetColorGradingBaseTexture(std::string& Path)
{
	TextureBuffer* lut = mLoadManager->GetTexture(Path);

	mGraphicManager->SetColorGradingBaseTexture(lut);
}

void GameEngine::SetColorGradingBaseTexture(TextureBuffer* base_lut)
{
	mGraphicManager->SetColorGradingBaseTexture(base_lut);
}

void GameEngine::SetColorGradingBlendTexture(std::string& Path)
{
	TextureBuffer* lut = mLoadManager->GetTexture(Path);

	mGraphicManager->SetColorGradingBlendTexture(lut);
}

void GameEngine::SetColorGradingBlendTexture(TextureBuffer* blend_lut)
{
	mGraphicManager->SetColorGradingBlendTexture(blend_lut);
}

void GameEngine::SetColorGradingBlendFactor(float blend_factor)
{
	mGraphicManager->SetColorGradingBlendFactor(blend_factor);
}

void GameEngine::AddOccluder(std::string mMeshName)
{
	MeshBuffer* mesh = mLoadManager->GetMeshBuffer(mMeshName);

	if (mesh == nullptr) return;

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

void GameEngine::KeyReset()
{
	//모든 키 리셋
	return mKeyManager->KeyReset();
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

void GameEngine::Sound_Play_BGM(std::string& SoundName)
{
	mSoundManager->SoundPlay(Sound_Category::BGM, SoundName);
}

void GameEngine::Sound_Pause_BGM(bool Pause)
{
	mSoundManager->PauseSound(Sound_Category::BGM, Pause);
}

void GameEngine::Sound_VolumeSet_BGM(float volume)
{
	mSoundManager->VolumeSet(Sound_Category::BGM, volume);
}

void GameEngine::Sound_STOP_BGM()
{
	mSoundManager->StopSound(Sound_Category::BGM);
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

void GameEngine::Sound_VolumeSet_SFX(float volume)
{
	mSoundManager->VolumeSet(Sound_Category::SFX, volume);
}

void GameEngine::Sound_STOP_SFX()
{
	mSoundManager->StopSound(Sound_Category::SFX);
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

void GameEngine::Sound_VolumeSet_Master(float volume)
{
	mSoundManager->VolumeSet(Sound_Category::Master, volume);
}

void GameEngine::SetFocus(bool focus)
{
	// 현재 Window Focus 상태에 따라 상태변화는 여기에서..
	mKeyManager->SetFocus(focus);
}
void GameEngine::SetWindowSize(int width, int height)
{
	int screen_width = GetSystemMetrics(SM_CXSCREEN);
	int screen_height = GetSystemMetrics(SM_CYSCREEN);

	ShowWindow(mHwnd, SW_SHOW);

	SetWindowLong(mHwnd, GWL_STYLE, WS_VISIBLE | WS_CLIPSIBLINGS | WS_CAPTION | WS_BORDER | WS_DLGFRAME | WS_SYSMENU | WS_GROUP | WS_TABSTOP | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);

	SetWindowPos(mHwnd, NULL, (screen_width / 2) - (width / 2), (screen_height / 2) - (height / 2), width, height, 0);
}

void GameEngine::SetWindowFullScreen()
{
	int screen_width = GetSystemMetrics(SM_CXSCREEN);
	int screen_height = GetSystemMetrics(SM_CYSCREEN);

	LONG info = GetWindowLong(mHwnd, GWL_STYLE);

	ShowWindow(mHwnd, SW_MAXIMIZE);

	SetWindowLong(mHwnd, GWL_STYLE,
		info & ~(WS_DLGFRAME | WS_THICKFRAME));

	SetWindowPos(mHwnd, HWND_TOPMOST, -1, -1, screen_width + 3, screen_height + 3, SWP_SHOWWINDOW);
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

	// RenderOption Function List 실행..
	RenderOptionFunction(mRenderOption);
}

GameObject* GameEngine::CreateInstance()
{
	// 새로운 Instance Object 생성..
	GameObject* newObject = new GameObject();

	// Object Mesh Data Renderer측에 추가..
	mGraphicManager->PushInstance(newObject->OneMeshData);

	return newObject;
}

void GameEngine::CreateObject()
{
	GameObject* light = Instance();
	light->AddComponent<Light>()->SetType(LIGHT_TYPE::DIRECTION_LIGHT);
}
