#define SAFE_DELETE(x) { if(x != nullptr) {delete x; x = nullptr;} }

#include "ModelParser.h"

#include "ASEFile.h"
#include "ASEParser.h"

using namespace std;
using namespace ParserData;
using namespace DirectX;
using namespace SimpleMath;

ASEParser::ASEParser()
	: m_MaterialData(nullptr), m_materialmap(nullptr), m_OneMesh(nullptr), m_IsAnimation(false), m_Animation(nullptr), m_lexer(nullptr)
{

}

ASEParser::~ASEParser()
{
	SAFE_DELETE(m_lexer);
}

void ASEParser::Initialize()
{
	m_lexer = new ASE::CASELexer;
}

void ASEParser::Release()
{
	SAFE_DELETE(m_lexer);

	for (Model* model : m_ModelList)
	{
		for (Mesh* mesh : model->m_MeshList)
		{
			ASEMesh* aseMesh = static_cast<ASEMesh*>(mesh);

			for (TVertex* tvertex : aseMesh->m_Mesh_TVertex)
			{
				SAFE_DELETE(tvertex);
			}
			for (Bone* bone : aseMesh->m_BoneList)
			{
				SAFE_DELETE(bone);
			}
			for (Face* face : aseMesh->m_MeshFace)
			{
				SAFE_DELETE(face);
			}
			for (Vertex* vertex : aseMesh->m_VertexList)
			{
				SAFE_DELETE(vertex);
			}
			for (IndexList* index : aseMesh->m_IndexList)
			{
				SAFE_DELETE(index);
			}
			aseMesh->m_MeshFace.clear();
			aseMesh->m_VertexList.clear();
			aseMesh->m_IndexList.clear();
		}
	}
}

ParserData::Model* ASEParser::LoadModel(std::string fileName)
{
	// 새로운 Model 생성..
	CreateModel();

	/// 파일을 로드한다.
	if (!m_lexer->Open(const_cast<char*>(fileName.c_str())))
	{
		TRACE("파일을 여는 중에 문제가 발생했습니다!");
		return FALSE;
	}

	/// 비재귀 버전..
	DataParsing();

	// Optimize & Parent Set..
	OptimizeData();

	// 필요없는 데이터 제거..
	ResetData();

	return m_Model;
}

