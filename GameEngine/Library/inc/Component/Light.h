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
	virtual ~Light();

private:
	friend class LightManager;

public:
	static Light* g_DirLight;

	static void SetDirectionLight(Light* light);

public:
	void Awake() override;
	void Start() override;
	void Update() override;

public:
	EATER_ENGINEDLL void SetColor(float r, float g, float b);	// All Light 전용 Light 색상

	EATER_ENGINEDLL void SetAngle(float angle);					// Spot Light 전용 Half Angle 값
	EATER_ENGINEDLL void SetAttenuate(float range);				// Spot Light 전용 Angle 기준 흐려지는 시작 지점
	EATER_ENGINEDLL void SetRange(float range);					// Spot & Point Light 전용 Light 적용 범위
	EATER_ENGINEDLL void SetPower(float power);					// All Light 전용 Light 강도

	EATER_ENGINEDLL void SetType(LIGHT_TYPE lightType);

	EATER_ENGINEDLL LIGHT_TYPE GetType() { return m_LightType; }
	EATER_ENGINEDLL float GetAngle();
	EATER_ENGINEDLL float GetAttenuate();
	EATER_ENGINEDLL float GetRange();
	EATER_ENGINEDLL float GetPower();
	EATER_ENGINEDLL Vector3 GetColor();
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

	
private:
	Transform* m_Transform;

	LIGHT_TYPE m_LightType;

	UINT m_LightIndex;

	DirectionalLightData* m_DirectionLight;
	PointLightData* m_PointLight;
	SpotLightData* m_SpotLight;

	float m_Angle;
	float m_AttStart;

	Vector3 m_PrevLook;
	Vector3 m_NowLook;
private:
	DirectX::SimpleMath::Vector3 m_CenterPos;
	float m_ShadowRadius;
};