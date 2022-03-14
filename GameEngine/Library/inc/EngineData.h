#pragma once
#include <map>
#include <queue>
#include <functional>
#include "ResourcesData.h"
#include "ParserData.h"
#include "LightHelper.h"
#include "TypeOptionHeader.h"

using namespace DirectX;
using namespace SimpleMath;
using namespace ParserData;

class Component;

class MaterialData
{
public:
	UINT Material_Index = 0;				// Material Index

	MaterialOption* Material_Option = nullptr;	// Material Data

	TextureBuffer* Albedo = nullptr;		// DiffuseMap Texture
	TextureBuffer* Normal = nullptr;		// NormalMap Texture
	TextureBuffer* Emissive = nullptr;		// Emissive Texture
	TextureBuffer* ORM = nullptr;			// AO(R) + Roughness(G) + Metallic(B) Texture

	Vector4 Color_Base;			// Base Color
	Vector4 Color_Add;			// Add Color
};

class TerrainData
{
public:
	std::vector<MaterialData*> Material_List;		// Material List
};

class OneParticle
{
public:
	bool Playing;
	Vector4 Color;

	Matrix* Tex;
	Matrix* World;
};

class ParticleData
{
public:
	PARTICLE_RENDER_OPTION RenderType;

	int Particle_Count;
	Vector3 Area_Radius;

	std::vector<OneParticle*> m_Particles;
};

class RayCastData
{
public:
	Vector3 RayStart = { 0,0,0 };
	Vector3 RayEnd = { 0,0,0 };
	Vector4 RayColor = { 0,1,0,1 };
};

class ColliderData
{
public:
	Matrix ColliderWorld = XMMatrixIdentity();
	Vector4 ColliderColor = { 1,1,1,1 };
};

class RenderOption
{
public:
	bool DebugMode;
};

/// <summary>
/// ���ӿ������� �׷��ȿ������� ������ �۷ι� ������
/// </summary>
class GlobalData
{
public:
	float mTime;		// Delta Time

	//ī�޶� ������
	Matrix mCamInvView;	// Camera Inverse XY View Matrix
	Matrix mCamView;	// Camera View Matrix
	Matrix mCamProj;	// Camera Proj Matrix
	Vector3 mCamPos;	// Camera Pos

	Matrix mCamVP;		// Camera Proj * Proj Matrix

	Matrix mTexSpace;	// Texture Space Matrix

	std::vector<DirectionalLightData*>	mDirectionLights;
	std::vector<PointLightData*>		mPointLights;
	std::vector<SpotLightData*>			mSpotLights;
	std::vector<MaterialOption*>		mMaterials;

	// Debug Data
	std::queue<RayCastData> mRayCastDebugData;
};

/// <summary>
/// ���ӿ������� �׷��ȿ������� ������ �Ѱ��� �޽� ������
/// </summary>
class MeshData
{
public:
	~MeshData()
	{
		IndexBuf = nullptr;
		VertexBuf = nullptr;
	}

public:
	OBJECT_TYPE ObjType = OBJECT_TYPE::DEFALT;		//������Ʈ Ÿ��
	
	UINT MeshIndex = 0;									// Instance ������ ���� Index
	UINT RenderMeshIndex = 0;							// Renderer �� ������ ���� Index
	UINT RenderListIndex = 0;							// Renderer �� ������ ���� Index

	bool Alpha = false;						// Alpha Mesh

	IndexBuffer* IndexBuf = nullptr;				//�ε��� ����
	VertexBuffer* VertexBuf = nullptr;				//���ؽ� ����

	std::vector<Matrix> BoneOffsetTM;				//�� ������ TM

	Matrix* mWorld = nullptr;				//�Ž��� ���� ���
	Matrix* mLocal = nullptr;				//�Ž��� ���� ���
	Matrix* mTexTM = nullptr;				//�Ž��� �ؽ��� ���

