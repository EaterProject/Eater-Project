#pragma once

#include <string>
#include <vector>
namespace ParserData
{
	class CMesh;
	class CModel;
}

class E_MeshManager
{
public:
	E_MeshManager();
	~E_MeshManager();

	void ChangeEaterFile(ParserData::CModel* FBXMesh);
	void SetFileName(std::string& FileName);
	void CreateBox();
private:
	void ChangeEaterFile_Static(ParserData::CMesh* OneMesh);
	void ChangeEaterFile_Skin(ParserData::CMesh* OneMesh);
private:
	void SetDataName(ParserData::CMesh* mMesh);
	void SetMatrix(ParserData::CMesh* mMesh);

	void SetBoneOffset(ParserData::CMesh* mMesh);
private:
	std::string CutFileName(std::string FilePath);
private:
	std::string SaveFileName;
	std::vector<int> MeshIndexList;
	bool BoneOffsetSave = false;
};

