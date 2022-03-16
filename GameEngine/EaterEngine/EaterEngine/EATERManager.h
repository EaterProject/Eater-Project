#pragma once
#include <windows.h>
#include <string>
#include <vector>
class GraphicEngineManager;
class LoadMeshData;
class ModelData;
class GameObject;
namespace ParserData
{
	class Mesh;
	class OneAnimation;
}

class EATERManager
{
public:
	EATERManager();
	~EATERManager();

	void Initialize(GraphicEngineManager* Graphic, CRITICAL_SECTION* _cs);
	void Load(std::string& Path, UINT parsingMode);
	void LoadScene(std::string& Path);
private:
	LoadMeshData* LoadStaticMesh(int index);
	LoadMeshData* LoadBoneMesh(int index);
	LoadMeshData* LoadSkinMesh(int index);
	void LoadAnimation(int index, std::string& Name);
private:
	//��ü������ �ε���
	void LinkBone(ModelData*);
	void LinkSkin();
	void LoadTM(int Index, LoadMeshData* model);
	void LoadBoneOffset(int index, LoadMeshData* mMesh);
	void LoadMaterial(int index, LoadMeshData* model);
	void LoadName(int index, LoadMeshData* model);
	void LoadGameObject(int index);
private:
	//�������� �ε���
	void CreateStaticObject(int index, GameObject* Object);					//����ƽ ������Ʈ�� �����Ѵ�
	void CreateSkinObject(int index, GameObject* Object);					//��Ų ������Ʈ�� �����Ѵ�
	void CreateParticleObject(int index, GameObject* Object);				//��ƼŬ ������Ʈ�� �����Ѵ�
	void Load_Component_MeshFilter(int index, GameObject* Object);			//MeshFilter Load
	void Load_Component_Transform(int index, GameObject* Object);	
	void Load_Component_Skinning(int index, GameObject* Object);//Transform Load
private:
	void LoadVertex(int index, ParserData::Mesh* mMesh);
	void LoadSkinVertex(int index, ParserData::Mesh* mMesh);
	void LoadIndex(int index, ParserData::Mesh* mMesh);

	void CreateKeyFrame(std::vector<ParserData::OneAnimation*>* Anime, int InputKeyCount);
private:
	std::string CutStr(std::string& Name);
	GraphicEngineManager*	m_Graphic;
	CRITICAL_SECTION*		m_CriticalSection;

	std::vector<LoadMeshData*> BoneList;
	std::vector<LoadMeshData*> SkinList;
};

