#include "E_ChangeManager.h"
#include "SimpleMath.h"
#include "GameObject.h"
#include "EaterHeader.h"
#include "MeshFilter.h"
#include "Transform.h"
#include "Material.h"
#include "EngineData.h"

E_ChangeManager::E_ChangeManager()
{
}

E_ChangeManager::~E_ChangeManager()
{
}

void E_ChangeManager::SetModelName(std::string mModelName)
{
	ModelName = mModelName;
}

void E_ChangeManager::Change_Static(int index, GameObject* Object)
{
	std::string ParentName		= EATER_GET_MAP(index, "ParentName");
	std::string NodeName		= EATER_GET_MAP(index, "NodeName");

	//1. 매쉬필터에서 만든 최상위 오브젝트 일때
	//2. FBX에서 뽑은 최상위 오브젝트 일때
	//3. 최상위가 아닌 오브젝트 일때
	std::string ObjectOriginalName = GetOriginalName(Object->Name);
	if (ObjectOriginalName == ModelName && ParentName == "RootNode")
	{
		MeshFilter* MF	= Object->GetComponent<MeshFilter>();
		Transform* TR	= Object->GetComponent<Transform>();
		//이름값을 입력해준다
		Change_Name(index, MF);
		//현재 월드를 기본 원본에 곱해서 로컬값으로 만들어준다
		Change_LocalTM(index, TR);
	}
	else if(NodeName == Object->Name)
	{
		MeshFilter* MF = Object->GetComponent<MeshFilter>();
		Transform* TR = Object->GetComponent<Transform>();
		//이름값을 입력해준다
		Change_Name(index, MF);
		//현재 월드를 기본 원본에 곱해서 로컬값으로 만들어준다
		Change_LocalTM(index, TR);
	}
}

void E_ChangeManager::Change_Skin(int index, GameObject* Object)
{
	std::string ParentName = EATER_GET_MAP(index, "ParentName");
	std::string NodeName = EATER_GET_MAP(index, "NodeName");
	std::string ObjectOriginalName = GetOriginalName(Object->Name);
	if (ParentName == "RootNode" && ObjectOriginalName == NodeName)
	{
		MeshFilter* MF = Object->GetComponent<MeshFilter>();
		Transform* TR = Object->GetComponent<Transform>();
		//이름값을 입력해준다
		Change_Name(index, MF);
		//현재 월드를 기본 원본에 곱해서 로컬값으로 만들어준다
		Change_LocalTM(index, TR);
		//Change_WorldTM(index, TR);
	}
}

void E_ChangeManager::Change_Bone(int index, GameObject* Object)
{
	std::string ParentName = EATER_GET_MAP(index, "ParentName");
	std::string NodeName = EATER_GET_MAP(index, "NodeName");

	std::string ObjectOriginalName = GetOriginalName(Object->Name);
	if (ObjectOriginalName == ModelName && ParentName == "RootNode")
	{
		MeshFilter* MF = Object->GetComponent<MeshFilter>();
		Transform* TR = Object->GetComponent<Transform>();
		//현재 월드를 기본 원본에 곱해서 로컬값으로 만들어준다
		Change_LocalTM(index, TR);
	}
}

