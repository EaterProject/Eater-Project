#include "MeshFilter.h"
#include "DebugManager.h"
#include "SkinningFilter.h"
#include "AnimationController.h"
#include "GameObject.h"
#include "LoadManager.h"
#include "Transform.h"
#include "EngineData.h"
#include "Animator.h"
#include "BaseManager.h"
#include "ObjectManager.h"
#include "Material.h"
#include "Terrain.h"
#include "MainHeader.h"
#include "TextureManager.h"
#include "GraphicsEngine.h"


MeshFilter::MeshFilter()
{
	MeshName = "";

	//Component Terrain���� ���� ����Ǿ����
	SetUp_Order = FUNCTION_ORDER_FIRST;
	Start_Order = FUNCTION_ORDER_FIRST;
}

MeshFilter::~MeshFilter()
{
	if (Materials != nullptr)
	{
		delete Materials;
		Materials = nullptr;
	}
}

void MeshFilter::Start()
{
	//Ŭ���̾�Ʈ�ʿ��� �ؽ����� �̸��� �ְ� �ִϸ��̼��� �ְ� ��� ��������
	if (isLoad_Mesh == true)
	{
		CreateMesh();
	}

	if (isLoad_Material == true)
	{
		CheckMaterial();
	}

	if (isLoad_Texture == true)
	{
		CheckTexture();
	}

	if (isLoad_Animation == true)
	{
		CheckAnimation();
	}

	//���൵�� �ؽ��ĳ� �޽��� �ٲܼ��ְ� ��� true
	isLoad_Mesh = true;
	isLoad_Material = true;
	isLoad_Texture = true;
	isLoad_Animation = true;
}

void MeshFilter::SetMeshName(std::string mMeshName)
{
	if (isLoad_Mesh == false)
	{
		isLoad_Mesh = true;
		MeshName = mMeshName;
	}
	else
	{
		MeshName = mMeshName;
		//�Ž��� ���Ӱ� �־��ֱ����� ���� ������ �ڽĿ�����Ʈ���� ����
		DeleteChildObject();
		CreateMesh();
	}
}

void MeshFilter::SetMaterialName(std::string mMatName)
{
	if (isLoad_Material == false)
	{
		isLoad_Material = true;
		MaterialName = mMatName;
	}
	else
	{
		MaterialName = mMatName;

		SetMaterial(mMatName);
	}
}

void MeshFilter::SetDiffuseTextureName(std::string mTextureName)
{
	if (isLoad_Texture == false)
	{
		isLoad_Texture = true;
		DiffuseTextureName = mTextureName;
	}
	else
	{
		DiffuseTextureName = mTextureName;
		SetTexture(mTextureName, DIFFUSE_TEXTURE);
	}
}

void MeshFilter::SetNormalTextureName(std::string mTextureName)
{
	if (isLoad_Texture == false)
	{
		isLoad_Texture = true;
		NormalTextureName = mTextureName;
	}
	else
	{
		NormalTextureName = mTextureName;
		SetTexture(mTextureName, NORMAL_TEXTURE);
	}
}

void MeshFilter::SetEmissiveTextureName(std::string mTextureName)
{
	if (isLoad_Texture == false)
	{
		isLoad_Texture = true;
		EmissiveTextureName = mTextureName;
	}
	else
	{
		EmissiveTextureName = mTextureName;
		SetTexture(mTextureName, EMISSIVE_TEXTURE);
	}
}

void MeshFilter::SetORMTextureName(std::string mTextureName)
{
	if (isLoad_Texture == false)
	{
		isLoad_Texture = true;
		ORMTextureName = mTextureName;
	}
	else
	{
		ORMTextureName = mTextureName;
		SetTexture(mTextureName, ORM_TEXTURE);
	}
}

void MeshFilter::SetAnimationName(std::string mAnimeName)
{
	isLoad_Animation = true;
	AnimationName = mAnimeName;
	CheckAnimation();
}

void MeshFilter::SetEmissiveFactor(float emissiveFactor)
{
	Materials->SetEmissiveFactor(emissiveFactor);
}

void MeshFilter::SetRoughnessFactor(float roughnessFactor)
{
	Materials->SetRoughnessFactor(roughnessFactor);
}

void MeshFilter::SetMetallicFactor(float metallicFactor)
{
	Materials->SetMetallicFactor(metallicFactor);
}

std::string MeshFilter::GetMeshName()
{
	return MeshName;
}

std::string MeshFilter::GetDiffuseTextureName()
{
	return DiffuseTextureName;
}

std::string MeshFilter::GetNormlaTextureName()
{
	return NormalTextureName;
}

std::string MeshFilter::GetORMTextureName()
{
	return ORMTextureName;
}

Material* MeshFilter::GetMaterial()
{
	return Materials;
}

void MeshFilter::CheckMaterial()
{
	if (MaterialName.empty() == false)
	{
		// Material Setting..
		SetMaterial(MaterialName);
	}
}

