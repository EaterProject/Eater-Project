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
	//기본 메쉬 버퍼
	void SetVertexBuffer(ParserData::CMesh* mMesh);
	void SetIndexBuffer(ParserData::CMesh* mMesh);

	//스키닝 매쉬 버퍼
	void SetSkinVertexBuffer(ParserData::CMesh* mMesh);
	bool FindInstanceIndex(int Index);

	std::vector<int> MeshIndexList;
	std::string SaveFileName;
};

