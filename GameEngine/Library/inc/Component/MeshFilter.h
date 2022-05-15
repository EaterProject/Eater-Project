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
	//�޽��� �̸��� ������ ������ �ε�
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
	//Transform�� �����Ѵ�
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

	bool isLoad_Model;			//�� �ε忩��
	bool isLoad_Mesh;			//�Ž� �ε忩��
	bool isLoad_Material;		//���͸��� �ε忩��
	bool isLoad_Texture;		//�ؽ��� �ε忩��
	bool isLoad_Animation;		//�ִϸ��̼� �ε忩��

	std::string ModelName;				//�ε��� �� �̸�
	std::string AnimationName;			//�ε��� �ִϸ��̼� �̸�

	std::string BufferName;				//�ε��� �Ž� �̸�
	std::string MaterialName;			//�ε��� ���͸��� �̸�
	std::string DiffuseTextureName;		//�ε��� �ؽ��� �̸�
	std::string NormalTextureName;		//�ε��� �ؽ��� �̸�
	std::string EmissiveTextureName;	//�ε��� �ؽ��� �̸�
	std::string ORMTextureName;			//�ε��� �ؽ��� �̸�

	//������Ʈ���� GameObject Ÿ������ ����
	std::vector<GameObject*>	BoneList;

	//�������� ����Ʈ
	std::vector<DirectX::SimpleMath::Matrix> BoneOffsetList;

	//������ ������Ʈ ����Ʈ
	std::vector<GameObject*> MeshList;
	
};

