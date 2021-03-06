#pragma warning(disable: 4996)

#include "EaterEngineAPI.h"
#include "GameEngine.h"
#include "PhysManager.h"
#include "PhysData.h"
#include "EngineData.h"
#include "DebugManager.h"
#include "Profiler/Profiler.h"

GameEngine* gGameEngine = nullptr;
GameObject* gPickObject = nullptr;

///엔진 관련
void EngineInitialize(HWND _g_hWnd, bool ON_DEBUG)
{
	gGameEngine = new GameEngine();
	gGameEngine->Initialize(_g_hWnd, ON_DEBUG);
	gGameEngine->Start();
	gGameEngine->SetFocus(true);
}

void EndEngine()
{
	gGameEngine->Finish();
}

void UpdateEngine()
{
	gGameEngine->Update();
}

void OnReSize(int X, int Y)
{
	GameEngine::WinSizeWidth	= X;
	GameEngine::WinSizeHeight	= Y;

	//처음 시작 상태가 아닐때
	if (gGameEngine != nullptr)
	{
		gGameEngine->OnResize(X, Y);
	}
}

void GetWindowSize(int* X, int* Y)
{
	*X = GameEngine::WinSizeWidth;
	*Y = GameEngine::WinSizeHeight;
}

///오브젝트 관련
 GameObject* Instance(std::string ObjName)
{
	return gGameEngine->Instance(ObjName);
}

 GameObject* Instance_Terrain(std::string ObjName)
 {
	 return gGameEngine->Instance_Terrain(ObjName);
 }

 GameObject* Instance_Particle(std::string ObjName, std::string FileName)
 {
	 return gGameEngine->Instance_Particle(ObjName, FileName);
 }

 GameObject* Instance_ParticleController(std::string ObjName/*= "GameObject"*/)
 {
	 return gGameEngine->Instance_ParticleController(ObjName);
 }

 GameObject* Instance_Camera(std::string ObjName)
 {
	 return gGameEngine->Instance_Camera(ObjName);
 }

 GameObject* Instance_Light(std::string ObjName, LIGHT_TYPE type)
 {
	 return gGameEngine->Instance_Light(ObjName, type);
 }

 GameObject* Instance_UI(std::string ObjName)
 {
	 return gGameEngine->Instance_UI(ObjName);
 }

 GameObject* Instance_Image(std::string ObjName/*= "Image"*/)
 {
	 return gGameEngine->Instance_Image(ObjName);
 }

 GameObject* Instance_Slider(std::string ObjName)
 {
	 return gGameEngine->Instance_Slider(ObjName);
 }

 GameObject* Instance_ImageFont(std::string ObjName)
 {
	 return gGameEngine->Instance_ImageFont(ObjName);
 }

 GameObject* Instance_Button(std::string ObjName)
 {
	 return gGameEngine->Instance_Button(ObjName);
 }

 GameObject* FindGameObjectTag(std::string TagName)
 {
	 return gGameEngine->FindGameObjectTag(TagName);
 }

 void FindGameObjectTags(std::string TagName, std::vector<GameObject*>* ObjectList)
 {
	 gGameEngine->FindGameObjectTags(TagName, ObjectList);
 }

 int FindTagNumber(std::string TagName)
 {
	 return  gGameEngine->FindTagNumber(TagName);
 }

 GameObject* FindGameObjectName(std::string ObjectName)
 {
	 return gGameEngine->FindGameObjectName(ObjectName);
 }

 Material* FindMaterialName(std::string MaterialName)
 {
	 return gGameEngine->FindMaterialName(MaterialName);
 }

 void Destroy(GameObject* obj)
 {
	 gGameEngine->Destroy(obj);
 }

 void DestroyAll()
 {
	 gGameEngine->DestroyAll();
 }

 TextureBuffer* GetTexture(std::string TextureName)
 {
	 return gGameEngine->GetTexture(TextureName);
 }

///씬 관련
 void ChoiceScene(std::string name)
{
	gGameEngine->ChoiceScene(name);
}

