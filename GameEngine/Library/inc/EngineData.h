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
	OBJECT_TYPE ObjType = OBJECT_TYPE::DEFALT;		//������Ʈ Ÿ��
	std::string Name;								//������Ʈ �̸�

	bool IsActive = true;							//������Ʈ Ȱ��ȭ ����

	void* Object;									//Grahpic ���� GameObject

	UINT ObjectIndex;								//������Ʈ�� ������ �ε���
	Vector4 HashColor;								//������Ʈ�� ������ Hash Color
	
	std::vector<Matrix> BoneOffsetTM;				//�� ������ TM

	Matrix* World = nullptr;						//�Ž��� ���� ���
	Matrix* Local = nullptr;						//�Ž��� ���� ���

public:
	static Vector4 HashToColor(int hash)
	{
		return Vector4( (float)((hash) & 0xff), 
						(float)((hash >> 8) & 0xff), 
						(float)((hash >> 16) & 0xff), 
						(float)((hash >> 24) & 0xff) );
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

// Mesh Sub Data
class MeshSubData
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
		delete Mesh_SubData;
		delete IndexBuf;
		delete VertexBuf;
	}

public:
	UINT BufferIndex = 0;		// Mesh Buffer Index

	MeshSubData* Mesh_SubData;	// Mesh Sub Data

	IndexBuffer* IndexBuf;		// Index Buffer
	VertexBuffer* VertexBuf;	// Vertex Buffer
};

// Material Sub Data
class MaterialSubData
{
public:
	Vector4 AddColor = Vector4(0.0f, 0.0f, 0.0, 1.0f);	// Add Color

	float EmissiveFactor = 1.0f;		// Emissive ����
	float RoughnessFactor = 1.0f;		// Roughness ����
	float MetallicFactor = 1.0f;		// Metallic ����

	bool Alpha = false;					// Alpha Mesh

	Vector2 Tile;						// X, Y Tiling
	Matrix TexTM;						// Material�� �ؽ��� ���
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

// Environment Buffer
class EnvironmentBuffer : public Resources
{
public:
	virtual ~EnvironmentBuffer()
	{
		delete Environment;
		delete Irradiance;
		delete Prefilter;
	};

	TextureBuffer* Environment = nullptr;
	TextureBuffer* Irradiance = nullptr;
	TextureBuffer* Prefilter = nullptr;
};

// Animation Data
namespace EngineData
{
class AnimationData
{
public:
	UINT AnimationIndex = 0;
	UINT FrameIndex = 0;
};
}

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
	Matrix CamInvView;	// Camera Inverse XY View Matrix
	Matrix CamView;		// Camera View Matrix
	Matrix CamProj;		// Camera Proj Matrix
	Matrix CamViewProj;	// Camera View Proj Matrix
	Vector3 CamPos;		// Camera Pos

	BoundingFrustum BoundFrustum;	// Bounding Frustum
	BoundingFrustum OriginFrustum;	// Bounding Frustum
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

	MeshBuffer*		Mesh_Buffer = nullptr;			// Mesh Buffer
	MaterialBuffer*	Material_Buffer	= nullptr;		// Material Buffer

	// �߰� ������
	EngineData::AnimationData*	Animation_Data = nullptr;		// Animation Data
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

	// Culling Data
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
		if (Parent != nullptr)
		{
			delete Parent;
		}

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
		for (auto Anim : AnimList)
		{
			int Size = (int)Anim.second.size();
			for (auto CAnim : Anim.second)
			{
				delete CAnim;
			}
		}
	}

	std::map<std::string, std::vector<CAnimation*>> AnimList;
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