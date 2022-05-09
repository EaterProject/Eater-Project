#pragma once

#include <string>
#include <vector>
#include "EditorData.h"
namespace ParserData
{
	class CMesh;
	class CModel;
}
struct InstanceMaterial;
class E_MaterialManager
{
public:
	E_MaterialManager();
	~E_MaterialManager();

	void ChangeEaterFile(ParserData::CModel* FBXMesh);
	void SetFileName(std::string& FileName);

	void Create(InstanceMaterial* mInstanceMaterial);
	void CreateBase(std::string Filename);
private:
	std::string SaveFileName;
	std::string CutStr(std::string& Path);
};

