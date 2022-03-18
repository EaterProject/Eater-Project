#pragma once

#include <string>
#include <vector>
namespace ParserData
{
	class Mesh;
	class Model;
}

class E_BufferManager
{
public:
	E_BufferManager();
	~E_BufferManager();
public:
	void ChangeEaterFile(ParserData::Model* FBXMesh);
	void SetFileName(std::string& FileName);
private:
	std::string SaveFileName;
};

