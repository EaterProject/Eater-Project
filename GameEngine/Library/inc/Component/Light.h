#pragma once
#include "Component.h"
#include "EaterEngineDLL.h"
#include "LightHelper.h"
#include "TypeOptionHeader.h"

/// <summary>
/// Light Base Class
/// </summary>

class Transform;

class Light : public Component
{
public:
	Light();
	virtual ~Light() = default;

public:
	friend class LightManager;

public:
	static Light* g_DirLight;

	static void SetDirectionLight(Light* light);

public:
	void Awake() override;
	void Start() override;
	void Update() override;

public:
	EATER_ENGINEDLL void SetColor(float r, float g, float b);
	EATER_ENGINEDLL void SetDirection(float x, float y, float z);

	EATER_ENGINEDLL void SetPosition(float x, float y, float z);
	EATER_ENGINEDLL void SetInAngle(float angle);
	EATER_ENGINEDLL void SetOutAngle(float angle);

	EATER_ENGINEDLL void SetRange(float range);
	EATER_ENGINEDLL void SetPower(float power);
	
	EATER_ENGINEDLL void SetType(LIGHT_TYPE lightType);

public:
	EATER_ENGINEDLL void SetShadowRadius(float radius);
	EATER_ENGINEDLL void SetCenterPos(DirectX::SimpleMath::Vector3 pos);
	
private:
	void SetIndex(UINT index);
	void SetLightViewProj();

private:
	DirectionalLightData* GetDirectionLight() { return m_DirectionLight; }
	PointLightData* GetPointLight() { return m_PointLight; }
	SpotLightData* GetSpotLight() { return m_SpotLight; }

public:
	LIGHT_TYPE GetType() { return m_LightType; }

private:
	Transform* m_Transform;

	LIGHT_TYPE m_LightType;

	UINT m_LightIndex;

	DirectionalLightData* m_DirectionLight;
	PointLightData* m_PointLight;
	SpotLightData* m_SpotLight;

	float m_InAngle;
	float m_OutAngle;

private:
	DirectX::SimpleMath::Vector3 m_CenterPos;
	float m_ShadowRadius;
};