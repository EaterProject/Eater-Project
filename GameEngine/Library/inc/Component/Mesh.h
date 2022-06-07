#pragma once
#include <string>
#include "EaterEngineDLL.h"
#include "EngineData.h"

class MeshManager;

class Mesh
{
public:
	EATER_ENGINEDLL Mesh();
	~Mesh();

public:
	friend class MeshManager;

public:
	void Release();

public:
	std::string Name;
	MeshBuffer* m_MeshData;
};

