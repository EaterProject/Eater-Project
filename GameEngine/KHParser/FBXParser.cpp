#define SAFE_DELETE(x) { if(x != nullptr) {delete x; x = nullptr;} }

#include "ModelParser.h"

#define FBXSDK_SHARED
#include <fbxsdk.h>
#pragma comment(lib, "libfbxsdk")

#include "BoneWeights.h"
#include "FBXParser.h"

using namespace fbxsdk;
using namespace ParserData;
using namespace DirectX;
using namespace SimpleMath;

void FBXParser::Initialize()
{
	// FBX SDK Manager ����..
	pManager = FbxManager::Create();

	if (!pManager)
		throw std::exception("error: unable to create FBX manager!\n");

	// Scene ����..
	pScene = FbxScene::Create(pManager, "My Scene");

	// IOSettings ��ü ���� �� ����
	FbxIOSettings* ios = FbxIOSettings::Create(pManager, IOSROOT);
	pManager->SetIOSettings(ios);

	FbxString lExtension = "dll";
	FbxString lPath = FbxGetApplicationDirectory();
	pManager->LoadPluginsDirectory(lPath.Buffer(), lExtension.Buffer());

	// FbxImporter ��ü ����
	pImporter = FbxImporter::Create(pManager, "");

	// Convert ��ü ����
	pConverter = new FbxGeometryConverter(pManager);

	if (!pScene)
		throw std::exception("error: unable to create FBX scene\n");
}

void FBXParser::Release()
{
	pImporter->Destroy();
	SAFE_DELETE(pConverter);
	pScene->Destroy();
	pManager->Destroy();

	for (CModel* model : m_ModelList)
	{
		for (CMesh* mesh : model->m_MeshList)
		{
			for (CFace* face : mesh->m_MeshFace)
			{
				SAFE_DELETE(face);
			}
			for (CVertex* vertex : mesh->m_VertexList)
			{
				SAFE_DELETE(vertex);
			}
			for (CIndexList* index : mesh->m_IndexList)
			{
				SAFE_DELETE(index);
			}
			mesh->m_MeshFace.clear();
			mesh->m_VertexList.clear();
			mesh->m_IndexList.clear();
		}
	}
}

ParserData::CModel* FBXParser::LoadModel(std::string fileName, UINT state)
{
	// Parsing �ɼ� ����..
	m_ParsingMode = state;

	// Scene ����..
	SceneSetting(fileName);

	// Scene���� RootNode ��������..
	fbxsdk::FbxNode* pRootNode = pScene->GetRootNode();

	// Ȥ�ö� RootNode�� ���� ��츦 ���..
	if (pRootNode == nullptr) return nullptr;

	/// Material Data Loading..
	LoadMaterial();

	/// Bone Data Loading..
	LoadNode(pRootNode, FbxNodeAttribute::eSkeleton);

	/// Mesh Data Loading..
	LoadNode(pRootNode, FbxNodeAttribute::eMesh);

	/// Mesh Data Optimizing..
	OptimizeData();

	// �ʿ���� ������ ����..
	ResetData();

	return m_Model;
}

void FBXParser::SceneSetting(std::string fileName)
{
	// Model ����..
	CreateModel();

	// ���� �̸��� �ɼ� ����..
	fbxFileName = fileName;

	if (!pImporter->Initialize(fbxFileName.c_str(), -1, pManager->GetIOSettings()))
		throw std::exception("error: initialize importer\n");

	// fbx ���� ������ Scene���� �����´�..
	pImporter->Import(pScene);

	// Scene ���� �ִ� �����͵��� ������ �������ش�..
	if (m_ParsingMode & SCALING)
	{
		FbxSystemUnit lFbxFileSystemUnit = pScene->GetGlobalSettings().GetSystemUnit();
		FbxSystemUnit lFbxOriginSystemUnit = pScene->GetGlobalSettings().GetOriginalSystemUnit();
		double factor = lFbxOriginSystemUnit.GetScaleFactor();

		const FbxSystemUnit::ConversionOptions lConversionOptions =
		{
		  true,
		  true,
		  true,
		  true,
		  true,
		  true
		};
		lFbxFileSystemUnit.m.ConvertScene(pScene, lConversionOptions);
		lFbxOriginSystemUnit.m.ConvertScene(pScene, lConversionOptions);
	}

	// Scene ������ �ﰢ��ȭ �� �� �ִ� ��� ��带 �ﰢ��ȭ ��Ų��..
	// 3D Max �ȿ��� Editable poly ���¶�� �� �۾��� ���ؾ� �Ѵ�..
	pConverter->Triangulate(pScene, true, true);
}

void FBXParser::CreateModel()
{
	m_Model = new CModel();
	m_ModelList.push_back(m_Model);
}

void FBXParser::ResetData()
{
	pMesh = nullptr;
	fbxMaterials.clear();
	fbxGeometries.clear();

	m_MaterialData = nullptr;
	m_OneAnimation = nullptr;

	m_AllBoneList.clear();
}

void FBXParser::LoadMaterial()
{
	// �ִϸ��̼Ǹ� �������..
	if (m_ParsingMode & ANIMATION_ONLY) return;

	// Scene�� �����ϴ� Material ������ŭ ����..
	int mcount = pScene->GetMaterialCount();
	for (int matIndex = 0; matIndex < pScene->GetMaterialCount(); matIndex++)
	{
		FbxSurfaceMaterial* material = pScene->GetMaterial(matIndex);
		m_MaterialData = new CMaterial();
		m_MaterialData->m_MaterialNumber = matIndex;

		// Material Data ����..
		SetMaterial(material);

		// Texture Data ����..
		//SetTextures(material);
		SetTexture(material, FbxSurfaceMaterial::sDiffuse);
		SetTexture(material, FbxSurfaceMaterial::sNormalMap);
		SetTexture(material, FbxSurfaceMaterial::sAmbient);
		SetTexture(material, FbxSurfaceMaterial::sEmissive);
		SetTexture(material, FbxSurfaceMaterial::sTransparencyFactor);

		m_Model->m_MaterialList.push_back(m_MaterialData);
		fbxMaterials.push_back(material);

		m_MaterialData = nullptr;
	}
}

