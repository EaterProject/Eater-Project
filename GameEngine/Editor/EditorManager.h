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
class EditorManager
{
public:
	EditorManager();
	~EditorManager();
	
	void Initialize();
	void SetPath(std::string Path,unsigned int Option = 0);
	void OpenEaterFile(std::string Path, int Type);
	void OpenEaterGameObject(GameObject* Object, ObjectOption* Option);
	void CreateMaterialData(InstanceMaterial* m);
private:
	void CreateAssetsFile();
	void LoadAssets();

	void LoadFolder(std::string& Path);
	void LoadFile(std::string& Path);

	YamlManager*	mYaml;
	FBXManager*		mFbx;
	EaterManager*	mEater;
};

