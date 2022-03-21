#pragma once
#include <filesystem>
#include <iostream>
#include <string>

class GameObject;
class YamlManager;
class FBXManager;
class EaterManager;
struct MeshOption;
class EditorManager
{
public:
	EditorManager();
	~EditorManager();
	
	void Initialize();
	void SetPath(std::string Path);
	void OpenEaterFile(std::string Path, int Type);
	void OpenEaterGameObject(GameObject* Object);
private:
	void CreateAssetsFile();
	void LoadAssets();

	void LoadFolder(std::string& Path);
	void LoadFile(std::string& Path);

	//파일 경로
	std::string SaveStaticFilePath		= "../Assets/Model/StaticModel";
	std::string SaveSkinFilePath		= "../Assets/Model/SkinModel";
	std::string SaveAnimationFilePath	= "../Assets/Model/Animation";
	std::string SaveMaterialFilePath	= "../Assets/Model/TerrainModel";
	std::string SaveMeshFilePath		= "../Assets/Model/Mesh";

	YamlManager*	mYaml;
	FBXManager*		mFbx;
	EaterManager*	mEater;
};

