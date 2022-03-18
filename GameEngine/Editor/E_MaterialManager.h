#pragma once

#include <string>
#include <vector>
namespace ParserData
{
	struct Mesh;
	struct Model;
}

class E_MaterialManager
{
public:
	E_MaterialManager();
	~E_MaterialManager();

	void ChangeEaterFile(ParserData::Model* FBXMesh);
	void SetFileName(std::string& FileName);
private:
	std::string SaveFileName;
	std::string CutStr(std::string& Path);
};

