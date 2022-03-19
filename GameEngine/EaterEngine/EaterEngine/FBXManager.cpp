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
	//FBX 파서 초기화
	FBXLoad = ModelParser::Create(MODEL_TYPE::FBX_MODEL);
	FBXLoad->Initialize();
	m_Graphic			= Graphic;
	m_CriticalSection	= _cs;

	LoadQuad();
}

void FBXManager::LoadTerrain(std::string Name, std::string MaskName, UINT parsingMode)
{
	ParserData::CModel* mMesh = FBXLoad->LoadModel(Name, parsingMode);
	int size =  (int)mMesh->m_MaterialList.size();
	ParserData::CMesh* TerrainMesh = mMesh->m_MeshList[0];

	// Terrain Mask Name 삽입..
	TerrainMesh->m_MeshType = TERRAIN_MESH;
	TerrainMesh->m_MaskName = MaskName;

	LoadMeshData* Data = new LoadMeshData();
	ModelData* model = new ModelData();
	Data->MeshType = TERRAIN_MESH;

	SetMatrixData(TerrainMesh, Data);
	SetNameData(TerrainMesh, Data);
	SetMeshData(TerrainMesh, Data);

	std::size_t Start	= Name.rfind('/')+1;
	std::size_t End		= Name.rfind('.') - Start;
	Name = Name.substr(Start, End);

	model->TopMeshList.push_back(Data);
	LoadManager::ModelList.insert({ Name, model });
}

