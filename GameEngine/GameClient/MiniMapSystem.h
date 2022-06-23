#pragma once
#include "EaterEngineAPI.h"

class GameObject;
class MiniMapSystem
{
private:
	// MiniMap Icon
	struct MiniMap_Icon
	{
		GameObject* Icon;
		GameObject* Target;
	};

private:
	MiniMapSystem();
	~MiniMapSystem();

public:
	static MiniMapSystem* Get();

public:
	void Release();

public:
	// MiniMap ���� �Լ�..
	GameObject* CreateMiniMap(std::string texture_name, PIVOT_TYPE pivot_type, ROTATE_ANGLE rotate_angle, Vector2 target_ratio, Vector2 position);

	// Icon ���� �Լ�..
	GameObject* CreateIcon(std::string texture_name, GameObject* target, bool isRotate = false);

	// ������ Icon Target ���� �Լ�..
	void ChangeIconTarget(GameObject* target, GameObject* change_target);

	// ������ Icon Texture ���� �Լ�..
	void ChangeIconTexture(GameObject* target, std::string texture_name);

	// ��� Icon Active ���� �Լ�..
	void SetIconListActive(bool enable);

	// Icon ���� �Լ�..
	void DeleteIcon(GameObject* target);

	// ��� Icon ���� �Լ�..
	void DeleteIconList();

public:
	// Target Icon �˻� �Լ�..
	GameObject* GetIcon(GameObject* target);

private:
	GameObject* m_MiniMap;
	std::vector<MiniMap_Icon> m_MiniMapIconList;

	ROTATE_ANGLE m_RotateAngle;

	Vector2 m_TargetRatio;

private:
	static MiniMapSystem* g_Instance;
};

