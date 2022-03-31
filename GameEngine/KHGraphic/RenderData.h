#pragma once
#include "ConvertData.h"

// Game Engine �� MeshData�� �����ϴ� DirectX 11 ���� Render Data Class
class RenderData
{
public:
	int m_InstanceLayerIndex = -1;				// Instance Layer Index..
	int m_InstanceIndex = -1;					// Instance Index..

	MeshRenderBuffer* m_Mesh;					// ��ȯ�� Mesh Data..
	MaterialRenderBuffer* m_Material;			// ��ȯ�� Material Data..
	TerrainRenderBuffer* m_TerrainBuffer;		// ��ȯ�� Terrain Data..

	ObjectData* m_ObjectData;
	ParticleData* m_ParticleData;
	ColliderData** m_ColliderData;
};

// ���� Instance�� �����ϱ� ���� Instance Layer Class
class InstanceLayer
{
public:
	void PushRenderData(RenderData* renderData)
	{
		m_MeshList.push_back(renderData);
	}

	void DeleteRenderData(UINT index)
	{
		m_MeshList.erase(std::next(m_MeshList.begin(), index));
	}

public:
	UINT m_LayerIndex = 0;
	UINT m_RenderCount = 0;

	InstanceRenderBuffer* m_Instance;		// Instancing ������ �Ǵ� Mesh Data..

	std::vector<RenderData*> m_RenderList;	// ���� ������ Instancing Layer Render List..
	std::vector<RenderData*> m_MeshList;	// Instancing ���ؿ� �ش��ϴ� Render Data List..
};