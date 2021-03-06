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
	NAVMESH,
	ANIMATION,
};



class EditorManager
{
public:
	EditorManager();
	~EditorManager();
	
	void Initialize();
	void ConvertData(std::string Path);
	void ConvertData(std::string Path,std::string ChangeName, CHANGE_TYPE Option);
	void ConvertAnimationData(std::string Path, std::string ChangeName, float min, float max);
	

	void OpenEaterFile(std::string Path, int Type);
	void OpenEaterGameObject(GameObject* Object, ObjectOption* Option);
	void CreateMaterialData(InstanceMaterial* m);

	void CreateMaterialData(GameObject* Obj, std::string& FilePath);
	void CreateParticleData(GameObject* Obj, std::string& FilePath);
private:
	void CreateAssetsFile();
	void CreateBaseObject();

	YamlManager*	mYaml;
	FBXManager*		mFbx;
	EaterManager*	mEater;
};

