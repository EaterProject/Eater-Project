#pragma once
#include "ConvertData.h"

// Game Engine 측 MeshData와 대응하는 DirectX 11 전용 Render Data Class
class RenderData
{
public:
	bool m_Draw = true;							// Culling 여부..

	int m_InstanceLayerIndex = -1;				// Instance Layer Index..

	MeshRenderBuffer*		m_Mesh;				// 변환된 Mesh Data..
	MaterialRenderBuffer*	m_Material;			// 변환된 Material Data..
	TerrainRenderBuffer*	m_Terrain;			// 변환된 Terrain Data..
	UIRenderBuffer*			m_UI;				// 변환된 UI Data..
	AnimationRenderBuffer*	m_Animation;		// 변환된 Animation Data..

	ObjectData*		m_ObjectData;
	AnimationData*	m_AnimationData;
	ParticleData*	m_ParticleData;
};

// Render Data를 관리하는 Layer들의 Base Layer Class
class BaseLayer
{
public:
	void PushRenderData(RenderData* renderData)
	{
		m_InstanceList.push_back(renderData);
	}

	void PopRenderData(RenderData* renderData)
	{
		for (UINT index = 0; index < m_InstanceList.size(); index++)
		{
			if (m_InstanceList[index] == renderData)
			{
				m_InstanceList.erase(std::next(m_InstanceList.begin(), index));
				return;
			}
		}
	}

public:
	std::vector<RenderData*> m_InstanceList;		// 해당 Layer에 속하는 UI Data List..
};

// 동일 Instance를 관리하기 위한 Instance Layer Class
class InstanceLayer : public BaseLayer
{
public:
	UINT m_LayerIndex = 0;

	InstanceRenderBuffer* m_Instance;		// Instancing 기준이 되는 Mesh Data..
};

// UI Order Layer를 관리하기 위한 UI Layer Class
class UILayer : public BaseLayer
{
public:
	UINT m_LayerIndex = 0;
};