void ASEParser::OptimizeVertex(ASEMesh* pMesh)
{
	bool new_VertexSet = true;
	size_t resize_VertexIndex = pMesh->m_VertexList.size();

	// 각각 Face마다 존재하는 3개의 Vertex 비교..
	for (unsigned int i = 0; i < pMesh->m_MeshFace.size(); i++)
	{
		for (int j = 0; j < 3; j++)
		{
			unsigned int vertexIndex = pMesh->m_MeshFace[i]->m_VertexIndex[j];

			Vertex* nowVertex = pMesh->m_VertexList[vertexIndex];

			// 텍스처가 있고, 설정하지 않았으면 텍스처 u,v 설정..
			if (pMesh->m_Mesh_NumTVertex > 0 && nowVertex->m_IsTextureSet == false)
			{
				nowVertex->m_UV.x = pMesh->m_Mesh_TVertex[pMesh->m_MeshFace[i]->m_TFace[j]]->m_U;
				nowVertex->m_UV.y = pMesh->m_Mesh_TVertex[pMesh->m_MeshFace[i]->m_TFace[j]]->m_V;
				nowVertex->m_IsTextureSet = true;
			}

			// 최초 인덱스 노말값 검사시엔 넣어주고 시작..
			if (nowVertex->m_IsNormalSet == false)
			{
				nowVertex->m_Normal = pMesh->m_MeshFace[i]->m_NormalVertex[j];
				nowVertex->m_IsNormalSet = true;
			}

			// Normal, U, V 값중 한개라도 다르면 Vertex 새로 생성..
			if ((pMesh->m_VertexList[vertexIndex]->m_Normal != pMesh->m_MeshFace[i]->m_NormalVertex[j]))
			{
				new_VertexSet = true;
			}

			if (pMesh->m_Mesh_NumTVertex > 0)
			{
				if ((pMesh->m_VertexList[vertexIndex]->m_UV.x != pMesh->m_Mesh_TVertex[pMesh->m_MeshFace[i]->m_TFace[j]]->m_U) ||
					(pMesh->m_VertexList[vertexIndex]->m_UV.y != pMesh->m_Mesh_TVertex[pMesh->m_MeshFace[i]->m_TFace[j]]->m_V))
				{
					new_VertexSet = true;
				}
			}

			if (new_VertexSet)
			{
				// 추가된 Vertex가 있다면 체크..
				if (resize_VertexIndex > pMesh->m_VertexList.size())
				{
					for (size_t k = pMesh->m_VertexList.size(); k < resize_VertexIndex; k++)
					{
						// 새로 추가한 Vertex와 동일한 데이터를 갖고있는 Face 내의 Vertex Index 수정..
						if ((pMesh->m_VertexList[k]->m_Indices == pMesh->m_MeshFace[i]->m_VertexIndex[j]) &&
							(pMesh->m_VertexList[k]->m_Normal == pMesh->m_MeshFace[i]->m_NormalVertex[j]))
						{
							if (pMesh->m_Mesh_NumTVertex > 0)
							{
								if ((pMesh->m_VertexList[k]->m_UV.x == pMesh->m_Mesh_TVertex[pMesh->m_MeshFace[i]->m_TFace[j]]->m_U) &&
									(pMesh->m_VertexList[k]->m_UV.y == pMesh->m_Mesh_TVertex[pMesh->m_MeshFace[i]->m_TFace[j]]->m_V))
								{
									pMesh->m_MeshFace[i]->m_VertexIndex[j] = (int)k;
									new_VertexSet = false;
									break;
								}
							}
							else
							{
								pMesh->m_MeshFace[i]->m_VertexIndex[j] = (int)k;
								new_VertexSet = false;
								break;
							}
						}
					}
				}

				// 새로 추가해야할 Vertex..
				if (new_VertexSet == true)
				{
					Vertex* newVertex = new Vertex;
					newVertex->m_Pos = nowVertex->m_Pos;
					newVertex->m_Indices = nowVertex->m_Indices;
					newVertex->m_Normal = pMesh->m_MeshFace[i]->m_NormalVertex[j];
					newVertex->m_BoneIndices = pMesh->m_VertexList[nowVertex->m_Indices]->m_BoneIndices;
					newVertex->m_BoneWeights = pMesh->m_VertexList[nowVertex->m_Indices]->m_BoneWeights;
					newVertex->m_IsNormalSet = true;

					if (pMesh->m_Mesh_NumTVertex > 0)
					{
						newVertex->m_UV.x = pMesh->m_Mesh_TVertex[pMesh->m_MeshFace[i]->m_TFace[j]]->m_U;
						newVertex->m_UV.y = pMesh->m_Mesh_TVertex[pMesh->m_MeshFace[i]->m_TFace[j]]->m_V;
						newVertex->m_IsTextureSet = true;
					}

					pMesh->m_VertexList.push_back(newVertex);
					pMesh->m_MeshFace[i]->m_VertexIndex[j] = (int)resize_VertexIndex;
					resize_VertexIndex++;
				}
			}
		}
	}

	// Tanget 값 설정..
	for (unsigned int i = 0; i < pMesh->m_MeshFace.size(); i++)
	{
		int index0 = pMesh->m_MeshFace[i]->m_VertexIndex[0];
		int index1 = pMesh->m_MeshFace[i]->m_VertexIndex[1];
		int index2 = pMesh->m_MeshFace[i]->m_VertexIndex[2];

		Vertex* vertex0 = pMesh->m_VertexList[index0];
		Vertex* vertex1 = pMesh->m_VertexList[index0];
		Vertex* vertex2 = pMesh->m_VertexList[index0];

		DirectX::SimpleMath::Vector3 ep1 = vertex1->m_Pos - vertex0->m_Pos;
		DirectX::SimpleMath::Vector3 ep2 = pMesh->m_VertexList[index2]->m_Pos - vertex0->m_Pos;

		DirectX::SimpleMath::Vector2 uv1 = { vertex1->m_UV.x - vertex0->m_UV.x,
											 vertex1->m_UV.y - vertex0->m_UV.y };
		DirectX::SimpleMath::Vector2 uv2 = { vertex2->m_UV.x - vertex0->m_UV.x,
											 vertex2->m_UV.y - vertex0->m_UV.y };

		float den = 1.0f / (uv1.x * uv2.y - uv2.x * uv1.y);

		// 현재 픽셀 쉐이더에서 연산을 통해 T, B, N을 얻는데
		// 픽셀 쉐이더 내의 연산은 버텍스 쉐이더의 연산에 비해 호출 횟수가 차원이 다르게 크므로 부하가 올 수 있다..
		// 법선맵의 픽셀의 색은 픽셀 쉐이더 안이 아니면 얻을수 없기 때문에 픽셀 쉐이더에서 연산을 한다고 한다..
		/// 현재 연산과정을 버텍스 쉐이더로 옮겨둠
		DirectX::SimpleMath::Vector3 tangent = (ep1 * uv2.y - ep2 * uv1.y) * den;
		tangent.Normalize();

		// 유사 정점은 값을 누적하여 쉐이더에서 평균값을 사용하도록 하자..
		vertex0->m_Tanget += tangent;
		vertex1->m_Tanget += tangent;
		vertex2->m_Tanget += tangent;
	}

	// 인덱스는 그냥 복사
	for (unsigned int i = 0; i < pMesh->m_MeshFace.size(); i++)
	{
		pMesh->m_IndexList.push_back(new IndexList);

		for (int j = 0; j < 3; j++)
		{
			pMesh->m_IndexList[i]->m_Index[j] = pMesh->m_MeshFace[i]->m_VertexIndex[j];
		}

	}
}