void FBXParser::LoadNode(fbxsdk::FbxNode* node, fbxsdk::FbxNodeAttribute::EType attribute)
{
	FbxNodeAttribute* nodeAttribute = node->GetNodeAttribute();
	const char* name = node->GetName();
	if (nodeAttribute != nullptr)
	{
		FbxNodeAttribute::EType attributeType = nodeAttribute->GetAttributeType();
		if (attributeType == attribute)
		{
			switch (nodeAttribute->GetAttributeType())
			{
			case FbxNodeAttribute::eSkeleton:
				ProcessSkeleton(node);
				LoadAnimation(node);
				break;
			case FbxNodeAttribute::eMesh:
				ProcessMesh(node);
				LoadAnimation(node);
				break;
			case FbxNodeAttribute::eMarker:
			case FbxNodeAttribute::eNurbs:
			case FbxNodeAttribute::ePatch:
			case FbxNodeAttribute::eCamera:
			case FbxNodeAttribute::eLight:
			case FbxNodeAttribute::eNull:
				break;
			default:
				break;
			}
		}
	}

	int count = node->GetChildCount();

	// ��� ����..
	for (int i = 0; i < node->GetChildCount(); ++i)
	{
		LoadNode(node->GetChild(i), attribute);
	}
}

void FBXParser::LoadAnimation(fbxsdk::FbxNode* node)
{
	float frameRate = (float)FbxTime::GetFrameRate(pScene->GetGlobalSettings().GetTimeMode());

	FbxTakeInfo* takeInfo = pImporter->GetTakeInfo(0);

	if (takeInfo == nullptr) return;

	FbxTime start = FbxTime(FbxLongLong(0x7fffffffffffffff));
	FbxTime stop = FbxTime(FbxLongLong(-0x7fffffffffffffff));

	FbxTimeSpan span = takeInfo->mLocalTimeSpan;

	double tempStart = span.GetStart().GetSecondDouble();
	double tempStop = span.GetStop().GetSecondDouble();

	if (tempStart < tempStop)
	{
		// �����ð� ���� �� �� �������� ��������� keyFrames�� �����
		m_KeyFrames = (int)((tempStop - tempStart) * (double)frameRate);
		m_TickFrame = (float)(tempStop - tempStart) / (float)m_KeyFrames;
		m_StartTime = (int)(tempStart)*m_KeyFrames;

		ProcessAnimation(node);
	}
}

void FBXParser::ProcessSkeleton(fbxsdk::FbxNode* node)
{
	// �ִϸ��̼Ǹ� ���� ���..
	if (m_ParsingMode & ANIMATION_ONLY) return;

	pMesh = node->GetMesh();

	// ���ο� Mesh ����..
	CreateMesh();

	m_OneMesh->m_NodeName = node->GetName();
	m_OneMesh->m_MeshType = BONE_MESH;

	// �� Node Parent ã��..
	const char* parentName = node->GetParent()->GetName();
	CMesh* parentMesh = FindMesh(parentName);
	m_OneMesh->m_ParentName = parentName;

	// �θ��� Mesh�� �����Ѵٸ� ChildList�� �߰�..
	if (parentMesh == nullptr)
	{
		m_OneMesh->m_TopNode = true;
	}
	else
	{
		m_OneMesh->m_Parent = parentMesh;
		parentMesh->m_ChildList.push_back(m_OneMesh);
	}

	// Node TRS ����..
	SetTransform(node);

	FbxSkeleton* fbxSkeleton = node->GetSkeleton();

	if (fbxSkeleton == nullptr) return;

	// �θ� Bone Index �ʱⰪ..
	// �θ� Bone Index�� -1 �̸� �ֻ��� Root Node..
	int parentBoneIndex = -1;

	// �ֻ��� ���(�θ���)���� Ȯ��..
	FbxNode* parentNode = node->GetParent();

	if (m_OneMesh->m_TopNode)
	{
		std::string nodeName = parentNode->GetName();
		parentBoneIndex = FindBoneIndex(nodeName);
	}

	// �ش� Bone Index ����..
	m_OneMesh->m_BoneIndex = (int)m_AllBoneList.size();

	// Bone Count �߰�..
	m_Model->m_BoneCount++;

	// ���ο� Bone ����..
	m_AllBoneList.push_back(m_OneMesh);
}

