#include "GameObject.h"
#include "Transform.h"
#include "BaseManager.h"
#include "LightManager.h"
#include "Light.h"
#include "EngineData.h"

#define SAFE_DELETE(x) { if(x != nullptr) {delete x; x = nullptr;} }

using namespace DirectX;
using namespace SimpleMath;

Light* Light::g_DirLight = nullptr;

Light::Light()
	:m_LightType(LIGHT_TYPE::NONE_LIGHT), m_Transform(nullptr)
{
	m_CenterPos = Vector3(0, 100, 0);
	m_ShadowRadius = sqrtf(10.0f * 10.0f + 15.0f * 15.0f) * 15;
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
}

void Light::Update()
{
	// �� ������ Light Position ����..
	switch (m_LightType)
	{
	case POINT_LIGHT:
		m_PointLight->Position = m_Transform->Position;
		break;
	case SPOT_LIGHT:
		m_SpotLight->Position = m_Transform->Position;
		break;
	default:
		break;
	}
}

void Light::SetColor(float r, float g, float b, float a)
{
	switch (m_LightType)
	{
	case DIRECTION_LIGHT:
		m_DirectionLight->Diffuse = { r, g, b, a };
		break;
	case POINT_LIGHT:
		m_PointLight->Diffuse = { r, g, b, a };
		break;
	case SPOT_LIGHT:
		m_SpotLight->Diffuse = { r, g, b, a };
		break;
	default:
		break;
	}
}

void Light::SetDirection(float x, float y, float z)
{
	switch (m_LightType)
	{
	case DIRECTION_LIGHT:
		m_DirectionLight->Direction = { x, y, z };

		// Light Direction �� ������ View Proj �缳��..
		SetLightViewProj();
		break;
	case SPOT_LIGHT:
		m_SpotLight->Direction = { x, y, z };

		// Light Direction �� ������ View Proj �缳��..
		SetLightViewProj();
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

void Light::SetAtt(float x, float y, float z)
{
	switch (m_LightType)
	{
	case SPOT_LIGHT:
		m_SpotLight->Att = { x, y, z };
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

void Light::SetSpot(float spot)
{
	switch (m_LightType)
	{
	case SPOT_LIGHT:
		m_SpotLight->Spot = spot;
		break;
	default:
		break;
	}
}

void Light::SetType(LIGHT_TYPE lightType)
{
	if (lightType == m_LightType) return;

	// ���� Light Data ����..
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

		// Light Data ����..
		LightManager::DeleteLight(this);
	}

	// Light Data ����..
	switch (lightType)
	{
	case DIRECTION_LIGHT:
		m_DirectionLight = new DirectionalLightData();

		// Main Directional Light ����..
		SetDirectionLight(this);
		break;
	case POINT_LIGHT:
		m_PointLight = new PointLightData();
		break;
	case SPOT_LIGHT:
		m_SpotLight = new SpotLightData();
		break;
	default:
		break;
	}

	// Light Type ����..
	m_LightType = lightType;

	// Light Direction �� ������ View Proj �缳��..
	SetLightViewProj();

	// Light Data ���..
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

	/// Light Direction �� ������ ���� �������־�� ��..
	// Light View, Proj ����..
	Vector3 lightPos = dir * -2.0f * m_ShadowRadius;

	// Ligh View
	lightView = XMMatrixLookAtLH(lightPos, m_CenterPos, Vector4(0.0f, 1.0f, 0.0f, 0.0f));

	Vector3 lightSpace = XMVector3TransformCoord(m_CenterPos, lightView);

	// ����� ���δ� ���� ���� ���� ���� ����..
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
