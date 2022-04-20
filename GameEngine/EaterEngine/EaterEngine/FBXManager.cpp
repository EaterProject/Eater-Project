#include "FBXManager.h"
#include "YAMLHeader.h"
#include "EaterHeader.h"
#include "ModelParser.h"
#include "LoadManager.h"
#include "EngineData.h"
#include "TextureManager.h"
#include "GraphicEngineManager.h"
#include "Material.h"
#include "Mesh.h"
#include "Animation.h"

#include <filesystem>
#include <iostream>

#define LERP(prev, next, time) ((prev * (1.0f - time)) + (next * time))
FBXManager::FBXManager()
{
	FBXLoad = nullptr;

	isFolder	= false;
	isSkinning	= false;
	isAnimation = false;

	m_CriticalSection	= nullptr;
	m_Graphic			= nullptr;
}

FBXManager::~FBXManager()
{
	m_CriticalSection	= nullptr;
	m_Graphic			= nullptr;
}

void FBXManager::Initialize(GraphicEngineManager* Graphic, CRITICAL_SECTION* _cs)
{
	//FBX �ļ� �ʱ�ȭ
	FBXLoad = ModelParser::Create(MODEL_TYPE::FBX_MODEL);
	FBXLoad->Initialize();
	m_Graphic			= Graphic;
	m_CriticalSection	= _cs;

	LoadQuad();
}

void FBXManager::LoadTerrain(std::string Name, std::string MaskName1, std::string MaskName2, UINT parsingMode)
{
	ParserData::CModel* mMesh = FBXLoad->LoadModel(Name, parsingMode);
	int size =  (int)mMesh->m_MaterialList.size();
	ParserData::CMesh* TerrainMesh = mMesh->m_MeshList[0];

	// Terrain Mask Name ����..
	TerrainMesh->m_MeshType = TERRAIN_MESH;
	TerrainMesh->m_MaskName1 = MaskName1;
	TerrainMesh->m_MaskName2 = MaskName2;

	LoadMeshData* Data = new LoadMeshData();
	ModelData* model = new ModelData();
	model->Name = Name;
	Data->MeshType = TERRAIN_MESH;

	SetMatrixData(TerrainMesh, Data);
	SetNameData(TerrainMesh, Data);
	SetMeshData(TerrainMesh, Data);

	std::size_t Start	= Name.rfind('/')+1;
	std::size_t End		= Name.rfind('.') - Start;
	Name = Name.substr(Start, End);

	model->TopMeshList.push_back(Data);
	LoadManager::ModelDataList.insert({ Name, model });
}

void FBXManager::CheckSkinning(std::string& Path)
{
	//+ ���ڿ��� ã�� ���ߴٸ� ��Ű�� �Ž��� �ƴ϶�¶�!!
	bool MeshLoad = false;
	std::string::size_type End = Path.rfind("+");
	int NameSize = (int)Path.size();

	if (NameSize < End)
	{
		isSkinning = false;
	}
	else
	{
		isSkinning = true;
	}
}

void FBXManager::CheckAnimation(std::string& Path)
{
	if (isSkinning == false) { return; }
	std::size_t Start	= Path.rfind('/')+1;
	std::size_t End		= Path.rfind('+') - Start;
	std::string MeshName = Path.substr(Start,End);
	isAnimation = LoadManager::FindModel(MeshName);
}

