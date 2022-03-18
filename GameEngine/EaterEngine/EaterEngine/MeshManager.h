#pragma once
#include <windows.h>
#include <vector>
#include <unordered_map>
#include "BaseManager.h"

class Mesh;
class MeshManager : public BaseManager
{
public:
	MeshManager();
	~MeshManager();

public:
	void Initialize();
	void Release();

	static void PushMesh(Mesh* mesh);
	static void DeleteMesh(UINT index);

private:
	static std::vector<std::pair<UINT, bool>> g_IndexList;
	static std::unordered_map<UINT, Mesh*> g_MeshList;
};

