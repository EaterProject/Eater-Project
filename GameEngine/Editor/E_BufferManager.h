#pragma once

#include <string>
#include <vector>
namespace ParserData
{
	class CMesh;
	class CModel;
}

class E_BufferManager
{
public:
	E_BufferManager();
	~E_BufferManager();
public:
	void ChangeEaterFile(ParserData::CModel* FBXMesh);
	void SetFileName(std::string& FileName);
private:
	void SetVertexBuffer(ParserData::CMesh* mMesh);
	void SetIndexBuffer(ParserData::CMesh* mMesh);
	bool FindInstanceIndex(int Index);

	std::vector<int> MeshIndexList;
	std::string SaveFileName;
};

