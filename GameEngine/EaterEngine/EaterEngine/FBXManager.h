#pragma once

#include "ParserData.h"
#include "SimpleMath.h"
#include <string>
#include <vector>
#include <map>

namespace ParserData
{
	class CModel;
	class CAnimation;
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
	void LoadTerrain(std::string Name, std::string MaskName1, std::string MaskName2, UINT parsingMode);
	void LoadQuad();
	void Load(std::string& Path, UINT parsingMode);
private:
	void LoadFile(std::string& Path, UINT parsingMode);
	void LoadAnimation(ModelData* SaveMesh, ParserData::CModel* MeshData,std::string& Path);
private:
	void CheckSkinning(std::string& Path);
	void CheckAnimation(std::string& Path);
private:
	void CreateSaveMesh(ParserData::CModel* mMesh , ModelData* SaveMesh,std::string& Path);
	void CreateKeyFrame(std::vector<ParserData::CAnimation*>* Anime, int InputKeyCount);	//�ִϸ��̼� Ű����
	LoadMeshData* CreateBaseMesh(ParserData::CMesh* mMesh);	//�⺻ �޽� ����
	LoadMeshData* CreateSkinMesh(ParserData::CMesh* mMesh);	//��Ų �Ž� ����
	LoadMeshData* CreateBoneMesh(ParserData::CMesh* mMesh);	//��Ų �Ž� ����
private:
	void SetMaterialData(ParserData::CMesh* mMesh, LoadMeshData* SaveData);	//���׸��� ���� ����
	void SetMatrixData(ParserData::CMesh* mMesh, LoadMeshData* SaveData);	//��Ʈ���� ���� ����
	void SetNameData(ParserData::CMesh* mMesh, LoadMeshData* SaveData);		//�̸� ���� ����
	void SetMeshData(ParserData::CMesh* mMesh, LoadMeshData* SaveData);	//�ε��� ���ؽ� ���� ����
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

