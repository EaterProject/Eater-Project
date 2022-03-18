#include "E_MeshManager.h"
#include "EaterHeader.h"
#include "ParserData.h"
E_MeshManager::E_MeshManager()
{

}

E_MeshManager::~E_MeshManager()
{

}

void E_MeshManager::ChangeEaterFile(ParserData::Model* FBXMesh)
{
	
	int MeshSize = FBXMesh->m_MeshList.size();
	for (int i = 0; i < MeshSize; i++)
	{
		ParserData::Mesh* OneMesh = FBXMesh->m_MeshList[i];
		switch (OneMesh->m_MeshType)
		{
		case MESH_TYPE::BONE_MESH:
		case MESH_TYPE::SKIN_MESH:
			ChangeEaterFile_Skin(OneMesh);
			break;
		case MESH_TYPE::STATIC_MESH:
			ChangeEaterFile_Static(OneMesh);
			break;
		case MESH_TYPE::NULL_MESH:
			break;
		case MESH_TYPE::TERRAIN_MESH:
			break;
		}
	}
}

void E_MeshManager::SetFileName(std::string& FileName)
{
	SaveFileName = FileName;
}

void E_MeshManager::ChangeEaterFile_Static(ParserData::Mesh* OneMesh)
{
	EATER_SET_NODE("STATIC");
	SetDataName(OneMesh);
	SetMatrix(OneMesh);
}

void E_MeshManager::ChangeEaterFile_Bone(ParserData::Mesh* OneMesh)
{





}

void E_MeshManager::ChangeEaterFile_Skin(ParserData::Mesh* OneMesh)
{
	if (OneMesh->m_MeshType == BONE_MESH)
	{


	}
	else if (OneMesh->m_MeshType == SKIN_MESH)
	{


	}
}

void E_MeshManager::SetDataName(ParserData::Mesh* mMesh)
{

	EATER_SET_MAP("ParentName", mMesh->m_ParentName);
	EATER_SET_MAP("NodeName", mMesh->m_NodeName);

	//모델 이름 
	std::string ModelName = SaveFileName + "_" + std::to_string(mMesh->m_MeshIndex);
	EATER_SET_MAP("MeshName", ModelName);

	//메테리얼 이름
	std::string MatName = SaveFileName + "_" + mMesh->m_MaterialData->m_MaterialName;
	EATER_SET_MAP("MaterialName",MatName);
}

void E_MeshManager::SetMatrix(ParserData::Mesh* mMesh)
{
	EATER_SET_LIST_START("WorldTM", 4, 4);
	EATER_SET_LIST(mMesh->m_WorldTM._11);
	EATER_SET_LIST(mMesh->m_WorldTM._12);
	EATER_SET_LIST(mMesh->m_WorldTM._13);
	EATER_SET_LIST(mMesh->m_WorldTM._14, true);

	EATER_SET_LIST(mMesh->m_WorldTM._21);
	EATER_SET_LIST(mMesh->m_WorldTM._22);
	EATER_SET_LIST(mMesh->m_WorldTM._23);
	EATER_SET_LIST(mMesh->m_WorldTM._24, true);

	EATER_SET_LIST(mMesh->m_WorldTM._31);
	EATER_SET_LIST(mMesh->m_WorldTM._32);
	EATER_SET_LIST(mMesh->m_WorldTM._33);
	EATER_SET_LIST(mMesh->m_WorldTM._34, true);

	EATER_SET_LIST(mMesh->m_WorldTM._41);
	EATER_SET_LIST(mMesh->m_WorldTM._42);
	EATER_SET_LIST(mMesh->m_WorldTM._43);
	EATER_SET_LIST(mMesh->m_WorldTM._44, true);

	DirectX::SimpleMath::Matrix SaveLocal = mMesh->m_LocalTM;;
	EATER_SET_LIST_START("LocalTM", 4, 4);
	EATER_SET_LIST(SaveLocal._11);
	EATER_SET_LIST(SaveLocal._12);
	EATER_SET_LIST(SaveLocal._13);
	EATER_SET_LIST(SaveLocal._14, true);

	EATER_SET_LIST(SaveLocal._21);
	EATER_SET_LIST(SaveLocal._22);
	EATER_SET_LIST(SaveLocal._23);
	EATER_SET_LIST(SaveLocal._24, true);

	EATER_SET_LIST(SaveLocal._31);
	EATER_SET_LIST(SaveLocal._32);
	EATER_SET_LIST(SaveLocal._33);
	EATER_SET_LIST(SaveLocal._34, true);

	EATER_SET_LIST(SaveLocal._41);
	EATER_SET_LIST(SaveLocal._42);
	EATER_SET_LIST(SaveLocal._43);
	EATER_SET_LIST(SaveLocal._44, true);
}

void E_MeshManager::SetIndexBuffer(ParserData::Mesh* mMesh)
{
	//인덱스를 저장

	int IndexCount = (int)mMesh->m_IndexList.size();
	EATER_SET_INDEX_START(IndexCount);
	for (int i = 0; i < IndexCount; i++)
	{
		ParserData::IndexList* index = mMesh->m_IndexList[i];
		EATER_SET_INDEX(index->m_Index[0], index->m_Index[1], index->m_Index[2]);
	}
}

void E_MeshManager::SetVertexBuffer(ParserData::Mesh* mMesh)
{
	//버텍스들을 저장

	int VertexCount = (int)mMesh->m_VertexList.size();
	EATER_SET_VERTEX_START(VertexCount, VERTEX_TYPE::BASE);
	for (int i = 0; i < VertexCount; i++)
	{
		ParserData::Vertex* V = mMesh->m_VertexList[i];
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

bool E_MeshManager::FindInstanceIndex(int Index)
{
	bool Instance = false;

	int size =  (int)MeshIndexList.size();
	for (int i = 0; i < size;i++)
	{
		if (MeshIndexList[i] == Index)
		{
			return true;
		}
	}

	//같은 인덱스를 찾지 못했다면 인덱스를 리스트에 넣어주고 매쉬정보를 저장
	MeshIndexList.push_back(Index);
	return false;
}

std::string E_MeshManager::CutFileName(std::string FilePath)
{
	size_t start = FilePath.rfind("/") + 1;
	size_t end = FilePath.length() - start;
	std::string FileType = FilePath.substr(start, end);
	return FileType;
}


