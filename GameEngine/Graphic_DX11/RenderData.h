#pragma once
#include "ConvertData.h"

// Game Engine 측 MeshData와 대응하는 DirectX 11 전용 Render Data Class
class RenderData
{
public:
	bool m_Draw = true;							// Culling 여부..

	int m_InstanceLayerIndex = -1;				// Instance Layer Index..
	int m_InstanceIndex = -1;					// Instance Index..

	MeshRenderBuffer*		m_Mesh;				// 변환된 Mesh Data..
	MaterialRenderBuffer*	m_Material;			// 변환된 Material Data..
	TerrainRenderBuffer*	m_Terrain;			// 변환된 Terrain Data..
	UIRenderBuffer*			m_UI;				// 변환된 UI Data..
	AnimationRenderBuffer*	m_Animation;		// 변환된 Animation Data..

	ObjectData*		m_ObjectData;
	AnimationData*	m_AnimationData;
	ParticleData*	m_ParticleData;
};

// 동일 Instance를 관리하기 위한 Instance Layer Class
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

	InstanceRenderBuffer* m_Instance;		// Instancing 기준이 되는 Mesh Data..

	std::vector<RenderData*> m_MeshList;	// Instancing 기준에 해당하는 Render Data List..
};