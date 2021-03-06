#pragma once
#include <string>
#include <vector>
#include "SimpleMath.h"

namespace ParserData
{
	class CModel;
	class CMesh;
}

class E_AnimationManager;
class E_MaterialManager;
class E_MeshManager;

class EaterManager;
class ModelParser;
struct ObjectOption;
class FBXManager
{
public:
	FBXManager();
	~FBXManager();

	ParserData::CModel* OpenFile(std::string& Path, UINT Option = 0);
	void Initialize(EaterManager* mManager);
private:
	ModelParser* FbxFactory;
	EaterManager* mEaterManager;
};