void ASEParser::RecombinationTM(ParserData::ASEMesh* pMesh)
{
	/// Negative Scale Check
	// 3D Max의 작업상 Mirroring으로 인해 Scale값이 음수가 나올 경우를 Negative Scale 이라고 한다
	// 처리를 안해줄 경우 Normal 처리나 Animation 처리시에 문제가 된다
	// Rotaion을 나타내는 축 Row1 ~ Row3 중 두축을 외적 한 값과 다른 한 축을 내적하여 음수이면 축이 반대이고 양수이면 축이 일치한다

	XMVECTOR crossVec = XMVector3Cross(pMesh->m_WorldTM.Right(), pMesh->m_WorldTM.Up());
	XMVECTOR dotVec = XMVector3Dot(crossVec, pMesh->m_WorldTM.Backward());

	XMFLOAT3 resultVec;

	XMStoreFloat3(&resultVec, dotVec);

	// 축 방향이 다른 경우만 처리해주자..
	if (resultVec.x < 0 || resultVec.y < 0 || resultVec.z < 0)
	{
		XMVECTOR nodeScale;
		XMVECTOR nodetRot;
		XMVECTOR nodePos;

		// 기존 매트릭스에서 분리..
		XMMatrixDecompose(&nodeScale, &nodetRot, &nodePos, pMesh->m_WorldTM);

		// 스케일 재조정..
		nodeScale *= -1;

		// 노말값 재조정..
		for (auto& k : pMesh->m_VertexList)
		{
			k->m_Normal *= -1;
		}

		/// 텍스쳐나 맵핑 데이터도 변환을 해주어야 할까..?

		// 매트릭스 재조합..
		pMesh->m_WorldTM = XMMatrixScalingFromVector(nodeScale) * XMMatrixRotationQuaternion(nodetRot) * XMMatrixTranslationFromVector(nodePos);
	}

	// WorldTM 역행렬을 구한다..
	DirectX::SimpleMath::Matrix iMatrix = pMesh->m_WorldTM.Invert();

	// 부모 노드가 없을경우 WorldTM 이 곧 LocalTM..
	// 부모 노드가 있을경우 LocalTM 을 구해주자..
	if (pMesh->m_Parent == nullptr)
	{
		pMesh->m_LocalTM = pMesh->m_WorldTM;
	}
	else
	{
		DirectX::SimpleMath::Matrix piMatrix = pMesh->m_Parent->m_WorldTM;
		pMesh->m_LocalTM = pMesh->m_WorldTM * piMatrix.Invert();
	}

	for (unsigned int i = 0; i < pMesh->m_VertexList.size(); i++)
	{
		pMesh->m_VertexList[i]->m_Pos = XMVector3Transform(pMesh->m_VertexList[i]->m_Pos, iMatrix);
	}

	// 애니메이션이 있을경우 사이즈를 구해두자..
	if (pMesh->m_Animation)
	{
		pMesh->m_Animation->m_TicksPerFrame = 1.0f / m_scenedata.m_FrameSpeed;
		pMesh->m_Animation->m_TotalFrame = m_scenedata.m_LastFrame - m_scenedata.m_FirstFrame;
	}
}

void ASEParser::OptimizeData()
{
	// 부모 자식 연결..
	for (unsigned int i = 0; i < m_MeshList.size(); i++)
	{
		for (unsigned int j = 0; j < m_MeshList.size(); j++)
		{
			if (i == j)
			{
				continue;
			}

			// 노드 이름과 다른 노드 부모 이름이 같다면 상속관계..
			if (m_MeshList[i]->m_NodeName == m_MeshList[j]->m_ParentName)
			{
				m_MeshList[j]->m_Parent = m_MeshList[i];
			}
		}
	}

	// Optimize Data
	for (unsigned int i = 0; i < m_MeshList.size(); i++)
	{
		OptimizeVertex(m_MeshList[i]);
		RecombinationTM(m_MeshList[i]);
		SetBoneTM(m_MeshList[i]);
	}

	m_Model->m_isAnimation = m_IsAnimation;
	m_Model->m_MaterialList = m_MaterialList;
	m_Model->m_MeshList.resize(m_MeshList.size());

	for (unsigned int i = 0; i < m_MeshList.size(); i++)
	{
		m_Model->m_MeshList[i] = m_MeshList[i];
	}
}

void ASEParser::SetBoneTM(ParserData::ASEMesh* pMesh)
{
	for (size_t i = 0; i < pMesh->m_BoneList.size(); i++)
	{
		for (size_t j = 0; j < m_MeshList.size(); j++)
		{
			if (m_MeshList[j]->m_NodeName == pMesh->m_BoneList[i]->m_BoneName)
			{
				// Bone NodeTM * Mesh NodeTM 역행렬 -> Mesh 기준 Bone의 OffsetTM
				DirectX::SimpleMath::Matrix boneoffsetTM = m_MeshList[j]->m_WorldTM * pMesh->m_WorldTM.Invert();

				// Bone OffsetTM 역행렬 -> Bone 기준 Mesh의 OffsetTM
				pMesh->m_BoneTMList.emplace_back(boneoffsetTM.Invert());
				pMesh->m_BoneMeshList.emplace_back(m_MeshList[j]);

				// Bone Setting..
				m_MeshList[j]->m_MeshType = BONE_MESH;
			}

			// Bone List size가 같아지면 더 돌 필요가 없다..
			if (pMesh->m_BoneTMList.size() == pMesh->m_BoneList.size())
			{
				break;
			}
		}
	}
}

void ASEParser::CreateModel()
{
	m_Model = nullptr;
	m_Model = new Model();
	m_ModelList.push_back(m_Model);
}

