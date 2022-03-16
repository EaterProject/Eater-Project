#include "EATERManager.h"
#include "EaterHeader.h"
#include "EngineData.h"
#include "GameObject.h"
#include "LoadManager.h"
#include "GraphicEngineManager.h"
#include "MainHeader.h"
#include "MeshFilter.h"
#include "Transform.h"
#include "AnimationController.h"

#define LERP(prev, next, time) ((prev * (1.0f - time)) + (next * time))
EATERManager::EATERManager()
{
	m_Graphic = nullptr;
	m_CriticalSection = nullptr;
}

EATERManager::~EATERManager()
{
	m_Graphic = nullptr;
	m_CriticalSection = nullptr;
}

void EATERManager::Initialize(GraphicEngineManager* Graphic, CRITICAL_SECTION* _cs)
{
	m_Graphic = Graphic;
	m_CriticalSection = _cs;
}

void EATERManager::Load(std::string& Path, UINT parsingMode)
{
	BoneList.clear();
	SkinList.clear();

	ModelData* SaveData = new ModelData();
	EATER_OPEN_FILE(Path);

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
			SaveData->TopMeshList.push_back(Data);
		}
		else if (NodeName == "BONE")
		{
			LoadMeshData* Data = LoadBoneMesh(i);
			BoneList.push_back(Data);
		}
		else if (NodeName == "SKIN")
		{
			LoadMeshData* Data = LoadSkinMesh(i);
			SaveData->TopSkinList.push_back(Data);
			SaveName.erase(SaveName.rfind('+'));
			SaveData->BoneOffsetList = Data->BoneTMList;
		}
		else if (NodeName == "TERRAIN")
		{

		}
		else if (NodeName == "ANIMATION")
		{
			LoadAnimation(i, Path);
			EATER_CLEAR_NODE();
			return;
		}
	}
	EATER_CLEAR_NODE();



	LinkBone(SaveData);

	LoadManager::ModelList.insert({ SaveName ,SaveData });
}

void EATERManager::LoadScene(std::string& Path)
{
	EATER_OPEN_FILE(Path);
	int Count = EATER_GET_NODE_COUNT();
	for (int i = 0; i < Count; i++)
	{
		std::string NodeName = EATER_GET_NODE_NAME(i);
		if (NodeName == "GAMEOBJECT")
		{
			LoadGameObject(i);
		}
	}
}

LoadMeshData* EATERManager::LoadStaticMesh(int index)
{
	LoadMeshData* model = new LoadMeshData();
	model->MeshType = MESH_TYPE::STATIC_MESH;
	LoadName(index, model);
	LoadMaterial(index, model);
	LoadTM(index, model);

	// 저장되있는 Mesh Index를 Load 기준 Index로 변환..
	int meshIndex = std::stoi(EATER_GET_MAP(index, "MeshIndex"));
	meshIndex = LoadManager::FindInstanceIndex(meshIndex);
	
	// 해당 Index의 Mesh Buffer 유무 체크..
	MeshBuffer* meshBuffer = LoadManager::GetMeshBuffer(meshIndex);

	// 만약 같은 Mesh Buffer가 없을경우 새로 생성해주고
	// 있을 경우 해당 Mesh Buffer로 설정..
	if (meshBuffer == nullptr)
	{
		ParserData::Mesh* mesh = new ParserData::Mesh();

		LoadVertex(index, mesh);
		LoadIndex(index, mesh);

		// 검사하고
		EnterCriticalSection(m_CriticalSection);
		m_Graphic->CreateMeshBuffer(mesh, model);
		LeaveCriticalSection(m_CriticalSection);
		
		// Mesh Buffer Index 설정..
		model->MeshBuf->BufferIndex = meshIndex;

		LoadManager::MeshBufferList.insert({ meshIndex, model->MeshBuf });
	}
	else
	{
		model->MeshBuf = meshBuffer;
	}

	return model;
}

LoadMeshData* EATERManager::LoadBoneMesh(int index)
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

LoadMeshData* EATERManager::LoadSkinMesh(int index)
{
	LoadMeshData* model = new LoadMeshData();
	model->MeshType = MESH_TYPE::SKIN_MESH;

	LoadMaterial(index, model);
	LoadName(index, model);
	LoadTM(index, model);
	LoadBoneOffset(index, model);

	// 저장되있는 Mesh Index를 Load 기준 Index로 변환..
	int meshIndex = std::stoi(EATER_GET_MAP(index, "MeshIndex"));
	meshIndex = LoadManager::FindInstanceIndex(meshIndex);

	// 해당 Index의 Mesh Buffer 유무 체크..
	MeshBuffer* meshBuffer = LoadManager::GetMeshBuffer(meshIndex);

	// 만약 같은 Mesh Buffer가 없을경우 새로 생성해주고
	// 있을 경우 해당 Mesh Buffer로 설정..
	if (meshBuffer == nullptr)
	{
		ParserData::Mesh* mesh = new ParserData::Mesh();

		LoadSkinVertex(index, mesh);
		LoadIndex(index, mesh);

		// 검사하고
		EnterCriticalSection(m_CriticalSection);
		m_Graphic->CreateMeshBuffer(mesh, model);
		LeaveCriticalSection(m_CriticalSection);

		// Mesh Buffer Index 설정..
		model->MeshBuf->BufferIndex = meshIndex;

		LoadManager::MeshBufferList.insert({ meshIndex, model->MeshBuf });
	}
	else
	{
		model->MeshBuf = meshBuffer;
	}

	return model;
}