void FBXManager::CreateKeyFrame(ParserData::CModelAnimation* Anime, int InputKeyCount)
{
	//���� �ִϸ��̼�
	std::vector<ParserData::CAnimation*>::iterator it = Anime->m_AnimationList.begin();

	Anime->m_TicksPerFrame /= (InputKeyCount + 3);
	Anime->m_EndFrame = (Anime->m_TotalFrame * (InputKeyCount + 3)) - (InputKeyCount + 3);

	for (it; it != Anime->m_AnimationList.end(); it++)
	{
		std::vector<ParserData::CFrame*> data = (*it)->m_AniData;
		//���Ӱ� ���� ������ ����Ʈ
		std::vector<ParserData::CFrame*> CreateData;

		int Size = (int)data.size();
		for (int i = 0; i < Size - 1; i++)
		{
			//������ ó����
			DirectX::SimpleMath::Vector3 Start_Pos = data[i]->m_LocalPos;
			DirectX::SimpleMath::Quaternion Start_Rot = data[i]->m_LocalRotQt;
			DirectX::SimpleMath::Vector3 Start_Scl = data[i]->m_LocalScale;
			float Start_Time = data[i]->m_Time;

			//������ ������
			int NextIndex = i + 1;
			DirectX::SimpleMath::Vector3 End_Pos = data[NextIndex]->m_LocalPos;
			DirectX::SimpleMath::Quaternion End_Rot = data[NextIndex]->m_LocalRotQt;
			DirectX::SimpleMath::Vector3 End_Scl = data[NextIndex]->m_LocalScale;
			float End_Time = data[NextIndex]->m_Time;

			///ó���� �־��ֱ�
			CreateData.push_back(data[i]);

			int KeyCount = InputKeyCount;		//������ Ű������
			float CountLerp = (float)(1 / InputKeyCount);	//������ Ű������ ������ ��ġ��������
			for (int j = 0; j <= KeyCount; j++)
			{
				//���ο� Ű ������ ����
				ParserData::CFrame* temp = new CFrame();
				temp->m_LocalPos = Vector3::Lerp(Start_Pos, End_Pos, CountLerp);
				temp->m_LocalRotQt = Quaternion::Lerp(Start_Rot, End_Rot, CountLerp);
				temp->m_LocalScale = Vector3::Lerp(Start_Scl, End_Scl, CountLerp);
				temp->m_Time = LERP(Start_Time, End_Time, CountLerp);

				CreateData.push_back(temp);
				CountLerp += 0.1f;
			}

			///�������� �־��ֱ�
			CreateData.push_back(data[NextIndex]);
		}

		(*it)->m_AniData = CreateData;
		//(*it)->m_TicksPerFrame /= (InputKeyCount + 3);
		//(*it)->m_EndFrame = (Size * (InputKeyCount + 3)) - (InputKeyCount + 3);
	}
}

LoadMeshData* FBXManager::CreateBaseMesh(ParserData::CMesh* mMesh)
{
	/// �⺻ �޽� ó��
	if (mMesh == nullptr) { return nullptr; }
	LoadMeshData* SaveMesh = new LoadMeshData();

	SetNameData(mMesh, SaveMesh);		//�̸� ������ ����
	SetMaterialData(mMesh, SaveMesh);	//���׸��� ������ ����
	SetMatrixData(mMesh, SaveMesh);		//��Ʈ���� ������ ����
	SetMeshData(mMesh, SaveMesh);		//�Ž� ������ ����

	return SaveMesh;
}

LoadMeshData* FBXManager::CreateSkinMesh(ParserData::CMesh* mMesh)
{
	///��Ų �Ž� ó��
	if (mMesh == nullptr) { return nullptr; }
	LoadMeshData* SaveMesh = new LoadMeshData();

	SetNameData(mMesh, SaveMesh);		//�̸� ������ ����
	SetMaterialData(mMesh, SaveMesh);	//���׸��� ������ ����
	SetMatrixData(mMesh, SaveMesh);		//��Ʈ���� ������ ����
	SetMeshData(mMesh, SaveMesh);		//�Ž� ������ ����

	return SaveMesh;
}

LoadMeshData* FBXManager::CreateBoneMesh(ParserData::CMesh* mMesh)
{
	///�� �Ž� ó��
	if (mMesh == nullptr) { return nullptr; }
	LoadMeshData* SaveMesh = new LoadMeshData();
	SaveMesh->BoneIndex = mMesh->m_BoneIndex;
	SetNameData(mMesh, SaveMesh);
	SetMatrixData(mMesh, SaveMesh);
	return SaveMesh;
}

