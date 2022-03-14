#pragma once
#include <map>
#include <string>

class GameObject;
class SceneSave
{
public:
	SceneSave();
	~SceneSave();

	void Save(std::string SaveFilePath,std::string SaveFileName,std::map<std::string, GameObject*>& Data);
	void Load(std::string FileName);
private:
	void SaveTransform(GameObject* Obj);
	void SaveMeshFilter(GameObject* Obj);
	void SaveAnimation(GameObject* Obj);
	void SaveObject(GameObject* Obj);

	void Load_Component_Transform(int index, GameObject* Object);
	void Load_Component_MeshFilter(int index, GameObject* Object);
	void Load_Component_Skinning(int index, GameObject* Object);

	void CreateStaticObject(int index, GameObject* Object);
	void CreateSkinObject(int index, GameObject* Object);
};

