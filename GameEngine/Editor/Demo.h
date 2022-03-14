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

	static GameObject* CreateObject(std::string MeshName);		//오브젝트 생성
	static GameObject* CreateTerrain(std::string MeshName);		//터레인 생성
	static GameObject* CreateLight();							//라이트 생성
	static GameObject* CreateParticle();						//파티클 생성

	static void MeshLoad(std::string Path);						//매쉬 로드
	static bool DeleteObject(std::string MeshName);				//오브젝스 삭제
	static void DeleteScene();									//모든 삭제

	static void SaveScene(std::string SaveFilePath,std::string SaveFileName);	//씬 저장
	static void LoadScene(std::string LoadScenePath);							//씬 로드

	static GameObject* FindMesh(std::string MeshName);		//매쉬를 찾는다
	static GameObject* FindMesh(std::string MeshName,std::string ParentName);
	static std::string FindMeshName(std::string MeshName);	//매쉬의 같은이름이있으면 변경해서 가져온다

	static std::map<std::string, GameObject*> ObjectList;
private:
	static GameObject* Object;
	static GameObject* CreateBaseObject(std::string ObjectName,std::string MeshName); 
	static GameObject* CreateSkinObject(std::string ObjectName, std::string MeshName);


	static SceneSave* SaveManager;
};

