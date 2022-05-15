#pragma once
#include <filesystem>
#include <iostream>
#include <string>

class GameObject;
class YamlManager;
class FBXManager;
class EaterManager;
struct ObjectOption;
struct InstanceMaterial;

enum class CHANGE_TYPE
{
	TERRAIN,
	COLLIDER,
	NAVMESH
};



class EditorManager
{
public:
	EditorManager();
	~EditorManager();
	
	void Initialize();
	void ConvertData(std::string Path);
	void ConvertData(std::string Path,std::string ChangeName, CHANGE_TYPE Option);
	

	void OpenEaterFile(std::string Path, int Type);
	void OpenEaterGameObject(GameObject* Object, ObjectOption* Option);
	void CreateMaterialData(InstanceMaterial* m);
private:
	void CreateAssetsFile();
	void CreateBaseObject();

	YamlManager*	mYaml;
	FBXManager*		mFbx;
	EaterManager*	mEater;
};

