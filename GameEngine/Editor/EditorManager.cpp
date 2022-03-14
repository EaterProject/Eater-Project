#include "EditorManager.h"
#include "FBXManager.h"
#include "YamlManager.h"
#include "EaterManager.h"
#include "EditorData.h"

EditorManager::EditorManager()
{
	mYaml	= nullptr;
	mFbx	= nullptr;
}

EditorManager::~EditorManager()
{
	delete mYaml;
	delete mFbx;
}

void EditorManager::Initialize()
{
	mYaml	= new YamlManager();
	mFbx	= new FBXManager();
}

void EditorManager::SetPath(std::string Path,MeshOption* Data)
{
	size_t start = Path.rfind(".")+1;
	size_t end = Path.length()- start;
	std::string FileType = Path.substr(start, end);

	if (FileType == "txt")
	{

	}
	else if (FileType == "Eater")
	{
		mEater->OpenFile(Path);
	}
	else if (FileType == "fbx")
	{
		mFbx->OpenFile(Path, Data);
	}
	else if (FileType == "unity")
	{
		mYaml->OpenFile(Path);
	}
}


