#pragma once
#include "Eater_Interface.h"
#include <windows.h>

namespace ParserData
{
	class CModelAnimation;
}

class Eater_LoadCamera;
class LoadMeshData;
class ModelData;
class GraphicEngineManager;

class Eater_LoadMesh: public Eater_Interface
{
public:
	Eater_LoadMesh();
	virtual~Eater_LoadMesh();

	// Eater_Interface을(를) 통해 상속됨
	virtual void LoadData(std::string& Path) override;
	void Initialize(GraphicEngineManager* Graphic, CRITICAL_SECTION* _cs);

private:
	LoadMeshData* LoadStaticMesh(int index);
	LoadMeshData* LoadBoneMesh(int index);
	LoadMeshData* LoadSkinMesh(int index);

	void LoadTM(int Index, LoadMeshData* model);
	void LoadBoneOffset(int index, LoadMeshData* model);
	void LoadMaterial(int index, LoadMeshData* model);
	void LoadMeshName(int index, LoadMeshData* model);
	void LoadName(int index, LoadMeshData* model);

	void LinkBone(ModelData* Data);
	void CreateKeyFrame(ParserData::CModelAnimation* Anime, int InputKeyCount);
	void LoadAnimation(int index, std::string& Name);
	std::vector<LoadMeshData*> BoneList;
	std::vector<LoadMeshData*> SkinList;

	Eater_LoadCamera* mCamera;

private:
	GraphicEngineManager* m_Graphic;
	CRITICAL_SECTION* m_CriticalSection;
};

