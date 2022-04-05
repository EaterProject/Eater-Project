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
	void CreateKeyFrame(std::vector<ParserData::CAnimation*>* Anime, int InputKeyCount);	//애니메이션 키생성
	LoadMeshData* CreateBaseMesh(ParserData::CMesh* mMesh);	//기본 메쉬 생성
	LoadMeshData* CreateSkinMesh(ParserData::CMesh* mMesh);	//스킨 매쉬 생성
	LoadMeshData* CreateBoneMesh(ParserData::CMesh* mMesh);	//스킨 매쉬 생성
private:
	void SetMaterialData(ParserData::CMesh* mMesh, LoadMeshData* SaveData);	//메테리얼 정보 삽입
	void SetMatrixData(ParserData::CMesh* mMesh, LoadMeshData* SaveData);	//메트릭스 정보 삽입
	void SetNameData(ParserData::CMesh* mMesh, LoadMeshData* SaveData);		//이름 정보 삽입
	void SetMeshData(ParserData::CMesh* mMesh, LoadMeshData* SaveData);	//인덱스 버텍스 버퍼 삽입
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

