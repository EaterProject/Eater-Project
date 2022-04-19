#pragma once

#include <string>
#include <vector>
namespace ParserData
{
	class CMesh;
	class CModel;
}
class OneTriangle;
class E_BufferManager
{
public:
	E_BufferManager();
	~E_BufferManager();
public:
	void ChangeEaterFile(ParserData::CModel* FBXMesh);									//기본값으로 MeshBuffer를 뽑는다
	void ChangeEaterFile_Pos(ParserData::CModel* FBXMesh,std::string NewFileName);		//매쉬에서 MeshBuffer만 뽑는다
	void ChangeEaterFile_NavMEsh(ParserData::CModel* FBXMesh, std::string NewFileName);	//매쉬에서 MeshBuffer만 뽑아서 네비매쉬로 만들어준다
	void SetFileName(std::string& FileName);
private:
	//기본 메쉬 버퍼
	void SetVertexBuffer(ParserData::CMesh* mMesh);
	void SetPosVertexBuffer(ParserData::CMesh* mMesh);
	void SetIndexBuffer(ParserData::CMesh* mMesh);
	void SetNavigationBuffer(ParserData::CMesh* mMesh);

	//스키닝 매쉬 버퍼
	void SetSkinVertexBuffer(ParserData::CMesh* mMesh);
	bool FindInstanceIndex(int Index);

	std::vector<int> MeshIndexList;
	std::string SaveFileName;

	std::vector<OneTriangle*> Data;
};

