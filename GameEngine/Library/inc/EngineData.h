#pragma once
#include <map>
#include <queue>
#include <unordered_map>
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
	RenderOption() 
	{
		SkyLight_Name.insert({ 0,"Default"});
	};
	UINT DebugOption = 0;
	UINT RenderingOption = 0;
	UINT PostProcessOption = 0;
public:
	// SSAO Option
	float AO_Radius = 0.25f;							// 샘플링시 비교할 Texel 반지름	(0.0 ~ 5.0)
	float AO_SurfaceEpsilon = 0.05f;					// 차폐되는 최소 깊이값			(0.0 ~ 1.0)
	int AO_BlurCount = 4;								// AO Blur Count				(1 ~ 8)

	/// 지워야댐
	float AO_FadeStart = 0.01f;							// AO 너비를 결정하는 시작 지점	(0.0 ~ 10.0)
	float AO_FadeEnd = 2.0f;							// AO 너비를 결정하는 끝 지점		(0.0 ~ 10.0)

	// Fog Option
	Vector3 FOG_Color = Vector3(0.9f, 0.745f, 0.35f);	// Fog 색상
	float FOG_MoveSpeed = 0.05f;						// Fog 움직이는 속도				(0.0 ~ 1.0)
	float FOG_StartDistance = 50.0f;					// Fog 시작 거리					(0.0 ~ 1000.0)
	float FOG_DistanceOffset = 8.0f;					// Fog 거리 오프셋				(0.0 ~ 100.0)
	float FOG_DistanceValue = 5.0f;						// Fog 거리 범위					(0.0 ~ 100.0)
	float FOG_HeightOffset = 3.0f;						// Fog 높이 오프셋				(0.0 ~ 100.0)
	float FOG_HeightValue = 3.0f;						// Fog 높이 범위					(0.0 ~ 100.0)

	// Bloom Option
	float BLOOM_Threshold_Min = 1.0f;					// Bloom 추출 최소 영역			(0.0 ~ 2.0)
	float BLOOM_Threshold_Max = 100.0f;					// Bloom 추출 최대 영역			(10.0 ~ 1000.0)
	float BLOOM_Factor = 0.25f;							// Bloom 혼합률					(0.0 ~ 1.0)

	// SkyCube Option
	float SkyCube_Size = 500.0f;						// SkyCube Map 크기				(1.0 ~ 5000.0)
	bool SkyCube_HDR = true;							// SkyCube HDR 여부
	std::string SkyCube_Name;							// SkyCube Name

	// SkyLight Option
	float SkyLight_Factor = 1.0f;						// SkyLight 강도					(0.0 ~ 5.0)
	std::map<UINT, std::string> SkyLight_Name;			// Key : SkyLight Index & Value : SkyLight Name
};

// Animation Data
class AnimationData
{
public:
	virtual ~AnimationData()
	{

	}

public:
	UINT PrevAnimationIndex = 0;					// Prev Animation Index
	UINT NextAnimationIndex = 0;					// Next Animation Index
	UINT PrevFrameIndex = 0;						// Prev Frame Index
	UINT NextFrameIndex = 0;						// Next Frame Index
	float FrameTime = 0.0f;							// Animation Frame Time
};

// Mesh Property
class MeshProperty
{
public:
	BoundingBox		BoundBox;
	BoundingSphere	BoundSphere;
};

// Mesh Buffer
class MeshBuffer : public Resources
{
public:
	virtual ~MeshBuffer()
	{
		delete Mesh_Property;
		delete IndexBuf;
		delete VertexBuf;
	}

public:
	UINT BufferIndex = 0;		// Mesh Buffer Index

	MeshProperty* Mesh_Property;	// Mesh Sub Data

	IndexBuffer* IndexBuf;		// Index Buffer
	VertexBuffer* VertexBuf;	// Vertex Buffer
};

// Material Property
class MaterialProperty
{
public:
	bool IsRelocation = false;			

