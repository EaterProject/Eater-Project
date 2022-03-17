#include "FBXManager.h"
#include "ModelParser.h";
#include "ParserData.h"
#include "TypeOptionHeader.h"
#include "EaterHeader.h"
#include "EaterSaveData.h"
#include "MainHeader.h"
#include "EditorData.h"
FBXManager::FBXManager()
{
	FbxFactory = ModelParser::Create(FBX_MODEL);
	FbxFactory->Initialize();
}

FBXManager::~FBXManager()
{
	FbxFactory->Release();
}

void FBXManager::OpenFile(std::string& Path, MeshOption* Data)
{
	mOption = Data;
	std::string name = Data->Name;
	std::string SaveFileName = Path;
	
	int SaveMode = Data->SaveType;
	switch (SaveMode)
	{
	case 0: // 기본메쉬
	case 1:	// 스킨
	case 2:	// 터레인
		SaveFileName = SaveFileName.substr(0, SaveFileName.rfind('/')+1);
		break;
	case 3: //애니메이션
		mMesh = FbxFactory->LoadModel(Path, ANIMATION_ONLY);
		SaveFileName = "../Assets/Mesh/Animation/";
		EATER_CREATE_FILE(name, SaveFileName);
		AnimationMesh(mMesh);
		EATER_CLOSE_FILE();
		return;
	}
	MeshIndexList.clear();
	mMesh = FbxFactory->LoadModel(Path, 0);

	OneMeshMaterialList.clear();
	EATER_CREATE_FILE(name, SaveFileName);
	int MeshCount = (int)mMesh->m_MeshList.size();
	int Anime = mMesh->m_isAnimation;

	for (int i = 0; i < MeshCount; i++)
	{
		ParserData::Mesh* temp = mMesh->m_MeshList[i];
		switch (SaveMode)
		{
		case 0:
			StaticMesh(temp, name);
			break;
		case 1:
			SkinMesh(temp,name);
			break;
		case 2:
			TerrainMesh(temp);
			break;
		}
	}
	EATER_CLOSE_FILE();



	MaterialSave(name);
}

void FBXManager::StaticMesh(ParserData::Mesh* mMesh, std::string FileName)
{
	EATER_SET_NODE("STATIC");

	if (FindInstanceIndex(mMesh->m_MeshIndex) == false)
	{
		SetParent(mMesh);
		SetMaterial(mMesh, FileName);
		SetMatrix(mMesh);
		SetVertex(mMesh);
		SetIndex(mMesh);
	}
	else
	{
		SetParent(mMesh);
		SetMatrix(mMesh);
	}
}

void FBXManager::BoneMesh(ParserData::Mesh* mMesh)
{
	EATER_SET_NODE("BONE");
	EATER_SET_MAP("BoneIndex", std::to_string(mMesh->m_BoneIndex));
	if (mMesh->m_TopNode == true)
	{
		EATER_SET_MAP("TopNode", "YES");
		SetBoneMatrix(mMesh,true);
	}
	else
	{
		EATER_SET_MAP("TopNode", "NO");
		SetBoneMatrix(mMesh, false);
	}
	SetParent(mMesh);
}

void FBXManager::SkinMesh(ParserData::Mesh* mMesh, std::string FileName)
{
	if (mMesh->m_MeshType == SKIN_MESH)
	{
		EATER_SET_NODE("SKIN");
		SetParent(mMesh);
		SetMatrix(mMesh);
		SetMaterial(mMesh, FileName);
		SetVertexSkin(mMesh);
		SetIndex(mMesh);
		SetBoneOffset(mMesh);
	}
	else
	{
		BoneMesh(mMesh);
	}
}

void FBXManager::TerrainMesh(ParserData::Mesh* mMesh)
{


}

