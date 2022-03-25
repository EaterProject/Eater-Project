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

// Render Option
class RenderOption
{
public:
	UINT DebugOption;
	UINT RenderingOption;
	UINT PostProcessOption;

public:
	bool operator == (const RenderOption& option)
	{
		if (DebugOption == option.DebugOption && RenderingOption == option.RenderingOption && PostProcessOption == option.PostProcessOption)
			return true;
		else
			return false;
	}
};

// Object Data
class ObjectData
{
public:
	OBJECT_TYPE ObjType = OBJECT_TYPE::DEFALT;		//오브젝트 타입

	bool Pick = false;								//오브젝트 선택 상태

	std::vector<Matrix> BoneOffsetTM;				//본 오프셋 TM

	Vector4 HashColor;								//매쉬의 고유한 Hash Color

	Matrix* World = nullptr;						//매쉬의 월드 행렬
	Matrix* Local = nullptr;						//매쉬의 로컬 행렬
};

// Mesh Buffer
class MeshBuffer : public Resources
{
public:
	virtual ~MeshBuffer()
	{
		delete IndexBuf;
		delete VertexBuf;
	}

public:
	UINT BufferIndex = 0;		// Mesh Buffer Index

	DirectX::BoundingBox BoundingBox;

	IndexBuffer* IndexBuf;		// Index Buffer
	VertexBuffer* VertexBuf;	// Vertex Buffer
};

// Material Sub Data
struct MaterialSubData
{
	MaterialSubData() = default;

	Vector4 AddColor = Vector4(0.0f, 0.0f, 0.0, 1.0f);	// Add Color

	float EmissiveFactor = 1.0f;		// Emissive 강도
	float RoughnessFactor = 1.0f;		// Roughness 강도
	float MetallicFactor = 1.0f;		// Metallic 강도

	bool Alpha = false;					// Alpha Mesh

	Vector2 Tile;						// X, Y Tiling
	Matrix TexTM;						// Material의 텍스쳐 행렬
};

// Material Buffer
class MaterialBuffer : public Resources
{
public:
	virtual ~MaterialBuffer()
	{
		delete Material_SubData;
	}

public:
	UINT BufferIndex = 0;					// Material Buffer Index

	MaterialSubData* Material_SubData = nullptr;	// Material SubData

	TextureBuffer* Albedo = nullptr;		// DiffuseMap Texture
	TextureBuffer* Normal = nullptr;		// NormalMap Texture
	TextureBuffer* Emissive = nullptr;		// Emissive Texture
	TextureBuffer* ORM = nullptr;			// AO(R) + Roughness(G) + Metallic(B) Texture
};

// Terrain Data
class TerrainData
{
public:
	Matrix Tex;										// Terrain Mesh 의 텍스쳐 행렬
	std::vector<MaterialBuffer*> Material_List;		// Material List
};

// Particle Data
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

// RayCast Data
class RayCastData
{
public:
	Vector3 RayStart = { 0,0,0 };
	Vector3 RayEnd = { 0,0,0 };
	Vector3 RayColor = { 0,1,0 };
};

// Collider Data
class ColliderData
{
public:
	Matrix ColliderWorld = XMMatrixIdentity();
	Vector3 ColliderColor = { 1,1,1 };
};

/// <summary>
/// 게임엔진에서 그래픽엔진으로 던저줄 글로벌 데이터
/// </summary>
class GlobalData
{
public:
	float Time;		// Delta Time

	//카메라 정보들
	Matrix CamInvView;	// Camera Inverse XY View Matrix
	Matrix CamView;		// Camera View Matrix
	Matrix CamProj;		// Camera Proj Matrix
	Matrix CamViewProj;	// Camera View Proj Matrix
	Vector3 CamPos;		// Camera Pos

	Matrix CamVP;		// Camera Proj * Proj Matrix

	Matrix TexSpace;	// Texture Space Matrix

	std::vector<DirectionalLightData*>	DirectionLights;
	std::vector<PointLightData*>		PointLights;
	std::vector<SpotLightData*>			SpotLights;

	// Debug Data
	std::queue<RayCastData> RayCastDebugData;
};

/// <summary>
/// 게임엔진에서 그래픽엔진으로 던저줄 한개의 메쉬 데이터
/// </summary>
class MeshData
{
public:
	~MeshData()
	{

	}

public:
	void* Render_Data;								// 변환된 Render Data

	ObjectData*		Object_Data = nullptr;			// Object Data

	MeshBuffer*		Mesh_Buffer = nullptr;			// Mesh Buffer
	MaterialBuffer*	Material_Buffer	= nullptr;		// Material Buffer

	// 추가 데이터
	TerrainData*	Terrain_Data	= nullptr;		// Terrain Data
	ParticleData*	Particle_Data	= nullptr;		// Particle Data

	// 디버그 데이터
	ColliderData* Collider_Data = nullptr;			// Collider Data
};

/// <summary>
/// 규황이 파서에서 버텍스와 인덱스를 버퍼로 변경해주고
/// 애니메이션과, 변경되지않는값들은 그냥 가져와서 합쳐놓는 클래스
/// </summary>
class LoadMeshData
{
public:
	~LoadMeshData()
	{
		BoneTMList = nullptr;

		Parent = nullptr;
	};

	MESH_TYPE MeshType;				// 매쉬 타입

	bool Top_Object = false;		//가장 최상위 오브젝트인지 여부

	std::string ParentName = "";	//부모의 이름
	std::string	Name = "";			//자기자신의 이름

	std::string BufferName	 = "";	//매쉬 이름
	std::string ModelName	 = "";	//매쉬 이름
	std::string MaterialName = "";	//매터리얼 이름

	Matrix WorldTM;					//월드 매트릭스
	Matrix LocalTM;					//로컬 매트릭스

	int BoneIndex = -1;				//본일경우 자신의 인덱스
	std::vector<Matrix>* BoneTMList = nullptr;	//본 매트릭스

	LoadMeshData* Parent = nullptr;			//부모 매쉬
	std::vector<LoadMeshData*> Child;		//자식 매쉬 리스트
};

/// <summary>
/// 한개의 모델을 저장하는 단위
/// 매쉬의 가장 최상위 오브젝트들만 보관하는 저장데이터
/// </summary>
class ModelData
{
public:
	~ModelData()
	{
		//최상위의 오브젝트를 재귀로 돌면서 포인터로 생성된것들 모두삭제
		//
	}

	std::vector<LoadMeshData*> TopMeshList;
	std::vector<LoadMeshData*> TopSkinList;
	std::vector<LoadMeshData*> TopBoneList;

	std::vector<Matrix>* BoneOffsetList = nullptr;	//본 매트릭스
	std::vector<LoadMeshData*> BoneList;			//본 매쉬
};

class ModelAnimationData
{
public:
	~ModelAnimationData()
	{

	}
	std::map<std::string, std::vector<CAnimation*>* > AnimList;
};

//컨퍼넌트들의 함수포인터를 저장할 구조체
class ComponentFunctionData
{
public:
	//함수 활성화 여부
	bool* Enabled = nullptr;

	//함수 포인터
	std::function<void()> FunctionPointer;

	//컨퍼넌트 포인터
	Component* ComponentPointer;

	//컨퍼넌트 순서
	int OrderCount = 0;
};