void FBXManager::CheckSkinning(std::string& Path)
{
	//+ 문자열을 찾지 못했다면 스키닝 매쉬가 아니라는뜻!!
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

void FBXManager::CreateKeyFrame(std::vector<ParserData::CAnimation*>* Anime, int InputKeyCount)
{
	//기존 애니메이션
	std::vector<ParserData::CAnimation*>::iterator it = Anime->begin();

	for (it; it != Anime->end(); it++)
	{
		std::vector<ParserData::CFrame*> data = (*it)->m_AniData;
		//새롭게 넣을 데이터 리스트
		std::vector<ParserData::CFrame*> CreateData;

		int Size = (int)data.size();
		for (int i = 0; i < Size - 1; i++)
		{
			//보간할 처음값
			DirectX::SimpleMath::Vector3 Start_Pos = data[i]->m_Pos;
			DirectX::SimpleMath::Quaternion Start_Rot = data[i]->m_RotQt;
			DirectX::SimpleMath::Vector3 Start_Scl = data[i]->m_Scale;
			float Start_Time = data[i]->m_Time;

			//보간할 다음값
			int NextIndex = i + 1;
			DirectX::SimpleMath::Vector3 End_Pos = data[NextIndex]->m_Pos;
			DirectX::SimpleMath::Quaternion End_Rot = data[NextIndex]->m_RotQt;
			DirectX::SimpleMath::Vector3 End_Scl = data[NextIndex]->m_Scale;
			float End_Time = data[NextIndex]->m_Time;

			///처음값 넣어주기
			CreateData.push_back(data[i]);

			int KeyCount = InputKeyCount;		//생성할 키프레임
			float CountLerp = (float)(1 / InputKeyCount);	//생성할 키프레임 보간할 위치값을구함
			for (int j = 0; j <= KeyCount; j++)
			{
				//새로운 키 프레임 생성
				ParserData::CFrame* temp = new CFrame();
				temp->m_Pos = Vector3::Lerp(Start_Pos, End_Pos, CountLerp);
				temp->m_RotQt = Quaternion::Lerp(Start_Rot, End_Rot, CountLerp);
				temp->m_Scale = Vector3::Lerp(Start_Scl, End_Scl, CountLerp);
				temp->m_Time = LERP(Start_Time, End_Time, CountLerp);

				CreateData.push_back(temp);
				CountLerp += 0.1f;
			}

			///마지막값 넣어주기
			CreateData.push_back(data[NextIndex]);
		}

		(*it)->m_AniData = CreateData;
		(*it)->m_TicksPerFrame /= (InputKeyCount + 3);
		(*it)->m_EndFrame = (Size * (InputKeyCount + 3)) - (InputKeyCount + 3);
	}
}

LoadMeshData* FBXManager::CreateBaseMesh(ParserData::CMesh* mMesh)
{
	/// 기본 메쉬 처리
	if (mMesh == nullptr) { return nullptr; }
	LoadMeshData* SaveMesh = new LoadMeshData();

	SetNameData(mMesh, SaveMesh);		//이름 데이터 삽입
	SetMaterialData(mMesh, SaveMesh);	//메테리얼 데이터 삽입
	SetMatrixData(mMesh, SaveMesh);		//매트릭스 데이터 삽입
	SetMeshData(mMesh, SaveMesh);		//매쉬 데이터 삽입

	return SaveMesh;
}

LoadMeshData* FBXManager::CreateSkinMesh(ParserData::CMesh* mMesh)
{
	///스킨 매쉬 처리
	if (mMesh == nullptr) { return nullptr; }
	LoadMeshData* SaveMesh = new LoadMeshData();

	SetNameData(mMesh, SaveMesh);		//이름 데이터 삽입
	SetMaterialData(mMesh, SaveMesh);	//메테리얼 데이터 삽입
	SetMatrixData(mMesh, SaveMesh);		//매트릭스 데이터 삽입
	SetMeshData(mMesh, SaveMesh);		//매쉬 데이터 삽입

	return SaveMesh;
}

LoadMeshData* FBXManager::CreateBoneMesh(ParserData::CMesh* mMesh)
{
	///본 매쉬 처리
	if (mMesh == nullptr) { return nullptr; }
	LoadMeshData* SaveMesh = new LoadMeshData();
	SaveMesh->BoneIndex = mMesh->m_BoneIndex;
	SetNameData(mMesh, SaveMesh);
	SetMatrixData(mMesh, SaveMesh);
	return SaveMesh;
}

void FBXManager::SetMaterialData(ParserData::CMesh* mMesh, LoadMeshData* SaveData)
{
	// Parsing Data 기반 Material 생성..
	CMaterial* mat = mMesh->m_MaterialData;

	if (mat != nullptr)
	{
		// Material 이름 설정..
		std::string matName = nowFileName + "_" + mat->m_MaterialName;

		// 해당 Material 유무 체크..
		Material* newMaterial = LoadManager::GetMaterial(matName);
		
		if (newMaterial == nullptr)
		{
			// 해당 Material이 없을 경우 생성..
			newMaterial = new Material();

			// Alpha 상태 설정..
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

			// 현재 Material 저장..
			LoadManager::MaterialList.insert({ matName, newMaterial });

			// Material Buffer Graphic 측 연동..
			m_Graphic->PushMaterial(newMaterial->m_MaterialData);
		}

		// 추후 로드를 위해 Material 이름 저장..
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
	// Mesh 이름 설정..
	std::string meshName = nowFileName + "_" + std::to_string(mMesh->m_MeshIndex);

	// 해당 Index의 Mesh Buffer 유무 체크..
	Mesh* meshBuffer = LoadManager::GetMesh(meshName);

	if (meshBuffer == nullptr)
	{
		// Mesh 생성..
		meshBuffer = new Mesh();
		meshBuffer->Name = meshName;

		EnterCriticalSection(m_CriticalSection);
		m_Graphic->CreateMeshBuffer(mMesh, &meshBuffer->m_MeshData);
		LeaveCriticalSection(m_CriticalSection);

		// 현재 Mesh 저장..
		LoadManager::MeshBufferList.insert({meshName, meshBuffer });

		// Mesh Buffer Graphic 측 연동..
		m_Graphic->PushMesh(meshBuffer->m_MeshData);
	}

	// 해당 Mesh Buffer 이름 삽입..
	// 추후 Mesh Filter Start 에서 Mesh Buffer를 가져오는 기준이 되는 이름..
	SaveData->MeshName = meshBuffer->Name;
}

std::string FBXManager::CutStr(std::string Path)
{
	//png 자름
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

			//현재 조사중인 메쉬와  조사당하는 메쉬의 부모이름이 같다면 연결
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
		//그냥 매쉬 
		std::size_t Start	= Path.rfind('/')+1;
		std::size_t End		= Path.rfind('.') - Start;
		return Path.substr(Start, End);
	}
	else
	{
		//스키닝 매쉬
		std::size_t Start = Path.rfind('/')+1;
		std::size_t End = Path.rfind('+') - Start;
		return Path.substr(Start, End);
	}
}

void FBXManager::CreateSaveMesh(ParserData::CModel* mMesh, ModelData* SaveMesh, std::string& Path)
{
	std::vector<LoadMeshData*> BaseMeshList;
	std::vector<LoadMeshData*> SkinMeshList;
	std::vector<LoadMeshData*> BoneMeshList;

	// 현재 파일 이름 설정..
	nowFileName = GetSaveName(Path);

	//모든 매쉬 조사
	int MeshSize = (int)mMesh->m_MeshList.size();
	for (int i = 0; i < MeshSize; i++)
	{
		ParserData::CMesh *OneMesh = mMesh->m_MeshList[i];
		LoadMeshData* SaveOneMesh = nullptr;

		//타입별로 LoadMeshData로 값을 채워준다
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

	//만들어진 정보들고 부모 자식관계로 링크
	LinkMesh(BaseMeshList, SaveMesh);
	LinkMesh(SkinMeshList, SaveMesh);
	LinkMesh(BoneMeshList, SaveMesh);

	
	LoadManager::ModelList.insert({ nowFileName, SaveMesh });
}

void FBXManager::LoadQuad()
{
	ModelData* SaveMesh = new ModelData();
	LoadMeshData* quad = new LoadMeshData();
	MeshBuffer* quadBuf = nullptr;
	// Quad Mesh 설정..
	quad->MeshType = QUAD_MESH;
	quad->MeshName = "Quad";

	ParserData::CMesh* mesh = new ParserData::CMesh();
	mesh->m_MeshType = QUAD_MESH;

	//버퍼값 읽어오기
	EnterCriticalSection(m_CriticalSection);
	m_Graphic->CreateMeshBuffer(mesh, &quadBuf);
	LeaveCriticalSection(m_CriticalSection);

	/// 매쉬 자체포멧 생기면 이름만 저장하자..
	quad->MeshBuffer_Data = quadBuf;

	SaveMesh->TopMeshList.push_back(quad);

	LoadManager::ModelList.insert({ "Quad" ,SaveMesh });

	// Mesh Buffer Graphic 측 연동..
	m_Graphic->PushMesh(quadBuf);

	delete mesh;
}

void FBXManager::Load(std::string& Path, UINT parsingMode)
{
	CheckSkinning(Path);	//경로에 이름에따라 스키닝 객체인지 아닌지 체크
	CheckAnimation(Path);	//애니메이션을 로드 해야하는지 체크

	///애니메이션만 로드할것인지
	if (isAnimation == true)
	{
		parsingMode |= ANIMATION_ONLY;
	}
	
	LoadFile(Path, parsingMode);
}

void FBXManager::LoadFile(std::string& Path, UINT parsingMode)
{
	ParserData::CModel* mMesh = FBXLoad->LoadModel(Path, parsingMode);
	ModelData* SaveMesh = new ModelData();
	LoadAnimation(SaveMesh, mMesh, Path);

	//애니메이션만 읽는다면 여기서 종료
	if (isAnimation == true) { return; }

	//로드한 매쉬를 연결시킨다
	CreateSaveMesh(mMesh, SaveMesh,Path);
}

void FBXManager::LoadAnimation(ModelData* SaveMesh, ParserData::CModel* MeshData, std::string& Path)
{
	//if (isAnimation == false) { return; }

	if (Path.rfind('+') == std::string::npos) { return; }

	//키프레임 생성
	//CreateKeyFrame(&(MeshData->m_AnimationList), 10);

	//첫번째키 생성
	std::string::size_type start	= Path.rfind("/") + 1;
	std::string::size_type End		= Path.rfind('+') - start;
	std::string SaveName			= Path.substr(start, End);

	//애니메이션이 없는경우 생성
	if (LoadManager::AnimationList.find(SaveName) == LoadManager::AnimationList.end())
	{
		ModelAnimationData* data = new ModelAnimationData();
		LoadManager::AnimationList.insert({ SaveName,data });
	}

	//두번째 키 생성
	start				= Path.rfind('+') + 1;
	size_t mid			= Path.rfind('.');
	End					= mid - start;
	std::string key		= Path.substr(start, End);

	//데이터 저장
	ModelAnimationData* temp = LoadManager::AnimationList[SaveName];
	temp->AnimList.insert({ key, &(MeshData->m_AnimationList) });
}