	SHADER_OPTION Shader_Option = SHADER_DEFAULT;			// Shader Option

	UINT SkyLightIndex = 0;									// SkyLight Index

	bool Alpha = false;										// Alpha 여부
	float AlphaFactor = 1.0f;								// Alpha 수치

	Vector3 AddColor = Vector3(0.0f, 0.0f, 0.0f);			// Add Color

	float RoughnessFactor = 0.0f;							// Roughness 강도 (0 ~ 1)
	float MetallicFactor = 0.0f;							// Metallic 강도 (0 ~ 1)

	Vector3 EmissiveColor = Vector3(1.0f, 1.0f, 1.0f);		// Emissive Color
	float EmissiveFactor = 1.0f;							// Emissive 강도

	Vector3 LimLightColor = Vector3(0.0f, 0.0f, 0.0f);		// LimLight Color
	float LimLightFactor = 0.0f;							// LimLight 강도
	float LimLightWidth = 0.0f;								// LimLight 범위

	bool OutLine = false;									// Out Line 여부
	Vector3 OutLineColor = Vector3(1.0f, 1.0f, 1.0f);		// OutLine Color
	float OutLineWidth = 0.0f;								// OutLine 범위

	bool Dissolve = true;									// Dissolve 여부
	Vector4 Innercolor = Vector4(1.0f, 0.0f, 0.0f, 1.0f);	// 경계 안쪽 색상
	Vector4 Outercolor = Vector4(1.0f, 1.0f, 1.0f, 1.0f);	// 경계 바깥쪽 색상

	Vector2 Tile;											// X, Y Tiling
	Matrix TexTM;											// Material의 텍스쳐 행렬

public:
	MaterialProperty& operator=(const MaterialProperty& material_property)
	{
		memcpy(this, &material_property, sizeof(MaterialProperty));

		return *this;
	}
};

// Material Property Block
class MaterialPropertyBlock : public MaterialProperty 
{
public:
	int Queue_Index;
};

// Material Buffer
class MaterialBuffer : public Resources
{
public:
	virtual ~MaterialBuffer()
	{
		delete Material_Property;
	}

public:
	UINT BufferIndex = 0;							// Material Buffer Index

	MaterialProperty* Material_Property = nullptr;	// Material SubData
	
	TextureBuffer* Albedo = nullptr;				// DiffuseMap Texture
	TextureBuffer* Normal = nullptr;				// NormalMap Texture
	TextureBuffer* Emissive = nullptr;				// Emissive Texture
	TextureBuffer* ORM = nullptr;					// AO(R) + Roughness(G) + Metallic(B) Texture
};

// Environment Buffer
class SkyLightBuffer : public Resources
{
public:
	virtual ~SkyLightBuffer()
	{
		delete Irradiance;
		delete Prefilter;
	};

	TextureBuffer* Irradiance = nullptr;				// Environment Irradiance Buffer
	TextureBuffer* Prefilter = nullptr;					// Environment Prefilter Buffer
};

// Collider Buffer
class ColliderBuffer
{
public:
	ColliderBuffer() = default;
	~ColliderBuffer() = default;

	int IndexArrayCount;
	int VertexArrayCount;

	Vector3* VertexArray;
	UINT* IndexArray;
};

// Camera Animation
class CameraAnimation
{
public:
	float OneFrame;
	int AddKeyCount;
	std::vector<Vector3> Position;
	std::vector<Vector3> Rotation;
};

// Terrain Data
class TerrainData
{
public:
	Matrix Tex;										// Terrain Mesh 의 텍스쳐 행렬
	std::vector<MaterialBuffer*> Material_List;		// Material List
};

// UI Property
class UIProperty
{
public:
	UI_TYPE UI_Option;	// UI Option
	
	Vector2 TexScale;		// UI Texture Offset Scale
	Vector2 TexPos;			// UI Texture Offset Position
	Vector2 TexFill;		// UI Texture Fill Range

	Vector4 ImageColor;		// UI Color

