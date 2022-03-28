#include "Eater_LoadMesh.h"
#include "EaterHeader.h"
#include "EngineData.h"
#include "LoadManager.h"
#include "Eater_LoadCamera.h"

#define LERP(prev, next, time) ((prev * (1.0f - time)) + (next * time))
Eater_LoadMesh::Eater_LoadMesh()
{
}

Eater_LoadMesh::~Eater_LoadMesh()
{

}

void Eater_LoadMesh::Initialize()
{
	mCamera = new Eater_LoadCamera();
	mCamera->Initialize();
}
void Eater_LoadMesh::LoadData(std::string& Path)
{
	BoneList.clear();
	SkinList.clear();

	ModelData* SaveData = new ModelData();
	EATER_OPEN_READ_FILE(Path);

	std::size_t start = Path.rfind('/') + 1;
	std::size_t End = Path.rfind('.') - start;
	std::string SaveName = Path.substr(start, End);

	int Count = EATER_GET_NODE_COUNT();
	for (int i = 0; i < Count; i++)
	{
		std::string NodeName = EATER_GET_NODE_NAME(i);

		if (NodeName == "STATIC")
		{
			LoadMeshData* Data = LoadStaticMesh(i);
			Data->ModelName = SaveName;
			SaveData->TopMeshList.push_back(Data);
		}
		else if (NodeName == "BONE")
		{
			LoadMeshData* Data = LoadBoneMesh(i);
			Data->ModelName = SaveName;
			BoneList.push_back(Data);
		}
		else if (NodeName == "SKIN")
		{
			LoadMeshData* Data = LoadSkinMesh(i);
			SaveData->TopSkinList.push_back(Data);
			Data->ModelName = SaveName;
			SaveData->BoneOffsetList = Data->BoneTMList;
		}
		else if (NodeName == "TERRAIN")
		{

		}
		else if (NodeName == "ANIMATION")
		{
			LoadAnimation(i, Path);
			EATER_CLOSE_READ_FILE();
			return;
		}
		else if (NodeName == "CAM_ANIMATION")
		{
			mCamera->SetPath(Path);
			mCamera->LoadData(i);
		}
	}
	EATER_CLOSE_READ_FILE();

	LinkBone(SaveData);


	std::map<std::string, ModelData*>::iterator Fine_it = LoadManager::ModelList.find(SaveName);
	std::map<std::string, ModelData*>::iterator End_it	= LoadManager::ModelList.end();
	if (Fine_it != End_it)
	{
		LoadManager::ModelList.erase(SaveName);
	}

	LoadManager::ModelList.insert({ SaveName ,SaveData });
}

LoadMeshData* Eater_LoadMesh::LoadStaticMesh(int index)
{
	LoadMeshData* model = new LoadMeshData();
	model->MeshType = MESH_TYPE::STATIC_MESH;
	LoadName(index, model);
	LoadTM(index, model);
	LoadMeshName(index, model);
	LoadMaterial(index, model);

	return model;
}

LoadMeshData* Eater_LoadMesh::LoadBoneMesh(int index)
{
	LoadMeshData* model = new LoadMeshData();
	model->MeshType = MESH_TYPE::BONE_MESH;
	model->ParentName = EATER_GET_MAP(index, "ParentName");
	model->Name = EATER_GET_MAP(index, "NodeName");
	model->BoneIndex = std::stoi(EATER_GET_MAP(index, "BoneIndex"));

	LoadTM(index, model);
	std::string TOP = EATER_GET_MAP(index, "TopNode");
	if (TOP == "NO")
	{
		model->Top_Object = false;
	}
	else
	{
		model->Top_Object = true;
	}

	return model;
}

LoadMeshData* Eater_LoadMesh::LoadSkinMesh(int index)
{
	LoadMeshData* model = new LoadMeshData();
	model->MeshType = MESH_TYPE::SKIN_MESH;

	LoadName(index, model);
	LoadTM(index, model);
	LoadBoneOffset(index, model);
	LoadMeshName(index, model);
	LoadMaterial(index, model);

	return model;
}

