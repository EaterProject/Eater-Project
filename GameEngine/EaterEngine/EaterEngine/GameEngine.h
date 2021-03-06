#pragma once
#include <windows.h>
#include <string>
#include "EaterEngineDLL.h"
#include "TypeOptionHeader.h"
#include "Utility.h"

/// <summary>
/// 전체 게임 엔진을 총괄하는 게임엔진 클래스
/// 커다란 게임엔진 틀
/// </summary>

namespace flatbuffers
{
	class FlatBufferBuilder;
}

//각종 매니저들
class LoadManager;
class ObjectManager;
class SceneManager;
class KeyinputManager;
class GraphicEngineManager;
class TimeManager;
class MaterialManager;
class LightManager;
class PhysManager;
class NetworkManager;
class Scene;
class NetworkManagerComponent;
class NavigationManager;
class EaterSound;

//게임 오브젝트
class GameObject;
class ModelData;
class Material;
class TextureBuffer;
class RenderOption;

class GameEngine
{
public:
	static CRITICAL_SECTION g_CS;

public:
	GameEngine();
	~GameEngine();
	
public:
	///클라이언트 에서 만질 함수들
	///게임 엔진 관련 함수들
	void Initialize(HWND gHwnd,bool mConsoleDebug = true);	//초기화
	void Start();											//추가 작업
	void Update();											//엔진 가동
	void Finish();											//엔진 중지
	void OnResize(int Change_Width, int Change_Height);		//리사이즈

	static int WinSizeWidth;	//윈도우 사이즈 가로
	static int WinSizeHeight;	//윈도우 사이즈 세로
	static Eater::Delegate<int, int> ResizeFunction;
	static Eater::Delegate<RenderOption*> RenderOptionFunction;

public:
	///오브젝트 관련(오브젝트 매니저)
	GameObject* Instance(std::string ObjName = "GameObject");			//obj 생성 (빈 게임오브젝트)
	GameObject* Instance_Terrain(std::string ObjName = "Terrain");
	GameObject* Instance_Particle(std::string ObjName = "Particle",std::string FileName = "Default");
	GameObject* Instance_ParticleController(std::string ObjName = "ParticleController");
	GameObject* Instance_Camera(std::string ObjName = "Camera");
	GameObject* Instance_Light(std::string ObjName = "Light", LIGHT_TYPE type = LIGHT_TYPE::POINT_LIGHT);
	GameObject* Instance_UI(std::string ObjName = "UI");
	GameObject* Instance_Image(std::string ObjName = "Image");
	GameObject* Instance_Slider(std::string ObjName = "Slider");
	GameObject* Instance_ImageFont(std::string ObjName = "Font");
	GameObject* Instance_Button(std::string ObjName = "Button");

public:
	///태그
	GameObject* FindGameObjectTag(std::string& TagName);
	void FindGameObjectTags(std::string& TagName,std::vector<GameObject*>* ObjectList);
	int	 FindTagNumber(std::string& TagName);
	GameObject* FindGameObjectName(std::string& ObjName);
	Material* FindMaterialName(std::string MaterialName);

public:
	TextureBuffer* GetTexture(std::string& TextureName);

public:
	///삭제
	void Destroy(GameObject* obj);		//obj 삭제
	void DestroyAll();

public:
	///스크린 관련
	void PushScene(Scene* mScene, std::string name);	//씬 생성		
	void ChoiceScene(std::string name);					//씬 선택
	void SetFullScreenBlur(bool enable, UINT blur_count = 0);

public:
	///로드 관련 
	void Load(std::string& Path, UINT MODE);
	void LoadTerrain(std::string mMeshName, std::string mMaskName1, std::string mMaskName2, UINT parsingMode);

	int  LoadMeshCount();
	int  LoadTextureCount();
	int  LoadAnimationCount();
	int	 LoadBufferCount();
	int  LoadMaterialCount();
	ModelData* GetLoadMeshData(std::string& Path);

public:
	///베이크 관련
	void BakeSkyLightMap(std::string& Path, bool hdri);
	void BakeAnimation();
	
	void BakeConvertSkyLightMap(std::string& Path, float angle, float threshold, bool hdri, UINT index);
	void BakeConvertSkyCubeMap(std::string& Path, float angle, float threshold, bool hdri);

