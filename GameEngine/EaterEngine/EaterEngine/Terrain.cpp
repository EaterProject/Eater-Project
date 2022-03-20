#include "Material.h"
#include "Terrain.h"
#include "LoadManager.h"
#include "EngineData.h"
#include "GameObject.h"
#include "Transform.h"
#include "MeshFilter.h"
#include "Collider.h"

Terrain::Terrain()
{
	// Terrain ���� ������ ����..
	m_TerrainData = new TerrainData();

	//�����۳�Ʈ�� Start�Լ��� ���ϳ��߿� ����
	Component::Start_Order = Component::FUNCTION_ORDER_LAST;
}

Terrain::~Terrain()
{

}

void Terrain::Awake()
{
	//Awake ������ ������ GetComponent ��
	mCollider	= gameobject->GetComponent<Collider>();
	mMeshFilter = gameobject->GetComponent<MeshFilter>();

	// ���ο� Material Layer ����..
	gameobject->OneMeshData->Terrain_Data = m_TerrainData;
}

void Terrain::SetUp()
{
	//�ʱ�ȭ ���� �־��ش�
	if (TerrainColliderName != "" && mCollider != nullptr)
	{
		mCollider->SetMeshCollider(TerrainColliderName);
	}

	if (TerrainMeshName != "" && mMeshFilter != nullptr)
	{
		mMeshFilter->SetMeshName(TerrainMeshName);
	}
}

void Terrain::Start()
{
	//�ʱ�ȭ �־��ذ����� ���۳�Ʈ�� ����
	int count = (int)DiffuseNameList.size();
	for (int i = 0; i < count; i++)
	{
		AddLayer(DiffuseNameList[i],NormalNameList[i], ORMNameList[i]);
	}

	if (isLoad_Data == true)
	{
		DataUpdate();
	}

	isLoad_Data = true;
}


void Terrain::AddLayer(std::string diffuseName, std::string normalName, std::string ormName)
{
	// ���ο� Material ����..
	Material* newMaterial = new Material();

	// ���ο� Material Data ����..
	MaterialBuffer* materialData = newMaterial->m_MaterialData;

	// Material Layer Texture ����..
	materialData->Albedo = LoadManager::GetTexture(diffuseName);
	materialData->Normal = LoadManager::GetTexture(normalName);
	materialData->ORM	 = LoadManager::GetTexture(ormName);

	m_TerrainData->Material_List.push_back(materialData);
	m_MaterialLayer.push_back(newMaterial);
}

void Terrain::DataUpdate()
{
	gameobject->OneMeshData->Terrain_Data->Tex = DirectX::SimpleMath::Matrix::CreateScale(1.0f / Tiling.x, 1.0f / Tiling.y, 1.0f);
}

void Terrain::SetTextureTiling(float scale_x, float scale_y)
{
	if (isLoad_Data == false)
	{
		isLoad_Data = true;

		Tiling.x = scale_x;
		Tiling.y = scale_y;
	}
	else
	{
		Tiling.x = scale_x;
		Tiling.y = scale_y;

		// ������ ������ ������Ʈ..
		DataUpdate();
	}
}

void Terrain::SetTextureTiling(float scale)
{
	if (isLoad_Data == false)
	{
		isLoad_Data = true;

		Tiling.x = scale;
		Tiling.y = scale;
	}
	else
	{
		Tiling.x = scale;
		Tiling.y = scale;

		DataUpdate();
	}
}

void Terrain::SetLayerName(std::string diffuseName, std::string normalName, std::string ormName)
{
	DiffuseNameList.push_back(diffuseName);
	NormalNameList.push_back(normalName);
	ORMNameList.push_back(ormName);
}

void Terrain::SetColliderName(std::string _TerrainColliderName)
{
	TerrainColliderName = _TerrainColliderName;
}

void Terrain::SetMeshName(std::string _TerrainMeshName)
{
	TerrainMeshName = _TerrainMeshName;
}

void Terrain::Release()
{
	// Terrain Material ����..
	for (Material* mat : m_MaterialLayer)
	{
		mat->Release();

		delete mat;
	}
	m_MaterialLayer.clear();

	// Terrain Data ����..
	m_TerrainData->Material_List.clear();

	delete m_TerrainData;
}