void FBXManager::SetMaterialData(ParserData::CMesh* mMesh, LoadMeshData* SaveData)
{
	// Parsing Data ��� Material ����..
	CMaterial* mat = mMesh->m_MaterialData;

	if (mat != nullptr)
	{
		// Material �̸� ����..
		std::string matName = nowFileName + "_" + mat->m_MaterialName;

		// �ش� Material ���� üũ..
		Material* newMaterial = LoadManager::GetMaterial(matName);
		
		if (newMaterial == nullptr)
		{
			// �ش� Material�� ���� ��� ����..
			newMaterial = new Material();

			// Alpha ���� ����..
			newMaterial->m_MaterialData->Material_SubData->Alpha = mat->m_Alpha;

			std::string mapName;
			if (mat->m_TextureBind & DIFFUSE_TEXTURE)
			{
				mapName = CutStr(mMesh->m_MaterialData->m_DiffuseMap->m_BitMap);
				newMaterial->m_MaterialData->Albedo = LoadManager::GetTexture(mapName);
			}

			if (mat->m_TextureBind & NORMAL_TEXTURE)
			{
				mapName = CutStr(mMesh->m_MaterialData->m_NormalMap->m_BitMap);
				newMaterial->m_MaterialData->Normal = LoadManager::GetTexture(mapName);
			}

			if (mat->m_TextureBind & EMISSIVE_TEXTURE)
			{
				mapName = CutStr(mMesh->m_MaterialData->m_EmissiveMap->m_BitMap);
				newMaterial->m_MaterialData->Emissive = LoadManager::GetTexture(mapName);
			}

			if (mat->m_TextureBind & ORM_TEXTURE)
			{
				mapName = CutStr(mMesh->m_MaterialData->m_ORMMap->m_BitMap);
				newMaterial->m_MaterialData->ORM = LoadManager::GetTexture(mapName);
			}

			newMaterial->m_MaterialData->Name = matName;

			// ���� Material ����..
			LoadManager::MaterialList.insert({ matName, newMaterial });
		}

		// ���� �ε带 ���� Material �̸� ����..
		SaveData->MaterialName = matName;
	}
}

void FBXManager::SetMatrixData(ParserData::CMesh* mMesh, LoadMeshData* SaveData)
{
	SaveData->WorldTM = mMesh->m_WorldTM;
	SaveData->LocalTM = mMesh->m_LocalTM;
}

void FBXManager::SetNameData(ParserData::CMesh* mMesh, LoadMeshData* SaveData)
{
	SaveData->Name			= mMesh->m_NodeName;
	SaveData->ParentName	= mMesh->m_ParentName;
	SaveData->MeshType		= mMesh->m_MeshType;
	SaveData->Top_Object	= mMesh->m_TopNode;
}

void FBXManager::SetMeshData(ParserData::CMesh* mMesh, LoadMeshData* SaveData)
{
	// Mesh �̸� ����..
	std::string meshName = nowFileName + "_" + std::to_string(mMesh->m_MeshIndex);

	// �ش� Index�� Mesh Buffer ���� üũ..
	Mesh* meshBuffer = LoadManager::GetMesh(meshName);

	if (meshBuffer == nullptr)
	{
		// Mesh ����..
		EnterCriticalSection(m_CriticalSection);
		meshBuffer = new Mesh();

		m_Graphic->CreateMeshBuffer(mMesh, &meshBuffer->m_MeshData);
		LeaveCriticalSection(m_CriticalSection);

		meshBuffer->Name = meshName;
		meshBuffer->m_MeshData->Name = meshName;

		// ���� Mesh ����..
		LoadManager::MeshBufferList.insert({meshName, meshBuffer });
	}

	// �ش� Mesh Buffer �̸� ����..
	// ���� Mesh Filter Start ���� Mesh Buffer�� �������� ������ �Ǵ� �̸�..
	SaveData->BufferName = meshBuffer->Name;
}

