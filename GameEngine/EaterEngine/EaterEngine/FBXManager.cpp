#include "FBXManager.h"
#include "YAMLHeader.h"
#include "EaterHeader.h"
#include "ModelParser.h"
#include "LoadManager.h"
#include "EngineData.h"
#include "TextureManager.h"
#include "GraphicEngineManager.h"
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

void FBXManager::LoadTerrain(std::string Name, std::string MaskName, UINT parsingMode)
{
	ParserData::Model * mMesh = FBXLoad->LoadModel(Name, parsingMode);
	int size =  (int)mMesh->m_MaterialList.size();
	ParserData::Mesh* TerrainMesh = mMesh->m_MeshList[0];

	LoadMeshData* Data = new LoadMeshData();
	ModelData* Model = new ModelData();


	SetMatrixData(TerrainMesh, Data);
	SetNameData(TerrainMesh, Data);

	Data->Mask_Name = MaskName;
	Data->MeshType = TERRAIN_MESH;
	SetBufferData(TerrainMesh, Data);

	std::size_t Start	= Name.rfind('/')+1;
	std::size_t End		= Name.rfind('.') - Start;
	Name = Name.substr(Start, End);

	Model->TopMeshList.push_back(Data);
	LoadManager::ModelList.insert({Name,Model });
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
	isAnimation = LoadManager::FindMesh(MeshName);
}

void FBXManager::CreateKeyFrame(std::vector<ParserData::OneAnimation*>* Anime, int InputKeyCount)
{
	//���� �ִϸ��̼�
	std::vector<ParserData::OneAnimation*>::iterator it = Anime->begin();

	for (it; it != Anime->end(); it++)
	{
		std::vector<ParserData::OneFrame*> data = (*it)->m_AniData;
		//���Ӱ� ���� ������ ����Ʈ
		std::vector<ParserData::OneFrame*> CreateData;

		int Size = (int)data.size();
		for (int i = 0; i < Size - 1; i++)
		{
			//������ ó����
			DirectX::SimpleMath::Vector3 Start_Pos = data[i]->m_Pos;
			DirectX::SimpleMath::Quaternion Start_Rot = data[i]->m_RotQt;
			DirectX::SimpleMath::Vector3 Start_Scl = data[i]->m_Scale;
			float Start_Time = data[i]->m_Time;

			//������ ������
			int NextIndex = i + 1;
			DirectX::SimpleMath::Vector3 End_Pos = data[NextIndex]->m_Pos;
			DirectX::SimpleMath::Quaternion End_Rot = data[NextIndex]->m_RotQt;
			DirectX::SimpleMath::Vector3 End_Scl = data[NextIndex]->m_Scale;
			float End_Time = data[NextIndex]->m_Time;

			///ó���� �־��ֱ�
			CreateData.push_back(data[i]);

			int KeyCount = InputKeyCount;		//������ Ű������
			float CountLerp = (float)(1 / InputKeyCount);	//������ Ű������ ������ ��ġ��������
			for (int j = 0; j <= KeyCount; j++)
			{
				//���ο� Ű ������ ����
				ParserData::OneFrame* temp = new OneFrame();
				temp->m_Pos = Vector3::Lerp(Start_Pos, End_Pos, CountLerp);
				temp->m_RotQt = Quaternion::Lerp(Start_Rot, End_Rot, CountLerp);
				temp->m_Scale = Vector3::Lerp(Start_Scl, End_Scl, CountLerp);
				temp->m_Time = LERP(Start_Time, End_Time, CountLerp);

				CreateData.push_back(temp);
				CountLerp += 0.1f;
			}

			///�������� �־��ֱ�
			CreateData.push_back(data[NextIndex]);
		}

		(*it)->m_AniData = CreateData;
		(*it)->m_TicksPerFrame /= (InputKeyCount + 3);
		(*it)->m_EndFrame = (Size * (InputKeyCount + 3)) - (InputKeyCount + 3);
	}
}