void E_ChangeManager::Change_Material(int index, GameObject* Object)
{
	//변경할 값
	MeshFilter* MF = Object->GetComponent<MeshFilter>();
	Material* mMaterialData = MF->GetMaterial();

	if (mMaterialData != nullptr)
	{
		//원본값
		std::string mMaterial	= MF->GetMaterialName();
		std::string mDiffuse	= mMaterialData->GetDiffuseName();
		std::string mNormal		= mMaterialData->GetNormalName();
		std::string mEmissive	= mMaterialData->GetEmissiveName();
		std::string mORM		= mMaterialData->GetORMName();


		std::string Tileing_X = EATER_GET_MAP(index, "Tileing_X");
		std::string Tileing_Y = EATER_GET_MAP(index, "Tileing_Y");


		float mMetallicF = mMaterialData->m_MaterialData->Material_Property->MetallicFactor;
		float mEmissiveF = mMaterialData->m_MaterialData->Material_Property->EmissiveFactor;
		float mRoughnessF = mMaterialData->m_MaterialData->Material_Property->RoughnessFactor;
		int	  mAlpha	 =	mMaterialData->m_MaterialData->Material_Property->Alpha;

		float AddColor_R = mMaterialData->m_MaterialData->Material_Property->AddColor.x;
		float AddColor_G = mMaterialData->m_MaterialData->Material_Property->AddColor.y;
		float AddColor_B = mMaterialData->m_MaterialData->Material_Property->AddColor.z;

		float LimColor_R = mMaterialData->m_MaterialData->Material_Property->LimLightColor.x;
		float LimColor_G = mMaterialData->m_MaterialData->Material_Property->LimLightColor.y;
		float LimColor_B = mMaterialData->m_MaterialData->Material_Property->LimLightColor.z;
		float LimFactor = mMaterialData->m_MaterialData->Material_Property->LimLightFactor;
		float LimWidth = mMaterialData->m_MaterialData->Material_Property->LimLightWidth;
		float SkyIndex = mMaterialData->m_MaterialData->Material_Property->SkyLightIndex;
		//타일링 빼줘야함 ....

		EATER_CHANGE_MAP(index, "MaterialName", mMaterial);
		EATER_CHANGE_MAP(index, "Alpha", std::to_string(mAlpha));
		EATER_CHANGE_MAP(index, "DiffuseMap", mDiffuse);
		EATER_CHANGE_MAP(index, "NormalMap", mNormal);
		EATER_CHANGE_MAP(index, "EmissiveMap", mEmissive);
		EATER_CHANGE_MAP(index, "ORMMap", mORM);

		EATER_CHANGE_MAP(index, "Emissive", std::to_string(mEmissiveF));
		EATER_CHANGE_MAP(index, "Roughness", std::to_string(mRoughnessF));
		EATER_CHANGE_MAP(index, "Metallic", std::to_string(mMetallicF));

		EATER_CHANGE_MAP(index, "Tileing_X", "1");
		EATER_CHANGE_MAP(index, "Tileing_Y", "1");

		EATER_CHANGE_MAP(index, "SkyLightIndex", std::to_string(SkyIndex));

		EATER_CHANGE_MAP(index, "AddColor_R", std::to_string(AddColor_R));
		EATER_CHANGE_MAP(index, "AddColor_G", std::to_string(AddColor_G));
		EATER_CHANGE_MAP(index, "AddColor_B", std::to_string(AddColor_B));


		EATER_CHANGE_MAP(index, "LimColor_R", std::to_string(LimColor_R));
		EATER_CHANGE_MAP(index, "LimColor_G", std::to_string(LimColor_G));
		EATER_CHANGE_MAP(index, "LimColor_B", std::to_string(LimColor_B));

		EATER_CHANGE_MAP(index, "LimFactor", std::to_string(LimFactor));
		EATER_CHANGE_MAP(index, "LimWidth", std::to_string(LimWidth));
		
		EATER_CLOSE_CHANGE_FILE(MF->GetMaterialName(), "../Assets/Texture/Material/", ".Emat");
	}
	else
	{
		EATER_CLOSE_READ_FILE();
	}
	
}

void E_ChangeManager::Change_Animation(int index, GameObject* Object)
{


}

