#pragma once

#include "ParserData.h"
#include "SimpleMath.h"
#include <string>
#include <vector>
#include <map>

namespace ParserData
{
	struct Model;
	class OneAnimation;
}
class ModelData;
class ModelParser;
class LoadMeshData;
class GraphicEngineManager;
class FBXManager
{
public:
	FBXManager();
	~FBXManager();

	void Initialize(GraphicEngineManager* Graphic, CRITICAL_SECTION* _cs);
	void LoadTerrain(std::string Name, std::string MaskName, UINT parsingMode);
	void LoadQuad();
	void Load(std::string& Path, UINT parsingMode);
private:
	void LoadFile(std::string& Path, UINT parsingMode);
	void LoadAnimation(ModelData* SaveMesh, ParserData::Model* MeshData,std::string& Path);
private:
	void CheckSkinning(std::string& Path);
	void CheckAnimation(std::string& Path);
private:
	void CreateSaveMesh(ParserData::Model* mMesh , ModelData* SaveMesh,std::string& Path);
	void CreateKeyFrame(std::vector<ParserData::OneAnimation*>* Anime, int InputKeyCount);	//�ִϸ��̼� Ű����
	LoadMeshData* CreateBaseMesh(ParserData::Mesh* mMesh);	//�⺻ �޽� ����
	LoadMeshData* CreateSkinMesh(ParserData::Mesh* mMesh);	//��Ų �Ž� ����
	LoadMeshData* CreateBoneMesh(ParserData::Mesh* mMesh);	//��Ų �Ž� ����
private:
	void SetMaterialData(ParserData::Mesh* mMesh, LoadMeshData* SaveData);	//���׸��� ���� ����
	void SetMatrixData(ParserData::Mesh* mMesh, LoadMeshData* SaveData);	//��Ʈ���� ���� ����
	void SetNameData(ParserData::Mesh* mMesh, LoadMeshData* SaveData);		//�̸� ���� ����
	void SetBufferData(ParserData::Mesh* mMesh, LoadMeshData* SaveData);	//�ε��� ���ؽ� ���� ����
private:
	std::string CutStr(std::string Path);
	void LinkMesh(std::vector<LoadMeshData*>& List, ModelData* SaveMesh);
	std::string GetSaveName(std::string& Path);
private:
	std::string nowFileName;
	bool isFolder;
	bool isSkinning;
	bool isAnimation;
private:
	ModelParser* FBXLoad;
	GraphicEngineManager* m_Graphic;
	CRITICAL_SECTION* m_CriticalSection;
};