LoadMeshData* FBXManager::CreateBaseMesh(ParserData::Mesh* mMesh)
{
	/// �⺻ �޽� ó��
	if (mMesh == nullptr) { return nullptr; }
	LoadMeshData* SaveMesh = new LoadMeshData();

	UINT meshIndex = LoadManager::FindInstanceIndex(mMesh->m_MeshIndex);
	
	std::map<int, LoadMeshData*>::iterator End_it = BufferIndexList.end();
	std::map<int, LoadMeshData*>::iterator Find_it = BufferIndexList.find(meshIndex);
	
	//��ã��
	if (End_it == Find_it)
	{
		SetNameData(mMesh, SaveMesh);		//�̸� ������ ����
		SetMaterialData(mMesh, SaveMesh);	//���׸��� ������ ����
		SetMatrixData(mMesh, SaveMesh);		//��Ʈ���� ������ ����
		SetBufferData(mMesh, SaveMesh);		//�ε��� ���� ���ؽ� ���� ����

		// ���� ������ Mesh Buffer Index ����..
		SaveMesh->MeshBuf->BufferIndex = meshIndex;

		BufferIndexList.insert({ meshIndex, SaveMesh });
	}
	else
	{
		SetNameData(mMesh, SaveMesh);		//�̸� ������ ����
		SetMaterialData(mMesh, SaveMesh);	//���׸��� ������ ����
		SetMatrixData(mMesh, SaveMesh);		//��Ʈ���� ������ ����

		SaveMesh->MeshBuf = Find_it->second->MeshBuf;
	}

	return SaveMesh;
}

LoadMeshData* FBXManager::CreateSkinMesh(ParserData::Mesh* mMesh)
{
	///��Ų �Ž� ó��
	if (mMesh == nullptr) { return nullptr; }
	LoadMeshData* SaveMesh = new LoadMeshData();

	UINT meshIndex = LoadManager::FindInstanceIndex(mMesh->m_MeshIndex);

	std::map<int, LoadMeshData*>::iterator End_it = BufferIndexList.end();
	std::map<int, LoadMeshData*>::iterator Find_it = BufferIndexList.find(meshIndex);

	//��ã��
	if (End_it == Find_it)
	{
		SetNameData(mMesh, SaveMesh);		//�̸� ������ ����
		SetMaterialData(mMesh, SaveMesh);	//���׸��� ������ ����
		SetMatrixData(mMesh, SaveMesh);		//��Ʈ���� ������ ����
		SetBufferData(mMesh, SaveMesh);		//�ε��� ���� ���ؽ� ���� ����

		// ���� ������ Mesh Buffer Index ����..
		SaveMesh->MeshBuf->BufferIndex = meshIndex;

		BufferIndexList.insert({ meshIndex, SaveMesh });
	}
	else
	{
		SetNameData(mMesh, SaveMesh);		//�̸� ������ ����
		SetMaterialData(mMesh, SaveMesh);	//���׸��� ������ ����
		SetMatrixData(mMesh, SaveMesh);		//��Ʈ���� ������ ����

		SaveMesh->MeshBuf = Find_it->second->MeshBuf;
	}

	return SaveMesh;
}

LoadMeshData* FBXManager::CreateBoneMesh(ParserData::Mesh* mMesh)
{
	///�� �Ž� ó��
	if (mMesh == nullptr) { return nullptr; }
	LoadMeshData* SaveMesh = new LoadMeshData();
	SaveMesh->BoneIndex = mMesh->m_BoneIndex;
	SetNameData(mMesh, SaveMesh);
	SetMatrixData(mMesh, SaveMesh);
	return SaveMesh;
}



void FBXManager::SetMaterialData(ParserData::Mesh* mMesh, LoadMeshData* SaveData)
{
	// Parsing Data ��� Material ����..
	CMaterial* mat = mMesh->m_MaterialData;
	//if (mat != nullptr)
	//{
	//	SaveData->Alpha = mMesh->m_MaterialData->m_Alpha;
	//
	//	if (mat->m_TextureBind & DIFFUSE_TEXTURE)
	//	{
	//		SaveData->AlbedoName = CutStr(mMesh->m_MaterialData->m_DiffuseMap->m_BitMap);
	//	}
	//
	//	if (mat->m_TextureBind & NORMAL_TEXTURE)
	//	{
	//		SaveData->NormalName = CutStr(mMesh->m_MaterialData->m_NormalMap->m_BitMap);
	//	}
	//
	//	if (mat->m_TextureBind & EMISSIVE_TEXTURE)
	//	{
	//		SaveData->EmissiveName = CutStr(mMesh->m_MaterialData->m_EmissiveMap->m_BitMap);
	//	}
	//
	//	if (mat->m_TextureBind & ORM_TEXTURE)
	//	{
	//		SaveData->ORMName = CutStr(mMesh->m_MaterialData->m_ORMMap->m_BitMap);
	//	}
	//}
}

