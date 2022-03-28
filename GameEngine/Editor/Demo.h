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

public:
	///Create
	static GameObject* Create_GameObject();						//�� ���� ������Ʈ ����
	static GameObject* Create_Object(std::string MeshName);		//������Ʈ ����
	static GameObject* Create_Terrain(std::string MeshName);	//�ͷ��� ����
	static GameObject* Create_Light();							//����Ʈ ����
	static GameObject* Create_Particle();						//��ƼŬ ����
	static GameObject* Create_Camera();							//ī�޶� ����
public:
	///Load,Save,Delete
	static void MeshLoad(std::string Path);						//Object �ε�
	static bool DeleteObject(std::string MeshName);				//Object ����
public:
	///Scene
	static void SaveScene(std::string SaveFilePath,std::string SaveFileName);	//�� ����
	static void LoadScene(std::string LoadScenePath);							//�� �ε�
public:
	///Find
	static GameObject* FindMesh(std::string MeshName);							//�Ž��� ã�´�
	static GameObject* FindMesh(std::string MeshName,std::string ParentName);	//�޽��� �ڽĿ�����Ʈ��ã�´�
	static std::string FindMeshName(std::string MeshName);						//�Ž��� �����̸��������� �����ؼ� �����´�
	static GameObject* FindMainCamera();										//���� ���� ī�޶� �����´�
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

