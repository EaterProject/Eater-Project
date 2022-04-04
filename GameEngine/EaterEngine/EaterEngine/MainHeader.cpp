#include "MainHeader.h"
#include "GameEngine.h"
#include "PhysManager.h"
#include "PhysData.h"
#include "EngineData.h"
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
	//gDebugManger->Delete();
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

 void LoadTerrainMesh(std::string mMeshName, std::string maskName, UINT parsingMode)
 {
	 gGameEngine->LoadTerrain(mMeshName, maskName, parsingMode);
 }

  void Load(std::string mPath, UINT Mode)
  {
	  gGameEngine->Load(mPath, Mode);
  }

  int LoadAssetsCount()
  {
	  return gGameEngine->LoadMeshCount();
  }

  ModelData* GetLoadMeshData(std::string mPath)
  {
	  return gGameEngine->GetLoadMeshData(mPath);
  }

  void LoadEnvironment(std::string mPath)
  {
	  gGameEngine->LoadEnvironment(mPath);
  }

  void SetEnvironment(bool enable)
  {
	  gGameEngine->SetEnvironment(enable);
  }

  int GetLoadMeshCount()
  {
	  return  gGameEngine->LoadMeshCount();
  }

  int GetLoadTextureCount()
  {
	  return  gGameEngine->LoadTextureCount();
  }

  int GetLoadAnimationCount()
  {
	  return  gGameEngine->LoadAnimationCount();
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

	 return (GameObject*)gGameEngine->Picking(x, y);
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

 void EditorSetting()
 {
	 gGameEngine->EditorSetting();
 }

 void DebugDrawLine(Vector3 start, Vector3 end, Vector3 color)
 {
	 gGameEngine->DebugDrawLine(start, end, color);
 }

 void DebugDrawLine(Vector3 start, Vector3 dir, float distance, Vector3 color)
 {
	 gGameEngine->DebugDrawLine(start, dir, distance, color);
 }

  void DebugPrint(std::string str)
 {
	  PROFILE_LOG(PROFILE_OUTPUT::CONSOLE, "[ Client ][ Debug ] %s", str.c_str());
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
