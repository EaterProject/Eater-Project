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
	GameEngine::WinSizeWidth = X;
	GameEngine::WinSizeHeight = Y;

	//처음 시작 상태가 아닐때
	if (gGameEngine != nullptr)
	{
		gGameEngine->OnResize(X, Y);
	}
}

///오브젝트 관련
 GameObject* Instance(std::string ObjName)
{
	return gGameEngine->Instance(ObjName);
}

 GameObject* InstanceTerrain(std::string ObjName)
 {
	 return gGameEngine->InstanceTerrain(ObjName);
 }

 GameObject* InstanceParticle(std::string ObjName)
 {
	 return gGameEngine->InstanceParticle(ObjName);
 }

 GameObject* InstanceCamera(std::string ObjName)
 {
	 return gGameEngine->InstanceCamera(ObjName);
 }

 GameObject* InstanceLight(std::string ObjName, LIGHT_TYPE type)
 {
	 return gGameEngine->InstanceLight(ObjName, type);
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

 void Destroy(GameObject* obj)
 {
	 gGameEngine->Destroy(obj);
 }

 void DestroyAll()
 {
	 gGameEngine->DestroyAll();
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

void BakeSkyLightMap(std::string mPath)
{
	gGameEngine->BakeSkyLightMap(mPath);
}

void BakeConvertCubeMap(std::string mPath, float angle)
{
	gGameEngine->BakeConvertCubeMap(mPath, angle, true, false, false);
}

void SetConvertCubeMap(std::string mPath, float angle, bool apply_skylight, bool apply_environment)
{
	gGameEngine->BakeConvertCubeMap(mPath, angle, false, apply_skylight, apply_environment);
}

void BakeAnimation()
{
	gGameEngine->BakeAnimation();
}

void SetEnvironment(std::string mPath)
{
	gGameEngine->SetEnvironment(mPath);
}

void SetSkyLight(std::string mPath)
{
	gGameEngine->SetSkyLight(mPath);
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

 float GetMousePosX()
{
	return gGameEngine->GetMousePosX();
}

 float GetMousePosY()
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

 /// 시간 관련
 float GetDeltaTime()
 {
	 return gGameEngine->GetdeltaTime();
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

	 PROFILE_LOG(PROFILE_OUTPUT::CONSOLE, cBuf);

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
