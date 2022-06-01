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
	UINT DebugOption = 0;
	UINT RenderingOption = 0;
	UINT PostProcessOption = 0;

public:
	// SSAO Option
	float AO_Radius = 0.25f;							// ���ø��� ���� Texel ������	(0.0 ~ 5.0)
	float AO_SurfaceEpsilon = 0.05f;					// ����Ǵ� �ּ� ���̰�			(0.0 ~ 1.0)
	int AO_BlurCount = 4;								// AO Blur Count				(1 ~ 8)

	/// �����ߴ�
	float AO_FadeStart = 0.01f;							// AO �ʺ� �����ϴ� ���� ����	(0.0 ~ 10.0)
	float AO_FadeEnd = 2.0f;							// AO �ʺ� �����ϴ� �� ����		(0.0 ~ 10.0)

	// Fog Option
	Vector3 FOG_Color = Vector3(0.9f, 0.745f, 0.35f);	// Fog ����
	float FOG_MoveSpeed = 0.05f;						// Fog �����̴� �ӵ�				(0.0 ~ 1.0)
	float FOG_StartDistance = 50.0f;					// Fog ���� �Ÿ�					(0.0 ~ 1000.0)
	float FOG_DistanceOffset = 8.0f;					// Fog �Ÿ� ������				(0.0 ~ 100.0)
	float FOG_DistanceValue = 5.0f;						// Fog �Ÿ� ����					(0.0 ~ 100.0)
	float FOG_HeightOffset = 3.0f;						// Fog ���� ������				(0.0 ~ 100.0)
	float FOG_HeightValue = 3.0f;						// Fog ���� ����					(0.0 ~ 100.0)

	// Bloom Option
	float BLOOM_Threshold_Min = 1.0f;					// Bloom ���� �ּ� ����			(0.0 ~ 2.0)
	float BLOOM_Threshold_Max = 100.0f;					// Bloom ���� �ִ� ����			(10.0 ~ 1000.0)
	float BLOOM_Factor = 0.25f;							// Bloom ȥ�շ�					(0.0 ~ 1.0)

	// SkyCube Option
	float SkyCube_Size = 500.0f;						// SkyCube Map ũ��				(1.0 ~ 5000.0)
	bool SkyCube_HDR = true;							// SkyCube HDR ����

	// SkyLight Option
	float SkyLight_Factor = 1.0f;						// SkyLight ����					(0.0 ~ 5.0)
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
	bool Alpha = false;					// Alpha Mesh
	bool OutLine = false;				// Out Line
	bool LimLight = false;				// Lim Light

	Vector3 AddColor = Vector3(0.0f, 0.0f, 0.0f);	// Add Color

	float RoughnessFactor = 0.0f;		// Roughness ���� (0 ~ 1)
	float MetallicFactor = 0.0f;		// Metallic ���� (0 ~ 1)

	Vector3 EmissiveColor = Vector3(1.0f, 1.0f, 1.0f);	// Emissive Color
	float EmissiveFactor = 1.0f;		// Emissive ����

	Vector3 LimLightColor = Vector3(0.0f, 0.0f, 0.0f);	// LimLight Color
	float LimLightFactor = 0.0f;		// LimLight ����
	float LimLightWidth = 0.0f;			// LimLight ����

	Vector3 OutLineColor = Vector3(1.0f, 1.0f, 1.0f);	// OutLine Color
	float OutLineWidth = 0.0f;			// OutLine ����

	Vector2 Tile;						// X, Y Tiling
	Matrix TexTM;						// Material�� �ؽ��� ���

public:
	MaterialProperty& operator=(const MaterialProperty& material_property)
	{
		memcpy(this, &material_property, sizeof(MaterialProperty));

		return *this;
	}
};

// Material Property Block
class MaterialPropertyBlock : public MaterialProperty {};

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
	Matrix Tex;										// Terrain Mesh �� �ؽ��� ���
	std::vector<MaterialBuffer*> Material_List;		// Material List
};

// UI Property
class UIProperty
{
public:
	Vector2 TexScale;
	Vector2 TexPos;

	Vector4 ImageColor;

	Matrix World;
};

// UI Buffer
class UIBuffer
{
public:
	UINT BufferIndex = 0;		// Mesh Buffer Index

