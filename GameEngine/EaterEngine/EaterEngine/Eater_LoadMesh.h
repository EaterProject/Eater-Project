#pragma once
#include "Eater_Interface.h"

namespace ParserData
{
	class CAnimation;
}

class Eater_LoadCamera;
class LoadMeshData;
class ModelData;
class Eater_LoadMesh: public Eater_Interface
{
public:
	Eater_LoadMesh();
	virtual~Eater_LoadMesh();

	// Eater_Interface을(를) 통해 상속됨
	virtual void LoadData(std::string& Path) override;
	virtual void Initialize();
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
	void CreateKeyFrame(std::vector<ParserData::CAnimation*>& Anime, int InputKeyCount);
	void LoadAnimation(int index, std::string& Name);
	std::vector<LoadMeshData*> BoneList;
	std::vector<LoadMeshData*> SkinList;

	Eater_LoadCamera* mCamera;
};