void Eater_LoadMesh::LoadTM(int Index, LoadMeshData* model)
{
	int WorldTMCount = EATER_GET_LIST_CHOICE(Index, "WorldTM");
	for (int i = 0; i < WorldTMCount; i++)
	{
		std::vector<float> Data;
		EATER_GET_LIST(&Data, i);
		switch (i)
		{
		case 0:
			model->WorldTM._11 = Data[0];
			model->WorldTM._12 = Data[1];
			model->WorldTM._13 = Data[2];
			model->WorldTM._14 = Data[3];
			break;
		case 1:
			model->WorldTM._21 = Data[0];
			model->WorldTM._22 = Data[1];
			model->WorldTM._23 = Data[2];
			model->WorldTM._24 = Data[3];
			break;
		case 2:
			model->WorldTM._31 = Data[0];
			model->WorldTM._32 = Data[1];
			model->WorldTM._33 = Data[2];
			model->WorldTM._34 = Data[3];
			break;
		case 3:
			model->WorldTM._41 = Data[0];
			model->WorldTM._42 = Data[1];
			model->WorldTM._43 = Data[2];
			model->WorldTM._44 = Data[3];
			break;
		}
	}

	int LocalTMCount = EATER_GET_LIST_CHOICE(Index, "LocalTM");
	for (int i = 0; i < LocalTMCount; i++)
	{
		std::vector<float> Data;
		EATER_GET_LIST(&Data, i);
		switch (i)
		{
		case 0:
			model->LocalTM._11 = Data[0];
			model->LocalTM._12 = Data[1];
			model->LocalTM._13 = Data[2];
			model->LocalTM._14 = Data[3];
			break;
		case 1:
			model->LocalTM._21 = Data[0];
			model->LocalTM._22 = Data[1];
			model->LocalTM._23 = Data[2];
			model->LocalTM._24 = Data[3];
			break;
		case 2:
			model->LocalTM._31 = Data[0];
			model->LocalTM._32 = Data[1];
			model->LocalTM._33 = Data[2];
			model->LocalTM._34 = Data[3];
			break;
		case 3:
			model->LocalTM._41 = Data[0];
			model->LocalTM._42 = Data[1];
			model->LocalTM._43 = Data[2];
			model->LocalTM._44 = Data[3];
			break;
		}
	}
}

void Eater_LoadMesh::LoadBoneOffset(int index, LoadMeshData* model)
{
	int BoneOffsetCount = EATER_GET_LIST_CHOICE(index, "BoneOffset");
	model->BoneTMList = new std::vector<Matrix>();
	model->BoneTMList->reserve(BoneOffsetCount);
	for (int i = 0; i < BoneOffsetCount; i++)
	{
		std::vector<float> Data;
		EATER_GET_LIST(&Data, i);
		DirectX::SimpleMath::Matrix TM;
		TM._11 = Data[0];
		TM._12 = Data[1];
		TM._13 = Data[2];
		TM._14 = Data[3];

		TM._21 = Data[4];
		TM._22 = Data[5];
		TM._23 = Data[6];
		TM._24 = Data[7];

		TM._31 = Data[8];
		TM._32 = Data[9];
		TM._33 = Data[10];
		TM._34 = Data[11];

		TM._41 = Data[12];
		TM._42 = Data[13];
		TM._43 = Data[14];
		TM._44 = Data[15];

		model->BoneTMList->push_back(TM);
	}
}

void Eater_LoadMesh::LoadMaterial(int index, LoadMeshData* model)
{
	std::string material = EATER_GET_MAP(index, "MaterialName");

	// Material Name 설정..
	if (material != "NO")
	{
		model->MaterialName = material;
	}
}

void Eater_LoadMesh::LoadMeshName(int index, LoadMeshData* model)
{
	std::string mesh = EATER_GET_MAP(index, "MeshName");

	// Mesh Name 설정..
	if (mesh != "NO")
	{
		model->BufferName = mesh;
	}
}

void Eater_LoadMesh::LoadName(int index, LoadMeshData* model)
{
	model->Name			= EATER_GET_MAP(index, "NodeName");
	model->ParentName	= EATER_GET_MAP(index, "ParentName");
	std::string Top		= EATER_GET_MAP(index, "TopNode");
	if (Top != "NO")
	{
		model->Top_Object = true;
	}
	else
	{
		model->Top_Object = false;
	}
}