void E_ChangeManager::Change_LocalTM(int Nodeindex, Transform* TR)
{
	DirectX::SimpleMath::Matrix Position_4x4;
	Vector3 position = TR->GetPosition();
	Position_4x4._11 = 1;				Position_4x4._12 = 0;			Position_4x4._13 = 0;			Position_4x4._14 = 0;
	Position_4x4._21 = 0;				Position_4x4._22 = 1;			Position_4x4._23 = 0;			Position_4x4._24 = 0;
	Position_4x4._31 = 0;				Position_4x4._32 = 0;			Position_4x4._33 = 1;			Position_4x4._34 = 0;
	Position_4x4._41 = position.x;		Position_4x4._42 = position.y;	Position_4x4._43 = position.z;	Position_4x4._44 = 1;

	Vector3 rotation = TR->GetRotation();
	float radX = rotation.x * 3.141592f / 180;
	float radY = rotation.y * 3.141592f / 180;
	float radZ = rotation.z * 3.141592f / 180;
	DirectX::XMMATRIX _P = DirectX::XMMatrixRotationX(radX);
	DirectX::XMMATRIX _Y = DirectX::XMMatrixRotationY(radY);
	DirectX::XMMATRIX _R = DirectX::XMMatrixRotationZ(radZ);
	DirectX::XMMATRIX Rotation_4x4 = _R * _Y * _P;

	DirectX::SimpleMath::Matrix Scale_4x4;
	Vector3 scale = TR->GetScale();
	Scale_4x4._11 = scale.x;	Scale_4x4._12 = 0;			Scale_4x4._13 = 0;			Scale_4x4._14 = 0;
	Scale_4x4._21 = 0;			Scale_4x4._22 = scale.y;	Scale_4x4._23 = 0;			Scale_4x4._24 = 0;
	Scale_4x4._31 = 0;			Scale_4x4._32 = 0;			Scale_4x4._33 = scale.z;	Scale_4x4._34 = 0;
	Scale_4x4._41 = 0;			Scale_4x4._42 = 0;			Scale_4x4._43 = 0;			Scale_4x4._44 = 1;

	Matrix ChangeTM = Scale_4x4 * Rotation_4x4 * Position_4x4;


	SimpleMath::Matrix Local_XM;
	int Count = EATER_GET_LIST_CHOICE(Nodeindex, "LocalTM");
	for (int k = 0; k < Count; k++)
	{
		std::vector<float> Data;
		EATER_GET_LIST(&Data, k);
		switch (k)
		{
		case 0:
			Local_XM._11 = Data[0];
			Local_XM._12 = Data[1];
			Local_XM._13 = Data[2];
			Local_XM._14 = Data[3];
			break;
		case 1:
			Local_XM._21 = Data[0];
			Local_XM._22 = Data[1];
			Local_XM._23 = Data[2];
			Local_XM._24 = Data[3];
			break;
		case 2:
			Local_XM._31 = Data[0];
			Local_XM._32 = Data[1];
			Local_XM._33 = Data[2];
			Local_XM._34 = Data[3];
			break;
		case 3:
			Local_XM._41 = Data[0];
			Local_XM._42 = Data[1];
			Local_XM._43 = Data[2];
			Local_XM._44 = Data[3];
			break;
		}
	}

	Local_XM *= ChangeTM;
	EATER_CHANGE_LIST(Nodeindex, "LocalTM", 0, 0, std::to_string(Local_XM._11));
	EATER_CHANGE_LIST(Nodeindex, "LocalTM", 1, 0, std::to_string(Local_XM._12));
	EATER_CHANGE_LIST(Nodeindex, "LocalTM", 2, 0, std::to_string(Local_XM._13));
	EATER_CHANGE_LIST(Nodeindex, "LocalTM", 3, 0, std::to_string(Local_XM._14));

	EATER_CHANGE_LIST(Nodeindex, "LocalTM", 0, 1, std::to_string(Local_XM._21));
	EATER_CHANGE_LIST(Nodeindex, "LocalTM", 1, 1, std::to_string(Local_XM._22));
	EATER_CHANGE_LIST(Nodeindex, "LocalTM", 2, 1, std::to_string(Local_XM._23));
	EATER_CHANGE_LIST(Nodeindex, "LocalTM", 3, 1, std::to_string(Local_XM._24));

	EATER_CHANGE_LIST(Nodeindex, "LocalTM", 0, 2, std::to_string(Local_XM._31));
	EATER_CHANGE_LIST(Nodeindex, "LocalTM", 1, 2, std::to_string(Local_XM._32));
	EATER_CHANGE_LIST(Nodeindex, "LocalTM", 2, 2, std::to_string(Local_XM._33));
	EATER_CHANGE_LIST(Nodeindex, "LocalTM", 3, 2, std::to_string(Local_XM._34));

	EATER_CHANGE_LIST(Nodeindex, "LocalTM", 0, 3, std::to_string(Local_XM._41));
	EATER_CHANGE_LIST(Nodeindex, "LocalTM", 1, 3, std::to_string(Local_XM._42));
	EATER_CHANGE_LIST(Nodeindex, "LocalTM", 2, 3, std::to_string(Local_XM._43));
	EATER_CHANGE_LIST(Nodeindex, "LocalTM", 3, 3, std::to_string(Local_XM._44));
}

