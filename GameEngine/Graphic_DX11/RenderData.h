#pragma once
#include "ConvertData.h"

// Game Engine �� MeshData�� �����ϴ� DirectX 11 ���� Render Data Class
class RenderData
{
public:
	bool m_Draw = true;							// Culling ����..

	int m_InstanceLayerIndex = -1;				// Instance Layer Index..

	MeshRenderBuffer*		m_Mesh;				// ��ȯ�� Mesh Data..
	MaterialRenderBuffer*	m_Material;			// ��ȯ�� Material Data..
	TerrainRenderBuffer*	m_Terrain;			// ��ȯ�� Terrain Data..
	UIRenderBuffer*			m_UI;				// ��ȯ�� UI Data..
	AnimationRenderBuffer*	m_Animation;		// ��ȯ�� Animation Data..

	ObjectData*		m_ObjectData;
	AnimationData*	m_AnimationData;
	ParticleData*	m_ParticleData;
};

// Render Data�� �����ϴ� Layer���� Base Layer Class
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
	std::vector<RenderData*> m_InstanceList;		// �ش� Layer�� ���ϴ� UI Data List..
};

// ���� Instance�� �����ϱ� ���� Instance Layer Class
class InstanceLayer : public BaseLayer
{
public:
	UINT m_LayerIndex = 0;

	InstanceRenderBuffer* m_Instance;		// Instancing ������ �Ǵ� Mesh Data..
};

// UI Order Layer�� �����ϱ� ���� UI Layer Class
class UILayer : public BaseLayer
{
public:
	UINT m_LayerIndex = 0;
};