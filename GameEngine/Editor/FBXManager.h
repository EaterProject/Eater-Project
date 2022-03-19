#pragma once
#include <string>
#include <vector>
#include "SimpleMath.h"

namespace ParserData
{
	class CModel;
	class CMesh;
}

class E_AnimationManager;
class E_MaterialManager;
class E_MeshManager;

class EaterManager;
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
	void Initialize(EaterManager* mManager);
private:
	//void StaticMesh(ParserData::CMesh* mMesh, std::string FileName);
	void BoneMesh(ParserData::CMesh* mMesh);
	void SkinMesh(ParserData::CMesh* mMesh, std::string FileName);
	void TerrainMesh(ParserData::CMesh* mMesh);
	void AnimationMesh(ParserData::CModel* mMesh);

	void MaterialSave(std::string FileName);
private:
	void SetParent(ParserData::CMesh* mMesh);
	void SetMatrix(ParserData::CMesh* mMesh);
	void SetIndex(ParserData::CMesh* mMesh);

	void SetVertexTerrain(ParserData::CMesh* mMesh);
	void SetVertex(ParserData::CMesh* mMesh);
	void SetVertexSkin(ParserData::CMesh* mMesh);
	void SetBoneOffset(ParserData::CMesh* mMesh);
private:
	std::string CutStr(std::string& Path);
	bool FindInstanceIndex(int Index);
private:
	std::vector<int> MeshIndexList;
	std::vector<EaterMaterialData> OneMeshMaterialList;
	ModelParser* FbxFactory;
	MeshOption* mOption;
	EaterManager* mEaterManager;
	ParserData::CModel* mMesh;
	
};

