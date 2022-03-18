#pragma once

#include <string>
#include <vector>
namespace ParserData
{
	class Mesh;
	class Model;
}

class E_MeshManager
{
public:
	E_MeshManager();
	~E_MeshManager();

	void ChangeEaterFile(ParserData::Model* FBXMesh);
	void SetFileName(std::string& FileName);
private:
	void ChangeEaterFile_Static(ParserData::Mesh* OneMesh);
	void ChangeEaterFile_Bone(ParserData::Mesh* OneMesh);
	void ChangeEaterFile_Skin(ParserData::Mesh* OneMesh);
private:
	void SetDataName(ParserData::Mesh* mMesh);
	void SetMatrix(ParserData::Mesh* mMesh);

	void SetIndexBuffer(ParserData::Mesh* mMesh);
	void SetVertexBuffer(ParserData::Mesh* mMesh);
	bool FindInstanceIndex(int Index);
private:
	std::string CutFileName(std::string FilePath);
private:
	std::string SaveFileName;
	std::vector<int> MeshIndexList;
};