void CreateSceneSub(Scene* mSceneTemp, std::string SceneName)
{
	gGameEngine->PushScene(mSceneTemp, SceneName);
}

void LoadTerrainMesh(std::string mMeshName, std::string maskName1, std::string maskName2, UINT parsingMode)
{	 
	gGameEngine->LoadTerrain(mMeshName, maskName1, maskName2, parsingMode);
}

void Load(std::string mPath, UINT Mode)
{
	gGameEngine->Load(mPath, Mode);
}

int LoadAssetsCount()
{
  return gGameEngine->LoadMeshCount();
}

void BakeSkyLightMap(std::string mPath, bool hdri)
{
	gGameEngine->BakeSkyLightMap(mPath, hdri);
}

void BakeConvertSkyCubeMap(std::string mPath, float angle, float threshold, bool hdri)
{
	gGameEngine->BakeConvertSkyCubeMap(mPath, angle, threshold, hdri);
}

void BakeConvertSkyLightMap(std::string mPath, float angle, float threshold, bool hdri, UINT index)
{
	gGameEngine->BakeConvertSkyLightMap(mPath, angle, threshold, hdri, index);
}

void BakeAnimation()
{
	gGameEngine->BakeAnimation();
}

void SaveConvertSkyLightMap(std::string mPath, std::string SaveName)
{
	gGameEngine->SaveConvertSkyLightMap(mPath, SaveName);
}

void SaveConvertSkyCubeMap(std::string mPath, std::string SaveName)
{
	gGameEngine->SaveConvertSkyCubeMap(mPath, SaveName);
}

void SaveSpriteToVolumeTexture_LUT(std::string fileName, std::string saveName, UINT pixelSize)
{
	gGameEngine->SaveSpriteToVolumeTexture_LUT(fileName, saveName, pixelSize);
}

void SetSkyCube(std::string mPath)
{
	gGameEngine->SetSkyCube(mPath);
}

void SetSkyLight(std::string mPath, UINT index)
{
	gGameEngine->SetSkyLight(mPath, index);
}

void SetColorGradingBaseTexture(std::string mPath)
{
	gGameEngine->SetColorGradingBaseTexture(mPath);
}

void SetColorGradingBaseTexture(TextureBuffer* base_lut)
{
	gGameEngine->SetColorGradingBaseTexture(base_lut);
}

void SetColorGradingBlendTexture(std::string mPath)
{
	gGameEngine->SetColorGradingBlendTexture(mPath);
}
void SetColorGradingBlendTexture(TextureBuffer* blend_lut)
{
	gGameEngine->SetColorGradingBlendTexture(blend_lut);
}

void SetColorGradingBlendFactor(float blend_factor)
{
	gGameEngine->SetColorGradingBlendFactor(blend_factor);
}

ModelData* GetLoadMeshData(std::string mPath)
{
  return gGameEngine->GetLoadMeshData(mPath);
}

int GetLoadMeshCount()
{
  return gGameEngine->LoadMeshCount();
}

int GetLoadTextureCount()
{
  return gGameEngine->LoadTextureCount();
}

int GetLoadAnimationCount()
{
  return gGameEngine->LoadAnimationCount();
}

int GetLoadMaterialCount()
{
	return gGameEngine->LoadMaterialCount();
}

int GetLoadBufferCount()
{
	return gGameEngine->LoadBufferCount();
}

void SetFullScreenBlur(bool enable, UINT blur_count /*= 0*/)
{
	gGameEngine->SetFullScreenBlur(enable, blur_count);
}

void AddOccluder(std::string mMeshName)
{
	gGameEngine->AddOccluder(mMeshName);
}

///키관련
 bool GetKeyDown(byte number)
{
	return gGameEngine->GetKeyDown(number);
}

 bool GetKeyUp(byte number)
{
	return gGameEngine->GetKeyUp(number);
}

 bool GetKey(byte number)
{
	return gGameEngine->GetKey(number);
}

 bool GetTogle(byte number)
{
	return gGameEngine->GetTogle(number);
}

 void KeyReset()
 {

 }

 int GetMousePosX()
{
	return gGameEngine->GetMousePosX();
}