	///저장 관련
	void SaveConvertSkyLightMap(std::string& Path, std::string& SaveName);
	void SaveConvertSkyCubeMap(std::string& Path, std::string& SaveName);
	void SaveSpriteToVolumeTexture_LUT(std::string& fileName, std::string& saveName, UINT pixelSize);

public:
	///텍스쳐 설정 관련
	void SetSkyCube(std::string& Path);
	void SetSkyLight(std::string& Path, UINT index);
	void SetColorGradingBaseTexture(std::string& Path);
	void SetColorGradingBaseTexture(TextureBuffer* base_lut);
	void SetColorGradingBlendTexture(std::string& Path);
	void SetColorGradingBlendTexture(TextureBuffer* blend_lut);
	void SetColorGradingBlendFactor(float blend_factor);

	///충돌체크 관련
	void AddOccluder(std::string mMeshName);

public:
	///키관련 
	bool  GetKeyDown(byte number);	//키다운
	bool  GetKeyUp(byte number);	//키업
	bool  GetKey(byte number);		//키누르고있을때
	bool  GetTogle(byte number);	//키 on/off
	void  KeyReset();				//키 리셋
	int GetMousePosX();			//마우스 위치 X
	int GetMousePosY();			//마우스 위치 Y
	void  SetMousePos(float x,float y);
	void  SetMousePosCenter();
	void  ShowMouseCursor(bool Show);
	void  MouseCursorClip(bool Clip);

public:
	///사운드 관련
	void Sound_Play_BGM(std::string& SoundName);
	void Sound_Pause_BGM(bool Pause);
	void Sound_VolumeSet_BGM(float volume);
	void Sound_STOP_BGM();
	void Sound_VolumeUP_BGM();
	void Sound_VolumeDown_BGM();
	void Sound_FrequencyUp_BGM();
	void Sound_FrequencyDown_BGM();
	void Sound_PitchUp_BGM();
	void Sound_PitchDown_BGM();

	void Sound_Play_SFX(std::string& SoundName);
	void Sound_Pause_SFX(bool Pause);
	void Sound_VolumeSet_SFX(float volume);
	void Sound_STOP_SFX();
	void Sound_VolumeUP_SFX();
	void Sound_VolumeDown_SFX();
	void Sound_FrequencyUp_SFX();
	void Sound_FrequencyDown_SFX();
	void Sound_PitchUp_SFX();
	void Sound_PitchDown_SFX();

	void Sound_VolumeSet_Master(float volume);

public:
	///윈도우 관련
	void SetFocus(bool focus);
	void SetWindowSize(int width, int height);
	void SetWindowFullScreen();
	void* Picking(int x, int y);

public:
	///카메라 관련
	GameObject* GetMainCamera();
	GameObject* GetDebugCamera();
	GameObject* GetDirectionLight();

public:
	///시간 관련
	float GetdeltaTime();
	void  SetSlowTime(float mTime);
public:
	///네트워크 관련
	void SetNetworkManager(NetworkManagerComponent* Manager);
	void NETWORK_SEND(flatbuffers::FlatBufferBuilder* Builder, int Type);
	void NETWORK_LOADING_COMPLETE(unsigned int Number);
	void NETWORK_CONNECT(int ServerPort, std::string  Local_Connect_IP);

public:
	///Scene Setting
	RenderOption* GetRenderOptionData();
	void RenderSetting();

private:
	GameObject* CreateInstance();
	void CreateObject();			//기본 생성 오브젝트

private:
	///각종 매니저들
	ObjectManager*			mObjectManager;		// 오브젝트 관리 매니저
	LoadManager*			mLoadManager;		// 로드 관련 매니저
	SceneManager*			mSceneManager;		// 스크린 관리 매니저
	KeyinputManager*		mKeyManager;		// 키인풋 관련 매니저
	GraphicEngineManager*	mGraphicManager;	// 그래픽엔진 관리 매니저
	TimeManager*			mTimeManager;		// 시간 관련 타임매니저
	LightManager*			mLightManager;		// 라이트 관리 매니저
	PhysManager*			mPhysManager;		// 물리 관리 매니저
	NetworkManager*			mNetworkManager;	// 네트워크 매니저
	EaterSound*				mSoundManager;		// 사운드 매니저
private:
	///클라쪽에서 받아야 하는 데이터
	HWND mHwnd; //핸들
	bool ConsoleDebug;
private:
	RenderOption* mRenderOption;

	GameObject* DebugCamera;
	GameObject* DirectionLight;
};

