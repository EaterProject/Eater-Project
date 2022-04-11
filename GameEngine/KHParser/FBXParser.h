#pragma once

class FBXParser : public ModelParser
{
public:
	FBXParser() = default;
	~FBXParser() = default;

public:
	void Initialize() override;
	void Release() override;
	ParserData::CModel* LoadModel(std::string fileName, UINT state) override;

	void SceneSetting(std::string fileName);
	void CreateModel();
	void ResetData();

	void LoadMaterial();
	void LoadNode(fbxsdk::FbxNode* node, fbxsdk::FbxNodeAttribute::EType attribute);
	void LoadAnimation(fbxsdk::FbxNode* node);

	void ProcessSkeleton(fbxsdk::FbxNode* node);
	void ProcessMesh(fbxsdk::FbxNode* node);

	bool ProcessBoneWeights(fbxsdk::FbxNode* node, std::vector<BoneWeights>& meshBoneWeights);

	void ProcessAnimation(fbxsdk::FbxNode* node);

	void OptimizeData();
	void OptimizeVertex(ParserData::CMesh* pMesh);
	void SetIndex(ParserData::CMesh* pMesh);
	void RecombinationTM(ParserData::CMesh* pMesh);

	DirectX::SimpleMath::Vector2 ConvertVector2(fbxsdk::FbxVector2 v2);
	DirectX::SimpleMath::Vector2 ConvertVector2(fbxsdk::FbxVector4 v4);
	DirectX::SimpleMath::Vector3 ConvertVector3(fbxsdk::FbxVector4 v4);
	DirectX::SimpleMath::Vector4 ConvertVector4(fbxsdk::FbxVector4 v4);
	DirectX::SimpleMath::Matrix ConvertMatrix(fbxsdk::FbxMatrix matrix);
	std::string ConvertFileRoute(const char* fileName);

	DirectX::SimpleMath::Vector3 GetPos(fbxsdk::FbxMesh* mesh, int vertexIndex);
	DirectX::SimpleMath::Vector3 GetNormal(fbxsdk::FbxMesh* mesh, int vertexIndex, int vertexCount);
	DirectX::SimpleMath::Vector2 GetUV(fbxsdk::FbxMesh* mesh, int vertexIndex, int uvIndex);
	void LinkMaterialByPolygon(fbxsdk::FbxMesh* mesh, int polygonIndex, int vertexIndex);

	void SetTransform(fbxsdk::FbxNode* node);

	int GetMaterialIndex(fbxsdk::FbxSurfaceMaterial* material);
	void SetMaterial(fbxsdk::FbxSurfaceMaterial* material);
	void SetTexture(fbxsdk::FbxSurfaceMaterial* material, const char* materialName);
	void CreateVertex(fbxsdk::FbxMesh* mesh, std::vector<BoneWeights>& boneWeights, int vertexCount);
	void CreateMesh();

	int FindBoneIndex(std::string boneName);
	ParserData::CMesh* FindMesh(std::string meshName);

private:
	fbxsdk::FbxManager* pManager;
	fbxsdk::FbxImporter* pImporter;
	fbxsdk::FbxScene* pScene;
	fbxsdk::FbxGeometryConverter* pConverter;

	fbxsdk::FbxMesh* pMesh;

	std::string fbxFileName;
	std::vector<fbxsdk::FbxSurfaceMaterial*> fbxMaterials;
	std::vector<fbxsdk::FbxGeometry*> fbxGeometries;

private:
	ParserData::CModel* m_Model;
	std::vector<ParserData::CModel*> m_ModelList;

	ParserData::CMaterial* m_MaterialData;			// Material Data Struct

	ParserData::CMesh* m_OneMesh;					// Mesh Data Struct

	std::vector<ParserData::CMesh*> m_AllBoneList;			// Skinning Object Bone List

	ParserData::CAnimation* m_OneAnimation;		// Object One Animation Data

	int m_KeyFrames;		// Animation Total Frame
	int m_StartTime;		// Animation Start Frame
	float m_TickFrame;		// One Frame Time

	UINT m_ParsingMode = 0;
};

inline DirectX::SimpleMath::Vector2 FBXParser::ConvertVector2(fbxsdk::FbxVector2 v2)
{
	return DirectX::SimpleMath::Vector2
	(
		static_cast<float>(v2.mData[0]),
		1.0f - static_cast<float>(v2.mData[1])
	);
}

inline DirectX::SimpleMath::Vector2 FBXParser::ConvertVector2(fbxsdk::FbxVector4 v4)
{
	return DirectX::SimpleMath::Vector2
	(
		static_cast<float>(v4.mData[0]),
		1.0f - static_cast<float>(v4.mData[1])
	);
}

inline DirectX::SimpleMath::Vector3 FBXParser::ConvertVector3(fbxsdk::FbxVector4 v4)
{
	// xyz -> xzy
	return DirectX::SimpleMath::Vector3
	(
		static_cast<float>(v4.mData[0]),
		static_cast<float>(v4.mData[2]),
		static_cast<float>(v4.mData[1])
	);
}

inline DirectX::SimpleMath::Vector4 FBXParser::ConvertVector4(fbxsdk::FbxVector4 v4)
{
	// xyzw -> xzyw
	return DirectX::SimpleMath::Vector4
	(
		static_cast<float>(v4.mData[0]),
		static_cast<float>(v4.mData[2]),
		static_cast<float>(v4.mData[1]),
		static_cast<float>(v4.mData[3])
	);
}

inline DirectX::SimpleMath::Matrix FBXParser::ConvertMatrix(fbxsdk::FbxMatrix matrix)
{
	FbxVector4 r1 = matrix.GetRow(0);
	FbxVector4 r2 = matrix.GetRow(1);
	FbxVector4 r3 = matrix.GetRow(2);
	FbxVector4 r4 = matrix.GetRow(3);

	// 2행, 3행 변경..
	return DirectX::SimpleMath::Matrix
	(
		ConvertVector4(r1),
		ConvertVector4(r3),
		ConvertVector4(r2),
		ConvertVector4(r4)
	);
}

inline std::string FBXParser::ConvertFileRoute(const char* fileName)
{
	std::string filePath = fileName;

	size_t indexSlash = filePath.rfind("\\") + 1;
	size_t indexDot = filePath.rfind(".");

	// 파일 임시경로..
	std::string fileRoute = g_TextureRoute;

	std::string fileame = filePath.substr(indexSlash, indexDot - indexSlash);
	fileRoute += filePath.substr(indexSlash, filePath.size() - indexSlash);

	return fileRoute;
}