	Matrix World;
};

// UI Buffer
class UIBuffer
{
public:
	UINT BufferIndex = 0;				// UI Buffer Index
	UINT BufferLayer = 0;				// UI Buffer Layer Order

	TextureBuffer* Albedo = nullptr;	// DiffuseMap Texture

	UIProperty* UI_Property = nullptr;	// UI Property
};

// Particle Data
class OneParticle
{
public:
	bool Playing;
	Vector4 Color;

	Vector3 Pos;

	Vector2 TexScale;
	Vector2 TexPos;

	Matrix* World;
	Matrix* Local;
};

class ParticleData
{
public:
	PARTICLE_RENDER_OPTION RenderType;

	int Particle_Count;

	float Particle_Strength;

	Vector3 Area_Radius;

	std::vector<OneParticle*> m_Particles;
};

// Debug Data
class DebugData
{
public:
	DEBUG_MESH_TYPE DebugType;

	Matrix World;

	Vector3 RayStart = { 0,0,0 };
	Vector3 RayEnd = { 0,0,0 };

	Vector3 Color = { 0,1,0 };
};

// Camera Data
class CameraData
{
public:
	~CameraData()
	{

	}

public:
	bool Culling = false;

	Matrix CamInvView;		// Camera Inverse XY View Matrix
	Matrix CamView;			// Camera View Matrix
	Matrix CamProj;			// Camera Proj Matrix
	Matrix CamProjTex;		// Camera Proj Tex Matrix
	Matrix CamOrthoProj;	// Camera Orth Proj Matrix
	Matrix CamViewProj;		// Camera View Proj Matrix
	Matrix CamViewProjTex;	// Camera View Proj Tex Matrix
	Vector3 CamPos;			// Camera Pos

	BoundingFrustum BoundFrustum;	// Bounding Frustum
	BoundingFrustum OriginFrustum;	// Bounding Frustum
};

// Object Data
class ObjectData
{
public:
	OBJECT_TYPE ObjType = OBJECT_TYPE::DEFALT;		//오브젝트 타입
	std::string Name;								//오브젝트 이름

	bool IsActive = true;							//오브젝트 활성화 여부
	bool IsStatic = false;							//정적 오브젝트 여부
	bool IsShadow = true;							//그림자 여부
	bool IsCull = true;								//컬링 여부

	void* Object;									//Grahpic 전용 GameObject

	UINT ObjectIndex;								//오브젝트의 고유한 인덱스
	Vector4 HashColor;								//오브젝트의 고유한 Hash Color

	std::vector<Matrix> BoneOffsetTM;				//본 오프셋 TM

	Matrix Local;									//매쉬의 로컬 행렬
	Matrix World;									//매쉬의 월드 행렬
	Matrix InvWorld;								//매쉬의 월드 역행렬

	MaterialPropertyBlock* Material_Block;

public:
	static Vector4 HashToColor(int hash)
	{
		return Vector4((float)((hash) & 0xff),
			(float)((hash >> 8) & 0xff),
			(float)((hash >> 16) & 0xff),
			(float)((hash >> 24) & 0xff));
	}
	static UINT ColorToHash(Vector4 color)
	{
		if (color.x < 0.0f || color.y < 0.0f || color.z < 0.0f || color.w < 0.0f)
			return -1;

		return	((int)color.x) +
			((int)color.y * 256) +
			((int)color.z * 65536) +
			((int)color.w * 16777216);
	}
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

	MeshBuffer*		 Mesh_Buffer		= nullptr;	// Mesh Buffer
	MaterialBuffer*	 Material_Buffer	= nullptr;	// Material Buffer
	AnimationBuffer* Animation_Buffer	= nullptr;	// Animation Buffer
	UIBuffer*		 UI_Buffer			= nullptr;	// UI Buffer

	// 추가 데이터
	AnimationData*	Animation_Data	= nullptr;		// Animation Data
	TerrainData*	Terrain_Data	= nullptr;		// Terrain Data
	ParticleData*	Particle_Data	= nullptr;		// Particle Data
};