	TextureBuffer* Albedo = nullptr;			// DiffuseMap Texture

	UIProperty* UI_Property = nullptr;			// UI Property
};

// Particle Data
class OneParticle
{
public:
	bool Playing;
	Vector4 Color;

	Vector2 TexScale;
	Vector2 TexPos;

	Matrix* World;
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

	Matrix CamInvView;	// Camera Inverse XY View Matrix
	Matrix CamView;		// Camera View Matrix
	Matrix CamProj;		// Camera Proj Matrix
	Matrix CamOrthoProj;	// Camera Orth Proj Matrix
	Matrix CamViewProj;	// Camera View Proj Matrix
	Vector3 CamPos;		// Camera Pos
	Vector3 CamLook;	// Camera Look

	BoundingFrustum BoundFrustum;	// Bounding Frustum
	BoundingFrustum OriginFrustum;	// Bounding Frustum
};

// Object Data
class ObjectData
{
public:
	OBJECT_TYPE ObjType = OBJECT_TYPE::DEFALT;		//������Ʈ Ÿ��
	std::string Name;								//������Ʈ �̸�

	bool IsActive = true;							//������Ʈ Ȱ��ȭ ����

	void* Object;									//Grahpic ���� GameObject

	UINT ObjectIndex;								//������Ʈ�� ������ �ε���
	Vector4 HashColor;								//������Ʈ�� ������ Hash Color

	std::vector<Matrix> BoneOffsetTM;				//�� ������ TM

	Matrix World;									//�Ž��� ���� ���
	Matrix InvWorld;								//�Ž��� ���� �����

	bool IsMaterialBlock = false;
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
/// ���ӿ������� �׷��ȿ������� ������ �Ѱ��� �޽� ������
/// </summary>
class MeshData
{
public:
	~MeshData()
	{

	}

public:
	void* Render_Data;								// ��ȯ�� Render Data

	ObjectData*		Object_Data = nullptr;			// Object Data

	MeshBuffer*		 Mesh_Buffer		= nullptr;	// Mesh Buffer
	MaterialBuffer*	 Material_Buffer	= nullptr;	// Material Buffer
	AnimationBuffer* Animation_Buffer	= nullptr;	// Animation Buffer
	UIBuffer*		 UI_Buffer			= nullptr;	// UI Buffer

	// �߰� ������
	AnimationData*	Animation_Data	= nullptr;		// Animation Data
	TerrainData*	Terrain_Data	= nullptr;		// Terrain Data
	ParticleData*	Particle_Data	= nullptr;		// Particle Data
};

/// <summary>
/// ���ӿ������� �׷��ȿ������� ������ �۷ι� ������
/// </summary>
class GlobalData
{
public:
	float Time;		// Delta Time

	Matrix TexSpace;	// Texture Space Matrix

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
/// ��Ȳ�� �ļ����� ���ؽ��� �ε����� ���۷� �������ְ�
/// �ִϸ��̼ǰ�, ��������ʴ°����� �׳� �����ͼ� ���ĳ��� Ŭ����
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

	MESH_TYPE MeshType;				// �Ž� Ÿ��

	bool Top_Object = false;		//���� �ֻ��� ������Ʈ���� ����

	std::string ParentName = "";	//�θ��� �̸�
	std::string	Name = "";			//�ڱ��ڽ��� �̸�

	std::string BufferName	 = "";	//�Ž� �̸�
	std::string ModelName	 = "";	//�Ž� �̸�
	std::string MaterialName = "";	//���͸��� �̸�

	Matrix WorldTM;					//���� ��Ʈ����
	Matrix LocalTM;					//���� ��Ʈ����

	int BoneIndex = -1;				//���ϰ�� �ڽ��� �ε���
	std::vector<Matrix> BoneTMList;			//�� ��Ʈ����

	LoadMeshData* Parent = nullptr;			//�θ� �Ž�
	std::vector<LoadMeshData*> Child;		//�ڽ� �Ž� ����Ʈ
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

	std::vector<Matrix> BoneOffsetList;				//�� ��Ʈ����
	std::vector<LoadMeshData*> BoneList;			//�� �Ž�
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