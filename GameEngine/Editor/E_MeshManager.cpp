#include "E_MeshManager.h"
#include "EaterHeader.h"
#include "ParserData.h"
E_MeshManager::E_MeshManager()
{

}

E_MeshManager::~E_MeshManager()
{

}

void E_MeshManager::ChangeEaterFile(ParserData::CModel* FBXMesh)
{
	
	int MeshSize = FBXMesh->m_MeshList.size();
	for (int i = 0; i < MeshSize; i++)
	{
		ParserData::CMesh* OneMesh = FBXMesh->m_MeshList[i];
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

void E_MeshManager::ChangeEaterFile_Static(ParserData::CMesh* OneMesh)
{
	EATER_SET_NODE("STATIC");
	SetDataName(OneMesh);
	SetMatrix(OneMesh);
}

void E_MeshManager::ChangeEaterFile_Skin(ParserData::CMesh* OneMesh)
{
	if (OneMesh->m_MeshType == BONE_MESH)
	{
		EATER_SET_NODE("BONE");

		EATER_SET_MAP("ParentName", OneMesh->m_ParentName);
		EATER_SET_MAP("NodeName", OneMesh->m_NodeName);
		EATER_SET_MAP("BoneIndex", std::to_string(OneMesh->m_BoneIndex));
		if (OneMesh->m_TopNode == true)
		{
			EATER_SET_MAP("TopNode", "YES");
		}
		else
		{
			EATER_SET_MAP("TopNode", "NO");
		}
		SetMatrix(OneMesh);
	}
	else if (OneMesh->m_MeshType == SKIN_MESH)
	{
		EATER_SET_NODE("SKIN");

		SetDataName(OneMesh);
		SetMatrix(OneMesh);
		SetBoneOffset(OneMesh);
	}
}

void E_MeshManager::SetDataName(ParserData::CMesh* mMesh)
{

	EATER_SET_MAP("ParentName", mMesh->m_ParentName);
	EATER_SET_MAP("NodeName", mMesh->m_NodeName);

	//모델 이름 
	std::string ModelName = SaveFileName + "_" + std::to_string(mMesh->m_MeshIndex);
	EATER_SET_MAP("MeshName", ModelName);

	//메테리얼 이름
	if (mMesh->m_MaterialData != nullptr)
	{
		std::string MatName = SaveFileName + "_" + mMesh->m_MaterialData->m_MaterialName;
		EATER_SET_MAP("MaterialName",MatName);
	}
	else
	{
		EATER_SET_MAP("MaterialName", "NO");
	}

}

void E_MeshManager::SetMatrix(ParserData::CMesh* mMesh)
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

void E_MeshManager::SetBoneOffset(ParserData::CMesh* mMesh)
{
	int index = (int)mMesh->m_BoneTMList.size();
	EATER_SET_LIST_START("BoneOffset", index, 16);
	for (int i = 0; i < index; i++)
	{
		DirectX::SimpleMath::Matrix* Data = &(mMesh->m_BoneTMList[i]);
		EATER_SET_LIST(Data->_11);
		EATER_SET_LIST(Data->_12);
		EATER_SET_LIST(Data->_13);
		EATER_SET_LIST(Data->_14);

		EATER_SET_LIST(Data->_21);
		EATER_SET_LIST(Data->_22);
		EATER_SET_LIST(Data->_23);
		EATER_SET_LIST(Data->_24);

		EATER_SET_LIST(Data->_31);
		EATER_SET_LIST(Data->_32);
		EATER_SET_LIST(Data->_33);
		EATER_SET_LIST(Data->_34);

		EATER_SET_LIST(Data->_41);
		EATER_SET_LIST(Data->_42);
		EATER_SET_LIST(Data->_43);
		EATER_SET_LIST(Data->_44, true);
	}
}

std::string E_MeshManager::CutFileName(std::string FilePath)
{
	size_t start = FilePath.rfind("/") + 1;
	size_t end = FilePath.length() - start;
	std::string FileType = FilePath.substr(start, end);
	return FileType;
}


