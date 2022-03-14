#pragma once

#include "EaterEngineDLL.h"
#include <vector>
#include <string>
#include "Component.h"
/// <summary>
/// �Ž������� �ļ����� �޾ƿ��� �����صδ� ���۳�Ʈ
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
	//�޽��� �̸��� ������ ������ �ε�
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
	//Transform�� �����Ѵ�
	void LinkHierarchy(Transform* my,Transform*parent);

	void CreateMesh();

	//�ʱ⼳�� Texture Setting
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
	

	bool isLoad_Mesh;			//�Ž� �ε忩��
	bool isLoad_Texture;		//�ؽ��� �ε� ����
	bool isLoad_Animation;		//�ִϸ��̼� �ε忩��

	std::string MeshName;		//�ε��� �Ž� �̸�
	std::string DiffuseTextureName;	//�ε��� �ؽ��� �̸�
	std::string NormlaTextureName;	//�ε��� �ؽ��� �̸�
	std::string ORMTextureName;	//�ε��� �ؽ��� �̸�
	std::string AnimationName;	//�ε��� �ִϸ��̼� �̸�

	Material* Materials;		//Material Data

	//������Ʈ���� GameObject Ÿ������ ����
	std::vector<GameObject*>	BoneList;

	//�������� ����Ʈ
	std::vector<DirectX::SimpleMath::Matrix> BoneOffsetList;

	//������ ������Ʈ ����Ʈ
	std::vector<GameObject*> MeshList;
	
};

