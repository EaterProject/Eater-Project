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
	// Terrain 전용 데이터 생성..
	m_TerrainData = new TerrainData();

	//이컨퍼넌트의 Start함수는 제일나중에 실행
	Component::Start_Order = Component::FUNCTION_ORDER_LAST;
}

Terrain::~Terrain()
{

}

void Terrain::Awake()
{
	//Awake 에서는 무조건 GetComponent 만
	mCollider	= gameobject->GetComponent<Collider>();
	mMeshFilter = gameobject->GetComponent<MeshFilter>();
}

void Terrain::SetUp()
{
	//초기화 값을 넣어준다
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
	//초기화 넣어준값으로 컨퍼넌트를 변경
	int count = (int)DiffuseNameList.size();
	for (int i = 0; i < count; i++)
	{
		AddLayer(DiffuseNameList[i],NormalNameList[i], ORMNameList[i]);
	}
}


void Terrain::AddLayer(std::string diffuseName, std::string normalName, std::string ormName)
{
	// 새로운 Material 생성..
	Material* newMaterial = new Material();

	// 새로운 Material Data 생성..
	MaterialData* materialData = newMaterial->m_MaterialData;

	// Material Layer Texture 삽입..
	materialData->Albedo = LoadManager::GetTexture(diffuseName);
	materialData->Normal = LoadManager::GetTexture(normalName);
	materialData->ORM	 = LoadManager::GetTexture(ormName);

	// 새로운 Material Layer 삽입..
	gameobject->OneMeshData->Terrain_Data = m_TerrainData;

	m_TerrainData->Material_List.push_back(materialData);
	m_MaterialLayer.push_back(newMaterial);
}

void Terrain::SetTextureTiling(float scale_x, float scale_y)
{
	gameobject->OneMeshData->Material_Data->Material_SubData->TexTM = DirectX::SimpleMath::Matrix::CreateScale(scale_x, scale_y, 1.0f);
}

void Terrain::SetTextureTiling(float scale)
{
	gameobject->OneMeshData->Material_Data->Material_SubData->TexTM = DirectX::SimpleMath::Matrix::CreateScale(scale, scale, 1.0f);
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
	// Terrain Material 해제..
	for (Material* mat : m_MaterialLayer)
	{
		mat->Release();

		delete mat;
	}
	m_MaterialLayer.clear();

	// Terrain Data 해제..
	m_TerrainData->Material_List.clear();

	delete m_TerrainData;
}