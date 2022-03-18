#pragma once

#include <string>
#include <vector>
namespace ParserData
{
	struct CMesh;
	struct CModel;
}

class E_MaterialManager
{
public:
	E_MaterialManager();
	~E_MaterialManager();

	void ChangeEaterFile(ParserData::CModel* FBXMesh);
	void SetFileName(std::string& FileName);
private:
	std::string SaveFileName;
	std::string CutStr(std::string& Path);
};

