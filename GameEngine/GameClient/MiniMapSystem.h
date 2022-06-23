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
	// MiniMap 생성 함수..
	GameObject* CreateMiniMap(std::string texture_name, PIVOT_TYPE pivot_type, ROTATE_ANGLE rotate_angle, Vector2 target_ratio, Vector2 position);

	// Icon 생성 함수..
	GameObject* CreateIcon(std::string texture_name, GameObject* target, bool isRotate = false);

	// 생성된 Icon Target 변경 함수..
	void ChangeIconTarget(GameObject* target, GameObject* change_target);

	// 생성된 Icon Texture 변경 함수..
	void ChangeIconTexture(GameObject* target, std::string texture_name);

	// 모든 Icon Active 설정 함수..
	void SetIconListActive(bool enable);

	// Icon 제거 함수..
	void DeleteIcon(GameObject* target);

	// 모든 Icon 제거 함수..
	void DeleteIconList();

public:
	// Target Icon 검색 함수..
	GameObject* GetIcon(GameObject* target);

private:
	GameObject* m_MiniMap;
	std::vector<MiniMap_Icon> m_MiniMapIconList;

	ROTATE_ANGLE m_RotateAngle;

	Vector2 m_TargetRatio;

private:
	static MiniMapSystem* g_Instance;
};

