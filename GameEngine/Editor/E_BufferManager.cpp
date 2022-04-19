#include "E_BufferManager.h"
#include "ParserData.h"
#include "EaterHeader.h"
#include "EngineData.h"
#include "EditorToolScene.h"

E_BufferManager::E_BufferManager()
{
}

E_BufferManager::~E_BufferManager()
{
	
}

void E_BufferManager::ChangeEaterFile(ParserData::CModel* FBXMesh)
{
	MeshIndexList.clear();
	
	int Size = (int)FBXMesh->m_MeshList.size();
	for (int i = 0; i < Size; i++)
	{
		ParserData::CMesh* mMesh = FBXMesh->m_MeshList[i];

		if (mMesh->m_MeshType == MESH_TYPE::BONE_MESH) { continue; }
		if (FindInstanceIndex(mMesh->m_MeshIndex) == true) { continue; }

		
		std::string FileName = SaveFileName + "_" + std::to_string(mMesh->m_MeshIndex);
		switch (mMesh->m_MeshType)
		{
		case MESH_TYPE::SKIN_MESH:
			EATER_OPEN_WRITE_FILE(FileName, "../Assets/Model/MeshBuffer/", ".Emesh");	//파일 생성
			EATER_SET_NODE("SKIN_BUFFER");												//노드 생성
			SetSkinVertexBuffer(mMesh);													//버텍스값 쓰기
			SetIndexBuffer(mMesh);														//인덱스값 쓰기
			EATER_CLOSE_WRITE_FILE();													//파일 닫기
			break;
		case MESH_TYPE::STATIC_MESH:
			EATER_OPEN_WRITE_FILE(FileName, "../Assets/Model/MeshBuffer/", ".Emesh");	//파일 생성
			EATER_SET_NODE("STATIC_BUFFER");											//노드 생성
			SetVertexBuffer(mMesh);														//버텍스값 쓰기
			SetIndexBuffer(mMesh);														//인덱스값 쓰기
			EATER_CLOSE_WRITE_FILE();													//파일 닫기
			break;
		}
		
		std::string LoadName = "../Assets/Model/MeshBuffer/" + FileName + ".Emesh";
		EditorToolScene::MeshLoad(LoadName);
	}
}

void E_BufferManager::ChangeEaterFile_Pos(ParserData::CModel* FBXMesh, std::string NewFileName)
{
	int Size = (int)FBXMesh->m_MeshList.size();
	for (int i = 0; i < Size; i++)
	{
		ParserData::CMesh* mMesh = FBXMesh->m_MeshList[i];
		std::string FileName = NewFileName;
		EATER_OPEN_WRITE_FILE(FileName, "../Assets/Model/MeshBuffer/", ".Emesh");
		EATER_SET_NODE("POSITION_BUFFER");
		SetPosVertexBuffer(mMesh);
		SetIndexBuffer(mMesh);

		EditorToolScene::MeshLoad(FileName);
	}
	EATER_CLOSE_WRITE_FILE();
}

void E_BufferManager::ChangeEaterFile_NavMEsh(ParserData::CModel* FBXMesh, std::string NewFileName)
{
	int Size = (int)FBXMesh->m_MeshList.size();
	for (int i = 0; i < Size; i++)
	{
		ParserData::CMesh* mMesh = FBXMesh->m_MeshList[i];
		std::string FileName = NewFileName;
		EATER_OPEN_WRITE_FILE(FileName, "../Assets/Model/MeshBuffer/", ".Nav");
		EATER_SET_NODE("NAVIGATION");
		SetNavigationBuffer(mMesh);
		//Demo::MeshLoad(FileName);
	}
	EATER_CLOSE_WRITE_FILE();
}

void E_BufferManager::SetFileName(std::string& FileName)
{
	SaveFileName = FileName;
}

void E_BufferManager::SetVertexBuffer(ParserData::CMesh* mMesh)
{
	int VertexCount = (int)mMesh->m_VertexList.size();
	EATER_SET_VERTEX_START(VertexCount, VERTEX_TYPE::BASE);
	for (int i = 0; i < VertexCount; i++)
	{
		ParserData::CVertex* V = mMesh->m_VertexList[i];
		EATER_VERTEX_BASE base;

		base.POS_X = V->m_Pos.x;
		base.POS_Y = V->m_Pos.y;
		base.POS_Z = V->m_Pos.z;

		base.UV_X = V->m_UV.x;
		base.UV_Y = V->m_UV.y;

		base.NOMAL_X = V->m_Normal.x;
		base.NOMAL_Y = V->m_Normal.y;
		base.NOMAL_Z = V->m_Normal.z;

		base.TANGENT_X = V->m_Tanget.x;
		base.TANGENT_Y = V->m_Tanget.y;
		base.TANGENT_Z = V->m_Tanget.z;

		EATER_SET_VERTEX(base);
	}
}

void E_BufferManager::SetPosVertexBuffer(ParserData::CMesh* mMesh)
{
	int VertexCount = (int)mMesh->m_VertexList.size();
	EATER_SET_VERTEX_START(VertexCount, VERTEX_TYPE::POS);

	for (int i = 0; i < VertexCount; i++)
	{
		ParserData::CVertex* V = mMesh->m_VertexList[i];
		EATER_VERTEX_POS Pos;
		Pos.POS_X = V->m_Pos.x;
		Pos.POS_Y = V->m_Pos.y;
		Pos.POS_Z = V->m_Pos.z;
		EATER_SET_VERTEX(Pos);
	}
}