void FBXManager::SetMatrixData(ParserData::Mesh* mMesh, LoadMeshData* SaveData)
{
	SaveData->WorldTM = mMesh->m_WorldTM;
	SaveData->LocalTM = mMesh->m_LocalTM;
}

void FBXManager::SetNameData(ParserData::Mesh* mMesh, LoadMeshData* SaveData)
{
	SaveData->Name			= mMesh->m_NodeName;
	SaveData->ParentName	= mMesh->m_ParentName;
	SaveData->MeshType		= mMesh->m_MeshType;
	SaveData->Top_Object	= mMesh->m_TopNode;
}

void FBXManager::SetBufferData(ParserData::Mesh* mMesh, LoadMeshData* SaveData)
{
	EnterCriticalSection(m_CriticalSection);
	m_Graphic->CreateMeshBuffer(mMesh, SaveData);
	LeaveCriticalSection(m_CriticalSection);
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

void FBXManager::CreateSaveMesh(ParserData::Model* mMesh, ModelData* SaveMesh, std::string& Path)
{
	std::vector<LoadMeshData*> BaseMeshList;
	std::vector<LoadMeshData*> SkinMeshList;
	std::vector<LoadMeshData*> BoneMeshList;

	// Buffer Mesh Index List �ʱ�ȭ..
	BufferIndexList.clear();

	//��� �Ž� ����
	int MeshSize = (int)mMesh->m_MeshList.size();
	for (int i = 0; i < MeshSize; i++)
	{
		ParserData::Mesh *OneMesh = mMesh->m_MeshList[i];
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
			SaveMesh->BoneOffsetList = &(OneMesh->m_BoneTMList);
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

	
	std::string Name = GetSaveName(Path);
	LoadManager::ModelList.insert({ Name,SaveMesh });
}

void FBXManager::LoadQuad()
{
	ModelData* SaveMesh = new ModelData();
	LoadMeshData* quad = new LoadMeshData();

	// Quad Mesh ����..
	quad->MeshType = QUAD_MESH;

	//���۰� �о����
	EnterCriticalSection(m_CriticalSection);
	m_Graphic->CreateMeshBuffer(nullptr, quad);
	LeaveCriticalSection(m_CriticalSection);
	SaveMesh->TopMeshList.push_back(quad);

	LoadManager::ModelList.insert({ "Quad" ,SaveMesh });
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
	ParserData::Model* mMesh = FBXLoad->LoadModel(Path, parsingMode);
	ModelData* SaveMesh = new ModelData();
	LoadAnimation(SaveMesh, mMesh, Path);

	//�ִϸ��̼Ǹ� �д´ٸ� ���⼭ ����
	if (isAnimation == true) { return; }

	//�ε��� �Ž��� �����Ų��
	CreateSaveMesh(mMesh, SaveMesh,Path);
}

void FBXManager::LoadAnimation(ModelData* SaveMesh, ParserData::Model* MeshData, std::string& Path)
{
	//if (isAnimation == false) { return; }

	if (Path.rfind('+') == std::string::npos) { return; }

	//Ű������ ����
	//CreateKeyFrame(&(MeshData->m_AnimationList), 10);

	//ù��°Ű ����
	std::string::size_type start	= Path.rfind("/") + 1;
	std::string::size_type End		= Path.rfind('+') - start;
	std::string SaveName			= Path.substr(start, End);

	//�ִϸ��̼��� ���°�� ����
	if (LoadManager::AnimationList.find(SaveName) == LoadManager::AnimationList.end())
	{
		ModelAnimationData* data = new ModelAnimationData();
		LoadManager::AnimationList.insert({ SaveName,data });
	}

	//�ι�° Ű ����
	start				= Path.rfind('+') + 1;
	size_t mid			= Path.rfind('.');
	End					= mid - start;
	std::string key		= Path.substr(start, End);

	//������ ����
	ModelAnimationData* temp = LoadManager::AnimationList[SaveName];
	temp->AnimList.insert({ key, &(MeshData->m_AnimationList) });
}


