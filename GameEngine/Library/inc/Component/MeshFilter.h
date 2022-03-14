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
	class OneAnimation;	
}

class ModelData;
class LoadMeshData;
class Transform;
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
	EATER_ENGINEDLL void SetMeshName(std::string mMeshName);
	EATER_ENGINEDLL void SetDiffuseTextureName(std::string mTextureName);
	EATER_ENGINEDLL void SetNormalTextureName(std::string mTextureName);
	EATER_ENGINEDLL void SetORMTextureName(std::string mTextureName);
	EATER_ENGINEDLL void SetAnimationName(std::string mTextureName);

	EATER_ENGINEDLL void SetEmissiveFactor(float emissiveFactor);
	EATER_ENGINEDLL void SetRoughnessFactor(float roughnessFactor);
	EATER_ENGINEDLL void SetMetallicFactor(float metallicFactor);

	EATER_ENGINEDLL std::string GetMeshName();
	EATER_ENGINEDLL std::string GetDiffuseTextureName();
	EATER_ENGINEDLL std::string GetNormlaTextureName();
	EATER_ENGINEDLL std::string GetORMTextureName();
public:
	void SetObjectData() override;

private:
	//Transform을 연결한다
	void LinkHierarchy(Transform* my,Transform*parent);

	void CreateMesh();

	//초기설정 Texture Setting
	void CheckTexture();
	void CheckAnimation();

	void SetTexture(std::string texName, UINT texType);

private:
	void DeleteChildObject();
	void CreateStaticMesh(LoadMeshData* mMesh,GameObject* Object);
	void CreateBoneMesh(LoadMeshData* mMesh, GameObject* Object);
	void CreateSkinMesh(LoadMeshData* mMesh, GameObject* Object);

	void SetMaterialData(LoadMeshData* LoadMesh, MeshData* mMesh);
	void SetMatrixData(LoadMeshData* LoadMesh, MeshData* mMesh,GameObject* obj);
	void SetBufferData(LoadMeshData* LoadMesh, MeshData* mMesh);
	void SetType(LoadMeshData* LoadMesh, MeshData* mMesh);
	

	bool isLoad_Mesh;			//매쉬 로드여부
	bool isLoad_Texture;		//텍스쳐 로드 여부
	bool isLoad_Animation;		//애니메이션 로드여부

	std::string MeshName;		//로드한 매쉬 이름
	std::string DiffuseTextureName;	//로드한 텍스쳐 이름
	std::string NormlaTextureName;	//로드한 텍스쳐 이름
	std::string ORMTextureName;	//로드한 텍스쳐 이름
	std::string AnimationName;	//로드한 애니메이션 이름

	Material* Materials;		//Material Data

	//본리스트들을 GameObject 타입으로 저장
	std::vector<GameObject*>	BoneList;

	//본오프셋 리스트
	std::vector<DirectX::SimpleMath::Matrix> BoneOffsetList;

	//렌더링 오브젝트 리스트
	std::vector<GameObject*> MeshList;
	
};

