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
			EATER_OPEN_WRITE_FILE(FileName, "../Assets/Model/MeshBuffer/", ".Emesh");	//���� ����
			EATER_SET_NODE("SKIN_BUFFER");												//��� ����
			SetSkinVertexBuffer(mMesh);													//���ؽ��� ����
			SetIndexBuffer(mMesh);														//�ε����� ����
			EATER_CLOSE_WRITE_FILE();													//���� �ݱ�
			break;
		case MESH_TYPE::STATIC_MESH:
			EATER_OPEN_WRITE_FILE(FileName, "../Assets/Model/MeshBuffer/", ".Emesh");	//���� ����
			EATER_SET_NODE("STATIC_BUFFER");											//��� ����
			SetVertexBuffer(mMesh);														//���ؽ��� ����
			SetIndexBuffer(mMesh);														//�ε����� ����
			EATER_CLOSE_WRITE_FILE();													//���� �ݱ�
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

void E_BufferManager::SetVertexBuffer
(
	float PosX, float PosY, float PosZ,
	float UV_X, float UV_Y,
	float Nomal_X, float Nomal_Y, float Nomal_Z,
	float Tanget_X, float Tanget_Y, float Tanget_Z
)
{
	EATER_SET_LIST(PosX);
	EATER_SET_LIST(PosY);
	EATER_SET_LIST(PosZ);
	EATER_SET_LIST(UV_X);
	EATER_SET_LIST(UV_Y);
	EATER_SET_LIST(Nomal_X);
	EATER_SET_LIST(Nomal_Y);
	EATER_SET_LIST(Nomal_Z);
	EATER_SET_LIST(Tanget_X);
	EATER_SET_LIST(Tanget_Y);
	EATER_SET_LIST(Tanget_Z, true);
}


