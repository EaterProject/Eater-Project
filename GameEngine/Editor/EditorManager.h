#pragma once
#include <string>

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
	void SetPath(std::string Path, MeshOption* Data);
private:
	YamlManager*	mYaml;
	FBXManager*		mFbx;
	EaterManager*	mEater;
};

