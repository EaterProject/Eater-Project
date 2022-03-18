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
	void SetVertexBuffer(ParserData::Mesh* mMesh);
	void SetIndexBuffer(ParserData::Mesh* mMesh);
	bool FindInstanceIndex(int Index);

	std::vector<int> MeshIndexList;
	std::string SaveFileName;
};

