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

	//Component Terrain보다 먼저 실행되어야함
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
	// GameObject Material Property Block 연결..
	gameobject->OneMeshData->Object_Data->Material_Block = m_PropertyBlock;
	
	//클라이언트쪽에서 텍스쳐의 이름을 넣고 애니메이션을 넣고 모두 끝난상태
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

	//실행도중 텍스쳐나 메쉬를 바꿀수있게 모두 true
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
		//매쉬를 새롭게 넣어주기전에 기존 생성한 자식오브젝트들을 삭제
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
	// 활성화와 동시에 기존 Property Data 복사..
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
	// 설정한 Texture가 있을경우 로드한 Texture 대신 해당 Texture 설정..
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

		//가져온 컨퍼넌트에 본 정보를 넘겨준다
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

	// 해당 Mesh가 없다면..
	if (mesh == nullptr) return;

	// 변경된 Mesh 그래픽 동기화..
	GraphicEngine::Get()->PushChangeInstance(gameobject->OneMeshData);

	// 해당 Mesh 설정..
	m_Mesh = mesh;
}

void MeshFilter::SetMaterial(std::string matName)
{
	Material* material = LoadManager::GetMaterial(matName);

	// 해당 Material이 없다면..
	if (material == nullptr)
	{
		if (m_Material == nullptr)
		{
			// 새로운 Material 생성..
			m_Material = MaterialManager::CreateDefaultMaterial();

			// 기본 색상 설정..
			//m_Material->SetAddColor(Vector3(1.0f, 1.0f, 1.0f));

			// 변경된 Material 그래픽 동기화..
			GraphicEngine::Get()->PushChangeInstance(gameobject->OneMeshData);
		}

		PROFILE_LOG(PROFILE_OUTPUT::CONSOLE, "[ Engine ][ MeshFilter ][ SetMaterial ] '%s' FAILED!!", matName.c_str());
		return;
	}

	// 변경된 Material 그래픽 동기화..
	GraphicEngine::Get()->PushChangeInstance(gameobject->OneMeshData);

	// 기본 Material이였다면 삭제..
	if (material->is_Default == false && m_Material && m_Material->is_Default)
	{
		MaterialManager::DestroyDefaultMaterial(m_Material);
	}

	// 해당 Material 설정..
	m_Material = material;
	gameobject->OneMeshData->Material_Buffer = m_Material->m_MaterialData;
}

void MeshFilter::SetTexture(std::string texName, UINT texType)
{
	if (m_Material == nullptr) return;

	// Texture 설정..
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
		//새로운 오브젝트 생성 (Transform은 자동으로 들어감)
		GameObject* ChildObject = Instance();
		MeshFilter* mMeshFilter = ChildObject->AddComponent<MeshFilter>();
		Animator* mAnimator = ChildObject->AddComponent<Animator>();

		//오브젝트에 연결 , Transform 에 연결
		Object->PushChildBoneObject(ChildObject);
		LinkHierarchy(ChildObject->GetTransform(), Object->GetTransform());

		//다음 자식으로 이동
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
	/// Load시 저장해 둿던 Material Name 기준으로 해당 Material Buffer 설정..
	Material* material = LoadManager::GetMaterial(LoadMesh->MaterialName);
	MeshFilter* meshFilter = obj->GetComponent<MeshFilter>();

	// 로드 여부에 따른 Material 설정..
	if (material)
	{
		// 로드한 Material 설정..
		meshFilter->m_Material = material;
	}
	else
	{
		// 새로운 Material 생성..
		meshFilter->m_Material = MaterialManager::CreateDefaultMaterial();
	}

	// Render Material Data 설정..
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
	/// Load시 저장해 둿던 Mesh Name 기준으로 해당 Mesh Buffer 설정..
	Mesh* mesh = LoadManager::GetMesh(LoadMesh->BufferName);
	MeshFilter* meshFilter = obj->GetComponent<MeshFilter>();

	// 로드 여부에 따른 Mesh 설정..
	if (mesh)
	{
		// 로드한 Mesh 설정..
		meshFilter->m_Mesh = mesh;
		meshFilter->BufferName = LoadMesh->BufferName;
		meshFilter->ModelName = LoadMesh->ModelName;
		// Render Mesh Data 설정..
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
	///이름으로 로드할 데이터를 찾아서 가져옴
	ModelData* mMesh = LoadManager::GetModelData(ModelName);
	Transform* Tr = gameobject->GetTransform();
	AnimationController* Controller = gameobject->GetComponent<AnimationController>();

	if (mMesh == nullptr) { return; }


	int MeshCount = (int)mMesh->TopMeshList.size();
	int BoneCount = (int)mMesh->TopBoneList.size();
	int SkinCount = (int)mMesh->TopSkinList.size();

	//기본 메쉬 생성
	for (int i = 0; i < MeshCount; i++)
	{
		if (MeshCount > 1)
		{
			//오브젝트 생성
			GameObject* Object = Instance();
			Object->AddComponent<MeshFilter>();

			//링크 연결
			gameobject->PushChildMeshObject(Object);
			LinkHierarchy(Object->GetTransform(), Tr);

			//자식객체로 이동
			CreateStaticMesh(mMesh->TopMeshList[i], Object);
		}
		else
		{
			GameObject* Object = this->gameobject;
			Transform* mTransform = Object->GetTransform();

			CreateStaticMesh(mMesh->TopMeshList[i], Object);
		}
	}

	//본 메쉬 생성
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

	//스킨 메쉬 생성
	for (int i = 0; i < SkinCount; i++)
	{
		//오브젝트 생성 컨퍼넌트 생성
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
