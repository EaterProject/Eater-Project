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

	if (isLoad_Texture == true) 
	{
		CheckTexture();
	}

	if (isLoad_Animation == true)
	{
		CheckAnimation();
	}
	//���൵�� �ؽ��ĳ� �޽��� �ٲܼ��ְ� ��� true
	isLoad_Texture		= true;
	isLoad_Mesh			= true;
	isLoad_Animation	= true;
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

void MeshFilter::SetDiffuseTextureName(std::string mTextureName)
{
	if (isLoad_Texture == false)
	{
		isLoad_Texture = true;
		DiffuseTextureName = mTextureName;
	}
	else
	{
		SetTexture(mTextureName, DIFFUSE_TEXTURE);
	}
}

void MeshFilter::SetNormalTextureName(std::string mTextureName)
{
	if (isLoad_Texture == false)
	{
		isLoad_Texture = true;
		NormlaTextureName = mTextureName;
	}
	else
	{
		SetTexture(mTextureName, NORMAL_TEXTURE);
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
		SetTexture(mTextureName, ORM_TEXTURE);
	}
}

void MeshFilter::SetAnimationName(std::string mAnimeName)
{
	isLoad_Animation = true;
	AnimationName = mAnimeName;
	CheckAnimation();
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
	 return NormlaTextureName;
}

 std::string MeshFilter::GetORMTextureName()
{
	 return ORMTextureName;
}

void MeshFilter::SetObjectData()
{
	// �⺻���� MeshFilter ������ Material�� ����..
	Materials = new Material();
	//
	// ������Ʈ ���� �� �߰� �۾�
	Materials->SetMeshData(gameobject->OneMeshData);
}

void MeshFilter::CheckTexture()
{
	// ������ Texture�� ������� �ε��� Texture ��� �ش� Texture ����..
	if (DiffuseTextureName.empty() == false)
	{
		// Diffuse Texture Setting..
		SetTexture(DiffuseTextureName, DIFFUSE_TEXTURE);
	}
	if (NormlaTextureName.empty() == false)
	{
		// Normal Texture Setting..
		SetTexture(NormlaTextureName, NORMAL_TEXTURE);
	}
	if (ORMTextureName.empty() == false)
	{
		// ORM Texture Setting..
		SetTexture(ORMTextureName, ORM_TEXTURE);
	}
	isLoad_Texture = true;
}

void MeshFilter::CheckAnimation()
{
	if (isLoad_Animation == false) { return; }
	
	ModelAnimationData* data		= LoadManager::GetAnimation(AnimationName);
	AnimationController* Controller = gameobject->GetComponent<AnimationController>();
	
	//������ ���۳�Ʈ�� �� ������ �Ѱ��ش�
	if (Controller != nullptr)
	{	
		Controller->SetBoneList(&BoneList);
		Controller->SetAnimeList(data);
	}
}

