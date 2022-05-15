#include "Eater_LoadBuffer.h"
#include "LoadManager.h"
#include "EngineData.h"
#include "EaterHeader.h"
#include "ParserData.h"
#include "EaterHeader.h"
#include "Mesh.h"
#include "GraphicEngineManager.h"
#include "EaterEngineAPI.h"
Eater_LoadBuffer::Eater_LoadBuffer()
{


}

Eater_LoadBuffer::~Eater_LoadBuffer()
{


}

void Eater_LoadBuffer::Initialize(GraphicEngineManager* Graphic, CRITICAL_SECTION* _cs)
{
	m_Graphic = Graphic;
	m_CriticalSection = _cs;
}

void Eater_LoadBuffer::LoadData(std::string& Path)
{
	//인덱스,버텍스 버퍼를 로드
	EATER_OPEN_READ_FILE(Path);
	int NodeCount = EATER_GET_NODE_COUNT();
	Mesh* meshBuffer = nullptr;
	for (int i = 0; i < NodeCount; i++)
	{
		std::string NodeName = EATER_GET_NODE_NAME(i);

		//이름 설정
		std::string SaveName = CutStr(Path);

		ParserData::CMesh* mMesh = new ParserData::CMesh();
		mMesh->m_NodeName = SaveName;

		if (NodeName == "SKIN_BUFFER")
		{
			LoadSkinBuffer(i, mMesh);
			LoadIndex(i, mMesh);
			Mesh* Buffer = CreateBuffer(mMesh);
			Buffer->Name = SaveName;
			Buffer->m_MeshData->Name = SaveName;
			LoadManager::MeshBufferList.insert({ SaveName,Buffer });
			delete mMesh;
		}
		else if (NodeName == "STATIC_BUFFER")
		{
			LoadStaticBuffer(i, mMesh);
			LoadIndex(i, mMesh);
			Mesh* Buffer = CreateBuffer(mMesh);
			Buffer->Name = SaveName;
			Buffer->m_MeshData->Name = SaveName; 
			LoadManager::MeshBufferList.insert({ SaveName,Buffer });
			delete mMesh;
		}
		else if (NodeName == "POSITION_BUFFER")
		{
			ColliderBuffer* Buffer = new ColliderBuffer();
			LoadPositionBuffer(i, Buffer);
			LoadIndex(i, Buffer);
			LoadManager::ColliderBufferList.insert({ SaveName,Buffer });
			delete mMesh;
		}
		else if (NodeName == "NAVIGATION")
		{
			LoadNavigationBuffer(i);
			delete mMesh;
		}

	}
	EATER_CLOSE_READ_FILE();
}

void Eater_LoadBuffer::LoadStaticBuffer(int index, ParserData::CMesh* mesh)
{
	mesh->m_MeshType = MESH_TYPE::STATIC_MESH;
	int VertexCount = EATER_GET_LIST_CHOICE(index, "Vertex");
	for (int i = 0; i < VertexCount; i++)
	{
		ParserData::CVertex* V = new ParserData::CVertex();

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

void Eater_LoadBuffer::LoadSkinBuffer(int index, ParserData::CMesh* mesh)
{
	mesh->m_MeshType = MESH_TYPE::SKIN_MESH;
	int VertexCount = EATER_GET_LIST_CHOICE(index, "Vertex");
	for (int i = 0; i < VertexCount; i++)
	{
		ParserData::CVertex* V = new ParserData::CVertex();

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
				V->m_BoneIndices.push_back((UINT)Data[Boneindex]);
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

void Eater_LoadBuffer::LoadPositionBuffer(int index, ColliderBuffer* mesh)
{
	//mesh->m_MeshType = MESH_TYPE::STATIC_MESH;
	
	const int VertexCount	= EATER_GET_LIST_CHOICE(index, "Vertex");
	mesh->VertexArrayCount	= VertexCount;
	mesh->VertexArray		= new Vector3[VertexCount];
	for (int i = 0; i < VertexCount; i++)
	{
		std::vector<float> Data;
		EATER_GET_LIST(&Data, i);
		mesh->VertexArray[i].x = Data[0];
		mesh->VertexArray[i].y = Data[1];
		mesh->VertexArray[i].z = Data[2];
	}
}

void Eater_LoadBuffer::LoadNavigationBuffer(int index)
{
	//int Count = EATER_GET_LIST_CHOICE(index, "TRIANGLE");
	//LoadManager::NavMeshData.resize(Count);
	//for (int i = 0; i < Count; i++)
	//{
	//	OneTriangle* OneData = new OneTriangle();
	//	std::vector<float> Data;
	//	EATER_GET_LIST(&Data, i);
	//	OneData->Index = Data[0];
	//	OneData->VertexPos[0] = { Data[1],Data[2] ,Data[3] };
	//	OneData->VertexPos[1] = { Data[4],Data[5] ,Data[6] };
	//	OneData->VertexPos[2] = { Data[7],Data[8] ,Data[9] };
	//	OneData->CenterPoint = { Data[10],Data[11] ,Data[12] };
	//	OneData->FriendFace[0] = Data[13];
	//	OneData->FriendFace[1] = Data[14];
	//	OneData->FriendFace[2] = Data[15];
	//	LoadManager::NavMeshData[i] = OneData;
	//}
}

void Eater_LoadBuffer::LoadIndex(int index, ParserData::CMesh* mMesh)
{
	int IndexCount = EATER_GET_LIST_CHOICE(index, "Index");
	mMesh->m_IndexList.reserve(IndexCount);
	for (int i = 0; i < IndexCount; i++)
	{
		ParserData::CIndexList* FACE = new ParserData::CIndexList();

		std::vector<float> Data;
		EATER_GET_LIST(&Data, i);

		FACE->m_Index[0] = (int)Data[0];
		FACE->m_Index[1] = (int)Data[1];
		FACE->m_Index[2] = (int)Data[2];

		mMesh->m_IndexList.push_back(FACE);
	}
}

void Eater_LoadBuffer::LoadIndex(int index, ColliderBuffer* mMesh)
{
	const int IndexCount = EATER_GET_LIST_CHOICE(index, "Index") *3;
	mMesh->IndexArrayCount = IndexCount;
	mMesh->IndexArray = new UINT[IndexCount];
	int Count = 0;
	for (int i = 0; i < IndexCount; i += 3)
	{
		std::vector<float> Data;
		EATER_GET_LIST(&Data, Count);
		mMesh->IndexArray[i]			= (UINT)Data[0];
		mMesh->IndexArray[i+1]		= (UINT)Data[1];
		mMesh->IndexArray[i+2]		= (UINT)Data[2];
		Count++;
	}
}

Mesh* Eater_LoadBuffer::CreateBuffer(ParserData::CMesh* mesh)
{
	//버퍼를 만든다
	EnterCriticalSection(m_CriticalSection);
	Mesh* meshBuffer = new Mesh();
	m_Graphic->CreateMeshBuffer(mesh, &meshBuffer->m_MeshData);
	LeaveCriticalSection(m_CriticalSection);
	return meshBuffer;
}