void MeshFilter::CheckTexture()
{
	// ������ Texture�� ������� �ε��� Texture ��� �ش� Texture ����..
	if (DiffuseTextureName.empty() == false)
	{
		// Diffuse Texture Setting..
		SetTexture(DiffuseTextureName, DIFFUSE_TEXTURE);
	}
	if (NormalTextureName.empty() == false)
	{
		// Normal Texture Setting..
		SetTexture(NormalTextureName, NORMAL_TEXTURE);
	}
	if (ORMTextureName.empty() == false)
	{
		// ORM Texture Setting..
		SetTexture(ORMTextureName, ORM_TEXTURE);
	}
}

void MeshFilter::CheckAnimation()
{
	if (isLoad_Animation == false) { return; }

	ModelAnimationData* data = LoadManager::GetAnimation(AnimationName);
	AnimationController* Controller = gameobject->GetComponent<AnimationController>();

	//������ ���۳�Ʈ�� �� ������ �Ѱ��ش�
	if (Controller != nullptr)
	{
		Controller->SetBoneList(&BoneList);
		Controller->SetAnimeList(data);
	}
}

void MeshFilter::SetMaterial(std::string matName)
{
	Material* material = LoadManager::GetMaterial(matName);

	// �ش� Material�� ���ٸ�..
	if (material == nullptr) return;

	// �ش� Material ����..
	Materials = material;
}

void MeshFilter::SetTexture(std::string texName, UINT texType)
{
	// Texture ����..
	switch (texType)
	{
	case DIFFUSE_TEXTURE:
		Materials->SetDiffuseTexture(texName);
		break;
	case NORMAL_TEXTURE:
		Materials->SetNormalTexture(texName);
		break;
	case EMISSIVE_TEXTURE:
		Materials->SetEmissiveTexture(texName);
		break;
	case ORM_TEXTURE:
		Materials->SetORMTexture(texName);
		break;
	default:
		break;
	}
}

void MeshFilter::DeleteChildObject()
{
	int MeshCount = gameobject->GetChildMeshCount();
	int BoneCount = gameobject->GetChildBoneCount();

	for (int i = 0; i < MeshCount; i++)
	{
		GameObject* ChildObject = gameobject->GetChildMesh(i);
		Destroy(ChildObject);
	}

	for (int j = 0; j < BoneCount; j++)
	{
		GameObject* ChildObject = gameobject->GetChildBone(j);
		Destroy(ChildObject);
	}
}

void MeshFilter::CreateStaticMesh(LoadMeshData* mMesh, GameObject* Object)
{
	MeshData* Data = Object->OneMeshData;

	Object->Name = mMesh->Name;

	SetMatrixData(mMesh, Data, Object);
	SetMaterialData(mMesh, Data);
	SetBufferData(mMesh, Data);
	SetType(mMesh, Data);

	int ChildCount = (int)mMesh->Child.size();
	for (int i = 0; i < ChildCount; i++)
	{
		GameObject* ChildObject = Instance();
		MeshFilter* mMeshFilter = ChildObject->AddComponent<MeshFilter>();

		Object->PushChildMeshObject(ChildObject);
		LinkHierarchy(ChildObject->GetTransform(), Object->GetTransform());

		CreateStaticMesh(mMesh->Child[i], ChildObject);
	}
}

void MeshFilter::CreateBoneMesh(LoadMeshData* mMesh, GameObject* Object)
{
	MeshData* Data = Object->OneMeshData;
	SetMatrixData(mMesh, Data, Object);
	int index = mMesh->BoneIndex;
	BoneList[index] = Object;
	Data->Object_Data->ObjType = OBJECT_TYPE::BONE;
	Object->Name = mMesh->Name;

	int ChildCount = (int)mMesh->Child.size();
	for (int i = 0; i < ChildCount; i++)
	{
		//���ο� ������Ʈ ���� (Transform�� �ڵ����� ��)
		GameObject* ChildObject = Instance();
		MeshFilter* mMeshFilter = ChildObject->AddComponent<MeshFilter>();
		Animator* mAnimator = ChildObject->AddComponent<Animator>();

		//������Ʈ�� ���� , Transform �� ����
		Object->PushChildBoneObject(ChildObject);
		LinkHierarchy(ChildObject->GetTransform(), Object->GetTransform());

		//���� �ڽ����� �̵�
		CreateBoneMesh(mMesh->Child[i], ChildObject);
	}
}

void MeshFilter::CreateSkinMesh(LoadMeshData* mMesh, GameObject* Object)
{
	MeshData* Data = Object->OneMeshData;

	if (Object->Name == "")
	{
		Object->Name = mMesh->Name;
	}

	SetMatrixData(mMesh, Data, Object);
	SetMaterialData(mMesh, Data);
	SetBufferData(mMesh, Data);

	Data->Object_Data->ObjType = OBJECT_TYPE::SKINNING;

	int ChildCount = (int)mMesh->Child.size();
	for (int i = 0; i < ChildCount; i++)
	{
		GameObject* ChildObject = Instance();
		MeshFilter* mMeshFilter = ChildObject->AddComponent<MeshFilter>();

		//LinkHierarchy(ChildObject->GetTransform(), Object->GetTransform());

		CreateSkinMesh(mMesh->Child[i], ChildObject);
	}
}

