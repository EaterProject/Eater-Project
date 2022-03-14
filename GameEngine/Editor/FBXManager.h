#pragma once
#include <string>
#include <vector>

namespace ParserData
{
	class Model;
	class Mesh;
}
class ModelParser;
struct MeshOption;
class FBXManager
{
public:
	FBXManager();
	~FBXManager();

	void OpenFile(std::string& Path, MeshOption* Data);

private:
	void StaticMesh(ParserData::Mesh* mMesh);
	void BoneMesh(ParserData::Mesh* mMesh);
	void SkinMesh(ParserData::Mesh* mMesh);
	void TerrainMesh(ParserData::Mesh* mMesh);
	void AnimationMesh(ParserData::Model* mMesh);
private:
	void SetParent(ParserData::Mesh* mMesh);
	void SetMatrix(ParserData::Mesh* mMesh);
	void SetMaterial(ParserData::Mesh* mMesh);
	void SetIndex(ParserData::Mesh* mMesh);

	void SetVertexTerrain(ParserData::Mesh* mMesh);
	void SetVertex(ParserData::Mesh* mMesh);
	void SetVertexSkin(ParserData::Mesh* mMesh);
	void SetBoneOffset(ParserData::Mesh* mMesh);
private:
	std::string CutStr(std::string& Path);
	bool FindInstanceIndex(int Index);
private:
	std::vector<int> MeshIndexList;
	ModelParser* FbxFactory;
	MeshOption* mOption;
	ParserData::Model* mMesh;
};

