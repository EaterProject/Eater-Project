#include "MiniMapSystem.h"
#include "GameObject.h"
#include "RectTransform.h"
#include "Image.h"

MiniMapSystem* MiniMapSystem::g_Instance = nullptr;

MiniMapSystem::MiniMapSystem()
{

}

MiniMapSystem::~MiniMapSystem()
{

}

MiniMapSystem* MiniMapSystem::Get()
{
	if (g_Instance == nullptr)
	{
		g_Instance = new MiniMapSystem();
	}

	return g_Instance;
}

void MiniMapSystem::Release()
{
	Destroy(m_MiniMap);

	DeleteIconList();
}

GameObject* MiniMapSystem::CreateMiniMap(std::string texture_name, PIVOT_TYPE pivot_type, ROTATE_ANGLE rotate_angle, Vector2 target_ratio, Vector2 position)
{
	if (m_MiniMap == nullptr)
	{
		m_MiniMap = Instance_Image("MiniMap");
	}

	Image* img = m_MiniMap->GetComponent<Image>();
	img->SetTexture(texture_name);
	
	RectTransform* tr = m_MiniMap->GetComponent<RectTransform>();
	tr->SetPivot(pivot_type);
	tr->SetPosition(position);

	m_RotateAngle = rotate_angle;
	m_TargetRatio = target_ratio;

	return m_MiniMap;
}

GameObject* MiniMapSystem::CreateIcon(std::string texture_name, GameObject* target, bool isRotate)
{
	GameObject* target_icon = Instance_Image();

	Image* img = target_icon->GetComponent<Image>();
	img->SetTexture(texture_name);

	RectTransform* tr = target_icon->GetComponent<RectTransform>();
	tr->SetPivot(PIVOT_TYPE::PIVOT_IMAGE);
	tr->SetPositionObject(target);
	tr->SetTargetImage(m_MiniMap, m_RotateAngle);
	tr->SetTargetRotation(isRotate);
	tr->SetTargetRatio(m_TargetRatio);

	MiniMap_Icon minimap_icon;
	minimap_icon.Icon = target_icon;
	minimap_icon.Target = target;

	m_MiniMapIconList.push_back(minimap_icon);

	return target_icon;
}

void MiniMapSystem::ChangeIconTarget(GameObject* target, GameObject* change_target)
{
	// 현재 Target 기준 Icon 검색..
	GameObject* target_icon = GetIcon(target);

	// Icon Target Object 변경..
	target_icon->GetComponent<RectTransform>()->SetPositionObject(change_target);
}

void MiniMapSystem::ChangeIconTexture(GameObject* target, std::string texture_name)
{
	// 현재 Target 기준 Icon 검색..
	GameObject* target_icon = GetIcon(target);

	// Icon Texture 변경..
	target_icon->GetComponent<Image>()->SetTexture(texture_name);
}

void MiniMapSystem::SetIconListActive(bool enable)
{
	for (MiniMap_Icon& target_icon : m_MiniMapIconList)
	{
		target_icon.Icon->SetActive(enable);
	}
}

void MiniMapSystem::DeleteIcon(GameObject* target)
{
	for (int i = 0; i < m_MiniMapIconList.size(); i++)
	{
		if (m_MiniMapIconList[i].Target == target)
		{
			Destroy(m_MiniMapIconList[i].Icon);
			m_MiniMapIconList.erase(std::next(m_MiniMapIconList.begin(), i));
			return;
		}
	}
}

void MiniMapSystem::DeleteIconList()
{
	for (MiniMap_Icon& target_icon : m_MiniMapIconList)
	{
		Destroy(target_icon.Icon);
	}

	m_MiniMapIconList.clear();
}

GameObject* MiniMapSystem::GetIcon(GameObject* target)
{
	for (MiniMap_Icon& target_icon : m_MiniMapIconList)
	{
		if (target_icon.Target == target)
		{
			return target_icon.Icon;
		}
	}

	return nullptr;
}