void FBXParser::ProcessMesh(fbxsdk::FbxNode* node)
{
	// �ִϸ��̼Ǹ� ���� ���..
	if (m_ParsingMode & ANIMATION_ONLY) return;

	pMesh = node->GetMesh();

	fbxsdk::FbxGeometry* pGeo = node->GetGeometry();

	// ���ο� Mesh ����..
	CreateMesh();

	// Instancing Mesh üũ..
	bool instancing = false;
	for (int i = 0; i < fbxGeometries.size(); i++)
	{
		if (pGeo == fbxGeometries[i])
		{
			// Instancing Mesh
			m_OneMesh->m_MeshIndex = i;
			instancing = true;
			break;
		}
	}

	if (instancing == false)
	{
		m_OneMesh->m_MeshIndex = (UINT)fbxGeometries.size();
		fbxGeometries.push_back(pGeo);
	}

	m_OneMesh->m_NodeName = node->GetName();

	// �� Node Parent ã��..
	const char* parentName = node->GetParent()->GetName();

	// �θ� Mesh ã��..
	CMesh* parentMesh = FindMesh(parentName);
	m_OneMesh->m_ParentName = parentName;

	// �θ��� Mesh�� �����Ѵٸ� ChildList�� �߰�..
	if (parentMesh == nullptr)
	{
		m_OneMesh->m_TopNode = true;
	}
	else
	{
		m_OneMesh->m_Parent = parentMesh;
		parentMesh->m_ChildList.push_back(m_OneMesh);
	}

	// Node TRS ����..
	SetTransform(node);

	// Mesh Vertex �� ����..
	int vertexTotalCount = pMesh->GetControlPointsCount();
	// Mesh Face �� ����..
	int polygonTotalCount = pMesh->GetPolygonCount();

	// ���� Vertex ������ ������� ó������ �ʱ� ����..
	if (vertexTotalCount < 1) return;

	// Vertex ������ŭ BoneWeight ����..
	// �ش� Vertex Index�� ������ Index�� ����ġ ��, Bone Index Data ����..
	std::vector<BoneWeights> boneWeights(vertexTotalCount);
	bool isSkin = ProcessBoneWeights(node, boneWeights);

	// Bone Data ���� ����� ���� Skinning Object �Ǻ�..
	if (isSkin)
	{
		m_OneMesh->m_MeshType = SKIN_MESH;
		m_Model->m_SkinCount++;
	}
	else
	{
		m_Model->m_StaticCount++;
	}

	// ���ο� ���ؽ� ����..
	CreateVertex(pMesh, boneWeights, vertexTotalCount);

	int vertexCount = 0; // ������ ����
	for (int pi = 0; pi < polygonTotalCount; pi++)
	{
		// Polygon ������ŭ Face ����..
		m_OneMesh->m_MeshFace.push_back(new CFace);

		CFace* nowFace = m_OneMesh->m_MeshFace[pi];

		for (int vi = 0; vi < 3; vi++)
		{
			int vertexIndex = pMesh->GetPolygonVertex(pi, vi);
			int uvIndex = pMesh->GetTextureUVIndex(pi, vi);

			if (vertexIndex < 0 || vertexIndex >= vertexTotalCount)	continue;

			Vector3 fbxNormal = GetNormal(pMesh, vertexIndex, vertexCount);
			Vector2 fbxUV = GetUV(pMesh, vertexIndex, uvIndex);

			// �ϴ� �ϳ��� Mesh �� �ϳ��� Material�� �����ϰ� ����..
			if (m_OneMesh->m_MaterialData == nullptr)
				LinkMaterialByPolygon(pMesh, pi, vertexIndex);

			// Face Vertex Index Data
			nowFace->m_VertexIndex[vi] = vertexIndex;
			// Face Vertex Normal Data
			nowFace->m_NormalVertex[vi] = fbxNormal;
			// Face Vertex UV Data
			nowFace->m_UVvertex[vi] = fbxUV;

			vertexCount++;
		}
	}
}


bool FBXParser::ProcessBoneWeights(fbxsdk::FbxNode* node, std::vector<BoneWeights>& meshBoneWeights)
{
	FbxMesh* mesh = node->GetMesh();

	int deformerCount = mesh->GetDeformerCount();

	// DeformerCount�� 1���� ������ Bone Data�� ���ٰ� ����..
	if (deformerCount < 1) return false;

	for (int i = 0; i < deformerCount; ++i)
	{
		FbxDeformer* deformer = mesh->GetDeformer(i);

		if (deformer == nullptr) continue;

		if (deformer->GetDeformerType() == FbxDeformer::eSkin)
		{
			FbxSkin* skin = reinterpret_cast<FbxSkin*>(mesh->GetDeformer(i, FbxDeformer::eSkin));

			if (skin == nullptr) continue;

			FbxCluster::ELinkMode linkMode = FbxCluster::ELinkMode::eNormalize;

			int clusterCount = skin->GetClusterCount();

			// Skin Mesh üũ..
			CMesh* skinMesh = FindMesh(node->GetName());

			// Bone ������ŭ List Size ����..
			skinMesh->m_BoneTMList.resize(m_AllBoneList.size());
			skinMesh->m_BoneMeshList.resize(m_AllBoneList.size());

			for (int clusterIndex = 0; clusterIndex < clusterCount; clusterIndex++)
			{
				FbxCluster* cluster = skin->GetCluster(clusterIndex);
				if (cluster == nullptr) continue;

				linkMode = cluster->GetLinkMode();
				FbxNode* pLinkNode = cluster->GetLink();

				if (pLinkNode == nullptr) continue;

				std::string lineNodeName = pLinkNode->GetName();

				// Bone Mesh üũ..
				CMesh* boneMesh = FindMesh(lineNodeName);

				// �ش� Bone Mesh�� �������..
				if (boneMesh == nullptr) continue;

				// Bone Index üũ..
				int boneIndex = FindBoneIndex(lineNodeName);


				FbxAMatrix matClusterTransformMatrix;
				FbxAMatrix matClusterLinkTransformMatrix;

				const FbxVector4 lT = node->GetGeometricTranslation(FbxNode::eDestinationPivot);
				const FbxVector4 lR = node->GetGeometricRotation(FbxNode::eDestinationPivot);
				const FbxVector4 lS = node->GetGeometricScaling(FbxNode::eDestinationPivot);

				FbxAMatrix geometryTransform = FbxAMatrix(lT, lR, lS);

				cluster->GetTransformMatrix(matClusterTransformMatrix);
				cluster->GetTransformLinkMatrix(matClusterLinkTransformMatrix);

				// Bone Matrix ����..
				Matrix clusterMatrix = ConvertMatrix(matClusterTransformMatrix);
				Matrix clusterlinkMatrix = ConvertMatrix(matClusterLinkTransformMatrix);
				Matrix geometryMatrix = ConvertMatrix(geometryTransform);

				Matrix offsetMatrix = clusterMatrix * clusterlinkMatrix.Invert() * geometryMatrix;

				// �ش� Bone Index�� Bone Offset & Mesh Data ����..
				skinMesh->m_BoneTMList[boneIndex] = offsetMatrix;
				skinMesh->m_BoneMeshList[boneIndex] = boneMesh;

				int c = cluster->GetControlPointIndicesCount();
				for (int j = 0; j < cluster->GetControlPointIndicesCount(); ++j)
				{
					int index = cluster->GetControlPointIndices()[j];
					double weight = cluster->GetControlPointWeights()[j];

					if (weight == 0)
					{
						continue;
					}
					meshBoneWeights[index].AddBoneWeight(boneIndex, (float)weight);
				}
			}

			switch (linkMode)
			{
			case FbxCluster::eNormalize:
			{
				// ����ġ ���� 1.0���� ����� �۾�..
				for (int i = 0; i < (int)meshBoneWeights.size(); ++i)
					meshBoneWeights[i].Normalize();
			}
			break;
			case FbxCluster::eAdditive:
				break;
			case FbxCluster::eTotalOne:
				break;
			}
		}
	}

	return true;
}