	// �߰� ������
	MaterialData*	Material_Data	= nullptr;		// Material Data
	TerrainData*	Terrain_Data	= nullptr;		// Terrain Data
	ParticleData*	Particle_Data	= nullptr;		// Particle Data

	// ����� ������
	ColliderData* Collider_Data = nullptr;			// Collider Data
};

/// <summary>
/// ��Ȳ�� �ļ����� ���ؽ��� �ε����� ���۷� �������ְ�
/// �ִϸ��̼ǰ�, ��������ʴ°����� �׳� �����ͼ� ���ĳ��� Ŭ����
/// </summary>
class LoadMeshData
{
public:
	~LoadMeshData()
	{
		IndexBuf = nullptr;
		VertexBuf = nullptr;

		Material = nullptr;
		Animation = nullptr;

		BoneTMList = nullptr;
		BoneList = nullptr;

		Parent = nullptr;
	};

	MESH_TYPE MeshType;				// �Ž� Ÿ��
	bool Alpha = false;
	bool Top_Object = false;		//���� �ֻ��� ������Ʈ���� ����

	int BoneIndex = -1;				//���ϰ�� �ڽ��� �ε���

	std::string ParentName = "";	//�θ��� �̸�
	std::string	Name = "";			//�ڱ��ڽ��� �̸�

	Matrix WorldTM;					//���� ��Ʈ����
	Matrix LocalTM;					//���� ��Ʈ����

	UINT MeshIndex;

	IndexBuffer* IndexBuf = nullptr;			//�ε��� ����
	VertexBuffer* VertexBuf = nullptr;			//���ؽ� ����

	std::string AlbedoName;
	std::string NormalName;
	std::string EmissiveName;
	std::string ORMName;

	std::vector<UINT> Index_List;		//	
	std::vector<Vector3> Vertex_List;	//

	ParserData::CMaterial* Material = nullptr;	//���׸��� ����
	ParserData::OneAnimation* Animation = nullptr;	//�ִϸ��̼� ����

	std::vector<Matrix>* BoneTMList = nullptr;	//�� ��Ʈ����
	std::vector<Mesh*>* BoneList = nullptr;		//�� �Ž�

	Vector3*	m_OriginVertexList		= nullptr;
	UINT*		m_OriginIndexList		= nullptr;
	int			m_OriginVertexListCount = 0;
	int			m_OriginIndexListCount	= 0;
	
	std::string Mask_Name;					// Terrain ���� Mask Name

	LoadMeshData* Parent = nullptr;			//�θ� �Ž�
	std::vector<LoadMeshData*> Child;		//�ڽ� �Ž� ����Ʈ

	Matrix* BoneOffset = nullptr;			//�� ��Ʈ����
};

/// <summary>
/// �Ѱ��� ���� �����ϴ� ����
/// �Ž��� ���� �ֻ��� ������Ʈ�鸸 �����ϴ� ���嵥����
/// </summary>
class ModelData
{
public:
	~ModelData()
	{
		//�ֻ����� ������Ʈ�� ��ͷ� ���鼭 �����ͷ� �����Ȱ͵� ��λ���
		//
	}

	UINT Model;

	std::vector<LoadMeshData*> TopMeshList;
	std::vector<LoadMeshData*> TopSkinList;
	std::vector<LoadMeshData*> TopBoneList;

	std::vector<Matrix>* BoneOffsetList = nullptr;	//�� ��Ʈ����
	std::vector<LoadMeshData*> BoneList;			//�� �Ž�
};

class ModelAnimationData
{
public:
	~ModelAnimationData()
	{

	}
	std::map<std::string, std::vector<OneAnimation*>* > AnimList;
};

//���۳�Ʈ���� �Լ������͸� ������ ����ü
class ComponentFunctionData
{
public:
	//�Լ� Ȱ��ȭ ����
	bool* Enabled = nullptr;

	//�Լ� ������ 
	std::function<void()> FunctionPointer;

	//���۳�Ʈ ������
	Component* ComponentPointer;

	//���۳�Ʈ ����
	int OrderCount = 0;
};