void EATERManager::LinkBone(ModelData* SaveData)
{
	///생성된 본들을 부모자식관계로 링크시켜줌
	int Count = BoneList.size();
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
				SaveData->TopBoneList.push_back(Front);
				SaveData->BoneList.push_back(Front);
				break;
			case MESH_TYPE::SKIN_MESH:
				SaveData->TopSkinList.push_back(Front);
				break;
			case MESH_TYPE::STATIC_MESH:
				SaveData->TopMeshList.push_back(Front);
				break;
			}
		}
	}
}

void EATERManager::LinkSkin()
{


}

void EATERManager::LoadTM(int Index, LoadMeshData* model)
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

void EATERManager::LoadVertex(int index, ParserData::Mesh* mesh)
{
	int VertexCount = EATER_GET_LIST_CHOICE(index, "Vertex");
	for (int i = 0; i < VertexCount; i++)
	{
		ParserData::Vertex* V = new ParserData::Vertex();

		std::vector<float> Data;
		EATER_GET_LIST(&Data, i);

		V->m_Pos.x = Data[0];
		V->m_Pos.y = Data[1];
		V->m_Pos.z = Data[2];

		V->m_UV.x = Data[3];
		V->m_UV.y = Data[4];

		V->m_Normal.x = Data[5];
		V->m_Normal.y = Data[6];
		V->m_Normal.z = Data[7];

		V->m_Tanget.x = Data[8];
		V->m_Tanget.y = Data[9];
		V->m_Tanget.z = Data[10];

		mesh->m_VertexList.push_back(V);
	}
}

void EATERManager::LoadSkinVertex(int index, ParserData::Mesh* mesh)
{
	int VertexCount = EATER_GET_LIST_CHOICE(index, "Vertex");
	for (int i = 0; i < VertexCount; i++)
	{
		ParserData::Vertex* V = new ParserData::Vertex();

		std::vector<float> Data;
		EATER_GET_LIST(&Data, i);

		V->m_Pos.x = Data[0];
		V->m_Pos.y = Data[1];
		V->m_Pos.z = Data[2];

		V->m_UV.x = Data[3];
		V->m_UV.y = Data[4];

		V->m_Normal.x = Data[5];
		V->m_Normal.y = Data[6];
		V->m_Normal.z = Data[7];

		V->m_Tanget.x = Data[8];
		V->m_Tanget.y = Data[9];
		V->m_Tanget.z = Data[10];

		int Boneindex = 11;
		for (int j = 0; j < 8; j++)
		{
			if (Data[Boneindex] != -1)
			{
				V->m_BoneIndices.push_back(Data[Boneindex]);
			}
			Boneindex++;
		}

		for (int j = 0; j < 8; j++)
		{
			if (Data[Boneindex] != -1)
			{
				V->m_BoneWeights.push_back(Data[Boneindex]);
			}
			Boneindex++;
		}


		mesh->m_VertexList.push_back(V);
	}
}

void EATERManager::LoadIndex(int index, ParserData::Mesh* mesh)
{
	int IndexCount = EATER_GET_LIST_CHOICE(index, "Index");
	mesh->m_IndexList.reserve(IndexCount);
	for (int i = 0; i < IndexCount; i++)
	{
		ParserData::IndexList* FACE = new ParserData::IndexList();

		std::vector<float> Data;
		EATER_GET_LIST(&Data, i);

		FACE->m_Index[0] = (int)Data[0];
		FACE->m_Index[1] = (int)Data[1];
		FACE->m_Index[2] = (int)Data[2];

		mesh->m_IndexList.push_back(FACE);
	}
}

void EATERManager::LoadBoneOffset(int index, LoadMeshData* mesh)
{
	int BoneOffsetCount = EATER_GET_LIST_CHOICE(index, "BoneOffset");
	mesh->BoneTMList = new std::vector<Matrix>();
	mesh->BoneTMList->reserve(BoneOffsetCount);
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
		mesh->BoneTMList->push_back(TM);
	}
}