void ASEParser::ResetData()
{
	m_MaterialData = nullptr;
	m_materialmap = nullptr;
	m_OneMesh = nullptr;
	m_Bone = nullptr;
	m_Animation = nullptr;

	m_MeshList.clear();
	m_MaterialList.clear();
}

void ASEParser::DataParsing()
{
	// 재귀 돌리다가 일정 이상 루프를 도는 경우(오류)를 체크하기 위한 변수
	int i = 0;

	// 인덱스 체크용 변수
	int m_Index = 0;

	LONG nowtoken = NULL;

	// 파일이 끝날때까지 계속해서 읽기..
	while (true)
	{
		// 현재 토큰 확인..
		nowtoken = m_lexer->GetToken(m_TokenString);

		switch (nowtoken)
		{
		case TOKEND_BLOCK_START:
			break;
		case TOKENR_HELPER_CLASS:
			break;

			//--------------------
			// 3DSMAX_ASCIIEXPORT
			//--------------------

		case TOKENR_3DSMAX_ASCIIEXPORT:
			Parsing_NumberInt();
			break;

			//--------------------
			// COMMENT
			//--------------------

		case TOKENR_COMMENT:
			Parsing_String();
			break;

			///----------------------------------
			/// SCENE
			///----------------------------------

		case TOKENR_SCENE:
			//
			break;
		case TOKENR_SCENE_FILENAME:
			m_scenedata.m_FileName = Parsing_String();
			break;
		case TOKENR_SCENE_FIRSTFRAME:
			m_scenedata.m_FirstFrame = Parsing_NumberInt();
			break;
		case TOKENR_SCENE_LASTFRAME:
			m_scenedata.m_LastFrame = Parsing_NumberInt();
			break;
		case TOKENR_SCENE_FRAMESPEED:
			m_scenedata.m_FrameSpeed = Parsing_NumberInt();
			break;
		case TOKENR_SCENE_TICKSPERFRAME:
			m_scenedata.m_TicksPerFrame = Parsing_NumberFloat();
			break;
		case TOKENR_SCENE_MESHFRAMESTEP:
		case TOKENR_SCENE_KEYFRAMESTEP:
		case TOKENR_SCENE_BACKGROUND_STATIC:
		case TOKENR_SCENE_AMBIENT_STATIC:
			break;

		case TOKENR_SCENE_ENVMAP:
		{
			// 그냥 안의 내용을 읽어버린다 }가 나올때까지
			while (nowtoken = m_lexer->GetToken(m_TokenString), nowtoken != TOKEND_BLOCK_END) {
			}
		}
		break;

		///----------------------------------
		/// MATERIAL_LIST
		///----------------------------------

		case TOKENR_MATERIAL:
			Create_MaterialData_to_list();
			m_MaterialData->m_MaterialNumber = Parsing_NumberInt();
			break;
		case TOKENR_MAP_DIFFUSE:
			Create_DiffuseMap_to_list();
			break;
		case TOKENR_MAP_BUMP:
			Create_BumpMap_to_list();
			break;
		case TOKENR_MAP_SPECULAR:
			Create_SpecularMap_to_list();
			break;
		case TOKENR_MAP_SHINE:
			Create_ShineMap_to_list();
			break;
		case TOKENR_MAP_GENERIC:
			break;

			///----------------------------------
			/// Material
			///----------------------------------

		case TOKENR_MATERIAL_COUNT:
			Parsing_NumberInt();
			break;
		case TOKENR_MATERIAL_NAME:
		{
			std::string fileName = Parsing_String();
			m_MaterialData->m_MaterialName = fileName.c_str();
		}
		break;
		case TOKENR_MATERIAL_CLASS:
			break;
		case TOKENR_MATERIAL_AMBIENT:
		{
			Vector3 ambient = Parsing_ChangeNumberVector3();
			m_MaterialData->m_Material_Ambient.x = ambient.x;
			m_MaterialData->m_Material_Ambient.y = ambient.y;
			m_MaterialData->m_Material_Ambient.z = ambient.z;
			m_MaterialData->m_Material_Ambient.w = 1.0f;
		}
			break;
		case TOKENR_MATERIAL_DIFFUSE:
		{
			Vector3 diffuse = Parsing_ChangeNumberVector3();
			m_MaterialData->m_Material_Diffuse.x = diffuse.x;
			m_MaterialData->m_Material_Diffuse.y = diffuse.y;
			m_MaterialData->m_Material_Diffuse.z = diffuse.z;
			m_MaterialData->m_Material_Diffuse.w = 1.0f;
		}
			break;
		case TOKENR_MATERIAL_SPECULAR:
		{
			Vector3 specular = Parsing_ChangeNumberVector3();
			m_MaterialData->m_Material_Specular.x = specular.x;
			m_MaterialData->m_Material_Specular.y = specular.y;
			m_MaterialData->m_Material_Specular.z = specular.z;
			m_MaterialData->m_Material_Specular.w = 1.0f;
		}
			break;
		case TOKENR_MATERIAL_SHINE:
			break;
		case TOKENR_MATERIAL_SHINESTRENGTH:
			m_MaterialData->m_Material_Shininess = Parsing_NumberFloat();
			break;
		case TOKENR_MATERIAL_TRANSPARENCY:
			m_MaterialData->m_Material_Transparency = Parsing_NumberFloat();
			break;
		case TOKENR_MATERIAL_WIRESIZE:
		case TOKENR_MATERIAL_SHADING:
		case TOKENR_MATERIAL_XP_FALLOFF:
		case TOKENR_MATERIAL_SELFILLUM:
		case TOKENR_MATERIAL_FALLOFF:
		case TOKENR_MATERIAL_XP_TYPE:
			break;

			///----------------------------------
			/// Material Map
			///----------------------------------

		case TOKENR_MAP_NAME:
			m_materialmap->m_MapName = Parsing_String();
			break;
		case TOKENR_MAP_CLASS:
		case TOKENR_MAP_SUBNO:
		case TOKENR_MAP_AMOUNT:
			break;
		case TOKENR_BITMAP:
		{
			string filePath = Parsing_String();

			size_t indexSlash = filePath.rfind("\\") + 1;
			size_t indexDot = filePath.rfind(".");

			// 파일 임시경로..
			string fileRoute = "../Resource/Textures/";

			string fileName = filePath.substr(indexSlash, indexDot - indexSlash);
			fileRoute += filePath.substr(indexSlash, filePath.size() - indexSlash);
			m_materialmap->m_BitMap = fileRoute.c_str();
		}
		break;
		case TOKENR_MAP_TYPE:
		case TOKENR_UVW_U_OFFSET:
		case TOKENR_UVW_V_OFFSET:
		case TOKENR_UVW_U_TILING:
		case TOKENR_UVW_V_TILING:
		case TOKENR_UVW_ANGLE:
		case TOKENR_UVW_BLUR:
		case TOKENR_UVW_BLUR_OFFSET:
		case TOKENR_UVW_NOUSE_AMT:
		case TOKENR_UVW_NOISE_SIZE:
		case TOKENR_UVW_NOISE_LEVEL:
		case TOKENR_UVW_NOISE_PHASE:
		case TOKENR_BITMAP_FILTER:
			break;

			//--------------------
			// GEOMOBJECT
			//--------------------

		case TOKENR_GROUP:
			//	한 개의 그룹 시작
			break;

		case TOKENR_HELPEROBJECT:
			Create_OneMesh_to_list();
			m_OneMesh->m_Type = eHelperObject;
			break;

		case TOKENR_SHAPEOBJECT:
			Create_OneMesh_to_list();
			m_OneMesh->m_Type = eShape;
			break;

		case TOKENR_GEOMOBJECT:
			Create_OneMesh_to_list();
			m_OneMesh->m_Type = eGeomobject;
			break;
		case TOKENR_LIGHTOBJECT:
			Create_OneMesh_to_list();
			m_OneMesh->m_Type = eLight;
			break;
		case TOKENR_CAMERAOBJECT:
			Create_OneMesh_to_list();
			m_OneMesh->m_Type = eCamera;
			break;

		case TOKENR_NODE_NAME:
			// 어쩄든 지금은 오브젝트들을 구별 할 수 있는 유일한 값이다.
			// 모드에 따라 넣어야 할 곳이 다르다.
		{
			m_OneMesh->m_NodeName = Parsing_String();
		}
		break;

		case TOKENR_NODE_PARENT:
			// 현 노드의 부모 노드의 정보.
			// 일단 입력을 하고, 나중에 정리하자.
			m_OneMesh->m_ParentName = Parsing_String();
			break;

			///----------------------------------
			/// NODE_TM
			///----------------------------------

		case TOKENR_NODE_TM:
			break;

			// Camera, Light는 NodeTM이 두번 나오므로
			// 두번째는 일단 무시하자..

		case TOKENR_INHERIT_POS:
		case TOKENR_INHERIT_ROT:
		case TOKENR_INHERIT_SCL:
		case TOKENR_TM_ROW0:
			if (m_OneMesh->m_WorldTM.Right() == DirectX::SimpleMath::Vector3())
				m_OneMesh->m_WorldTM.Right(Parsing_ChangeNumberVector3());
			break;
		case TOKENR_TM_ROW1:
			if (m_OneMesh->m_WorldTM.Backward() == DirectX::SimpleMath::Vector3())
				m_OneMesh->m_WorldTM.Backward(Parsing_ChangeNumberVector3());
			break;
		case TOKENR_TM_ROW2:
			if (m_OneMesh->m_WorldTM.Up() == DirectX::SimpleMath::Vector3())
				m_OneMesh->m_WorldTM.Up(Parsing_ChangeNumberVector3());
			break;
		case TOKENR_TM_ROW3:
			if (m_OneMesh->m_WorldTM.Translation() == DirectX::SimpleMath::Vector3())
				m_OneMesh->m_WorldTM.Translation(Parsing_ChangeNumberVector3());
			break;
		case TOKENR_TM_POS:
		case TOKENR_TM_ROTAXIS:
		case TOKENR_TM_ROTANGLE:
		case TOKENR_TM_SCALE:
		case TOKENR_TM_SCALEAXIS:
		case TOKENR_TM_SCALEAXISANG:
			break;

			///----------------------------------
			/// Light
			///----------------------------------

		case TOKENR_LIGHT_TYPE:
		case TOKENR_LIGHT_SHADOWS:
		case TOKENR_LIGHT_USELIGHT:
		case TOKENR_LIGHT_SPOTSHAPE:
		case TOKENR_LIGHT_USEGLOBAL:
		case TOKENR_LIGHT_ABSMAPBIAS:
		case TOKENR_LIGHT_OVERSHOOT:
		case TOKENR_LIGHT_SETTINGS:
		case TOKENR_LIGHT_COLOR:
		case TOKENR_LIGHT_INTENS:
		case TOKENR_LIGHT_ASPECT:
		case TOKENR_LIGHT_HOTSPOT:
		case TOKENR_LIGHT_FALLOFF:
		case TOKENR_LIGHT_TDIST:
		case TOKENR_LIGHT_MAPBIAS:
		case TOKENR_LIGHT_MAPRANGE:
		case TOKENR_LIGHT_MAPSIZE:
		case TOKENR_LIGHT_RAYBIAS:
			break;

			///----------------------------------
			/// Camera
			///----------------------------------

		case TOKENR_CAMERA_TYPE:
		case TOKENR_CAMERA_SETTINGS:
		case TOKENR_CAMERA_NEAR:
		case TOKENR_CAMERA_FAR:
		case TOKENR_CAMERA_FOV:
		case TOKENR_CAMERA_TDIST:
			break;

			///----------------------------------
			/// Bone
			///----------------------------------
		case TOKENR_MESH_NUMBONE:
		{
			// 이게 있다면 이것은 Skinned Mesh라고 단정을 짓는다.
			if (Parsing_NumberInt() > 0)
			{
				m_OneMesh->m_MeshType = SKIN_MESH;
			}
		}
		break;
		case TOKENR_MESH_NUMSKINWEIGHT:
			break;

		case TOKENR_SKIN_INITTM:
			break;
		case TOKENR_BONE_PROPERTY:
			break;
		case TOKENR_BONE_LIST:
			break;
		case TOKENR_BONE:
		{
			Create_BoneData_to_list();
			m_Bone->m_BoneNumber = Parsing_NumberInt();
		}
		break;
		case TOKENR_BONE_NAME:
			m_Bone->m_BoneName = Parsing_String();

		case TOKENR_MESH_WVERTEXS:
			break;

		case TOKENR_MESH_WEIGHT:
			m_Index = Parsing_NumberInt();
			break;
		case TOKENR_BONE_BLENGING_WEIGHT:
		{
			int boneNum = Parsing_NumberInt();
			float boneWeight = Parsing_NumberFloat();

			m_OneMesh->m_VertexList[m_Index]->m_BoneIndices.push_back(boneNum);
			m_OneMesh->m_VertexList[m_Index]->m_BoneWeights.push_back(boneWeight);
		}
		break;

		///----------------------------------
		/// MESH
		///----------------------------------

		case TOKENR_MESH:
			break;
		case TOKENR_TIMEVALUE:
			break;
		case TOKENR_MESH_NUMVERTEX:
			// Vertex 총 개수 저장..
			m_OneMesh->m_Mesh_NumVertex = Parsing_NumberInt();
			break;

			/// MESH_VERTEX_LIST

		case TOKENR_MESH_VERTEX_LIST:
			// 이전에 넣어둔 Vertex 개수만큼 생성해두자..
		{
			for (int i = 0; i < m_OneMesh->m_Mesh_NumVertex; i++)
			{
				m_OneMesh->m_VertexList.push_back(new Vertex);
			}
		}
		break;
		case TOKENR_MESH_VERTEX:
		{
			m_Index = Parsing_NumberInt();
			m_OneMesh->m_VertexList[m_Index]->m_Indices = m_Index;
			m_OneMesh->m_VertexList[m_Index]->m_Pos = Parsing_ChangeNumberVector3();
		}
		break;

		case TOKENR_MESH_NUMFACES:
			// Face 총 개수 저장..
			m_OneMesh->m_Mesh_NumFaces = Parsing_NumberInt();
			break;

			/// MESH_FACE_LIST
		case TOKENR_MESH_FACE_LIST:
			// 이전에 넣어둔 Face 개수만큼 생성해두자..
			for (int i = 0; i < m_OneMesh->m_Mesh_NumFaces; i++)
			{
				m_OneMesh->m_MeshFace.push_back(new Face);
			}
			break;
		case TOKENR_MESH_FACE:
		{
			m_Index = Parsing_NumberInt();

			Parsing_String();
			m_OneMesh->m_MeshFace[m_Index]->m_VertexIndex[0] = Parsing_NumberInt();
			Parsing_String();
			m_OneMesh->m_MeshFace[m_Index]->m_VertexIndex[2] = Parsing_NumberInt();
			Parsing_String();
			m_OneMesh->m_MeshFace[m_Index]->m_VertexIndex[1] = Parsing_NumberInt();
		}
		break;

		case TOKENR_MESH_NUMCVFACES:
			break;
		case TOKENR_MESH_CFACELIST:
			break;
		case TOKENR_MESH_CFACE:
			break;
		case TOKENR_MESH_NORMALS:
			break;

		case TOKENR_MESH_FACENORMAL:
		{
			m_Index = Parsing_NumberInt();

			Parsing_ChangeNumberVector3();
			// 다음 토큰
			m_lexer->GetToken(m_TokenString);
			Parsing_NumberInt();
			m_OneMesh->m_MeshFace[m_Index]->m_NormalVertex[0] = Parsing_ChangeNumberVector3();
			// 다음 토큰
			m_lexer->GetToken(m_TokenString);
			Parsing_NumberInt();
			m_OneMesh->m_MeshFace[m_Index]->m_NormalVertex[2] = Parsing_ChangeNumberVector3();
			// 다음 토큰
			m_lexer->GetToken(m_TokenString);
			Parsing_NumberInt();
			m_OneMesh->m_MeshFace[m_Index]->m_NormalVertex[1] = Parsing_ChangeNumberVector3();
		}
		break;

		case TOKENR_MESH_VERTEXNORMAL:
			break;

			///----------------------------------
			/// MESH_TVERTLIST
			///----------------------------------

		case TOKENR_MESH_NUMTVERTEX:
			/// 블랜더의 경우 TVERTEX는 여러번 들어오므로 누적시켜주자..
		{
			m_OneMesh->m_Mesh_SumTVertex = m_OneMesh->m_Mesh_NumTVertex;
			m_OneMesh->m_Mesh_NumTVertex += Parsing_NumberInt();
		}
		break;

		case TOKENR_MESH_TVERTLIST:
			// 추가된 tVertex 개수만큼 생성..
			for (int i = m_OneMesh->m_Mesh_SumTVertex; i < m_OneMesh->m_Mesh_NumTVertex; i++)
			{
				m_OneMesh->m_Mesh_TVertex.push_back(new ParserData::TVertex);
			}
			break;
		case TOKENR_MESH_TVERT:
		{
			/// 누적 현재 인덱스 체크.. 
			m_Index = Parsing_NumberInt() + m_OneMesh->m_Mesh_SumTVertex;

			m_OneMesh->m_Mesh_TVertex[m_Index]->m_U = Parsing_NumberFloat();
			m_OneMesh->m_Mesh_TVertex[m_Index]->m_V = 1.0f - Parsing_NumberFloat();
			m_OneMesh->m_Mesh_TVertex[m_Index]->m_W = Parsing_NumberFloat();
		}
		break;
		case TOKENR_MESH_NUMTVFACES:
			break;
		case TOKENR_MESH_TFACELIST:
			break;
		case TOKENR_MESH_TFACE:
		{
			m_Index = Parsing_NumberInt();

			m_OneMesh->m_MeshFace[m_Index]->m_TFace[0] = Parsing_NumberInt();
			m_OneMesh->m_MeshFace[m_Index]->m_TFace[2] = Parsing_NumberInt();
			m_OneMesh->m_MeshFace[m_Index]->m_TFace[1] = Parsing_NumberInt();
		}
		break;

		///----------------------------------
		/// Animation
		///----------------------------------

		case TOKENR_TM_ANIMATION:
			Create_AnimationData_to_mesh(m_OneMesh);
			m_OneMesh->m_Type = eAnimation;
			m_OneMesh->m_Animation->m_AniData.push_back(new OneFrame);
			break;
		case TOKENR_CONTROL_POS_TRACK:
			break;
		case TOKENR_CONTROL_POS_SAMPLE:
		{
			m_Animation->m_AniData.back()->m_Time = (Parsing_NumberFloat() / m_scenedata.m_TicksPerFrame) - m_scenedata.m_FirstFrame;
			m_Animation->m_AniData.back()->m_Pos = Parsing_ChangeNumberVector3();
			break;
		}
		case TOKENR_CONTROL_ROT_TRACK:
			break;
		case TOKENR_CONTROL_ROT_SAMPLE:
		{
			float _time = 0;
			float _angle = 0;
			Vector3 _rot = Vector3();
			_time = (Parsing_NumberFloat() / m_scenedata.m_TicksPerFrame) - m_scenedata.m_FirstFrame;
			_rot = Parsing_ChangeNumberVector3();
			_angle = Parsing_NumberFloat();

			m_Animation->m_AniData.back()->m_Time = _time;

			Quaternion nowQT = XMQuaternionRotationAxis(_rot, _angle);

			// quaternion 값은 누적시켜주자..
			if (m_Animation->m_AniData.size() > 1)
			{
				m_Animation->m_AniData.back()->m_RotQt = XMQuaternionMultiply(m_Animation->m_AniData[m_Animation->m_AniData.size() - 2]->m_RotQt, nowQT);
			}
			else
			{
				m_Animation->m_AniData.back()->m_RotQt = nowQT;
			}
			break;
		}
		case TOKENR_CONTROL_SCALE_TRACK:
			break;
		case TOKENR_CONTROL_SCALE_SAMPLE:
		{
			m_Animation->m_AniData.back()->m_Scale = Parsing_ChangeNumberVector3();
			//m_animation->m_scale.push_back(new CAnimation_scl);
			//m_animation->m_scale.back()->m_time = (Parsing_NumberFloat() / m_OneMesh->m_scenedata.m_ticksperframe) - m_OneMesh->m_scenedata.m_firstframe;
			//m_animation->m_scale.back()->m_scale = Parsing_ChangeNumberVector3();
			//m_animation->m_scale.back()->m_scaleaxis = Parsing_ChangeNumberVector3();
			//m_animation->m_scale.back()->m_scaleaxisang = Parsing_NumberFloat();
			break;
		}
		case TOKENR_MATERIAL_REF:
		{
			int materialIndex = Parsing_NumberInt();

			// Mesh에 해당하는 Material Data 삽입
			m_OneMesh->m_MaterialData = m_MaterialList[materialIndex];
		}
		break;

		case TOKENR_WIREFRAME_COLOR:
			break;
		case TOKEND_END:
			// 아마도 이건 파일의 끝이 나타났을때인것 같은데. while을 탈출해야 하는데?

			//AfxMessageBox("파일의 끝을 본 것 같습니다!", MB_OK, NULL);
			TRACE("TRACE: 파싱중: 파일의 끝을 봤습니다!\n");
			return;

			break;

			/// 위의 아무것도 해당하지 않을때
		default:
			// 아무것도 하지 않는다.
			break;

		}	// switch()


		///-----------------------------------------------
		/// 안전 코드.
		i++;
		if (i > 1000000)
		{
			TRACE("루프를 백만번 돌았습니다!");
			return;
		}
		/// 안전 코드.
		///-----------------------------------------------

	}		// while()

	return;
}


