#include "Component.h"
#include "Transform.h"
#include "EngineData.h"
#include "ParticleSystem.h"
#include "Particle.h"
#include "TimeManager.h"
#include "LoadManager.h"
#include "GameObject.h"
#include <random>

#define LERP(prev, next, time) ((prev * (1.0f - time)) + (next * time))

Particle::Particle()
{
}

Particle::~Particle()
{

}

void Particle::Awake()
{
	m_Transform = gameobject->transform;

	m_ParticleData->World = m_Transform->GetWorld();
	m_ParticleData->Local = m_Transform->GetLocal();
}

void Particle::Start()
{
	// Object Type ����..
	gameobject->OneMeshData->Object_Data->ObjType = OBJECT_TYPE::PARTICLE;

	// �ʱ� Data ����..
	DataUpdate();
}

void Particle::Update()
{
	if (m_Playing == false) return;

	// �ش� ��ƼŬ ������Ʈ..
	if (m_LifeTime > 0.0f)
	{
		float dTime = mTimeManager->DeltaTime();

		m_TexNowTime += dTime;
		m_AniNowTime += dTime;
		m_LifeTime -= dTime;

		m_NextFrame = (int)(m_TexNowTime / m_TexOneFrame);

		// Texture Animation..
		if (m_TexFrame != m_NextFrame)
		{
			m_TexFrame = m_NextFrame;

			if (m_TexFrame >= m_TexTotalFrame)
			{
				m_TexNowFrame = m_TexTotalFrame - 1;
			}
			else
			{
				m_TexNowFrame = m_TexFrame;
			}

			// UV ���� �� ����..
			if (m_AniType & TEXTURE_ANI)
			{
				// Texture ��� ���� ����..
				m_ParticleData->TexPos.x = (m_TexNowFrame % m_WidthCount) * m_ParticleData->TexScale.x;
				m_ParticleData->TexPos.y = (m_TexNowFrame / m_WidthCount) * m_ParticleData->TexScale.y;
			}
		}

		m_NextFrame = (int)(m_AniNowTime / m_AniOneFrame) + 1;
		
		// Pos & Rot & Scale & Color Animation..
		if (m_AniFrame != m_NextFrame)
		{
			m_AniFrame = m_NextFrame;
			m_AniPrevFrame = m_AniFrame - 1;
			m_AniNextFrame = m_AniFrame;

			if (m_AniPrevFrame < 0)
			{
				m_AniPrevFrame = 0;
			}
			if (m_AniNextFrame > m_AniTotalFrame)
			{
				m_AniNextFrame = m_AniTotalFrame;
				m_AniPrevFrame = m_AniTotalFrame - 1;
			}

			// Position Data ���� �� ����..
			if (m_AniType & POSITION_ANI)
			{
				// Position ���� ����..
				m_PrevPos = m_StartPos + m_OnePos * (float)m_AniPrevFrame;
				m_NextPos = m_StartPos + m_OnePos * (float)m_AniNextFrame;
			}

			// Rotation Data ���� �� ����..
			if (m_AniType & ROTATION_ANI)
			{
				// Rotation ���� ����..
				m_PrevRot = m_StartRot + m_OneRot * (float)m_AniPrevFrame;
				m_NextRot = m_StartRot + m_OneRot * (float)m_AniNextFrame;
			}

			// Scale Data ���� �� ����..
			if (m_AniType & SCALE_ANI)
			{
				if (m_AniNextFrame <= m_ScaleTurningFrame)
				{
					// Scale ���� ����..
					m_PrevScale = m_StartScale + m_OneScale * m_AniPrevFrame;
					m_NextScale = m_StartScale + m_OneScale * m_AniNextFrame;
				}
				else
				{
					// Scale ���� ����..
					m_PrevScale = m_StartScale + m_OneScale * (m_AniTotalFrame - m_AniPrevFrame);
					m_NextScale = m_StartScale + m_OneScale * (m_AniTotalFrame - m_AniNextFrame);
				}
			}

			// Color Data ���� �� ����..
			if (m_AniType & COLOR_ANI)
			{
				if (m_AniNextFrame <= m_ColorTurningFrame)
				{
					// Color ���� ����..
					m_PrevColor = m_StartColor + m_OneColor * (float)m_AniPrevFrame;
					m_NextColor = m_StartColor + m_OneColor * (float)m_AniNextFrame;
				}
				else
				{
					// Color ���� ����..
					m_PrevColor = m_StartColor + m_OneColor * (float)(m_AniTotalFrame - m_AniPrevFrame);
					m_NextColor = m_StartColor + m_OneColor * (float)(m_AniTotalFrame - m_AniNextFrame);
				}
			}
		}

		m_OneTickFrame = (m_AniNowTime / m_AniFrame) / m_AniOneFrame;

		// ��ƼŬ ������ ����..
		if (m_AniType & COLOR_ANI)
		{
			m_NowColor = Vector4::Lerp(m_PrevColor, m_NextColor, m_OneTickFrame);
			m_ParticleData->Color = m_NowColor;
		}
		if (m_AniType & POSITION_ANI)
		{
			m_NowPos = Vector3::Lerp(m_PrevPos, m_NextPos, m_OneTickFrame);
			m_ParticleData->Pos = m_NowPos;
			m_Transform->SetPosition(m_NowPos);
		}
		if (m_AniType & ROTATION_ANI)
		{
			m_NowRot.z = LERP(m_PrevRot, m_NextRot, m_OneTickFrame);
			m_Transform->SetRotate(m_NowRot);
		}
		if (m_AniType & SCALE_ANI)
		{
			m_NowScale = LERP(m_PrevScale, m_NextScale, m_OneTickFrame);
			m_Transform->SetScale(m_NowScale);
		}
	}
	else
	{
		Reset();
	}
}