void EATERManager::LoadAnimation(int index, std::string& Name)
{
	//첫번째 키 생성
	std::size_t Start_Mesh = Name.rfind('/') + 1;
	std::size_t End_Mesh = Name.rfind('+') - Start_Mesh;
	std::string MeshName = Name.substr(Start_Mesh, End_Mesh);

	std::size_t Start_Anime = Name.rfind('+') + 1;
	std::size_t End_Anime = Name.rfind('.') - Start_Anime;
	std::string AnimationName = Name.substr(Start_Anime, End_Anime);


	ModelAnimationData* Data = nullptr;
	std::vector<ParserData::OneAnimation*>* AnimeData = nullptr;

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
	Data->AnimList[AnimationName] = new std::vector<ParserData::OneAnimation*>();


	float m_TicksPerFrame = std::stof(EATER_GET_MAP(index, "TickFrame"));
	float m_TotalFrame = std::stof(EATER_GET_MAP(index, "TotalFrame"));
	float m_StartFrame = std::stoi(EATER_GET_MAP(index, "StartFrame"));
	float m_EndFrame = std::stoi(EATER_GET_MAP(index, "EndFrame"));

	int BoneCount = std::stoi(EATER_GET_MAP(index, "BoneCount"));
	for (int k = 0; k < BoneCount; k++)
	{
		ParserData::OneAnimation* OneAnime = new OneAnimation();
		OneAnime->m_EndFrame = (int)m_EndFrame;
		OneAnime->m_StartFrame = (int)m_StartFrame;
		OneAnime->m_TotalFrame = (int)m_TotalFrame;
		OneAnime->m_TicksPerFrame = m_TicksPerFrame;


		//두번째 키 생성
		int AnimationCount = EATER_GET_LIST_CHOICE(index, std::to_string(k));
		for (int i = 0; i < AnimationCount; i++)
		{
			ParserData::OneFrame* Frame = new ParserData::OneFrame();
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

std::string EATERManager::CutStr(std::string& Name)
{
	std::size_t Start = 0;
	std::size_t End = Name.rfind('.');
	return Name.substr(Start, End);
}

void EATERManager::LoadMaterial(int index, LoadMeshData* model)
{
	std::string material = EATER_GET_MAP(index, "Material");

	// 메테리얼 이름 설정..
	if (material != "NO")
	{
		model->MaterialName = material;
	}
}

void EATERManager::LoadName(int index, LoadMeshData* model)
{
	model->Name = EATER_GET_MAP(index, "NodeName");
	model->ParentName = EATER_GET_MAP(index, "ParentName");
}

void EATERManager::LoadGameObject(int index)
{
	GameObject* Object = Instance();
	std::string Type = EATER_GET_MAP(index, "MeshType");

	if (Type == "BASE")
	{
		CreateStaticObject(index, Object);
	}
	else if (Type == "SKIN")
	{
		CreateSkinObject(index, Object);
	}
}

void EATERManager::CreateStaticObject(int index, GameObject* Object)
{
	Load_Component_Transform(index, Object);
	Load_Component_MeshFilter(index, Object);
}

void EATERManager::CreateSkinObject(int index, GameObject* Object)
{
	Load_Component_Transform(index, Object);
	Load_Component_Skinning(index, Object);
}

void EATERManager::CreateParticleObject(int index, GameObject* Object)
{

}

void EATERManager::Load_Component_MeshFilter(int index, GameObject* Object)
{
	MeshFilter* mMeshFilter = Object->AddComponent<MeshFilter>();
	int Count = EATER_GET_LIST_CHOICE(index, "MeshFilter");
	for (int i = 0; i < Count; i++)
	{
		std::vector<std::string> Data;
		EATER_GET_LIST(&Data, i);
		mMeshFilter->SetMeshName(Data[0]);
	}
}

void EATERManager::Load_Component_Transform(int index, GameObject* Object)
{
	Transform* mTransform = Object->GetTransform();
	EATER_GET_LIST_CHOICE(index, "Transform");
	std::vector<float> Data;
	EATER_GET_LIST(&Data, 0);
	mTransform->Position = { Data[0],Data[1] ,Data[2] };
	EATER_GET_LIST(&Data, 1);
	mTransform->Rotation = { Data[0],Data[1] ,Data[2] };
	EATER_GET_LIST(&Data, 2);
	mTransform->Scale = { Data[0],Data[1] ,Data[2] };
}

void EATERManager::Load_Component_Skinning(int index, GameObject* Object)
{
	AnimationController* AC = Object->AddComponent<AnimationController>();
	MeshFilter* MF = Object->AddComponent<MeshFilter>();
	std::string MeshName;

	int Count = EATER_GET_LIST_CHOICE(index, "MeshFilter");
	for (int i = 0; i < Count; i++)
	{
		std::vector<std::string> Data;
		EATER_GET_LIST(&Data, i);
		MF->SetMeshName(Data[0]);
		MeshName = Data[0];
	}

	Count = EATER_GET_LIST_CHOICE(index, "Animation");
	for (int i = 0; i < Count; i++)
	{
		std::vector<std::string> Data;
		EATER_GET_LIST(&Data, 0);
		MF->SetAnimationName(MeshName);
		AC->Choice(Data[0]);
		AC->Play(1, true);
	}
}

void EATERManager::CreateKeyFrame(std::vector<ParserData::OneAnimation*>* Anime, int InputKeyCount)
{
	//기존 애니메이션
	std::vector<ParserData::OneAnimation*>::iterator it = Anime->begin();

	for (it; it != Anime->end(); it++)
	{
		std::vector<ParserData::OneFrame*> data = (*it)->m_AniData;
		//새롭게 넣을 데이터 리스트
		std::vector<ParserData::OneFrame*> CreateData;

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
				ParserData::OneFrame* temp = new OneFrame();
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