int ASEParser::Parsing_NumberLong()
{
	LONG			token;
	LONG			tempNumber;

	token = m_lexer->GetToken(m_TokenString);	//ASSERT(token == TOKEND_NUMBER);
	tempNumber = strtoul(m_TokenString, NULL, 10);

	return			tempNumber;
}

float ASEParser::Parsing_NumberFloat()
{
	LONG			token;
	float			tempNumber;

	token = m_lexer->GetToken(m_TokenString);	//ASSERT(token == TOKEND_NUMBER);
	tempNumber = (float)atof(m_TokenString);

	return			tempNumber;
}

LPSTR ASEParser::Parsing_String()
{
	m_lexer->GetToken(m_TokenString);

	return m_TokenString;
}

int ASEParser::Parsing_NumberInt() {

	LONG			token;
	int				tempNumber;

	token = m_lexer->GetToken(m_TokenString);	//ASSERT(token == TOKEND_NUMBER);
	tempNumber = (int)atoi(m_TokenString);

	return			tempNumber;
}

Vector3 ASEParser::Parsing_ChangeNumberVector3()
{
	LONG	token;
	Vector3	tempVector3;

	token = m_lexer->GetToken(m_TokenString);
	tempVector3.x = (float)atof(m_TokenString);
	token = m_lexer->GetToken(m_TokenString);
	tempVector3.z = (float)atof(m_TokenString);
	token = m_lexer->GetToken(m_TokenString);
	tempVector3.y = (float)atof(m_TokenString);

	return tempVector3;
}