void FBXParser::ProcessAnimation(fbxsdk::FbxNode* node)
{
	// �ִϸ��̼Ǹ� �������..
	if (m_ParsingMode & ANIMATION_ONLY)
	{
		FbxMesh* mesh = node->GetMesh();

		if (mesh != nullptr)
		{
			int deformerCount = mesh->GetDeformerCount();

			// DeformerCount�� 0���� ũ�� Skinning Mesh..
			if (deformerCount > 0) return;
		}
	}
	else
	{
		// ���� ��Ű�� ������Ʈ��� �ִϸ��̼� �����ʹ� Bone�� ����Ǿ� �����Ƿ�..
		if (m_OneMesh->m_MeshType == SKIN_MESH) return;
	}

	FbxNodeAttribute* nodeAttribute = node->GetNodeAttribute();
	if (nodeAttribute != nullptr)
	{
		// �� Node Parent ã��..
		const char* parentName = node->GetParent()->GetName();
		CMesh* parentMesh = FindMesh(parentName);

		// �θ��� Mesh�� �������� ������ TopNode List�� �߰�..
		if (parentMesh == nullptr)
		{
			m_Model->m_TopNodeList.push_back(true);
		}
		else
		{
			m_Model->m_TopNodeList.push_back(false);
		}

		// ���ο� Animaiton Data ����..
		m_OneAnimation = new CAnimation();

		// Animation ����(�� �ε����� �ִϸ��̼� �ε��� ��ġ)..
		m_Model->m_AnimationList.push_back(m_OneAnimation);

		// Animation ������ �������..
		m_Model->m_isAnimation = true;

		// �� ������ ��� �ð�..
		m_OneAnimation->m_TicksPerFrame = m_TickFrame;

		// Animation ���� ������..
		m_OneAnimation->m_StartFrame = m_StartTime;
		m_OneAnimation->m_EndFrame = m_KeyFrames - 1;

		// Animation �� ������..
		m_OneAnimation->m_TotalFrame = m_KeyFrames;

		// Animation Data ����..
		FbxTime::EMode timeMode = pScene->GetGlobalSettings().GetTimeMode();
		for (FbxLongLong index = 0; index < m_OneAnimation->m_TotalFrame; index++)
		{
			FbxTime takeTime;
			takeTime.SetFrame(m_OneAnimation->m_StartFrame + index, timeMode);

			// Local Transform = �θ� Bone�� Global Transform�� Inverse Transform * �ڽ� Bone�� Global Transform
			FbxAMatrix nodeTransform = node->EvaluateLocalTransform(takeTime);

			DirectX::SimpleMath::Matrix nodeTRS = ConvertMatrix(nodeTransform);

			XMVECTOR scale;
			XMVECTOR rot;
			XMVECTOR pos;

			XMMatrixDecompose(&scale, &rot, &pos, nodeTRS);

			CFrame* newAni = new CFrame;

			newAni->m_Time = (float)index;
			newAni->m_Pos = DirectX::SimpleMath::Vector3(pos);
			newAni->m_RotQt = Quaternion(rot);
			newAni->m_Scale = DirectX::SimpleMath::Vector3(scale);

			m_OneAnimation->m_AniData.push_back(newAni);
		}

		// �ش� Mesh�� �ִϸ��̼� ����..
		if ((m_ParsingMode & ANIMATION_ONLY) != ANIMATION_ONLY)
		{
			m_OneMesh->m_Animation = m_OneAnimation;
		}
	}
}

void FBXParser::OptimizeData()
{
	// �ִϸ��̼Ǹ� �������..
	if (m_ParsingMode & ANIMATION_ONLY) return;

	// ���� �����͸� �������..
	if (m_ParsingMode & ORIGIN_ONLY)
	{
		for (unsigned int i = 0; i < m_Model->m_MeshList.size(); i++)
		{
			SetIndex(m_Model->m_MeshList[i]);
		}

		return;
	}

	// Optimize Data
	for (unsigned int i = 0; i < m_Model->m_MeshList.size(); i++)
	{
		OptimizeVertex(m_Model->m_MeshList[i]);
		//RecombinationTM(m_Model->m_MeshList[i]);
	}
}

