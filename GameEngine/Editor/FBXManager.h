#pragma once
#include <string>
#include <vector>
#include "SimpleMath.h"

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

	struct EaterMaterialData
	{
		std::string MeshName	= "Object";
		std::string DiffuseMap	= "NO";
		std::string NormalMap	= "NO";
		std::string EmissiveMap = "NO";
		std::string ORMMap		= "NO";
		bool Alpha			= false;
		float Roughness		= 0;
		float Metallic		= 0;
		DirectX::SimpleMath::Vector2 Tileing		= {1,1};
		DirectX::SimpleMath::Vector4 BaseColor	= { 1,0.5f,0.5f,1 };
		DirectX::SimpleMath::Vector4 AddColor	= { 0,0,0,0 };
	};

	void OpenFile(std::string& Path, MeshOption* Data);
private:
	void StaticMesh(ParserData::Mesh* mMesh, std::string FileName);
	void BoneMesh(ParserData::Mesh* mMesh);
	void SkinMesh(ParserData::Mesh* mMesh, std::string FileName);
	void TerrainMesh(ParserData::Mesh* mMesh);
	void AnimationMesh(ParserData::Model* mMesh);

	void MaterialSave(std::string FileName);
private:
	void SetParent(ParserData::Mesh* mMesh);
	void SetMatrix(ParserData::Mesh* mMesh);
	void SetMaterial(ParserData::Mesh* mMesh,std::string FileName);
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
	std::vector<EaterMaterialData> OneMeshMaterialList;
	ModelParser* FbxFactory;
	MeshOption* mOption;
	ParserData::Model* mMesh;
	
};