int GetMousePosY()
{
	return gGameEngine->GetMousePosY();
}

  void SetMousePos(float x, float y)
 {
	  gGameEngine->SetMousePos(x, y);
 }

  void SetMousePosCenter()
  {
	  gGameEngine->SetMousePosCenter();
  }

 void ShowMouseCursor(bool Cursor)
 {
	 gGameEngine->ShowMouseCursor(Cursor);
 }

 void MouseCursorClip(bool Clip)
 {
	 gGameEngine->MouseCursorClip(Clip);
 }

 void WindowFocus(bool focus)
 {
	 if (gGameEngine == nullptr) return;

	 gGameEngine->SetFocus(focus);
 }

 void SetWindowSize(int width, int height)
 {
	 if (gGameEngine == nullptr) return;

	 gGameEngine->SetWindowSize(width, height);
 }

 void SetWindowFullScreen()
 {
	 if (gGameEngine == nullptr) return;

	 gGameEngine->SetWindowFullScreen();
 }

 GameObject* Picking(int x, int y)
 {
	 if (gGameEngine == nullptr) return nullptr;

	 void* object = gGameEngine->Picking(x, y);

	 if (object)
	 {
		 return (GameObject*)object;
	 }

	 return nullptr;
 }

 GameObject* GetMainCamera()
 {
	 return gGameEngine->GetMainCamera();
 }

 GameObject* GetDebugCamera()
 {
	 return gGameEngine->GetDebugCamera();
 }

 GameObject* GetDirectionLight()
 {
	 return gGameEngine->GetDirectionLight();
 }

 void Sound_Play_BGM(std::string& Name)
 {
	 gGameEngine->Sound_Play_BGM(Name);
 }

 void Sound_Play_BGM(std::string&& Name)
 {
	 gGameEngine->Sound_Play_BGM(Name);
 }

 void Sound_Stop_BGM()
 {
	 gGameEngine->Sound_STOP_BGM();
 }

 void Sound_Pause_BGM(bool Pause)
 {
	 gGameEngine->Sound_Pause_BGM(Pause);
 }

 void Sound_VolumeSet_BGM(float value)
 {
	 gGameEngine->Sound_VolumeSet_BGM(value);
 }

 void Sound_VolumeUP_BGM()
 {
	 gGameEngine->Sound_VolumeUP_BGM();
 }

 void Sound_VolumeDown_BGM()
 {
	 gGameEngine->Sound_VolumeDown_BGM();
 }

 void Sound_FrequencyUp_BGM()
 {
	 gGameEngine->Sound_FrequencyUp_BGM();
 }

 void Sound_FrequencyDown_BGM()
 {
	 gGameEngine->Sound_FrequencyDown_BGM();
 }

 void Sound_PitchUp_BGM()
 {
	 gGameEngine->Sound_PitchUp_BGM();
 }

 void Sound_PitchDown_BGM()
 {
	 gGameEngine->Sound_PitchDown_BGM();
 }

 void Sound_Play_SFX(std::string& SoundName)
 {
	 gGameEngine->Sound_Play_SFX(SoundName);
 }

 void Sound_Play_SFX(std::string&& SoundName)
 {
	 gGameEngine->Sound_Play_SFX(SoundName);
 }

 void Sound_Stop_SFX()
 {
	 gGameEngine->Sound_STOP_SFX();
 }

 void Sound_Pause_SFX(bool Pause)
 {
	 gGameEngine->Sound_Pause_SFX(Pause);
 }

 void Sound_VolumeSet_SFX(float value)
 {
	 gGameEngine->Sound_VolumeSet_SFX(value);
 }

 void Sound_VolumeUP_SFX()
 {
	 gGameEngine->Sound_VolumeUP_SFX();
 }

 void Sound_VolumeDown_SFX()
 {
	 gGameEngine->Sound_VolumeDown_SFX();
 }

 void Sound_FrequencyUp_SFX()
 {
	 gGameEngine->Sound_FrequencyUp_SFX();
 }

 void Sound_FrequencyDown_SFX()
 {
	 gGameEngine->Sound_FrequencyDown_SFX();
 }

 void Sound_PitchUp_SFX()
 {
	 gGameEngine->Sound_PitchUp_SFX();
 }

 void Sound_PitchDown_SFX()
 {
	 gGameEngine->Sound_PitchDown_SFX();
 }

 void Sound_VolumeSet_Master(float value)
 {
	 gGameEngine->Sound_VolumeSet_Master(value);
 }

 void Sound_Play_BGM(std::string Name)
 {
	 gGameEngine->Sound_Play_BGM(Name);
 }

 /// 시간 관련
 float GetDeltaTime()
 {
	 return gGameEngine->GetdeltaTime();
 }

  void SetSlowDeltaTime(float mSlow)
 {
	  gGameEngine->SetSlowTime(mSlow);
 }

 bool RayCast(PhysRayCast* ray)
 {
	 return PhysManager::RayCast(ray);
 }

 void RenderSetting()
 {
	 return gGameEngine->RenderSetting();
 }

 RenderOption* GetRenderOptionData()
 {
	 return gGameEngine->GetRenderOptionData();
 }

 void DebugDrawLine(Vector3 start, Vector3 end, Vector3 color)
 {
	 DebugManager::DebugDrawLine(start, end, color);
 }

 void DebugDrawLine(Vector3 start, Vector3 dir, float distance, Vector3 color)
 {
	 DebugManager::DebugDrawLine(start, dir, distance, color);
 }

 void DebugDrawCircle(float scale, DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 rot, DirectX::SimpleMath::Vector3 color)
 {
	 DebugManager::DebugDrawCircle(scale, pos, rot, color);
 }

 void DebugDrawCircle(float scale, DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Quaternion rot, DirectX::SimpleMath::Vector3 color)
 {
	 DebugManager::DebugDrawCircle(scale, pos, rot, color);
 }

 void DebugDrawBox(DirectX::SimpleMath::Vector3 scale, DirectX::SimpleMath::Vector3 rot, DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 color)
 {
	 DebugManager::DebugDrawBox(scale, rot, pos, color);
 }

 void DebugDrawBox(DirectX::SimpleMath::Vector3 scale, DirectX::SimpleMath::Quaternion rot, DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 color)
 {
	 DebugManager::DebugDrawBox(scale, rot, pos, color);
 }

 void DebugDrawSphere(float scale, DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 color)
 {
	 DebugManager::DebugDrawSphere(scale, pos, color);
 }

 void DebugPrint(const char* message, ...)
 {
	 int length;
	 va_list args;

	 va_start(args, message);

	 // 가변인자 문자열 길이 측정..
	 length = _vscprintf(message, args) + 1;

	 // 가변인자 문자열 변환..
	 char* cBuf = new char[length];
	 memset(cBuf, NULL, length);

	 vsprintf(cBuf, message, args);

	 va_end(args);

	 PROFILE_LOG(PROFILE_OUTPUT::VS_CODE, cBuf);

	 delete[] cBuf;
 }

 void Network_SetManager(NetworkManagerComponent* Manager)
 {
	 gGameEngine->SetNetworkManager(Manager);
 }

 void Network_Send(flatbuffers::FlatBufferBuilder* Builder , int Type)
 {
	 gGameEngine->NETWORK_SEND(Builder, Type);
 }

 void Network_Loading_complete(unsigned int Number)
 {
	 gGameEngine->NETWORK_LOADING_COMPLETE(Number);
 }

 void Network_Connect(int ServerPort, std::string  Local_Connect_IP)
 {
	 gGameEngine->NETWORK_CONNECT(ServerPort, Local_Connect_IP);
 }