void MeshFilter::SetMaterialData(LoadMeshData* LoadMesh, MeshData* mMesh)
{
	/// Material �����ͼ� �����������
	Material* material = LoadManager::GetMaterial(LoadMesh->MaterialName);

	// �ε� ���ο� ���� Material ����..
	if (material)
	{
		// �ε��� Material ����..
		Materials = material;
	}
	else
	{
		// ���ο� Material ����..
		Materials = new Material();
	}

	// Render Material Data ����..
	mMesh->Material_Buffer = Materials->m_MaterialData;
}

void MeshFilter::SetMatrixData(LoadMeshData* LoadMesh, MeshData* mMesh, GameObject* Object)
{
	Transform* mTransform = Object->GetTransform();

	mTransform->Load_Local = LoadMesh->LocalTM;
	mTransform->Load_World = LoadMesh->WorldTM;

	mMesh->Object_Data->World = &mTransform->Load_Local;
	mMesh->Object_Data->Local = &mTransform->Load_World;
}

void MeshFilter::SetBufferData(LoadMeshData* LoadMesh, MeshData* mMesh)
{
	mMesh->Mesh_Buffer = LoadMesh->MeshBuffer_Data;
}

void MeshFilter::SetType(LoadMeshData* LoadMesh, MeshData* mMesh)
{
	switch (LoadMesh->MeshType)
	{
	case MESH_TYPE::STATIC_MESH:
		mMesh->Object_Data->ObjType = OBJECT_TYPE::BASE;
		break;
	case MESH_TYPE::TERRAIN_MESH:
		mMesh->Object_Data->ObjType = OBJECT_TYPE::TERRAIN;
		break;
	case MESH_TYPE::QUAD_MESH:
		mMesh->Object_Data->ObjType = OBJECT_TYPE::TEXTURE;
		break;
	case MESH_TYPE::SKIN_MESH:
		mMesh->Object_Data->ObjType = OBJECT_TYPE::SKINNING;
		break;
	case MESH_TYPE::BONE_MESH:
		mMesh->Object_Data->ObjType = OBJECT_TYPE::BONE;
		break;
	}
}

void MeshFilter::LinkHierarchy(Transform* my, Transform* parent)
{
	my->SetParent(parent);
	parent->SetChild(my);
}

void MeshFilter::CreateMesh()
{
	///�̸����� �ε��� �����͸� ã�Ƽ� ������
	ModelData* mMesh = LoadManager::GetMesh(MeshName);
	Transform* Tr = gameobject->GetTransform();

	if (mMesh == nullptr) { return; }


	int MeshCount = (int)mMesh->TopMeshList.size();
	int BoneCount = (int)mMesh->TopBoneList.size();
	int SkinCount = (int)mMesh->TopSkinList.size();

	//�⺻ �޽� ����
	for (int i = 0; i < MeshCount; i++)
	{
		if (MeshCount > 1)
		{
			//������Ʈ ����
			GameObject* Object = Instance();
			MeshFilter* mMeshFilter = Object->AddComponent<MeshFilter>();

			//��ũ ����
			gameobject->PushChildMeshObject(Object);
			LinkHierarchy(Object->GetTransform(), Tr);

			//�ڽİ�ü�� �̵�
			CreateStaticMesh(mMesh->TopMeshList[i], Object);
		}
		else
		{
			GameObject* Object = this->gameobject;
			Transform* mTransform = Object->GetTransform();

			CreateStaticMesh(mMesh->TopMeshList[i], Object);
		}
	}

	//�� �޽� ����
	if (mMesh->BoneOffsetList != nullptr)
	{
		BoneList.resize(mMesh->BoneOffsetList->size());
	}
	for (int i = 0; i < BoneCount; i++)
	{
		GameObject* Object = Instance();
		MeshFilter* mMeshFilter = Object->AddComponent<MeshFilter>();
		Animator* mAnimator = Object->AddComponent<Animator>();

		LinkHierarchy(Object->GetTransform(), Tr);
		gameobject->PushChildBoneObject(Object);

		int index = mMesh->TopBoneList[i]->BoneIndex;
		BoneList[index] = Object;

		CreateBoneMesh(mMesh->TopBoneList[i], Object);
	}

	//��Ų �޽� ����
	for (int i = 0; i < SkinCount; i++)
	{
		//������Ʈ ���� ���۳�Ʈ ����
		GameObject* Object = Instance("Skin");
		SkinningFilter* SKFilter = Object->AddComponent<SkinningFilter>();

		SKFilter->PushBoneList(&BoneList);
		SKFilter->PushBone_OffsetList(mMesh->BoneOffsetList);

		gameobject->PushChildMeshObject(Object);

		CreateSkinMesh(mMesh->TopSkinList[i], Object);
	}
}