void ASEParser::Create_OneMesh_to_list()
{
	ASEMesh* temp = new ASEMesh;
	m_OneMesh = temp;
	m_MeshList.push_back(m_OneMesh);
}

void ASEParser::Create_MaterialData_to_list()
{
	CMaterial* temp = new CMaterial;
	m_MaterialData = temp;
	m_MaterialList.push_back(m_MaterialData);
}

void ASEParser::Create_DiffuseMap_to_list()
{
	MaterialMap* temp = new MaterialMap;
	m_materialmap = temp;
	m_MaterialData->m_MapList.push_back(temp);
	m_MaterialData->m_DiffuseMap = temp;
	m_MaterialData->m_TextureBind |= DIFFUSE_TEXTURE;
}

void ASEParser::Create_BumpMap_to_list()
{
	MaterialMap* temp = new MaterialMap;
	m_materialmap = temp;
	m_MaterialData->m_MapList.push_back(temp);
	m_MaterialData->m_NormalMap = temp;
	m_MaterialData->m_TextureBind |= NORMAL_TEXTURE;
}

void ASEParser::Create_SpecularMap_to_list()
{
	MaterialMap* temp = new MaterialMap;
	m_materialmap = temp;
	m_MaterialData->m_MapList.push_back(temp);
	m_MaterialData->m_ORMMap = temp;
	m_MaterialData->m_TextureBind |= ORM_TEXTURE;
}

void ASEParser::Create_ShineMap_to_list()
{
	MaterialMap* temp = new MaterialMap;
	m_materialmap = temp;
	m_MaterialData->m_MapList.push_back(temp);
	m_MaterialData->m_EmissiveMap = temp;
	m_MaterialData->m_TextureBind |= EMISSIVE_TEXTURE;
}

void ASEParser::Create_AnimationData_to_mesh(Mesh* nowMesh)
{
	OneAnimation* temp = new OneAnimation;
	m_Animation = temp;
	m_Animation->m_TicksPerFrame = m_scenedata.m_TicksPerFrame;
	m_Animation->m_StartFrame = m_scenedata.m_FirstFrame;
	m_Animation->m_EndFrame = m_scenedata.m_LastFrame;
	nowMesh->m_Animation = m_Animation;
	m_IsAnimation = true;
}

void ASEParser::Create_BoneData_to_list()
{
	Bone* temp = new Bone;
	m_Bone = temp;
	m_OneMesh->m_BoneList.push_back(temp);
}