void FBXManager::AnimationMesh(ParserData::Model* mMesh)
{
	EATER_SET_NODE("ANIMATION");
	float m_TicksPerFrame = mMesh->m_AnimationList[0]->m_TicksPerFrame;
	float m_TotalFrame	= mMesh->m_AnimationList[0]->m_TotalFrame;
	float m_StartFrame	= mMesh->m_AnimationList[0]->m_StartFrame;
	float m_EndFrame	= mMesh->m_AnimationList[0]->m_EndFrame;
	int AnimationCount = (int)mMesh->m_AnimationList.size();

	EATER_SET_MAP("TickFrame", std::to_string(m_TicksPerFrame));
	EATER_SET_MAP("TotalFrame", std::to_string(m_TotalFrame));
	EATER_SET_MAP("StartFrame", std::to_string(m_StartFrame));
	EATER_SET_MAP("EndFrame", std::to_string(m_EndFrame));
	EATER_SET_MAP("BoneCount", std::to_string(AnimationCount));

	for (int i = 0; i < AnimationCount; i++)
	{
		int FrameCount = (int)mMesh->m_AnimationList[i]->m_AniData.size();
		EATER_SET_LIST_START(std::to_string(i), FrameCount, 11);
		for (int j = 0; j < FrameCount; j++)
		{
			ParserData::OneFrame* Frame = mMesh->m_AnimationList[i]->m_AniData[j];

			EATER_SET_LIST(Frame->m_Pos.x);
			EATER_SET_LIST(Frame->m_Pos.y);
			EATER_SET_LIST(Frame->m_Pos.z);

			if (i == 0)
			{
				float Rot_X = mOption->Rotation.x;
				float Rot_Y = mOption->Rotation.y;
				float Rot_Z = mOption->Rotation.z;
				Quaternion OptionRot	= XMQuaternionRotationRollPitchYaw(Rot_X, Rot_Y, Rot_Z);
				Quaternion LocalRot		= Frame->m_RotQt;
				Quaternion Change		= LocalRot* OptionRot;
				EATER_SET_LIST(Change.x);
				EATER_SET_LIST(Change.y);
				EATER_SET_LIST(Change.z);
				EATER_SET_LIST(Change.w);
			}
			else
			{
				EATER_SET_LIST(Frame->m_RotQt.x);
				EATER_SET_LIST(Frame->m_RotQt.y);
				EATER_SET_LIST(Frame->m_RotQt.z);
				EATER_SET_LIST(Frame->m_RotQt.w);
			}

			EATER_SET_LIST(Frame->m_Scale.x);
			EATER_SET_LIST(Frame->m_Scale.y);
			EATER_SET_LIST(Frame->m_Scale.z);

			EATER_SET_LIST(Frame->m_Time, true);
		}
	}
}

void FBXManager::MaterialSave(std::string FileName)
{
	int Size = (int)OneMeshMaterialList.size();
	for (int i = 0; i < Size; i++)
	{
		std::string Name = OneMeshMaterialList[i].MeshName;
		EATER_CREATE_FILE(Name, "../Assets/Mesh/Material/",".EMAT");
		EATER_SET_NODE("EATERMAT");
		EATER_SET_MAP("Diffuse", OneMeshMaterialList[i].DiffuseMap);
		EATER_SET_MAP("Normal", OneMeshMaterialList[i].NormalMap);
		EATER_SET_MAP("Emissive", OneMeshMaterialList[i].EmissiveMap);
		EATER_SET_MAP("ORM", OneMeshMaterialList[i].ORMMap);
		if (OneMeshMaterialList[i].Alpha == true) 
		{
			EATER_SET_MAP("Alpha", "YES");
		}
		else
		{
			EATER_SET_MAP("Alpha", "NO");
		}
		EATER_SET_MAP("Roughness", std::to_string(OneMeshMaterialList[i].Roughness));
		EATER_SET_MAP("Metallic", std::to_string(OneMeshMaterialList[i].Metallic));

		EATER_SET_MAP("Tileing_X", std::to_string(OneMeshMaterialList[i].Tileing.x));
		EATER_SET_MAP("Tileing_Y", std::to_string(OneMeshMaterialList[i].Tileing.y));

		EATER_SET_MAP("BaseColor_R", std::to_string(OneMeshMaterialList[i].BaseColor.x));
		EATER_SET_MAP("BaseColor_G", std::to_string(OneMeshMaterialList[i].BaseColor.y));
		EATER_SET_MAP("BaseColor_B", std::to_string(OneMeshMaterialList[i].BaseColor.z));
		EATER_SET_MAP("BaseColor_A", std::to_string(OneMeshMaterialList[i].BaseColor.w));

		EATER_SET_MAP("AddColor_R", std::to_string(OneMeshMaterialList[i].BaseColor.x));
		EATER_SET_MAP("AddColor_G", std::to_string(OneMeshMaterialList[i].BaseColor.y));
		EATER_SET_MAP("AddColor_B", std::to_string(OneMeshMaterialList[i].BaseColor.z));
		EATER_SET_MAP("AddColor_A", std::to_string(OneMeshMaterialList[i].BaseColor.w));

		EATER_CLOSE_FILE();
	}
	OneMeshMaterialList.clear();
}

void FBXManager::SetParent(ParserData::Mesh* mMesh)
{
	EATER_SET_MAP("ParentName"	, mMesh->m_ParentName);
	EATER_SET_MAP("NodeName"	, mMesh->m_NodeName);
	EATER_SET_MAP("MeshIndex"	, std::to_string(mMesh->m_MeshIndex));
}