/// <summary>
/// 게임엔진에서 그래픽엔진으로 던저줄 글로벌 데이터
/// </summary>
class GlobalData
{
public:
	float Time;		// Delta Time

	CameraData* MainCamera_Data;
	std::vector<CameraData*> CameraList;

	std::vector<DirectionalLightData*>	DirectionLightList;
	std::vector<PointLightData*>		PointLightList;
	std::vector<SpotLightData*>			SpotLightList;

	// Culling Mesh
	std::vector<MeshBuffer*> OccluderList;

	// Debug Data
	std::queue<DebugData>	 Debug_Data;
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
		for (auto k : Child)
		{
			delete k;
		}
		Parent = nullptr;
	};

	MESH_TYPE MeshType;				// 매쉬 타입

	bool Top_Object = false;		//가장 최상위 오브젝트인지 여부
	bool IsActive	= true;
	bool IsShadow	= true;
	bool IsCull		= true;

	std::string ParentName = "";	//부모의 이름
	std::string	Name = "";			//자기자신의 이름

	std::string BufferName	 = "";	//매쉬 이름
	std::string ModelName	 = "";	//매쉬 이름
	std::string MaterialName = "";	//매터리얼 이름

	Matrix WorldTM;					//월드 매트릭스
	Matrix LocalTM;					//로컬 매트릭스

	int BoneIndex = -1;				//본일경우 자신의 인덱스
	std::vector<Matrix> BoneTMList;			//본 매트릭스

	LoadMeshData* Parent = nullptr;			//부모 매쉬
	std::vector<LoadMeshData*> Child;		//자식 매쉬 리스트
};

class LoadParticleData
{
public:
	LoadParticleData() = default;
	~LoadParticleData() = default;
public:
	std::string TextrueName;
	int		RenderType		= 0;
	int		ColorType		= 0;
	int		ScaleType		= 0;
	int		MaxParticle		= 0;
	float	DelayTime		= 0.0f;
	float	RateOverTime	= 7.0f;
	float	Strength		= 1.0f;
	Vector3 Radius			= {0,0,0};
	Vector2 Tiling			= { 0,0 };
	Vector3 StartForce_Min	= {0,0,0};
	Vector3 StartForce_Max	= {0,0,0};
	Vector4 StartColor_Min	= { 0,0,0,0 };
	Vector4 StartColor_Max	= { 0,0,0,0 };
	Vector2 StartLifeTime	= { 0,0 };
	Vector2 StartScale		= { 0,0 };
	Vector2 StartRotation	= { 0,0 };
	Vector3 LifeForce_Min	= {0,0,0};
	Vector3 LifeForce_Max	= {0,0,0};
	Vector4 LifeColor_Min	= { 0,0,0,0 };
	Vector4 LifeColor_Max	= { 0,0,0,0 };
	Vector2 LifeScale		= { 0,0 };
	Vector2 LifeRotation	= { 0,0 };
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
		for (auto k : TopMeshList)
		{
			if (k != nullptr)
			{
				delete k;
			}
		}

		for (auto k : TopSkinList)
		{
			if (k != nullptr)
			{
				delete k;
			}
		}

		for (auto k : TopBoneList)
		{
			if (k != nullptr)
			{
				delete k;
			}
		}
	}

public:
	std::string Name;

	std::vector<LoadMeshData*> TopMeshList;
	std::vector<LoadMeshData*> TopSkinList;
	std::vector<LoadMeshData*> TopBoneList;

	std::vector<Matrix> BoneOffsetList;				//본 매트릭스
	std::vector<LoadMeshData*> BoneList;			//본 매쉬
};

class ModelAnimationData
{
public:
	~ModelAnimationData()
	{

	}

	std::string Name;

	UINT AnimationCount = 0;

	std::unordered_map<std::string, CModelAnimation*> AnimList;
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