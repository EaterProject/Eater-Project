#pragma once

#include <string>
#include <vector>
#include <map>
#include "SimpleMath.h"
#include "TypeOptionHeader.h"

typedef enum MESH_TYPE
{
	NULL_MESH,
	STATIC_MESH,
	BONE_MESH,
	SKIN_MESH,
	TERRAIN_MESH,
	QUAD_MESH
}MESH_TYPE;

typedef enum TEXTURE_TYPE : UINT
{
	DIFFUSE_TEXTURE		= 0x00000001,
	NORMAL_TEXTURE		= 0x00000010,
	ORM_TEXTURE			= 0x00000100,
	EMISSIVE_TEXTURE	= 0x00001000,
}TEXTURE_TYPE;

typedef enum MODEL_TYPE
{
	FBX_MODEL,
	ASE_MODEL
}MODEL_TYPE;

typedef enum IMAGE_TYPE
{
	FLOAT_IMAGE,
	CHAR_IMAGE,
}IMAGE_TYPE;

typedef enum EXTENSION_TYPE
{
	PNG_IMAGE,
	BMP_IMAGE,
}EXTENSION_TYPE;

namespace ParserData
{
	struct CFrame
	{
		float							m_Time;
		DirectX::SimpleMath::Vector3	m_Pos;
		DirectX::SimpleMath::Quaternion m_RotQt;
		DirectX::SimpleMath::Vector3	m_Scale;
	};
	
	class CAnimation
	{
	public:
		CAnimation();
		CAnimation(const CAnimation& ani) = default;
		~CAnimation();

	public:
		std::vector<CFrame*>	m_AniData;

		float	m_TicksPerFrame;
		int		m_TotalFrame;
		int		m_StartFrame;
		int		m_EndFrame;
		int		m_NowIndex;
		int		m_NextIndex;
	};
}

namespace ParserData
{
	struct CIndexList
	{
		int m_Index[3];
	};

	struct CVertex
	{
	public:
		CVertex();
		~CVertex();

	public:
		DirectX::SimpleMath::Vector3 m_Pos;
		DirectX::SimpleMath::Vector3 m_Normal;
		DirectX::SimpleMath::Vector3 m_Tanget;
		DirectX::SimpleMath::Vector2 m_UV;

		bool m_IsNormalSet;		// Normal Data Setting 여부
		bool m_IsTextureSet;	// Texture Data Setting 여부

		/// Skinned Data
		std::vector<float> m_BoneWeights;	// BoneWeight List
		std::vector<UINT> m_BoneIndices;	// BoneIndex List

		// 인덱스
		int m_Indices;
	};

	struct CFace
	{
	public:
		CFace();
		~CFace() = default;

	public:
		int	m_VertexIndex[3];	// Face Vertex List Index
		int	m_TFace[3];			// Texture Coordinate
		
		DirectX::SimpleMath::Vector3 m_NormalVertex[3];		// Vertex Normal
		DirectX::SimpleMath::Vector3 m_TangentVertex[3];	// Vertex Tangent
		DirectX::SimpleMath::Vector2 m_UVvertex[3];			// Vertex UV
	};

	struct CMaterialMap
	{
		std::string	m_MapName;
		std::string	m_BitMap;
	};

	struct CMaterial
	{
	public:
		CMaterial();
		~CMaterial();

	public:
		int	m_MaterialNumber;
		std::string	m_MaterialName;

		DirectX::SimpleMath::Vector4 m_Material_Ambient;
		DirectX::SimpleMath::Vector4 m_Material_Diffuse;
		DirectX::SimpleMath::Vector4 m_Material_Specular;
		DirectX::SimpleMath::Vector4 m_Material_Emissive;

		float m_Material_Shininess;
		float m_Material_Transparency;
		float m_Material_Reflectivity;

		bool m_Alpha;

		UINT m_TextureBind;					// Texture Bind Type

		CMaterialMap* m_DiffuseMap = nullptr;		// DiffuseMap Data
		CMaterialMap* m_NormalMap = nullptr;			// NormalMap Data
		CMaterialMap* m_EmissiveMap = nullptr;		// EmissiveMap Data
		CMaterialMap* m_ORMMap = nullptr;			// AO(R) + Roughness(G) + Metallic(B) Map Data

		std::vector<CMaterialMap*> m_MapList;
	};

	class CMesh
	{
	public:
		CMesh();
		CMesh(const CMesh& mesh) = default;
		virtual ~CMesh();

	public:
		std::string	m_NodeName; 
		std::string	m_ParentName;

		UINT m_MeshIndex;						// Geometry Index

		/// Object Type Data
		MESH_TYPE m_MeshType;
		bool m_TopNode;

		int m_BoneIndex;

		DirectX::SimpleMath::Matrix m_WorldTM;		// Mesh WorldTM
		DirectX::SimpleMath::Matrix m_LocalTM;		// Mesh LocalTM

		CMesh* m_Parent;								// Mesh Parent
		std::vector<CMesh*> m_ChildList;				// Mesh Child List

		/// Material Data
		CMaterial*					m_MaterialData;		// Mesh Material Data

		/// Face Data (파싱용)
		std::vector<CFace*>			m_MeshFace;			// Face List

		/// Animation Data
		CAnimation*				m_Animation;		// Animation Data

		/// Skinning Data
		std::vector<DirectX::SimpleMath::Matrix> m_BoneTMList;		// Bone Offset TM List
		std::vector<CMesh*> m_BoneMeshList;							// Bone Mesh List

		/// Original Data
		std::vector<DirectX::SimpleMath::Vector3> m_OriginVertexList;
		std::vector<UINT> m_OriginIndexList;

		/// Final Data
		std::vector<CVertex*>		m_VertexList;		/// Vertex List
		std::vector<CIndexList*>		m_IndexList;		/// Index List
	};
}

namespace ParserData
{
	struct Scenedata
	{
	public:
		Scenedata();

	public:
		std::string			m_FileName;
		int					m_FirstFrame;
		int					m_LastFrame;
		int					m_FrameSpeed;
		float				m_TicksPerFrame;
	};

	struct TVertex
	{
		TVertex();

		float m_U, m_V, m_W;
	};

	struct Bone
	{
		Bone();

		std::string	m_BoneName;
		int			m_BoneNumber;
		int			m_parent_bone_number;
	};

	class ASEMesh : public CMesh
	{
	public:
		ASEMesh();
		~ASEMesh();

	public:
		int	m_Type;
		
		int	m_Mesh_NumVertex;
		int	m_Mesh_NumFaces;

		int	m_Mesh_NumTVertex;						
		int	m_Mesh_SumTVertex;						
		std::vector<TVertex*> m_Mesh_TVertex;

		
		std::vector<Bone*> m_BoneList;
	};
}

namespace ParserData
{
	struct CModel
	{
		CModel();
		~CModel();

		bool m_isAnimation;											// Animation 유무

		std::vector<ParserData::CAnimation*> m_AnimationList;		// Animation List
		std::vector<ParserData::CMaterial*> m_MaterialList;			// Material List
		std::vector<ParserData::CMesh*> m_MeshList;	 				// Mesh List
	};

	struct ImageData
	{
		IMAGE_TYPE type;
		unsigned int channels;

		int width;
		int height;
		void* imgColor;
	};
}

#include "ParserData.inl"