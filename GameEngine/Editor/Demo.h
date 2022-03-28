#pragma once

#include "Scene.h"
#include <string>
#include "EditorData.h"
#include <map>
#include <stack>

class SceneSave;
class GameObject;
class AnimationController;
class Demo : public Scene
{
public:
	Demo();
	~Demo();
public:
	// Scene을(를) 통해 상속됨
	virtual void Awake() override;
	virtual void Update() override;
	virtual void End() override;
	virtual void ThreadFunction();

public:
	///Create
	static GameObject* Create_GameObject();						//빈 게임 오브젝트 생성
	static GameObject* Create_Object(std::string MeshName);		//오브젝트 생성
	static GameObject* Create_Terrain(std::string MeshName);	//터레인 생성
	static GameObject* Create_Light();							//라이트 생성
	static GameObject* Create_Particle();						//파티클 생성
	static GameObject* Create_Camera();							//카메라 생성
public:
	///Load,Save,Delete
	static void MeshLoad(std::string Path);						//Object 로드
	static bool DeleteObject(std::string MeshName);				//Object 삭제
public:
	///Scene
	static void SaveScene(std::string SaveFilePath,std::string SaveFileName);	//씬 저장
	static void LoadScene(std::string LoadScenePath);							//씬 로드
public:
	///Find
	static GameObject* FindMesh(std::string MeshName);							//매쉬를 찾는다
	static GameObject* FindMesh(std::string MeshName,std::string ParentName);	//메쉬의 자식오브젝트를찾는다
	static std::string FindMeshName(std::string MeshName);						//매쉬의 같은이름이있으면 변경해서 가져온다
	static GameObject* FindMainCamera();										//현재 메인 카메라를 가져온다
	static std::map<std::string, GameObject*> ObjectList;
private:
	static GameObject* CamObject;
	static GameObject* DebugCamObject;
	static GameObject* CreateBaseObject(std::string ObjectName, std::string MeshName); 
	static GameObject* CreateSkinObject(std::string ObjectName, std::string MeshName);
private:
	///Manager
	static SceneSave* SaveManager;
};