void FBXManager::SetMatrix(ParserData::Mesh* mMesh)
{
	EATER_SET_LIST_START("WorldTM", 4, 4);
	EATER_SET_LIST(mMesh->m_WorldTM._11);
	EATER_SET_LIST(mMesh->m_WorldTM._12);
	EATER_SET_LIST(mMesh->m_WorldTM._13);
	EATER_SET_LIST(mMesh->m_WorldTM._14,true);

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

	
	DirectX::SimpleMath::Matrix Local		= mMesh->m_LocalTM;
	DirectX::SimpleMath::Matrix Change		= mOption->GetMatrix();
	DirectX::SimpleMath::Matrix SaveLocal	=  Local * Change;

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

void FBXManager::SetBoneMatrix(ParserData::Mesh* mMesh,bool TopBone)
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

	DirectX::SimpleMath::Matrix SaveLocal = mMesh->m_LocalTM;
	if (TopBone == true)
	{
		DirectX::SimpleMath::Matrix Local = mMesh->m_LocalTM;
		DirectX::SimpleMath::Matrix Change = mOption->GetBoneMatrix();
		SaveLocal = Local * Change;
	}


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

void FBXManager::SetMaterial(ParserData::Mesh* mMesh, std::string FileName)
{
	if (mMesh->m_MaterialData == nullptr) { return; }
	
	EaterMaterialData Data;
	Data.MeshName = FileName + "_" + mMesh->m_MaterialData->m_MaterialName;
	EATER_SET_MAP("MaterialName", Data.MeshName);
	if (mMesh->m_MaterialData->m_Alpha == true)
	{
		Data.Alpha = "YES";
	}


	if (mMesh->m_MaterialData->m_DiffuseMap != nullptr)
	{
		Data.DiffuseMap = CutStr(mMesh->m_MaterialData->m_DiffuseMap->m_BitMap);
	}
	
	if (mMesh->m_MaterialData->m_NormalMap != nullptr) 
	{
		Data.NormalMap = CutStr(mMesh->m_MaterialData->m_NormalMap->m_BitMap);
	}
	

	if (mMesh->m_MaterialData->m_EmissiveMap != nullptr)
	{
		Data.EmissiveMap = CutStr(mMesh->m_MaterialData->m_EmissiveMap->m_BitMap);
	}
	
	if (mMesh->m_MaterialData->m_ORMMap != nullptr)
	{
		Data.ORMMap = CutStr(mMesh->m_MaterialData->m_ORMMap->m_BitMap);
	}


	OneMeshMaterialList.push_back(Data);
}

void FBXManager::SetVertexTerrain(ParserData::Mesh* mMesh)
{
	int VertexCount = (int)mMesh->m_OriginVertexList.size();
	EATER_SET_LIST_START("Vertex", VertexCount, 3);
	for (int i = 0; i < VertexCount; i++)
	{
		EATER_SET_LIST(mMesh->m_OriginVertexList[i].x);
		EATER_SET_LIST(mMesh->m_OriginVertexList[i].y);
		EATER_SET_LIST(mMesh->m_OriginVertexList[i].z,true);
	}


	int IndexCount = (int)mMesh->m_OriginIndexList.size();
	EATER_SET_LIST_START("Index", IndexCount, 1);
	for (int i = 0; i < IndexCount; i++)
	{
		EATER_SET_LIST((int)mMesh->m_OriginIndexList[i],true);
	}
}

void FBXManager::SetVertex(ParserData::Mesh* mMesh)
{
	int VertexCount = (int)mMesh->m_VertexList.size();
	

	EATER_SET_VERTEX_START(VertexCount,VERTEX_TYPE::BASE);
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

void FBXManager::SetVertexSkin(ParserData::Mesh* mMesh)
{
	int VertexCount		= (int)mMesh->m_VertexList.size();
	int BoneIndexCount	= (int)mMesh->m_VertexList[0]->m_BoneIndices.size();

	EATER_SET_VERTEX_START(VertexCount, VERTEX_TYPE::SKIN);
	for (int i = 0; i < VertexCount; i++)
	{
		ParserData::Vertex* V = mMesh->m_VertexList[i];
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

		int BoneIndexCount		= (int)mMesh->m_VertexList[i]->m_BoneIndices.size();
		for (int j = 0; j < 8; j++)
		{
			if (j < BoneIndexCount)
			{
				skin.BONE_INDEX[j]		= V->m_BoneIndices[j];
				skin.BONE_WEIGHTS[j]	= V->m_BoneWeights[j];
			}
			else
			{
				skin.BONE_INDEX[j]		= -1;
				skin.BONE_WEIGHTS[j]	= -1;
			}
		}

		EATER_SET_VERTEX(skin);
	}
}

void FBXManager::SetBoneOffset(ParserData::Mesh* mMesh)
{
	int index = (int)mMesh->m_BoneTMList.size();
	EATER_SET_LIST_START("BoneOffset", index ,16);
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
		EATER_SET_LIST(Data->_44,true);
	}
}

std::string FBXManager::CutStr(std::string& Path)
{
	size_t start = Path.rfind("/") + 1;
	size_t end = Path.length() - start;
	std::string FileType = Path.substr(start, end);
	return FileType;
}

bool FBXManager::FindInstanceIndex(int Index)
{
	bool Instance = false;
	std::vector<int>::iterator Start_it = MeshIndexList.begin();
	std::vector<int>::iterator End_it	= MeshIndexList.end();
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

void FBXManager::SetIndex(ParserData::Mesh* mMesh)
{
	int IndexCount = (int)mMesh->m_IndexList.size();
	EATER_SET_INDEX_START(IndexCount);

	for (int i = 0; i < IndexCount; i++)
	{
		ParserData::IndexList* index = mMesh->m_IndexList[i];
		EATER_SET_INDEX(index->m_Index[0], index->m_Index[1], index->m_Index[2]);
	}
}