std::string FBXManager::CutStr(std::string Path)
{
	//png �ڸ�
	std::size_t Start	= 0;
	std::size_t End		= Path.rfind('.');
	return Path.substr(Start, End);
}

void FBXManager::LinkMesh(std::vector<LoadMeshData*>& List, ModelData* SaveMesh)
{
	int Count = (int)List.size();
	LoadMeshData* Front = nullptr;
	LoadMeshData* Back	= nullptr;

	for(int i = 0; i<Count;i++)
	{
		for (int j = 0; j < Count; j++)
		{
			Front	= List[i];
			Back	= List[j];

			if (Front == Back) { continue; }

			//���� �������� �޽���  ������ϴ� �޽��� �θ��̸��� ���ٸ� ����
			if (Front->Name == Back->ParentName)
			{
				Front->Child.push_back(Back);
				Back->Parent = Front;
			}
		}

		if (Front->Top_Object == true)
		{
			switch (Back->MeshType)
			{
			case MESH_TYPE::BONE_MESH:
				SaveMesh->TopBoneList.push_back(Front);
				break;
			case MESH_TYPE::SKIN_MESH:
				SaveMesh->TopSkinList.push_back(Front);
				SaveMesh->BoneList.push_back(Front);
				break;
			case MESH_TYPE::STATIC_MESH:
				SaveMesh->TopMeshList.push_back(Front);
				break;
			}
		}
	}
}

std::string FBXManager::GetSaveName(std::string& Path)
{
	if (Path.rfind('+') == std::string::npos)
	{
		//�׳� �Ž� 
		std::size_t Start	= Path.rfind('/')+1;
		std::size_t End		= Path.rfind('.') - Start;
		return Path.substr(Start, End);
	}
	else
	{
		//��Ű�� �Ž�
		std::size_t Start = Path.rfind('/')+1;
		std::size_t End = Path.rfind('+') - Start;
		return Path.substr(Start, End);
	}
}

void FBXManager::CreateSaveMesh(ParserData::CModel* mMesh, std::string& Path)
{
	std::vector<LoadMeshData*> BaseMeshList;
	std::vector<LoadMeshData*> SkinMeshList;
	std::vector<LoadMeshData*> BoneMeshList;

	// ���� ���� �̸� ����..
	nowFileName = GetSaveName(Path);

	// ���ο� �Ž� ����..
	ModelData* SaveMesh = new ModelData();

	// Mesh Name ����..
	SaveMesh->Name = nowFileName;

	//��� �Ž� ����
	int MeshSize = (int)mMesh->m_MeshList.size();
	for (int i = 0; i < MeshSize; i++)
	{
		ParserData::CMesh *OneMesh = mMesh->m_MeshList[i];
		LoadMeshData* SaveOneMesh = nullptr;

		//Ÿ�Ժ��� LoadMeshData�� ���� ä���ش�
		switch (OneMesh->m_MeshType)
		{
		case MESH_TYPE::BONE_MESH:
			SaveOneMesh = CreateBoneMesh(OneMesh);
			BoneMeshList.push_back(SaveOneMesh);
			SaveMesh->BoneList.push_back(SaveOneMesh);
			break;
		case MESH_TYPE::SKIN_MESH:
			SaveOneMesh = CreateSkinMesh(OneMesh);
			SkinMeshList.push_back(SaveOneMesh);
			SaveMesh->BoneOffsetList = std::move(OneMesh->m_BoneTMList);
			break;
		case MESH_TYPE::STATIC_MESH:
			SaveOneMesh = CreateBaseMesh(OneMesh);
			BaseMeshList.push_back(SaveOneMesh);
			break;
		}
	}

	int BaseCount = (int)BaseMeshList.size();
	int SkinCount = (int)SkinMeshList.size();
	int BoneCount = (int)BoneMeshList.size();

	//������� ������� �θ� �ڽİ���� ��ũ
	LinkMesh(BaseMeshList, SaveMesh);
	LinkMesh(SkinMeshList, SaveMesh);
	LinkMesh(BoneMeshList, SaveMesh);
	
	LoadManager::ModelDataList.insert({ nowFileName, SaveMesh });
}