void Eater_LoadMesh::LinkBone(ModelData* Data)
{
	///생성된 본들을 부모자식관계로 링크시켜줌
	int Count = (int)BoneList.size();
	LoadMeshData* Front = nullptr;
	LoadMeshData* Back = nullptr;

	for (int i = 0; i < Count; i++)
	{
		for (int j = 0; j < Count; j++)
		{
			Front = BoneList[i];
			Back = BoneList[j];

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
				Data->TopBoneList.push_back(Front);
				Data->BoneList.push_back(Front);
				break;
			case MESH_TYPE::SKIN_MESH:
				Data->TopSkinList.push_back(Front);
				break;
			case MESH_TYPE::STATIC_MESH:
				Data->TopMeshList.push_back(Front);
				break;
			}
		}
	}
}

void Eater_LoadMesh::CreateKeyFrame(std::vector<ParserData::CAnimation*>* Anime, int InputKeyCount)
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

void Eater_LoadMesh::LoadAnimation(int index, std::string& Name)
{
	//첫번째 키 생성
	std::size_t Start_Mesh = Name.rfind('/') + 1;
	std::size_t End_Mesh = Name.rfind('+')+1 - Start_Mesh;
	std::string MeshName = Name.substr(Start_Mesh, End_Mesh);

	std::size_t Start_Anime = Name.rfind('+') + 1;
	std::size_t End_Anime = Name.rfind('.') - Start_Anime;
	std::string AnimationName = Name.substr(Start_Anime, End_Anime);


	ModelAnimationData* Data = nullptr;
	std::vector<ParserData::CAnimation*>* AnimeData = nullptr;

	//다른 애니메이션이 없다면 새롭게 생성
	if (LoadManager::AnimationList.find(MeshName) == LoadManager::AnimationList.end())
	{
		Data = new ModelAnimationData();
		LoadManager::AnimationList.insert({ MeshName,Data });
	}
	else
	{
		Data = LoadManager::AnimationList[MeshName];
	}
	Data->AnimList[AnimationName] = new std::vector<ParserData::CAnimation*>();


	float m_TicksPerFrame = std::stof(EATER_GET_MAP(index, "TickFrame"));
	float m_TotalFrame = std::stof(EATER_GET_MAP(index, "TotalFrame"));
	float m_StartFrame = std::stof(EATER_GET_MAP(index, "StartFrame"));
	float m_EndFrame = std::stof(EATER_GET_MAP(index, "EndFrame"));

	int BoneCount = std::stoi(EATER_GET_MAP(index, "BoneCount"));
	for (int k = 0; k < BoneCount; k++)
	{
		ParserData::CAnimation* OneAnime = new CAnimation();
		OneAnime->m_EndFrame = (int)m_EndFrame;
		OneAnime->m_StartFrame = (int)m_StartFrame;
		OneAnime->m_TotalFrame = (int)m_TotalFrame;
		OneAnime->m_TicksPerFrame = m_TicksPerFrame;


		//두번째 키 생성
		int AnimationCount = EATER_GET_LIST_CHOICE(index, std::to_string(k));
		for (int i = 0; i < AnimationCount; i++)
		{
			ParserData::CFrame* Frame = new ParserData::CFrame();
			std::vector<float> Data;
			EATER_GET_LIST(&Data, i);
			int Size = (int)Data.size();
			Frame->m_Pos.x = Data[0];
			Frame->m_Pos.y = Data[1];
			Frame->m_Pos.z = Data[2];

			Frame->m_RotQt.x = Data[3];
			Frame->m_RotQt.y = Data[4];
			Frame->m_RotQt.z = Data[5];
			Frame->m_RotQt.w = Data[6];

			Frame->m_Scale.x = Data[7];
			Frame->m_Scale.y = Data[8];
			Frame->m_Scale.z = Data[9];
			Frame->m_Time = Data[10];
			OneAnime->m_AniData.push_back(Frame);
		}
		Data->AnimList[AnimationName]->push_back(OneAnime);
	}
	CreateKeyFrame(Data->AnimList[AnimationName], 10);
}