void MeshFilter::SetTexture(std::string texName, UINT texType)
{
	// ���� Mesh Data..
	MeshData* data = gameobject->OneMeshData;

	// ���� Mesh�� Material..
	MaterialData* material = data->Material_Data;

	// ���� Texture Buffer..
	TextureBuffer* texBuffer = LoadManager::GetTexture(texName);

	// �ش� Texture�� Load���� ���� ��� ���� Texture ���..
	if (texBuffer == nullptr && texType == DIFFUSE_TEXTURE)
	{
		texBuffer = LoadManager::GetTexture("Dump");
		DiffuseTextureName = "Dump";
	}

	// Texture ����..
	switch (texType)
	{
	case DIFFUSE_TEXTURE:
		material->Albedo = texBuffer;
		DiffuseTextureName = texName;
		break;
	case NORMAL_TEXTURE:
		material->Normal = texBuffer;
		NormlaTextureName = texName;
		break;
	case ORM_TEXTURE:
		material->ORM = texBuffer;
		ORMTextureName = texName;
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

	if (Object->Name == "")
	{
		Object->Name = mMesh->Name;
	}

	SetMatrixData(mMesh, Data, Object);
	SetMaterialData(mMesh, Data);
	SetBufferData(mMesh, Data);
	SetType(mMesh, Data);

	// Mesh Index ����..
	Data->MeshIndex = mMesh->MeshIndex;

	int ChildCount = (int)mMesh->Child.size();
	for (int i = 0; i < ChildCount; i++)
	{
		GameObject* ChildObject		= Instance();
		MeshFilter* mMeshFilter		= ChildObject->AddComponent<MeshFilter>();

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
	Data->ObjType	= OBJECT_TYPE::BONE;
	Object->Name = mMesh->Name;
	

	int ChildCount = (int)mMesh->Child.size();
	for (int i = 0; i < ChildCount; i++)
	{
		//���ο� ������Ʈ ���� (Transform�� �ڵ����� ��)
		GameObject* ChildObject = Instance();
		MeshFilter* mMeshFilter = ChildObject->AddComponent<MeshFilter>();
		Animator*	mAnimator	= ChildObject->AddComponent<Animator>();

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

	Data->Material_Data = new MaterialData();
	SetMatrixData(mMesh, Data, Object);
	SetMaterialData(mMesh, Data);
	SetBufferData(mMesh, Data);

	Data->ObjType = OBJECT_TYPE::SKINNING;

	// Mesh Index ����..
	Data->MeshIndex = mMesh->MeshIndex;

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
	mMesh->Alpha = LoadMesh->Alpha;

	mMesh->Material_Data->Albedo	= LoadManager::GetTexture(LoadMesh->AlbedoName);
	mMesh->Material_Data->Normal	= LoadManager::GetTexture(LoadMesh->NormalName);
	mMesh->Material_Data->Emissive	= LoadManager::GetTexture(LoadMesh->EmissiveName);
	mMesh->Material_Data->ORM		= LoadManager::GetTexture(LoadMesh->ORMName);

	if (mMesh->Material_Data->Albedo == nullptr)
	{
		mMesh->Material_Data->Albedo = LoadManager::GetTexture("Dump");
	}
}

void MeshFilter::SetMatrixData(LoadMeshData* LoadMesh, MeshData* mMesh,GameObject* Object)
{
	Transform* mTransform = Object->GetTransform();

	mTransform->Load_Local = LoadMesh->LocalTM;
	mTransform->Load_World = LoadMesh->WorldTM;

	mMesh->mWorld = &mTransform->Load_Local;
	mMesh->mLocal = &mTransform->Load_World;
}

void MeshFilter::SetBufferData(LoadMeshData* LoadMesh, MeshData* mMesh)
{
	mMesh->MeshIndex	= LoadMesh->MeshIndex;
	mMesh->IndexBuf		= LoadMesh->IndexBuf;
	mMesh->VertexBuf	= LoadMesh->VertexBuf;
}

void MeshFilter::SetType(LoadMeshData* LoadMesh, MeshData* mMesh)
{
	switch (LoadMesh->MeshType)
	{
	case MESH_TYPE::STATIC_MESH:
		mMesh->ObjType = OBJECT_TYPE::BASE;
		break;
	case MESH_TYPE::TERRAIN_MESH:
		mMesh->ObjType = OBJECT_TYPE::TERRAIN;
		break;
	case MESH_TYPE::QUAD_MESH:
		mMesh->ObjType = OBJECT_TYPE::TEXTURE;
		break;
	case MESH_TYPE::SKIN_MESH:
		mMesh->ObjType = OBJECT_TYPE::SKINNING;
		break;
	case MESH_TYPE::BONE_MESH:
		mMesh->ObjType = OBJECT_TYPE::BONE;
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
			GameObject* Object		= Instance();
			MeshFilter*	mMeshFilter = Object->AddComponent<MeshFilter>();
			
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
		Animator*	mAnimator	= Object->AddComponent<Animator>();

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
