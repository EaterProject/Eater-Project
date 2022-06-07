#include "MeshFilter.h"
#include "SkinningFilter.h"
#include "AnimationController.h"
#include "GameObject.h"
#include "LoadManager.h"
#include "Transform.h"
#include "EngineData.h"
#include "Animator.h"
#include "ObjectManager.h"
#include "Material.h"
#include "Mesh.h"
#include "Terrain.h"
#include "EaterEngineAPI.h"
#include "TextureManager.h"
#include "GraphicEngineAPI.h"
#include "MaterialManager.h"
#include "Profiler/Profiler.h"

MeshFilter::MeshFilter()
{
	ModelName = "";

	m_PropertyBlock = new MaterialPropertyBlock();

	//Component Terrain���� ���� ����Ǿ����
	SetUp_Order = FUNCTION_ORDER_FIRST;
	Start_Order = FUNCTION_ORDER_FIRST;
}

MeshFilter::~MeshFilter()
{
	delete m_PropertyBlock;

	if (m_Material && m_Material->is_Default)
	{
		MaterialManager::DestroyDefaultMaterial(m_Material);
	}
}

void MeshFilter::Start()
{
	// GameObject Material Property Block ����..
	gameobject->OneMeshData->Object_Data->Material_Block = m_PropertyBlock;
	
	//Ŭ���̾�Ʈ�ʿ��� �ؽ����� �̸��� �ְ� �ִϸ��̼��� �ְ� ��� ��������
	if (isLoad_Model == true)
	{
		CreateModel();
	}

	if (isLoad_Mesh == true)
	{
		CheckMesh();
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
	isLoad_Model = true;
	isLoad_Mesh = true;
	isLoad_Material = true;
	isLoad_Texture = true;
	isLoad_Animation = true;
}

void MeshFilter::SetModelName(std::string mModelName)
{
	if (isLoad_Model == false)
	{
		isLoad_Model = true;
		ModelName = mModelName;
	}
	else
	{
		ModelName = mModelName;
		//�Ž��� ���Ӱ� �־��ֱ����� ���� ������ �ڽĿ�����Ʈ���� ����
		DeleteChildObject();
		CreateModel();
	}
}

void MeshFilter::SetMeshName(std::string mMeshName)
{
	if (isLoad_Mesh == false)
	{
		isLoad_Mesh = true;
		BufferName = mMeshName;
	}
	else
	{
		BufferName = mMeshName;
		SetBuffer(mMeshName);
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
	if (isLoad_Animation == false)
	{
		isLoad_Animation = true;
		AnimationName = mAnimeName;
	}
	else
	{
		AnimationName = mAnimeName;
		CheckAnimation();
	}
}

void MeshFilter::SetEmissiveColor(DirectX::SimpleMath::Vector3 color)
{
	m_Material->SetEmissiveColor(color);
}

void MeshFilter::SetEmissiveFactor(float emissiveFactor)
{
	m_Material->SetEmissiveFactor(emissiveFactor);
}

void MeshFilter::SetRoughnessFactor(float roughnessFactor)
{
	m_Material->SetRoughnessFactor(roughnessFactor);
}

void MeshFilter::SetMetallicFactor(float metallicFactor)
{
	m_Material->SetMetallicFactor(metallicFactor);
}

void MeshFilter::SetSkyLightIndex(int skyLightIndex)
{
	m_Material->SetSkyLightIndex(skyLightIndex);
}

void MeshFilter::SetMaterialPropertyBlock(bool enable)
{
	// Ȱ��ȭ�� ���ÿ� ���� Property Data ����..
	if (enable)
	{
		(*(MaterialProperty*)m_PropertyBlock) = (*m_Material->m_MaterialData->Material_Property);
	}

	gameobject->OneMeshData->Object_Data->IsMaterialBlock = enable;
}

std::string MeshFilter::GetBufferName()
{
	return BufferName;
}

std::string MeshFilter::GetModelName()
{
	return ModelName;
}

std::string MeshFilter::GetMaterialName()
{
	if (m_Material != nullptr)
	{
		return m_Material->Name;
	}
	else
	{
		return "";
	}
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
	return m_Material;
}

MaterialProperty* MeshFilter::GetMaterialProperty()
{
	return m_Material->m_MaterialData->Material_Property;
}

MaterialPropertyBlock* MeshFilter::GetMaterialPropertyBlock()
{
	return m_PropertyBlock;
}

void MeshFilter::CheckMesh()
{
	if (BufferName.empty() == false)
	{
		// Mesh Setting..
		SetBuffer(BufferName);
	}
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
	if (AnimationName.empty() == false)
	{
		Animation* animation = LoadManager::GetAnimation(AnimationName);
		AnimationController* Controller = gameobject->GetComponent<AnimationController>();

		//������ ���۳�Ʈ�� �� ������ �Ѱ��ش�
		if (Controller != nullptr)
		{
			Controller->SetBoneList(&BoneList);
			Controller->SetAnimation(animation);
		}
	}
}

void MeshFilter::SetBuffer(std::string meshName)
{
	Mesh* mesh = LoadManager::GetMesh(BufferName);

	// �ش� Mesh�� ���ٸ�..
	if (mesh == nullptr) return;

	// ����� Mesh �׷��� ����ȭ..
	GraphicEngine::Get()->PushChangeInstance(gameobject->OneMeshData);

	// �ش� Mesh ����..
	m_Mesh = mesh;
}

void MeshFilter::SetMaterial(std::string matName)
{
	Material* material = LoadManager::GetMaterial(matName);

	// �ش� Material�� ���ٸ�..
	if (material == nullptr)
	{
		if (m_Material == nullptr)
		{
			// ���ο� Material ����..
			m_Material = MaterialManager::CreateDefaultMaterial();

			// �⺻ ���� ����..
			//m_Material->SetAddColor(Vector3(1.0f, 1.0f, 1.0f));

			// ����� Material �׷��� ����ȭ..
			GraphicEngine::Get()->PushChangeInstance(gameobject->OneMeshData);
		}

		PROFILE_LOG(PROFILE_OUTPUT::CONSOLE, "[ Engine ][ MeshFilter ][ SetMaterial ] '%s' FAILED!!", matName.c_str());
		return;
	}

	// ����� Material �׷��� ����ȭ..
	GraphicEngine::Get()->PushChangeInstance(gameobject->OneMeshData);

	// �⺻ Material�̿��ٸ� ����..
	if (material->is_Default == false && m_Material && m_Material->is_Default)
	{
		MaterialManager::DestroyDefaultMaterial(m_Material);
	}

	// �ش� Material ����..
	m_Material = material;
	gameobject->OneMeshData->Material_Buffer = m_Material->m_MaterialData;
}

void MeshFilter::SetTexture(std::string texName, UINT texType)
{
	if (m_Material == nullptr) return;

	// Texture ����..
	switch (texType)
	{
	case DIFFUSE_TEXTURE:
		m_Material->SetDiffuseTexture(texName);
		break;
	case NORMAL_TEXTURE:
		m_Material->SetNormalTexture(texName);
		break;
	case EMISSIVE_TEXTURE:
		m_Material->SetEmissiveTexture(texName);
		break;
	case ORM_TEXTURE:
		m_Material->SetORMTexture(texName);
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
	SetMaterialData(mMesh, Data, Object);
	SetMeshData(mMesh, Data, Object);
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
	Object->GetComponent<MeshFilter>()->ModelName = mMesh->ModelName;

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

	Object->Name = mMesh->Name;
	SetMatrixData(mMesh, Data, Object);
	SetMaterialData(mMesh, Data, Object);
	SetMeshData(mMesh, Data, Object);

	Data->Object_Data->ObjType = OBJECT_TYPE::SKINNING;

	int ChildCount = (int)mMesh->Child.size();
	for (int i = 0; i < ChildCount; i++)
	{
		GameObject* ChildObject = Instance();
		MeshFilter* mMeshFilter = ChildObject->AddComponent<MeshFilter>();

		LinkHierarchy(ChildObject->GetTransform(), Object->GetTransform());

		CreateSkinMesh(mMesh->Child[i], ChildObject);
	}
}

void MeshFilter::SetMaterialData(LoadMeshData* LoadMesh, MeshData* mMesh, GameObject* obj)
{
	/// Load�� ������ �x�� Material Name �������� �ش� Material Buffer ����..
	Material* material = LoadManager::GetMaterial(LoadMesh->MaterialName);
	MeshFilter* meshFilter = obj->GetComponent<MeshFilter>();

	// �ε� ���ο� ���� Material ����..
	if (material)
	{
		// �ε��� Material ����..
		meshFilter->m_Material = material;
	}
	else
	{
		// ���ο� Material ����..
		meshFilter->m_Material = MaterialManager::CreateDefaultMaterial();
	}

	// Render Material Data ����..
	mMesh->Material_Buffer = meshFilter->m_Material->m_MaterialData;
}

void MeshFilter::SetMatrixData(LoadMeshData* LoadMesh, MeshData* mMesh, GameObject* Object)
{
	Transform* mTransform = Object->GetTransform();

	mTransform->Load_Local = LoadMesh->LocalTM;
	mTransform->Load_World = LoadMesh->WorldTM;
}

void MeshFilter::SetMeshData(LoadMeshData* LoadMesh, MeshData* mMesh, GameObject* obj)
{
	/// Load�� ������ �x�� Mesh Name �������� �ش� Mesh Buffer ����..
	Mesh* mesh = LoadManager::GetMesh(LoadMesh->BufferName);
	MeshFilter* meshFilter = obj->GetComponent<MeshFilter>();

	// �ε� ���ο� ���� Mesh ����..
	if (mesh)
	{
		// �ε��� Mesh ����..
		meshFilter->m_Mesh = mesh;
		meshFilter->BufferName = LoadMesh->BufferName;
		meshFilter->ModelName = LoadMesh->ModelName;
		// Render Mesh Data ����..
		mMesh->Mesh_Buffer = mesh->m_MeshData;
	}
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

void MeshFilter::CreateModel()
{
	///�̸����� �ε��� �����͸� ã�Ƽ� ������
	ModelData* mMesh = LoadManager::GetModelData(ModelName);
	Transform* Tr = gameobject->GetTransform();
	AnimationController* Controller = gameobject->GetComponent<AnimationController>();

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
			Object->AddComponent<MeshFilter>();

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
	if (!mMesh->BoneOffsetList.empty())
	{
		BoneList.resize(mMesh->BoneOffsetList.size());
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
		Object->AddComponent<MeshFilter>();

		SKFilter->PushBoneList(&BoneList);
		SKFilter->PushBone_OffsetList(mMesh->BoneOffsetList);

		if (Controller != nullptr)
		{
			Controller->SetSkin(Object);
		}

		LinkHierarchy(Object->GetTransform(), this->gameobject->GetTransform());

		gameobject->PushChildMeshObject(Object);
		CreateSkinMesh(mMesh->TopSkinList[i], Object);
	}
}
