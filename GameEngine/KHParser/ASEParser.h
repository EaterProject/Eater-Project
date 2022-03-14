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

	void OptimizeVertex(ParserData::ASEMesh* pMesh);		// ���ؽ�(�븻), �ؽ��� ������ ����ȭ�� �� �ش�.
	void RecombinationTM(ParserData::ASEMesh* pMesh);
	void OptimizeData();
	void SetBoneTM(ParserData::ASEMesh* pMesh);

	void CreateModel();
	void ResetData();

private:
	ParserData::Model* m_Model;
	std::vector<ParserData::Model*> m_ModelList;

private:
	// ��ū�� �����س��� ���� ��Ʈ��
	char m_TokenString[256];

private:
	ParserData::Scenedata m_scenedata;					// ���� Scene�� �����Ͱ� �� ����ü

	/// ��Ʈ���� ����
	ParserData::CMaterial* m_MaterialData;				// Material�� �����Ͱ� �� ����ü
	ParserData::MaterialMap* m_materialmap;
	std::vector<ParserData::CMaterial*> m_MaterialList;	// Material���� ����Ʈ

	/// �� ���� N���� �޽ð� ������ �� �ִ�.
	ParserData::ASEMesh* m_OneMesh;						// �޽� �ϳ��� �����Ͱ� �� ����ü
	std::vector<ParserData::ASEMesh*> m_MeshList;	 			// �޽õ�

	ParserData::Bone* m_Bone;

	/// �ִϸ��̼�
	bool m_IsAnimation;
	ParserData::OneAnimation* m_Animation;					// �� ������Ʈ�� �ִϸ��̼� ������(�Ľ̿�)

	std::string m_TexRoute;

private:
	ASE::CASELexer* m_lexer;

private:
	/// �Ľ��� �ð� �Ǵ� ������Լ�.
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