void FBXParser::OptimizeVertex(ParserData::CMesh* pMesh)
{
	if (pMesh->m_VertexList.empty()) return;

	int orgin_VertexCount = (int)pMesh->m_VertexList.size();
	bool new_VertexSet = false;
	size_t resize_VertexIndex = pMesh->m_VertexList.size();

	// ���� Face���� �����ϴ� 3���� Vertex ��..
	for (unsigned int i = 0; i < pMesh->m_MeshFace.size(); i++)
	{
		CFace* nowFace = pMesh->m_MeshFace[i];

		for (int j = 0; j < 3; j++)
		{
			unsigned int vertexIndex = nowFace->m_VertexIndex[j];

			CVertex* nowVertex = pMesh->m_VertexList[vertexIndex];
			Vector3 nowNormal = nowFace->m_NormalVertex[j];
			Vector2 nowUV = nowFace->m_UVvertex[j];
			int nowIndex = nowFace->m_VertexIndex[j];

			// �ؽ�ó�� �ְ�, �������� �ʾ����� �ؽ�ó u,v ����..
			if (nowVertex->m_IsTextureSet == false)
			{
				nowVertex->m_UV = nowUV;
				nowVertex->m_IsTextureSet = true;
			}

			// ���� �ε��� �븻�� �˻�ÿ� �־��ְ� ����..
			if (nowVertex->m_IsNormalSet == false)
			{
				nowVertex->m_Normal = nowNormal;
				nowVertex->m_IsNormalSet = true;
			}

			// Normal, U, V ���� �Ѱ��� �ٸ��� Vertex ���� ����..
			if (nowVertex->m_Normal != nowNormal)
			{
				new_VertexSet = true;
			}

			if (nowVertex->m_UV != nowUV)
			{
				new_VertexSet = true;
			}

			if (new_VertexSet)
			{
				// �߰��� Vertex�� �ִٸ� üũ..
				if (resize_VertexIndex > orgin_VertexCount)
				{
					for (size_t k = orgin_VertexCount; k < resize_VertexIndex; k++)
					{
						CVertex* checkVertex = pMesh->m_VertexList[k];

						// ���� �߰��� Vertex�� ������ �����͸� �����ִ� Face ���� Vertex Index ����..
						if (checkVertex->m_Indices == nowIndex)
						{
							if (checkVertex->m_Normal == nowNormal)
							{
								if (checkVertex->m_UV == nowUV)
								{
									nowFace->m_VertexIndex[j] = (int)k;
									new_VertexSet = false;
									break;
								}
							}
						}
					}
				}
			}

			// ���� �߰��ؾ��� Vertex..
			if (new_VertexSet == true)
			{
				CVertex* newVertex = new CVertex();
				newVertex->m_Pos = nowVertex->m_Pos;
				newVertex->m_Indices = nowVertex->m_Indices;
				newVertex->m_Normal = nowNormal;
				newVertex->m_BoneIndices = nowVertex->m_BoneIndices;
				newVertex->m_BoneWeights = nowVertex->m_BoneWeights;
				newVertex->m_IsNormalSet = true;

				newVertex->m_UV = nowUV;
				newVertex->m_IsTextureSet = true;

				pMesh->m_VertexList.push_back(newVertex);
				nowFace->m_VertexIndex[j] = (int)resize_VertexIndex;
				resize_VertexIndex++;
				new_VertexSet = false;
			}
		}
	}

	// Tanget �� ����..
	for (unsigned int i = 0; i < pMesh->m_MeshFace.size(); i++)
	{
		int index0 = pMesh->m_MeshFace[i]->m_VertexIndex[0];
		int index1 = pMesh->m_MeshFace[i]->m_VertexIndex[1];
		int index2 = pMesh->m_MeshFace[i]->m_VertexIndex[2];

		CVertex* vertex0 = pMesh->m_VertexList[index0];
		CVertex* vertex1 = pMesh->m_VertexList[index1];
		CVertex* vertex2 = pMesh->m_VertexList[index2];

		DirectX::SimpleMath::Vector3 ep1 = vertex1->m_Pos - vertex0->m_Pos;
		DirectX::SimpleMath::Vector3 ep2 = vertex2->m_Pos - vertex0->m_Pos;

		DirectX::SimpleMath::Vector2 uv1 = { vertex1->m_UV.x - vertex0->m_UV.x,
											 vertex1->m_UV.y - vertex0->m_UV.y };
		DirectX::SimpleMath::Vector2 uv2 = { vertex2->m_UV.x - vertex0->m_UV.x,
											 vertex2->m_UV.y - vertex0->m_UV.y };

		float value = uv1.x * uv2.y - uv2.x * uv1.y;
		float den = 0.0f;

		if (value != 0.0f)
		{
			den = 1.0f / value;
		}
		
		// ���� �ȼ� ���̴����� ������ ���� T, B, N�� ��µ�
		// �ȼ� ���̴� ���� ������ ���ؽ� ���̴��� ���꿡 ���� ȣ�� Ƚ���� ������ �ٸ��� ũ�Ƿ� ���ϰ� �� �� �ִ�..
		// �������� �ȼ��� ���� �ȼ� ���̴� ���� �ƴϸ� ������ ���� ������ �ȼ� ���̴����� ������ �Ѵٰ� �Ѵ�..
		/// ���� ��������� ���ؽ� ���̴��� �Űܵ�
		DirectX::SimpleMath::Vector3 tangent = (ep1 * uv2.y - ep2 * uv1.y) * den;

		// ���� ������ ���� �����Ͽ� ���̴����� ��հ��� ����ϵ��� ����..
		vertex0->m_Tanget += tangent;
		vertex1->m_Tanget += tangent;
		vertex2->m_Tanget += tangent;
	}

	for (unsigned int i = 0; i < pMesh->m_VertexList.size(); i++)
	{
		pMesh->m_VertexList[i]->m_Tanget.Normalize();
	}

	SetIndex(pMesh);
}

void FBXParser::SetIndex(ParserData::CMesh* pMesh)
{
	// �ε����� �׳� ����
	for (unsigned int i = 0; i < pMesh->m_MeshFace.size(); i++)
	{
		pMesh->m_IndexList.push_back(new CIndexList);

		CIndexList* nowIndexList = pMesh->m_IndexList[i];
		CFace* nowFace = pMesh->m_MeshFace[i];

		for (unsigned int j = 0; j < 3; j++)
		{
			switch (j)
			{
			case 0:
				nowIndexList->m_Index[j] = nowFace->m_VertexIndex[0];
				break;
			case 1:
				nowIndexList->m_Index[j] = nowFace->m_VertexIndex[2];
				break;
			case 2:
				nowIndexList->m_Index[j] = nowFace->m_VertexIndex[1];
				break;
			default:
				break;
			}
		}
	}

	pMesh->m_MeshFace.clear();
}

void FBXParser::RecombinationTM(ParserData::CMesh* pMesh)
{
	/// Negative Scale Check
	// 3D Max�� �۾��� Mirroring���� ���� Scale���� ������ ���� ��츦 Negative Scale �̶�� �Ѵ�
	// ó���� ������ ��� Normal ó���� Animation ó���ÿ� ������ �ȴ�
	// Rotaion�� ��Ÿ���� �� Row1 ~ Row3 �� ������ ���� �� ���� �ٸ� �� ���� �����Ͽ� �����̸� ���� �ݴ��̰� ����̸� ���� ��ġ�Ѵ�
	//
	XMVECTOR crossVec = XMVector3Cross(pMesh->m_WorldTM.Right(), pMesh->m_WorldTM.Up());
	XMVECTOR dotVec = XMVector3Dot(crossVec, pMesh->m_WorldTM.Backward());

	XMFLOAT3 resultVec;

	XMStoreFloat3(&resultVec, dotVec);

	return;

	// �� ������ �ٸ� ��츸 ó��������..
	if (resultVec.x < 0 || resultVec.y < 0 || resultVec.z < 0)
	{
		XMVECTOR nodeScale;
		XMVECTOR nodetRot;
		XMVECTOR nodePos;

		// ���� ��Ʈ�������� �и�..
		XMMatrixDecompose(&nodeScale, &nodetRot, &nodePos, pMesh->m_WorldTM);

		// ������ ������..
		nodeScale *= -1;

		// �븻�� ������..
		for (auto& k : pMesh->m_VertexList)
		{
			k->m_Normal *= -1;
		}

		/// �ؽ��ĳ� ���� �����͵� ��ȯ�� ���־�� �ұ�..?

		// ��Ʈ���� ������..
		pMesh->m_WorldTM = XMMatrixScalingFromVector(nodeScale) * XMMatrixRotationQuaternion(nodetRot) * XMMatrixTranslationFromVector(nodePos);
	}
}

