#pragma once
#include "ConvertData.h"

class RenderData
{
public:
	int m_InstanceLayerIndex = -1;				// Instance Layer Index
	int m_InstanceIndex = -1;					// Instance Index

	TerrainRenderBuffer* m_TerrainBuffer;		// 변환된 Terrain Data..

	ObjectData* m_ObjectData;
	ParticleData* m_ParticleData;
	ColliderData* m_ColliderData;
};

class InstanceLayer
{
public:
	UINT m_LayerIndex = 0;

	InstanceRenderBuffer* m_Instance;		// Instancing 기준이 되는 Mesh Data..

	std::vector<RenderData*> m_MeshList;	// Instancing 기준에 해당하는 Render Data List..
};