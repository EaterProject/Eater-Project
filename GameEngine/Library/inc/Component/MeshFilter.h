#pragma once

#include "EaterEngineDLL.h"
#include <vector>
#include <string>
#include "Component.h"
/// <summary>
/// 매쉬정보를 파서에서 받아오고 저장해두는 컨퍼넌트
/// </summary>


namespace DirectX
{
	namespace SimpleMath
	{
		struct Matrix;
	}
}

namespace ParserData
{
	class CAnimation;	
}

class ModelData;
class LoadMeshData;
class Transform;
class Mesh;
class Material;
class MeshData;

class  MeshFilter : public Component
{
public:
	EATER_ENGINEDLL MeshFilter();
	virtual ~MeshFilter();

public:
	virtual void Start() override;

public:
	//메쉬의 이름을 넣으면 데이터 로드
	EATER_ENGINEDLL void SetModelName(std::string mModelName);
	EATER_ENGINEDLL void SetMeshName(std::string mMeshName);
	EATER_ENGINEDLL void SetMaterialName(std::string mMatName);
	EATER_ENGINEDLL void SetAnimationName(std::string mTextureName);

	EATER_ENGINEDLL void SetDiffuseTextureName(std::string mTextureName);
	EATER_ENGINEDLL void SetNormalTextureName(std::string mTextureName);
	EATER_ENGINEDLL void SetEmissiveTextureName(std::string mTextureName);
	EATER_ENGINEDLL void SetORMTextureName(std::string mTextureName);

	EATER_ENGINEDLL void SetEmissiveFactor(float emissiveFactor);
	EATER_ENGINEDLL void SetRoughnessFactor(float roughnessFactor);
	EATER_ENGINEDLL void SetMetallicFactor(float metallicFactor);

	EATER_ENGINEDLL std::string GetBufferName();
	EATER_ENGINEDLL std::string GetModelName();
	EATER_ENGINEDLL std::string GetMaterialName();
	EATER_ENGINEDLL std::string GetDiffuseTextureName();
	EATER_ENGINEDLL std::string GetNormlaTextureName();
	EATER_ENGINEDLL std::string GetORMTextureName();

	EATER_ENGINEDLL Material* GetMaterial();

public:
	Mesh* m_Mesh;
	Material* m_Material;

private:
	//Transform을 연결한다
	void LinkHierarchy(Transform* my,Transform*parent);

	void CreateModel();

	void CheckMesh();
	void CheckMaterial();
	void CheckTexture();
	void CheckAnimation();

	void SetBuffer(std::string meshName);
	void SetMaterial(std::string matName);
	void SetTexture(std::string texName, UINT texType);

private:
	void DeleteChildObject();
	void CreateStaticMesh(LoadMeshData* mMesh,GameObject* Object);
	void CreateBoneMesh(LoadMeshData* mMesh, GameObject* Object);
	void CreateSkinMesh(LoadMeshData* mMesh, GameObject* Object);

	void SetMaterialData(LoadMeshData* LoadMesh, MeshData* mMesh, GameObject* obj);
	void SetMatrixData(LoadMeshData* LoadMesh, MeshData* mMesh, GameObject* obj);
	void SetMeshData(LoadMeshData* LoadMesh, MeshData* mMesh, GameObject* obj);
	void SetType(LoadMeshData* LoadMesh, MeshData* mMesh);

	bool isLoad_Model;			//모델 로드여부
	bool isLoad_Mesh;			//매쉬 로드여부
	bool isLoad_Material;		//매터리얼 로드여부
	bool isLoad_Texture;		//텍스쳐 로드여부
	bool isLoad_Animation;		//애니메이션 로드여부

	std::string ModelName;				//로드한 모델 이름
	std::string AnimationName;			//로드한 애니메이션 이름

	std::string BufferName;				//로드한 매쉬 이름
	std::string MaterialName;			//로드한 매터리얼 이름
	std::string DiffuseTextureName;		//로드한 텍스쳐 이름
	std::string NormalTextureName;		//로드한 텍스쳐 이름
	std::string EmissiveTextureName;	//로드한 텍스쳐 이름
	std::string ORMTextureName;			//로드한 텍스쳐 이름

	//본리스트들을 GameObject 타입으로 저장
	std::vector<GameObject*>	BoneList;

	//본오프셋 리스트
	std::vector<DirectX::SimpleMath::Matrix> BoneOffsetList;

	//렌더링 오브젝트 리스트
	std::vector<GameObject*> MeshList;
	
};