void FBXManager::LoadQuad()
{
	ModelData* SaveMesh = new ModelData();
	LoadMeshData* quad = new LoadMeshData();
	Mesh* quadMesh = new Mesh();

	// Quad Mesh ����..
	SaveMesh->Name = "Quad";

	quadMesh->Name = "Quad";
	quadMesh->m_MeshData->Name = "Quad";

	quad->MeshType = QUAD_MESH;
	quad->BufferName = "Quad";

	ParserData::CMesh* mesh = new ParserData::CMesh();
	mesh->m_MeshType = QUAD_MESH;

	//���۰� �о����
	EnterCriticalSection(m_CriticalSection);
	m_Graphic->CreateMeshBuffer(mesh, &quadMesh->m_MeshData);
	LeaveCriticalSection(m_CriticalSection);

	SaveMesh->TopMeshList.push_back(quad);

	LoadManager::ModelDataList.insert({ "Quad" , SaveMesh });
	LoadManager::MeshBufferList.insert({ "Quad", quadMesh });

	delete mesh;
}

void FBXManager::Load(std::string& Path, UINT parsingMode)
{
	CheckSkinning(Path);	//��ο� �̸������� ��Ű�� ��ü���� �ƴ��� üũ
	CheckAnimation(Path);	//�ִϸ��̼��� �ε� �ؾ��ϴ��� üũ

	///�ִϸ��̼Ǹ� �ε��Ұ�����
	if (isAnimation == true)
	{
		parsingMode |= ANIMATION_ONLY;
	}
	
	LoadFile(Path, parsingMode);
}

void FBXManager::LoadFile(std::string& Path, UINT parsingMode)
{
	ParserData::CModel* mMesh = FBXLoad->LoadModel(Path, parsingMode);
	LoadAnimation(mMesh, Path);

	//�ִϸ��̼Ǹ� �д´ٸ� ���⼭ ����
	if (isAnimation == true) { return; }

	//�ε��� �Ž��� �����Ų��
	CreateSaveMesh(mMesh, Path);
}

void FBXManager::LoadAnimation(ParserData::CModel* MeshData, std::string& Path)
{
	//if (isAnimation == false) { return; }

	if (Path.rfind('+') == std::string::npos) { return; }

	//Ű������ ����
	//CreateKeyFrame(MeshData->m_ModelAnimation, 10);

	//ù��°Ű ����
	std::string::size_type start	= Path.rfind("/") + 1;
	std::string::size_type End		= Path.rfind('+') - start;
	std::string MeshName			= Path.substr(start, End);

	ModelAnimationData* Data = nullptr;

	//�ִϸ��̼��� ���°�� ����
	if (LoadManager::AnimationList.find(MeshName) == LoadManager::AnimationList.end())
	{
		Animation* animation = new Animation();
		animation->Name = MeshName;

		Data = animation->m_AnimationData;
		Data->Name = MeshName;

		LoadManager::AnimationList.insert({ MeshName, animation });
	}
	else
	{
		Data = LoadManager::GetAnimationData(MeshName);
	}

	//�ι�° Ű ����
	start				= Path.rfind('+') + 1;
	size_t mid			= Path.rfind('.');
	End					= mid - start;
	std::string key		= Path.substr(start, End);

	//������ ����
	MeshData->m_ModelAnimation->m_Index = (int)Data->AnimList.size();

	Data->AnimationCount++;

	Data->AnimList.insert({ key, std::move(MeshData->m_ModelAnimation) });
}