void E_ChangeManager::Change_WorldTM(int Nodeindex, Transform* TR)
{
	DirectX::SimpleMath::Matrix Position_4x4;
	Vector3 position = TR->GetPosition();
	Position_4x4._11 = 1;				Position_4x4._12 = 0;			Position_4x4._13 = 0;			Position_4x4._14 = 0;
	Position_4x4._21 = 0;				Position_4x4._22 = 1;			Position_4x4._23 = 0;			Position_4x4._24 = 0;
	Position_4x4._31 = 0;				Position_4x4._32 = 0;			Position_4x4._33 = 1;			Position_4x4._34 = 0;
	Position_4x4._41 = position.x;		Position_4x4._42 = position.y;	Position_4x4._43 = position.z;	Position_4x4._44 = 1;

	Vector3 rotation = TR->GetRotation();
	float radX = rotation.x * 3.141592f / 180;
	float radY = rotation.y * 3.141592f / 180;
	float radZ = rotation.z * 3.141592f / 180;
	DirectX::XMMATRIX _P = DirectX::XMMatrixRotationX(radX);
	DirectX::XMMATRIX _Y = DirectX::XMMatrixRotationY(radY);
	DirectX::XMMATRIX _R = DirectX::XMMatrixRotationZ(radZ);
	DirectX::XMMATRIX Rotation_4x4 = _R * _Y * _P;

	DirectX::SimpleMath::Matrix Scale_4x4;
	Vector3 scale = TR->GetScale();
	Scale_4x4._11 = scale.x;	Scale_4x4._12 = 0;			Scale_4x4._13 = 0;			Scale_4x4._14 = 0;
	Scale_4x4._21 = 0;			Scale_4x4._22 = scale.y;	Scale_4x4._23 = 0;			Scale_4x4._24 = 0;
	Scale_4x4._31 = 0;			Scale_4x4._32 = 0;			Scale_4x4._33 = scale.z;	Scale_4x4._34 = 0;
	Scale_4x4._41 = 0;			Scale_4x4._42 = 0;			Scale_4x4._43 = 0;			Scale_4x4._44 = 1;

	Matrix ChangeTM = Scale_4x4 * Rotation_4x4 * Position_4x4;


	SimpleMath::Matrix World_XM;
	int Count = EATER_GET_LIST_CHOICE(Nodeindex, "WorldTM");
	for (int k = 0; k < Count; k++)
	{
		std::vector<float> Data;
		EATER_GET_LIST(&Data, k);
		switch (k)
		{
		case 0:
			World_XM._11 = Data[0];
			World_XM._12 = Data[1];
			World_XM._13 = Data[2];
			World_XM._14 = Data[3];
			break;
		case 1:
			World_XM._21 = Data[0];
			World_XM._22 = Data[1];
			World_XM._23 = Data[2];
			World_XM._24 = Data[3];
			break;
		case 2:
			World_XM._31 = Data[0];
			World_XM._32 = Data[1];
			World_XM._33 = Data[2];
			World_XM._34 = Data[3];
			break;
		case 3:
			World_XM._41 = Data[0];
			World_XM._42 = Data[1];
			World_XM._43 = Data[2];
			World_XM._44 = Data[3];
			break;
		}
	}

	World_XM *= ChangeTM;
	EATER_CHANGE_LIST(Nodeindex, "WorldTM", 0, 0, std::to_string(World_XM._11));
	EATER_CHANGE_LIST(Nodeindex, "WorldTM", 1, 0, std::to_string(World_XM._12));
	EATER_CHANGE_LIST(Nodeindex, "WorldTM", 2, 0, std::to_string(World_XM._13));
	EATER_CHANGE_LIST(Nodeindex, "WorldTM", 3, 0, std::to_string(World_XM._14));

	EATER_CHANGE_LIST(Nodeindex, "WorldTM", 0, 1, std::to_string(World_XM._21));
	EATER_CHANGE_LIST(Nodeindex, "WorldTM", 1, 1, std::to_string(World_XM._22));
	EATER_CHANGE_LIST(Nodeindex, "WorldTM", 2, 1, std::to_string(World_XM._23));
	EATER_CHANGE_LIST(Nodeindex, "WorldTM", 3, 1, std::to_string(World_XM._24));

	EATER_CHANGE_LIST(Nodeindex, "WorldTM", 0, 2, std::to_string(World_XM._31));
	EATER_CHANGE_LIST(Nodeindex, "WorldTM", 1, 2, std::to_string(World_XM._32));
	EATER_CHANGE_LIST(Nodeindex, "WorldTM", 2, 2, std::to_string(World_XM._33));
	EATER_CHANGE_LIST(Nodeindex, "WorldTM", 3, 2, std::to_string(World_XM._34));

	EATER_CHANGE_LIST(Nodeindex, "WorldTM", 0, 3, std::to_string(World_XM._41));
	EATER_CHANGE_LIST(Nodeindex, "WorldTM", 1, 3, std::to_string(World_XM._42));
	EATER_CHANGE_LIST(Nodeindex, "WorldTM", 2, 3, std::to_string(World_XM._43));
	EATER_CHANGE_LIST(Nodeindex, "WorldTM", 3, 3, std::to_string(World_XM._44));
}

