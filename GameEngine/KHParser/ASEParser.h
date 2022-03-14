//------------------------------------------------------------
// ASE Parser
//------------------------------------------------------------

enum eParsingmode
{
	eNone, eGeomobject, eAnimation, eShape, eCamera, eLight, eHelperObject,
	eSkin_InitTM, eBone_List,
	eEnd,
};

class ASEParser : public ModelParser
{
public:
	ASEParser();
	~ASEParser();

public:
	void Initialize() override;
	void Release() override;
	ParserData::Model* LoadModel(std::string fileName) override;

	void OptimizeVertex(ParserData::ASEMesh* pMesh);		// 버텍스(노말), 텍스쳐 값으로 최적화를 해 준다.
	void RecombinationTM(ParserData::ASEMesh* pMesh);
	void OptimizeData();
	void SetBoneTM(ParserData::ASEMesh* pMesh);

	void CreateModel();
	void ResetData();

private:
	ParserData::Model* m_Model;
	std::vector<ParserData::Model*> m_ModelList;

private:
	// 토큰을 구별해내기 위한 스트링
	char m_TokenString[256];

private:
	ParserData::Scenedata m_scenedata;					// 현재 Scene의 데이터가 들어갈 구조체

	/// 매트리얼 관련
	ParserData::CMaterial* m_MaterialData;				// Material의 데이터가 들어갈 구조체
	ParserData::MaterialMap* m_materialmap;
	std::vector<ParserData::CMaterial*> m_MaterialList;	// Material들의 리스트

	/// 한 씬에 N개의 메시가 존재할 수 있다.
	ParserData::ASEMesh* m_OneMesh;						// 메시 하나의 데이터가 들어갈 구조체
	std::vector<ParserData::ASEMesh*> m_MeshList;	 			// 메시들

	ParserData::Bone* m_Bone;

	/// 애니메이션
	bool m_IsAnimation;
	ParserData::OneAnimation* m_Animation;					// 한 오브젝트의 애니메이션 데이터(파싱용)

	std::string m_TexRoute;

private:
	ASE::CASELexer* m_lexer;

private:
	/// 파싱을 맡게 되는 비재귀함수.
	void DataParsing();

	int Parsing_NumberLong();			// long
	float Parsing_NumberFloat();		// float
	LPSTR Parsing_String();				// string
	int Parsing_NumberInt();			// int
	DirectX::SimpleMath::Vector3 Parsing_ChangeNumberVector3();	// Float3 -> Vector3

	void Create_OneMesh_to_list();				// Create New MEsh
	void Create_MaterialData_to_list();			// Create New Material
	void Create_DiffuseMap_to_list();			// Material DiffuseMap Data
	void Create_BumpMap_to_list();				// Material BumpMap Data
	void Create_SpecularMap_to_list();			// Material SpecularMap Data
	void Create_ShineMap_to_list();				// Material ShineMap Data
	void Create_AnimationData_to_mesh(ParserData::Mesh* nowMesh);
	void Create_BoneData_to_list();
};