DirectX::SimpleMath::Vector3 FBXParser::GetPos(fbxsdk::FbxMesh* mesh, int vertexIndex)
{
	FbxVector4 fbxPos;

	fbxPos = mesh->GetControlPointAt(vertexIndex);

	return ConvertVector3(fbxPos);
}

DirectX::SimpleMath::Vector3 FBXParser::GetNormal(fbxsdk::FbxMesh* mesh, int vertexIndex, int vertexCount)
{
	if (mesh->GetElementNormalCount() < 1) return DirectX::SimpleMath::Vector3();

	const FbxGeometryElementNormal* vertexNormal = mesh->GetElementNormal();

	FbxVector4 fbxNormal;

	switch (vertexNormal->GetMappingMode()) // ���� ��� 
	{
	case FbxGeometryElement::eByControlPoint: // control point mapping 
	{
		switch (vertexNormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			fbxNormal = vertexNormal->GetDirectArray().GetAt(vertexIndex);
		}
		break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int m_Index = vertexNormal->GetIndexArray().GetAt(vertexIndex); // �ε����� ���´�. 
			fbxNormal = vertexNormal->GetDirectArray().GetAt(m_Index);
		}
		break;
		}
	}
	break;

	case FbxGeometryElement::eByPolygonVertex:
	{
		switch (vertexNormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			fbxNormal = vertexNormal->GetDirectArray().GetAt(vertexCount);
		}
		break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int m_Index = vertexNormal->GetIndexArray().GetAt(vertexCount); // �ε����� ���´�. 
			fbxNormal = vertexNormal->GetDirectArray().GetAt(m_Index);
		}
		break;
		}
	}
	break;
	}

	return ConvertVector3(fbxNormal);
}


DirectX::SimpleMath::Vector2 FBXParser::GetUV(fbxsdk::FbxMesh* mesh, int vertexIndex, int uvIndex)
{
	if (mesh->GetElementUVCount() < 1) return DirectX::SimpleMath::Vector2();

	FbxGeometryElementUV* vertexUV = mesh->GetLayer(0)->GetUVs();

	FbxVector4 fbxUV;

	switch (vertexUV->GetMappingMode()) // ���� ��� 
	{
	case FbxGeometryElement::eByControlPoint: // control point mapping 
	{
		switch (vertexUV->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			fbxUV = vertexUV->GetDirectArray().GetAt(vertexIndex);
		}
		break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int m_Index = vertexUV->GetIndexArray().GetAt(vertexIndex); // �ε����� ���´�. 
			fbxUV = vertexUV->GetDirectArray().GetAt(m_Index);
		}
		break;
		}
	}
	break;

	case FbxGeometryElement::eByPolygonVertex:
	{
		switch (vertexUV->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		case FbxGeometryElement::eIndexToDirect:
		{
			fbxUV = vertexUV->GetDirectArray().GetAt(uvIndex);
		}
		break;
		}
	}
	break;
	}

	return ConvertVector2(fbxUV);
}

void FBXParser::LinkMaterialByPolygon(fbxsdk::FbxMesh* mesh, int polygonIndex, int vertexIndex)
{
	FbxNode* node = mesh->GetNode();
	if (node == nullptr) return;

	FbxLayerElementMaterial* fbxMaterial = mesh->GetLayer(0)->GetMaterials();
	if (fbxMaterial == nullptr) return;

	int mappingIndex = 0;
	switch (fbxMaterial->GetMappingMode())
	{
	case FbxLayerElement::eAllSame:
		mappingIndex = 0;
		break;
	case FbxLayerElement::eByControlPoint:
		mappingIndex = vertexIndex;
		break;
	case FbxLayerElement::eByPolygonVertex:
		mappingIndex = polygonIndex * 3;
		break;
	case FbxLayerElement::eByPolygon:
		mappingIndex = polygonIndex;
		break;
	}

	if (mappingIndex < 0) return;

	FbxLayerElement::EReferenceMode refMode = fbxMaterial->GetReferenceMode();
	if (refMode == FbxLayerElement::eDirect)
	{
		if (mappingIndex < node->GetMaterialCount())
		{
			// materialIndex : �츮�� �ο��� �ε���
			int materialIndex = GetMaterialIndex(node->GetMaterial(mappingIndex));
			m_OneMesh->m_MaterialData = m_Model->m_MaterialList[materialIndex];
		}
	}
	else if (refMode == FbxLayerElement::eIndexToDirect)
	{
		FbxLayerElementArrayTemplate<int>& indexArr = fbxMaterial->GetIndexArray();

		if (mappingIndex < indexArr.GetCount())
		{
			int tempIndex = indexArr.GetAt(mappingIndex);

			if (tempIndex < node->GetMaterialCount())
			{
				// materialIndex : �츮�� �ο��� �ε���
				int materialIndex = GetMaterialIndex(node->GetMaterial(tempIndex));
				m_OneMesh->m_MaterialData = m_Model->m_MaterialList[materialIndex];
			}
		}
	}
}

void FBXParser::SetTransform(fbxsdk::FbxNode* node)
{
	if (node == nullptr) return;

	FbxMatrix worldpos = pScene->GetAnimationEvaluator()->GetNodeGlobalTransform(node);
	FbxMatrix localpos = pScene->GetAnimationEvaluator()->GetNodeLocalTransform(node);

	DirectX::SimpleMath::Matrix world = ConvertMatrix(worldpos);
	DirectX::SimpleMath::Matrix local = ConvertMatrix(localpos);

	//if (m_ParsingMode & ROTATE_AXIS)
	//{
	//	const auto yaw = -90.0f * DirectX::XM_PI / 180.0f;
	//
	//	DirectX::SimpleMath::Quaternion q = XMQuaternionRotationRollPitchYaw(yaw, 0.0f, 0.0f);
	//
	//	world *= XMMatrixRotationQuaternion(q);
	//	local *= XMMatrixRotationQuaternion(q);
	//}

	m_OneMesh->m_WorldTM = world;
	m_OneMesh->m_LocalTM = local;
}