void Particle::SetPlay(const PARTICLE_DESC* particleDesc)
{
	m_Playing = true;

	// Animation Type �缳��..
	m_AniType = 0;

	// ���� ��ƼŬ ����ð� ����..
	m_LifeTime = particleDesc->LifeTime;

	// ���� ��ƼŬ Data ����..
	m_ParticleData->Playing = true;

	// Texture Frame Data ����..
	m_TexOneFrame = particleDesc->LifeTime / (float)m_TexTotalFrame;
	m_TexFrame = 1;
	m_TexNowFrame = 0;

	// Animation Frame Data ����..
	m_AniOneFrame = particleDesc->LifeTime / (float)m_AniTotalFrame;
	m_AniFrame = 1;
	m_AniNextFrame = 0;

	// ��ƼŬ ���� ����..
	Vector4 maxColor = particleDesc->StartColor * m_SystemDesc->LifeTimeMaxColor;
	Vector4 minColor = particleDesc->StartColor * m_SystemDesc->LifeTimeMinColor;

	switch (m_SystemDesc->ColorType)
	{
	case PARTICLE_LIFETIME_OPTION::NONE:
		m_StartColor = maxColor;
		m_PrevColor = maxColor;
		m_OneColor = Vector4(0, 0, 0, 0);
		break;
	case PARTICLE_LIFETIME_OPTION::UP:
		m_StartColor = minColor;
		m_PrevColor = minColor;
		m_OneColor = (maxColor - minColor) / (float)m_AniTotalFrame;
		break;
	case PARTICLE_LIFETIME_OPTION::DOWN:
		m_StartColor = maxColor;
		m_PrevColor = maxColor;
		m_OneColor = (minColor - maxColor) / (float)m_AniTotalFrame;
		break;
	case PARTICLE_LIFETIME_OPTION::UPDOWN:
		m_StartColor = minColor;
		m_PrevColor = minColor;
		m_OneColor = (maxColor - minColor) / (float)m_ColorTurningFrame;
		break;
	default:
		break;
	}

	m_NextColor = m_PrevColor + m_OneColor;
	m_ParticleData->Color = m_PrevColor;

	// ��ƼŬ ������ ����..
	float maxScale = particleDesc->StartScale * m_SystemDesc->LifeTimeMaxScale;
	float minScale = particleDesc->StartScale * m_SystemDesc->LifeTimeMinScale;

	// LifeTime Size Option..
	switch (m_SystemDesc->ScaleType)
	{
	case PARTICLE_LIFETIME_OPTION::NONE:
		m_StartScale = maxScale;
		m_PrevScale = maxScale;
		m_OneScale = 0;
		break;
	case PARTICLE_LIFETIME_OPTION::UP:
		m_StartScale = minScale;
		m_PrevScale = minScale;
		m_OneScale = (maxScale - minScale) / (float)m_AniTotalFrame;
		break;
	case PARTICLE_LIFETIME_OPTION::DOWN:
		m_StartScale = maxScale;
		m_PrevScale = maxScale;
		m_OneScale = (minScale - maxScale) / (float)m_AniTotalFrame;
		break;
	case PARTICLE_LIFETIME_OPTION::UPDOWN:
		m_StartScale = minScale;
		m_PrevScale = minScale;
		m_OneScale = (maxScale - minScale) / (float)m_ScaleTurningFrame;
		break;
	default:
		break;
	}

	Transform* transform = gameobject->transform;

	m_NextScale = m_PrevScale + m_OneScale;
	transform->SetScale(m_PrevScale);

	// ��ƼŬ ȸ�� ����..
	m_StartRot = particleDesc->StartRot;
	m_OneRot = particleDesc->LifeRot / (float)m_AniTotalFrame;
	m_PrevRot = m_StartRot;
	m_NextRot = m_PrevRot + m_OneRot;
	m_NowRot.z = m_PrevRot;
	transform->SetRotate(m_NowRot);

	// ��ƼŬ ��ġ ����..
	m_StartPos = particleDesc->StartPos;
	m_OnePos = (particleDesc->StartForce + particleDesc->LifeForce) / (float)m_AniTotalFrame;
	m_PrevPos = m_StartPos;
	m_NextPos = m_PrevPos + m_OnePos;
	m_ParticleData->Pos = m_PrevPos;
	transform->SetPosition(m_PrevPos);

	// Animation Type ����..
	if (m_OneColor != XMVectorZero())	m_AniType |= COLOR_ANI;
	if (m_OnePos != XMVectorZero())		m_AniType |= POSITION_ANI;
	if (m_OneRot != 0.0f)				m_AniType |= ROTATION_ANI;
	if (m_OneScale != 0.0f)				m_AniType |= SCALE_ANI;
	if (m_TexTotalFrame > 1)			m_AniType |= TEXTURE_ANI;
}

