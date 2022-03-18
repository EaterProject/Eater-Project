#pragma once
#include <string>
#include "EaterEngineDLL.h"

class MeshBuffer;
class MeshManager;

class Mesh
{
public:
	EATER_ENGINEDLL Mesh();
	~Mesh();

public:
	friend class MeshManager;

public:
	//EATER_ENGINEDLL void SetLOD();

public:
	void Release();

public:
	std::string Name;
	MeshBuffer* m_MeshData;
};

