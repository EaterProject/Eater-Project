#pragma once
#include <string>

namespace ParserData
{
	class Model;
	class Mesh;
}


class E_MaterialManager;
class E_MeshManager;
class E_AnimationManager;

class EaterManager
{
public:
	EaterManager();
	~EaterManager();

	void LoadEaterFile(std::string& Path);
	void Load_FBX_File(std::string& Path,ParserData::Model* FBXMesh);
	void Initialize();
private:
	static const int E_STATIC_MESH	= 0;
	static const int E_SKIN_MESH	= 1;
	static const int E_ANIME_MESH	= 2;
	static const int E_MAT_MESH		= 3;
private:
	std::string CutFileName(std::string FilePath);
	

	E_MeshManager*		MeshManager;
	E_AnimationManager* AnimationManager;
	E_MaterialManager*	MaterialManager;
	E_BufferManager*	BufferManager;
};