void E_BufferManager::SetIndexBuffer(ParserData::CMesh* mMesh)
{
	int IndexCount = (int)mMesh->m_IndexList.size();
	EATER_SET_INDEX_START(IndexCount);
	for (int i = 0; i < IndexCount; i++)
	{
		ParserData::CIndexList* index = mMesh->m_IndexList[i];
		EATER_SET_INDEX(index->m_Index[0], index->m_Index[1], index->m_Index[2]);
	}
}

void E_BufferManager::SetNavigationBuffer(ParserData::CMesh* mMesh)
{
	int VertexCount = (int)mMesh->m_VertexList.size();
	int IndexCount	= (int)mMesh->m_IndexList.size();
	Data.clear();
	Data.resize(IndexCount);
	for (int i = 0; i < IndexCount; i++)
	{
		OneTriangle* OneData = new OneTriangle();
		//한개의 페이스를 가져온다
		int Point01Index  = mMesh->m_IndexList[i]->m_Index[0];
		int Point02Index  = mMesh->m_IndexList[i]->m_Index[1];
		int Point03Index  = mMesh->m_IndexList[i]->m_Index[2];

		//페이스에 해당하는 버텍스 3개를 가져온다
		OneData->VertexPos[0] = mMesh->m_VertexList[Point01Index]->m_Pos;
		OneData->VertexPos[1] = mMesh->m_VertexList[Point02Index]->m_Pos;
		OneData->VertexPos[2] = mMesh->m_VertexList[Point03Index]->m_Pos;

		//중심점위치를 구한다
		OneData->CreateCenterPoint();
		//인덱스 값 넣어주기
		OneData->Index = i;

		for (int j = 0; j < IndexCount; j++)
		{
			//같은 인덱스는 비교하지않는다
			if (i == j) { continue; }

			int Count = 0;
			for (int k = 0; k < 3; k++)
			{
				if (Point01Index == mMesh->m_IndexList[j]->m_Index[k])
				{
					Count++;
				}
			}

			for (int k = 0; k < 3; k++)
			{
				if (Point02Index == mMesh->m_IndexList[j]->m_Index[k])
				{
					Count++;
				}
			}

			for (int k = 0; k < 3; k++)
			{
				if (Point03Index == mMesh->m_IndexList[j]->m_Index[k])
				{
					Count++;
				}
			}

			//만약 같은인덱스가 2개이상이라면 인접한 삼각형이라 생각한다
			if(Count >= 2)
			{
				for (int m = 0; m < 3; m++)
				{
					if (OneData->FriendFace[m] == -1)
					{
						OneData->FriendFace[m] = j;
						break;
					}
				}
			}
		}
		Data[OneData->Index] = OneData;
	}
	
	//모두 구했으니 값을 저장한다
	int Count = (int)Data.size();
	EATER_SET_LIST_START("TRIANGLE", Count, 16);
	for (int i = 0; i < Count; i++)
	{
		EATER_SET_LIST((int)Data[i]->Index);
		for (int j = 0; j < 3; j++)
		{
			EATER_SET_LIST(Data[i]->VertexPos[j].x);
			EATER_SET_LIST(Data[i]->VertexPos[j].y);
			EATER_SET_LIST(Data[i]->VertexPos[j].z);
		}
		EATER_SET_LIST(Data[i]->CenterPoint.x);
		EATER_SET_LIST(Data[i]->CenterPoint.y);
		EATER_SET_LIST(Data[i]->CenterPoint.z);

		EATER_SET_LIST(Data[i]->FriendFace[0]);
		EATER_SET_LIST(Data[i]->FriendFace[1]);
		EATER_SET_LIST(Data[i]->FriendFace[2],true);
	}


}

void E_BufferManager::SetSkinVertexBuffer(ParserData::CMesh* mMesh)
{
	int VertexCount = (int)mMesh->m_VertexList.size();
	EATER_SET_VERTEX_START(VertexCount, VERTEX_TYPE::SKIN);
	for (int i = 0; i < VertexCount; i++)
	{
		ParserData::CVertex* V = mMesh->m_VertexList[i];
		EATER_VERTEX_SKIN skin;

		skin.POS_X = V->m_Pos.x;
		skin.POS_Y = V->m_Pos.y;
		skin.POS_Z = V->m_Pos.z;

		skin.UV_X = V->m_UV.x;
		skin.UV_Y = V->m_UV.y;

		skin.NOMAL_X = V->m_Normal.x;
		skin.NOMAL_Y = V->m_Normal.y;
		skin.NOMAL_Z = V->m_Normal.z;

		skin.TANGENT_X = V->m_Tanget.x;
		skin.TANGENT_Y = V->m_Tanget.y;
		skin.TANGENT_Z = V->m_Tanget.z;

		int BoneIndexCount = (int)mMesh->m_VertexList[i]->m_BoneIndices.size();
		for (int j = 0; j < 8; j++)
		{
			if (j < BoneIndexCount)
			{
				skin.BONE_INDEX[j] = V->m_BoneIndices[j];
				skin.BONE_WEIGHTS[j] = V->m_BoneWeights[j];
			}
			else
			{
				skin.BONE_INDEX[j] = -1;
				skin.BONE_WEIGHTS[j] = -1;
			}
		}

		EATER_SET_VERTEX(skin);
	}
}

bool E_BufferManager::FindInstanceIndex(int Index)
{
	bool Instance = false;
	std::vector<int>::iterator Start_it = MeshIndexList.begin();
	std::vector<int>::iterator End_it = MeshIndexList.end();
	for (Start_it; Start_it != End_it; Start_it++)
	{
		//같은 인덱스를 찾았다면 매쉬를 저장하지 않아도된다
		if ((*Start_it) == Index)
		{
			return true;
		}
	}

	//같은 인덱스를 찾지 못했다면 인덱스를 리스트에 넣어주고 매쉬정보를 저장
	MeshIndexList.push_back(Index);
	return false;
}
