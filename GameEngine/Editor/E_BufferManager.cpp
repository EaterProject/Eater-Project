#include "E_BufferManager.h"
#include "ParserData.h"
#include "EaterHeader.h"

E_BufferManager::E_BufferManager()
{
}

E_BufferManager::~E_BufferManager()
{
}

void E_BufferManager::ChangeEaterFile(ParserData::CModel* FBXMesh)
{
	MeshIndexList.clear();
	int Size = FBXMesh->m_MeshList.size();
	for (int i = 0; i < Size; i++)
	{
		ParserData::CMesh* mMesh = FBXMesh->m_MeshList[i];

		if (FindInstanceIndex(mMesh->m_MeshIndex) == true) { continue; }

		std::string FileName = SaveFileName + "_" + std::to_string(mMesh->m_MeshIndex);
		EATER_CREATE_FILE(FileName, "../Assets/Model/MeshBuffer/", ".Emesh");

		EATER_SET_NODE("BUFFER");

		SetVertexBuffer(mMesh);
		SetIndexBuffer(mMesh);

		EATER_CLOSE_FILE();
	}
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

bool E_BufferManager::FindInstanceIndex(int Index)
{
	bool Instance = false;
	std::vector<int>::iterator Start_it = MeshIndexList.begin();
	std::vector<int>::iterator End_it = MeshIndexList.end();
	for (Start_it; Start_it != End_it; Start_it++)
	{
		//���� �ε����� ã�Ҵٸ� �Ž��� �������� �ʾƵ��ȴ�
		if ((*Start_it) == Index)
		{
			return true;
		}
	}

	//���� �ε����� ã�� ���ߴٸ� �ε����� ����Ʈ�� �־��ְ� �Ž������� ����
	MeshIndexList.push_back(Index);
	return false;
}