int FBXParser::GetMaterialIndex(fbxsdk::FbxSurfaceMaterial* material)
{
	for (unsigned int index = 0; index < fbxMaterials.size(); index++)
	{
		if (fbxMaterials[index] == material)
			return index;
	}

	return -1;
}

void FBXParser::SetMaterial(fbxsdk::FbxSurfaceMaterial* material)
{
	m_MaterialData->m_MaterialName = material->GetName();

	if (material->GetClassId().Is(FbxSurfacePhong::ClassId))
	{
		// Ambient Data
		m_MaterialData->m_Material_Ambient.x = static_cast<float>(((FbxSurfacePhong*)material)->Ambient.Get()[0]) * 10.0f;
		m_MaterialData->m_Material_Ambient.y = static_cast<float>(((FbxSurfacePhong*)material)->Ambient.Get()[1]) * 10.0f;
		m_MaterialData->m_Material_Ambient.z = static_cast<float>(((FbxSurfacePhong*)material)->Ambient.Get()[2]) * 10.0f;
		m_MaterialData->m_Material_Ambient.w = 1.0f;

		// Diffuse Data
		m_MaterialData->m_Material_Diffuse.x = static_cast<float>(((FbxSurfacePhong*)material)->Albedo.Get()[0]);
		m_MaterialData->m_Material_Diffuse.y = static_cast<float>(((FbxSurfacePhong*)material)->Albedo.Get()[1]);
		m_MaterialData->m_Material_Diffuse.z = static_cast<float>(((FbxSurfacePhong*)material)->Albedo.Get()[2]);
		m_MaterialData->m_Material_Diffuse.w = 1.0f;

		// Specular Data
		m_MaterialData->m_Material_Specular.x = static_cast<float>(((FbxSurfacePhong*)material)->Specular.Get()[0]);
		m_MaterialData->m_Material_Specular.y = static_cast<float>(((FbxSurfacePhong*)material)->Specular.Get()[1]);
		m_MaterialData->m_Material_Specular.z = static_cast<float>(((FbxSurfacePhong*)material)->Specular.Get()[2]);
		m_MaterialData->m_Material_Specular.w = 1.0f;

		// Emissive Data
		m_MaterialData->m_Material_Emissive.x = static_cast<float>(((FbxSurfacePhong*)material)->Emissive.Get()[0]);
		m_MaterialData->m_Material_Emissive.y = static_cast<float>(((FbxSurfacePhong*)material)->Emissive.Get()[1]);
		m_MaterialData->m_Material_Emissive.z = static_cast<float>(((FbxSurfacePhong*)material)->Emissive.Get()[2]);
		m_MaterialData->m_Material_Emissive.w = 1.0f;

		// Transparecy Data
		m_MaterialData->m_Material_Transparency = static_cast<float>(((FbxSurfacePhong*)material)->TransparencyFactor.Get());
		
		// Shininess Data
		m_MaterialData->m_Material_Shininess = static_cast<float>(((FbxSurfacePhong*)material)->Shininess.Get());

		// Reflectivity Data
		m_MaterialData->m_Material_Reflectivity = static_cast<float>(((FbxSurfacePhong*)material)->ReflectionFactor.Get());
	}
	else if (material->GetClassId().Is(FbxSurfaceLambert::ClassId))
	{
		// Ambient Data
		m_MaterialData->m_Material_Ambient.x = static_cast<float>(((FbxSurfaceLambert*)material)->Ambient.Get()[0]);
		m_MaterialData->m_Material_Ambient.y = static_cast<float>(((FbxSurfaceLambert*)material)->Ambient.Get()[1]);
		m_MaterialData->m_Material_Ambient.z = static_cast<float>(((FbxSurfaceLambert*)material)->Ambient.Get()[2]);
		m_MaterialData->m_Material_Ambient.w = 1.0f;

		// Diffuse Data
		m_MaterialData->m_Material_Diffuse.x = static_cast<float>(((FbxSurfaceLambert*)material)->Albedo.Get()[0]);
		m_MaterialData->m_Material_Diffuse.y = static_cast<float>(((FbxSurfaceLambert*)material)->Albedo.Get()[1]);
		m_MaterialData->m_Material_Diffuse.z = static_cast<float>(((FbxSurfaceLambert*)material)->Albedo.Get()[2]);
		m_MaterialData->m_Material_Diffuse.w = 1.0f;

		// Emissive Data
		m_MaterialData->m_Material_Emissive.x = static_cast<float>(((FbxSurfaceLambert*)material)->Emissive.Get()[0]);
		m_MaterialData->m_Material_Emissive.y = static_cast<float>(((FbxSurfaceLambert*)material)->Emissive.Get()[1]);
		m_MaterialData->m_Material_Emissive.z = static_cast<float>(((FbxSurfaceLambert*)material)->Emissive.Get()[2]);
		m_MaterialData->m_Material_Emissive.w = 1.0f;

		// Transparecy Data
		m_MaterialData->m_Material_Transparency = static_cast<float>(((FbxSurfaceLambert*)material)->TransparencyFactor.Get());
	}
}

