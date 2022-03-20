#pragma once

#define SAFE_DELETE(x) { if(x != nullptr) {delete x; x = nullptr;} }

namespace ParserData
{
	inline CAnimation::CAnimation()
	{
		m_TicksPerFrame = 0.0f;
		m_TotalFrame = 0;
		m_StartFrame = 0;
		m_EndFrame = 0;
		m_NowIndex = 0;
		m_NextIndex = 1;
	}
	inline CAnimation::~CAnimation()
	{
		for (CFrame* frame : m_AniData)
		{
			SAFE_DELETE(frame);
		}

		m_AniData.clear();
	}

	inline CVertex::CVertex()
	{
		m_IsNormalSet = false;
		m_IsTextureSet = false;
	}

	inline CVertex::~CVertex()
	{
		m_BoneWeights.clear();
		m_BoneIndices.clear();
	}

	inline CFace::CFace()
	{
		m_VertexIndex[0] = 0;
		m_VertexIndex[1] = 0;
		m_VertexIndex[2] = 0;
	}

	inline Scenedata::Scenedata()
	{
		m_FirstFrame	= 0;
		m_LastFrame		= 0;
		m_FrameSpeed	= 0;
		m_TicksPerFrame = 0;
	}

	inline TVertex::TVertex()
	{
		m_U = 0;
		m_V = 0;
		m_W = 0;
	}

	inline Bone::Bone()
	{
		m_BoneNumber = 0;
		m_parent_bone_number = 0;
	}

	inline CMaterial::CMaterial()
	{
		m_TextureBind			= 0;

		m_MaterialNumber		= 0;
		m_Material_Shininess	= 0.0f;
		m_Material_Transparency = 0.0f;
		m_Material_Reflectivity = 0.0f;

		m_Alpha = false;
	}

	inline CMaterial::~CMaterial()
	{
		m_DiffuseMap	= nullptr;
		m_NormalMap		= nullptr;
		m_ORMMap	= nullptr;
		m_EmissiveMap		= nullptr;

		for (CMaterialMap* matMap : m_MapList)
		{
			SAFE_DELETE(matMap);
		}

		m_MapList.clear();
	}

	inline CMesh::CMesh()
	{
		m_MeshType			= MESH_TYPE::STATIC_MESH;
		m_MeshIndex			= 0;
		m_MaterialData		= nullptr;
		m_Animation			= nullptr;

		m_TopNode			= false;
	}

	inline CMesh::~CMesh()
	{
		for (CFace* face : m_MeshFace)
		{
			SAFE_DELETE(face);
		}

		for (CVertex* vertex : m_VertexList)
		{
			SAFE_DELETE(vertex);
		}

		for (CIndexList* index : m_IndexList)
		{
			SAFE_DELETE(index);
		}

		SAFE_DELETE(m_Animation);

		m_MaterialData = nullptr;

		m_BoneTMList.clear();
		m_MeshFace.clear();
		m_VertexList.clear();
		m_IndexList.clear();
	}

	inline ASEMesh::ASEMesh()
	{
		m_Type				= 0;
		m_Mesh_NumVertex	= 0;
		m_Mesh_NumFaces		= 0;
		m_Mesh_NumTVertex	= 0;
		m_Mesh_SumTVertex	= 0;

		m_Parent = nullptr;
	}

	inline ASEMesh::~ASEMesh()
	{
		m_Parent = nullptr;

		for (TVertex* tV : m_Mesh_TVertex)
		{
			SAFE_DELETE(tV);
		}

		for (Bone* bone : m_BoneList)
		{
			SAFE_DELETE(bone);
		}

		m_Mesh_TVertex.clear();
		m_BoneList.clear();
	}

	inline CModel::CModel()
	{
		m_isAnimation = false;
	}

	inline CModel::~CModel()
	{
		for (CMaterial* mat : m_MaterialList)
		{
			SAFE_DELETE(mat);
		}

		for (CMesh* mesh : m_MeshList)
		{
			SAFE_DELETE(mesh);
		}

		m_MaterialList.clear();
		m_MeshList.clear();
	}
}


