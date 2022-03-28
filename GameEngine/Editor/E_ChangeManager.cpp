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

	//오브젝트의 이름이 모델이름과 같다면 최상위로 묶어놓은 오브젝트
	//파일에서 ParentName이 RootNode라고 되어있는 모두 변경해줘야한다
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

	if (NodeName == Object->Name)
	{
		MeshFilter* MF = Object->GetComponent<MeshFilter>();
		Transform* TR = Object->GetComponent<Transform>();
		//이름값을 입력해준다
		Change_Name(index, MF);
		//현재 월드를 기본 원본에 곱해서 로컬값으로 만들어준다
		Change_LocalTM(index, TR);
	}
}

void E_ChangeManager::Change_Bone(int index, GameObject* Object)
{
	std::string ParentName = EATER_GET_MAP(index, "ParentName");
	std::string NodeName = EATER_GET_MAP(index, "NodeName");

	if (NodeName == Object->Name)
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
		std::string mMaterial = MF->GetMaterialName();
		std::string mDiffuse = mMaterialData->GetDiffuseName();
		std::string mNormal = mMaterialData->GetNormalName();
		std::string mEmissive = mMaterialData->GetEmissiveName();
		std::string mORM = mMaterialData->GetORMName();

		std::string Tileing_X = EATER_GET_MAP(index, "Tileing_X");
		std::string Tileing_Y = EATER_GET_MAP(index, "Tileing_Y");


		float mMetallicF = mMaterialData->m_MaterialData->Material_SubData->MetallicFactor;
		float mRoughnessF = mMaterialData->m_MaterialData->Material_SubData->RoughnessFactor;

		float AddColor_R = mMaterialData->m_MaterialData->Material_SubData->AddColor.x;
		float AddColor_G = mMaterialData->m_MaterialData->Material_SubData->AddColor.y;
		float AddColor_B = mMaterialData->m_MaterialData->Material_SubData->AddColor.z;
		//타일링 빼줘야함 ....

		EATER_CHANGE_MAP(index, "MaterialName", mMaterial);
		EATER_CHANGE_MAP(index, "Alpha", "NO");
		EATER_CHANGE_MAP(index, "Diffuse", mDiffuse);
		EATER_CHANGE_MAP(index, "Normal", mNormal);
		EATER_CHANGE_MAP(index, "Emissive", mEmissive);
		EATER_CHANGE_MAP(index, "ORM", mORM);

		EATER_CHANGE_MAP(index, "Roughness", std::to_string(mMetallicF));
		EATER_CHANGE_MAP(index, "Metallic", std::to_string(mRoughnessF));

		EATER_CHANGE_MAP(index, "Tileing_X", "1");
		EATER_CHANGE_MAP(index, "Tileing_Y", "1");

		EATER_CHANGE_MAP(index, "AddColor_R", std::to_string(AddColor_R));
		EATER_CHANGE_MAP(index, "AddColor_G", std::to_string(AddColor_G));
		EATER_CHANGE_MAP(index, "AddColor_B", std::to_string(AddColor_B));
		EATER_CHANGE_MAP(index, "AddColor_A", "1");

		EATER_CLOSE_CHANGE_FILE(MF->GetMaterialName(), "../Assets/Texture/Material/", ".Emat");
	}
	else
	{
		EATER_CLOSE_READ_FILE();
	}
	
}

void E_ChangeManager::Change_LocalTM(int Nodeindex, Transform* TR)
{
	DirectX::SimpleMath::Matrix Position_4x4;
	Position_4x4._11 = 1;				Position_4x4._12 = 0;			Position_4x4._13 = 0;			Position_4x4._14 = 0;
	Position_4x4._21 = 0;				Position_4x4._22 = 1;			Position_4x4._23 = 0;			Position_4x4._24 = 0;
	Position_4x4._31 = 0;				Position_4x4._32 = 0;			Position_4x4._33 = 1;			Position_4x4._34 = 0;
	Position_4x4._41 = TR->Position.x;		Position_4x4._42 = TR->Position.y;	Position_4x4._43 = TR->Position.z;	Position_4x4._44 = 1;

	float radX = TR->Rotation.x * 3.141592f / 180;
	float radY = TR->Rotation.y * 3.141592f / 180;
	float radZ = TR->Rotation.z * 3.141592f / 180;
	DirectX::XMMATRIX _P = DirectX::XMMatrixRotationX(radX);
	DirectX::XMMATRIX _Y = DirectX::XMMatrixRotationY(radY);
	DirectX::XMMATRIX _R = DirectX::XMMatrixRotationZ(radZ);
	DirectX::XMMATRIX Rotation_4x4 = _R * _Y * _P;

	DirectX::SimpleMath::Matrix Scale_4x4;
	Scale_4x4._11 = TR->Scale.x;	Scale_4x4._12 = 0;				Scale_4x4._13 = 0;				Scale_4x4._14 = 0;
	Scale_4x4._21 = 0;				Scale_4x4._22 = TR->Scale.y;	Scale_4x4._23 = 0;				Scale_4x4._24 = 0;
	Scale_4x4._31 = 0;				Scale_4x4._32 = 0;				Scale_4x4._33 = TR->Scale.z;	Scale_4x4._34 = 0;
	Scale_4x4._41 = 0;				Scale_4x4._42 = 0;				Scale_4x4._43 = 0;				Scale_4x4._44 = 1;

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

	Local_XM = Local_XM * ChangeTM;
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

void E_ChangeManager::Change_Name(int Nodeindex, MeshFilter* MF)
{
	//파일 에서 이릅값을 가져온다
	std::string CMeshName		= EATER_GET_MAP(Nodeindex, "MeshName");
	std::string CMaterialName	= EATER_GET_MAP(Nodeindex, "MaterialName");

	//MeshFilter에서 이름을가져온다
	std::string BufferName		= MF->GetBufferName();
	std::string MaterialName	= MF->GetMaterialName();
	std::string ModelName		= MF->GetModelName();

	//파일에서 읽은 이름과 현재 오브젝트의 메테리얼이름이같다면
	//현재 메테리얼의 이름으로 변경해준다
	if (BufferName != CMeshName && BufferName != "")
	{
		EATER_CHANGE_MAP(Nodeindex, "MeshName", BufferName);
	}

	if (MaterialName != CMaterialName && MaterialName!= "")
	{
		EATER_CHANGE_MAP(Nodeindex, "MaterialName", MaterialName);
	}
}


std::string E_ChangeManager::GetOriginalName(std::string Original)
{
	std::size_t Start	= 0;
	std::size_t end		= Original.rfind('(');
	return Original.substr(Start, end);
}