void FBXParser::SetTexture(fbxsdk::FbxSurfaceMaterial* material, const char* materialName)
{
	unsigned int textureIndex = 0;

	FbxProperty property = material->FindProperty(materialName);

	if (property.IsValid())
	{
		unsigned int textureCount = property.GetSrcObjectCount<FbxTexture>();
		for (unsigned int i = 0; i < textureCount; ++i)
		{
			FbxLayeredTexture* layeredTexture = property.GetSrcObject<FbxLayeredTexture>(i);
			if (layeredTexture)
			{
				throw std::exception("Layered Texture is currently unsupported\n");
			}
			else
			{
				FbxTexture* texture = property.GetSrcObject<FbxTexture>(i);
				if (texture)
				{
					std::string textureType = property.GetNameAsCStr();
					FbxFileTexture* fileTexture = FbxCast<FbxFileTexture>(texture);
					FbxTexture::EBlendMode blemd = fileTexture->CurrentTextureBlendMode;

					FbxTexture::EAlphaSource alphasource = fileTexture->GetAlphaSource();
					FbxTexture::EBlendMode alphaModes = fileTexture->GetBlendMode();
					FbxDouble alpha = fileTexture->GetDefaultAlpha();
					bool issalpha = fileTexture->GetPremultiplyAlpha();

					FbxDouble alpha2 = fileTexture->Alpha;
					if (fileTexture)
					{
						// ��ȯ�� ���� ���..
						std::string fileRoute = ConvertFileRoute(fileTexture->GetFileName());
						std::string count = std::to_string(m_Model->m_MaterialList.size());
						if (textureType == "DiffuseColor")
						{
							std::string mapName("Diffuse Map" + count);
							m_MaterialData->m_DiffuseMap = new CMaterialMap();
							m_MaterialData->m_TextureBind |= DIFFUSE_TEXTURE;
							m_MaterialData->m_DiffuseMap->m_MapName = mapName;
							m_MaterialData->m_DiffuseMap->m_BitMap = fileRoute;
							m_MaterialData->m_MapList.push_back(m_MaterialData->m_DiffuseMap);
						}
						else if (textureType == "AmbientColor")
						{
							std::string mapName("ORM Map" + count);
							m_MaterialData->m_ORMMap = new CMaterialMap();
							m_MaterialData->m_TextureBind |= ORM_TEXTURE;
							m_MaterialData->m_ORMMap->m_MapName = mapName;
							m_MaterialData->m_ORMMap->m_BitMap = fileRoute;
							m_MaterialData->m_MapList.push_back(m_MaterialData->m_ORMMap);
						}
						else if (textureType == "NormalMap")
						{
							std::string mapName("Normal Map" + count);
							m_MaterialData->m_NormalMap = new CMaterialMap();
							m_MaterialData->m_TextureBind |= NORMAL_TEXTURE;
							m_MaterialData->m_NormalMap->m_MapName = mapName;
							m_MaterialData->m_NormalMap->m_BitMap = fileRoute;
							m_MaterialData->m_MapList.push_back(m_MaterialData->m_NormalMap);
						}
						else if (textureType == "EmissiveColor")	// MaskMap
						{
							if (m_MaterialData->m_ORMMap == nullptr)
							{
								std::string mapName("ORM Map" + count);
								m_MaterialData->m_ORMMap = new CMaterialMap();
								m_MaterialData->m_TextureBind |= ORM_TEXTURE;
								m_MaterialData->m_ORMMap->m_MapName = mapName;
								m_MaterialData->m_ORMMap->m_BitMap = fileRoute;
								m_MaterialData->m_MapList.push_back(m_MaterialData->m_ORMMap);
							}
							else
							{
								std::string mapName("Emissive Map" + count);
								m_MaterialData->m_EmissiveMap = new CMaterialMap();
								m_MaterialData->m_TextureBind |= EMISSIVE_TEXTURE;
								m_MaterialData->m_EmissiveMap->m_MapName = mapName;
								m_MaterialData->m_EmissiveMap->m_BitMap = fileRoute;
								m_MaterialData->m_MapList.push_back(m_MaterialData->m_EmissiveMap);
							}
						}
						else if (textureType == "TransparencyFactor")
						{
							m_MaterialData->m_Alpha = true;

							std::string mapName("Diffuse Map" + count);
							m_MaterialData->m_DiffuseMap = new CMaterialMap();
							m_MaterialData->m_TextureBind |= DIFFUSE_TEXTURE;
							m_MaterialData->m_DiffuseMap->m_MapName = mapName;
							m_MaterialData->m_DiffuseMap->m_BitMap = fileRoute;
							m_MaterialData->m_MapList.push_back(m_MaterialData->m_DiffuseMap);
						}
					}
				}
			}
		}
	}
}

void FBXParser::CreateVertex(fbxsdk::FbxMesh* mesh, std::vector<BoneWeights>& boneWeights, int vertexCount)
{
	FbxVector4 fbxPos;

	// Vertex ������ŭ Vertex ����..
	for (int vertexIndex = 0; vertexIndex < vertexCount; vertexIndex++)
	{
		// ���ο� Vertex ����..
		m_OneMesh->m_VertexList.push_back(new CVertex);

		// ���� Vertex..
		CVertex* nowVertex = m_OneMesh->m_VertexList[vertexIndex];

		// �ش� Vertex�� Position..
		fbxPos = mesh->GetControlPointAt(vertexIndex);

		// ��ȯ�� Vertex�� Position..
		nowVertex->m_Pos = ConvertVector3(fbxPos);

		// �ش� Vertex�� Index..
		nowVertex->m_Indices = vertexIndex;

		// ���� Bone Data..
		BoneWeights nowBone = boneWeights[vertexIndex];

		// Bone Weight Data
		for (unsigned int boneIndex = 0; boneIndex < nowBone.m_BoneWeights.size(); boneIndex++)
		{
			// ���� Weight..
			Weight nowWeight = nowBone.m_BoneWeights[boneIndex];

			nowVertex->m_BoneIndices.push_back(nowWeight.m_BoneNumber);
			nowVertex->m_BoneWeights.push_back(nowWeight.m_BoneWeight);
		}
	}
}

void FBXParser::CreateMesh()
{
	m_OneMesh = nullptr;
	m_OneMesh = new ParserData::CMesh();
	m_Model->m_MeshList.push_back(m_OneMesh);
}

int FBXParser::FindBoneIndex(std::string boneName)
{
	for (CMesh* bone : m_AllBoneList)
	{
		if (bone->m_NodeName == boneName)
			return bone->m_BoneIndex;
	}

	return -1;
}

ParserData::CMesh* FBXParser::FindMesh(std::string meshName)
{
	for (CMesh* mesh : m_Model->m_MeshList)
	{
		if (mesh->m_NodeName == meshName)
			return mesh;
	}

	return nullptr;
}