void Particle::DataUpdate()
{
	// Frame ����..
	m_TexTotalFrame = m_SystemDesc->Total_Frame;

	m_AniTotalFrame = 60;

	// Option�� ���� Scale Turning Frame ����..
	if (m_SystemDesc->ScaleType == UPDOWN)
		m_ScaleTurningFrame = m_AniTotalFrame / 2;
	else
		m_ScaleTurningFrame = m_AniTotalFrame;

	// Option�� ���� Color Turning Frame ����..
	if (m_SystemDesc->ColorType == UPDOWN)
		m_ColorTurningFrame = m_AniTotalFrame / 2;
	else
		m_ColorTurningFrame = m_AniTotalFrame;

	m_WidthCount = m_SystemDesc->Tile_Width;
	m_HeightCount = m_SystemDesc->Tile_Height;

	m_ParticleData->TexPos = Vector2(0.0f, 0.0f);
	m_ParticleData->TexScale = Vector2(1.0f / m_SystemDesc->Tile_Width, 1.0f / m_SystemDesc->Tile_Height);
}

void Particle::Reset()
{
	m_ParticleData->Playing = false;

	m_Playing = false;
	m_LifeTime = 0.0f;
	m_TexNowTime = 0.0f;
	m_TexFrame = 1;
	m_AniNowTime = 0.0f;
	m_AniFrame = 1;
}

void Particle::Release()
{
	m_Transform = nullptr;

	// ���� Particle ���� �����ϴ� �����ʹ� ParticleSystem���� ������..
	// �����͸� �ʱ�ȭ..
	m_SystemDesc = nullptr;
	m_ParticleData = nullptr;
}