void E_ChangeManager::Change_Name(int Nodeindex, MeshFilter* MF)
{
	//파일 에서 이릅값을 가져온다
	std::string CMeshName		= EATER_GET_MAP(Nodeindex, "MeshName");
	std::string CMaterialName	= EATER_GET_MAP(Nodeindex, "MaterialName");

	//MeshFilter에서 이름을가져온다
	std::string BufferName		= MF->GetBufferName();
	std::string MaterialName	= MF->GetMaterialName();
	std::string ModelName		= MF->GetModelName();
	//std::string NodeName		= MF->gameobject->Name;
	MF->gameobject->OneMeshData->Object_Data->IsActive;

	//파일에서 읽은 이름과 현재 오브젝트의 메테리얼이름이같다면
	//현재 메테리얼의 이름으로 변경해준다
	if (BufferName != "")
	{
		EATER_CHANGE_MAP(Nodeindex, "MeshName", BufferName);
	}

	if (MaterialName != CMaterialName && MaterialName!= "")
	{
		EATER_CHANGE_MAP(Nodeindex, "MaterialName", MaterialName);
	}
	//EATER_CHANGE_MAP(Nodeindex, "NodeName", NodeName);

	EATER_CHANGE_MAP(Nodeindex, "IsActive", std::to_string(MF->gameobject->OneMeshData->Object_Data->IsActive));
	EATER_CHANGE_MAP(Nodeindex, "IsShadow", std::to_string(MF->gameobject->OneMeshData->Object_Data->IsShadow));
	EATER_CHANGE_MAP(Nodeindex, "IsCull", std::to_string(MF->gameobject->OneMeshData->Object_Data->IsCull));
}


std::string E_ChangeManager::GetOriginalName(std::string Original)
{
	std::size_t Start	= 0;
	std::size_t end		= Original.rfind('(');
	return Original.substr(Start, end);
}

