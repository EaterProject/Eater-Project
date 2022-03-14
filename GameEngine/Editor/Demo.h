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
	// Scene��(��) ���� ��ӵ�
	virtual void Awake() override;
	virtual void Update() override;
	virtual void End() override;
	virtual void ThreadFunction();

	static GameObject* CreateObject(std::string MeshName);		//������Ʈ ����
	static GameObject* CreateTerrain(std::string MeshName);		//�ͷ��� ����
	static GameObject* CreateLight();							//����Ʈ ����
	static GameObject* CreateParticle();						//��ƼŬ ����

	static void MeshLoad(std::string Path);						//�Ž� �ε�
	static bool DeleteObject(std::string MeshName);				//�������� ����
	static void DeleteScene();									//��� ����

	static void SaveScene(std::string SaveFilePath,std::string SaveFileName);	//�� ����
	static void LoadScene(std::string LoadScenePath);							//�� �ε�

	static GameObject* FindMesh(std::string MeshName);		//�Ž��� ã�´�
	static GameObject* FindMesh(std::string MeshName,std::string ParentName);
	static std::string FindMeshName(std::string MeshName);	//�Ž��� �����̸��������� �����ؼ� �����´�

	static std::map<std::string, GameObject*> ObjectList;
private:
	static GameObject* Object;
	static GameObject* CreateBaseObject(std::string ObjectName,std::string MeshName); 
	static GameObject* CreateSkinObject(std::string ObjectName, std::string MeshName);


	static SceneSave* SaveManager;
};

