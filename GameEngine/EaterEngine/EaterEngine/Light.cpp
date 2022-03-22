#include "GameObject.h"
#include "Transform.h"
#include "BaseManager.h"
#include "LightManager.h"
#include "Light.h"
#include "EngineData.h"

#define PI    ((FLOAT)  3.141592654f)
#define SAFE_DELETE(x) { if(x != nullptr) {delete x; x = nullptr;} }

using namespace DirectX;
using namespace SimpleMath;

Light* Light::g_DirLight = nullptr;

Light::Light()
	:m_LightType(LIGHT_TYPE::NONE_LIGHT), m_Transform(nullptr)
{
	m_CenterPos = Vector3(0, 100, 0);
	m_ShadowRadius = sqrtf(10.0f * 10.0f + 15.0f * 15.0f) * 15;

	m_Angle = 25.0f;
}

void Light::SetDirectionLight(Light* light)
{
	g_DirLight = light;
}

void Light::Awake()
{
	m_Transform = gameobject->GetTransform();
}

void Light::Start()
{
	gameobject->OneMeshData->Object_Data->ObjType = OBJECT_TYPE::LIGHT;
	
	switch (m_LightType)
	{
	case DIRECTION_LIGHT:
		break;
	case POINT_LIGHT:
		break;
	case SPOT_LIGHT:
		m_Transform->Rotation.x -= 90.0f;
		break;
	default:
		break;
	}
}

void Light::Update()
{
	// 매 프레임 Light Position 연동..
	switch (m_LightType)
	{
	case POINT_LIGHT:
		m_PointLight->Position = m_Transform->Position;
		break;
	case SPOT_LIGHT:
		m_SpotLight->Position = m_Transform->Position;
		m_SpotLight->Rotate.x = PI * m_Transform->Rotation.x / 180.0f;
		m_SpotLight->Rotate.y = PI * m_Transform->Rotation.y / 180.0f;
		m_SpotLight->Rotate.z = PI * m_Transform->Rotation.z / 180.0f;
		m_SpotLight->Direction = m_Transform->GetLocalPosition_Look();
		break;
	default:
		break;
	}
}

void Light::SetColor(float r, float g, float b)
{
	switch (m_LightType)
	{
	case DIRECTION_LIGHT:
		m_DirectionLight->Diffuse = { r, g, b };
		break;
	case POINT_LIGHT:
		m_PointLight->Diffuse = { r, g, b };
		break;
	case SPOT_LIGHT:
		m_SpotLight->Diffuse = { r, g, b };
		break;
	default:
		break;
	}
}

void Light::SetPosition(float x, float y, float z)
{
	switch (m_LightType)
	{
	case POINT_LIGHT:
		m_PointLight->Position = { x, y, z };
		m_Transform->Position = { x, y, z };
		break;
	case SPOT_LIGHT:
		m_SpotLight->Position = { x, y, z };
		m_Transform->Position = { x, y, z };
		break;
	default:
		break;
	}
}

void Light::SetAngle(float angle)
{
	switch (m_LightType)
	{
	case SPOT_LIGHT:
		m_Angle = PI * angle / 180.0f;
		m_SpotLight->AttRange = cosf(m_Angle * 0.5f) - cosf(m_Angle);
		m_SpotLight->Angle = m_Angle;
		break;
	default:
		break;
	}
}

void Light::SetRange(float range)
{
	switch (m_LightType)
	{
	case POINT_LIGHT:
		m_PointLight->Range = range;
		break;
	case SPOT_LIGHT:
		m_SpotLight->Range = range;
		break;
	default:
		break;
	}
}

void Light::SetPower(float power)
{
	switch (m_LightType)
	{
	case DIRECTION_LIGHT:
		m_DirectionLight->Power = power;
		break;
	case POINT_LIGHT:
		m_PointLight->Power = power;
		break;
	case SPOT_LIGHT:
		m_SpotLight->Power = power;
		break;
	default:
		break;
	}
}

void Light::SetType(LIGHT_TYPE lightType)
{
	if (lightType == m_LightType) return;

	// 현재 Light Data 제거..
	if (m_LightType != NONE_LIGHT)
	{
		switch (m_LightType)
		{
		case DIRECTION_LIGHT:
			SAFE_DELETE(m_DirectionLight);
			break;
		case POINT_LIGHT:
			SAFE_DELETE(m_PointLight);
			break;
		case SPOT_LIGHT:
			SAFE_DELETE(m_SpotLight);
			break;
		default:
			break;
		}

		// Light Data 제거..
		LightManager::DeleteLight(this);
	}

	// Light Data 생성..
	switch (lightType)
	{
	case DIRECTION_LIGHT:
		m_DirectionLight = new DirectionalLightData();

		// Main Directional Light 설정..
		SetDirectionLight(this);
		break;
	case POINT_LIGHT:
		m_PointLight = new PointLightData();
		break;
	case SPOT_LIGHT:
		m_SpotLight = new SpotLightData();
		m_SpotLight->AttRange = cosf(m_SpotLight->Angle);
		break;
	default:
		break;
	}

	// Light Type 변경..
	m_LightType = lightType;

	// Light Direction 값 변동시 View Proj 재설정..
	SetLightViewProj();

	// Light Data 등록..
	LightManager::PushLight(this);
}

void Light::SetCenterPos(DirectX::SimpleMath::Vector3 pos)
{
	m_CenterPos = pos;
}

void Light::SetShadowRadius(float radius)
{
	m_ShadowRadius = radius;
}

void Light::SetIndex(UINT index)
{
	m_LightIndex = index;
}

void Light::SetLightViewProj()
{
	Vector3 dir;

	switch (m_LightType)
	{
	case DIRECTION_LIGHT:
		dir = m_DirectionLight->Direction;
		break;
	case POINT_LIGHT:
		dir = Vector3(1.0f, 1.0f, 1.0f);
		break;
	case SPOT_LIGHT:
		dir = m_SpotLight->Direction;
		break;
	default:
		return;
	}

	DirectX::SimpleMath::Matrix lightView;
	DirectX::SimpleMath::Matrix lightProj;

	/// Light Direction 값 변동시 같이 실행해주어야 함..
	// Light View, Proj 설정..
	Vector3 lightPos = dir * -2.0f * m_ShadowRadius;

	// Ligh View
	lightView = XMMatrixLookAtLH(lightPos, m_CenterPos, Vector4(0.0f, 1.0f, 0.0f, 0.0f));

	Vector3 lightSpace = XMVector3TransformCoord(m_CenterPos, lightView);

	// 장면을 감싸는 광원 공간 직교 투영 상자..
	float l = lightSpace.x - m_ShadowRadius;
	float b = lightSpace.y - m_ShadowRadius;
	float n = lightSpace.z - m_ShadowRadius;
	float r = lightSpace.x + m_ShadowRadius;
	float t = lightSpace.y + m_ShadowRadius;
	float f = lightSpace.z + m_ShadowRadius;

	// Light Proj
	lightProj = XMMatrixOrthographicOffCenterLH(l, r, b, t, n, f);

	// Light View Proj Update..
	switch (m_LightType)
	{
	case DIRECTION_LIGHT:
		m_DirectionLight->LightViewProj = lightView * lightProj;
		break;
	case POINT_LIGHT:
		m_PointLight->LightViewProj = lightView * lightProj;
		break;
	case SPOT_LIGHT:
		m_SpotLight->LightViewProj = lightView * lightProj;
		break;
	default:
		return;
	}
}
