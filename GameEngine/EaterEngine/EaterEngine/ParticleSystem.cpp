#include "EngineData.h"
#include "Component.h"
#include "Transform.h"
#include "ParticleSystem.h"
#include "Particle.h"

#include "Material.h"
#include "GameObject.h"
#include "TimeManager.h"
#include "LoadManager.h"
#include "MeshFilter.h"
#include "RandomNumber.h"

ParticleSystem::ParticleSystem()
{
	m_SystemDesc = new PARTICLE_SHARE_DESC();
	m_ParticleDesc = new PARTICLE_DESC();

	m_ParticleData = new ParticleData();

	m_RandomLifeTime = new RandomFloat();
	m_RandomStartColor = new RandomVector4();
	m_RandomStartForce = new RandomVector3();
	m_RandomStartPosition = new RandomVector3();
	m_RandomStartSize = new RandomFloat();
	m_RandomStartRotation = new RandomFloat();

	m_RandomLifeTimeForce = new RandomVector3();
	m_RandomLifeTimeRotation = new RandomFloat();
}

ParticleSystem::~ParticleSystem()
{
	Release();
}

void ParticleSystem::Awake()
{
	// Awake���� GetComponent�� �ʿ��� Component �̸� ���´�..
	m_MeshFilter = gameobject->GetComponent<MeshFilter>();
}

void ParticleSystem::SetUp()
{
	// Particle DiffuseMap ����..
	m_MeshFilter->SetDiffuseTextureName(m_DiffuseName);

	// Particle Mesh ����..
	m_MeshFilter->SetModelName(m_ParticleMeshName);
}

void ParticleSystem::Start()
{
	// Particle System Data ����..
	gameobject->OneMeshData->Object_Data->ObjType = OBJECT_TYPE::PARTICLE_SYSTEM;
	gameobject->OneMeshData->Particle_Data = m_ParticleData;

	mTimeManager->ResetTime();
}

void ParticleSystem::Update()
{
	float dTime = mTimeManager->DeltaTime();

	if (m_Playing == false) return;

	if (m_NowDelayTime > 0.0f)
	{
		m_TickTime += dTime;

		if (m_TickTime >= m_NowDelayTime)
		{
			m_TickTime = 0.0f;
			m_NowDelayTime = 0.0f;

			// �����ð��� ������ ���� ����..
			StartPlay();
		}
	}
	else if (m_NowPlayTime > 0.0f)
	{
		m_TickTime += dTime;
		m_NowPlayTime -= dTime;

		// ���� ��ƼŬ ����� �ð��� �����ٸ� ����..
		if (m_RateOverTime <= m_TickTime)
		{
			m_PlayCount = (int)(m_TickTime / m_RateOverTime);
			CreateParticle();
		}
	}
	else
	{
		if (m_Looping)
		{
			// ���� ��ƼŬ ����� �ð��� �����ٸ� ����..
			if (m_RateOverTime <= m_TickTime)
			{
				m_PlayCount = (int)(m_TickTime / m_RateOverTime);
				CreateParticle();
			}

			m_NowPlayTime = 10.0f;
		}
		else
		{
			// �� ���� ��ƼŬ ����..
			StartNextParticle();

			// �ʱ�ȭ..
			m_Playing = false;
			m_TickTime = 0.0f;
			m_NowPlayTime = 0.0f;
		}
	}
}

void ParticleSystem::DataUpdate()
{
	// Data ����� ȣ���ϸ� Particle Data Update..
	for (Particle* particle : m_Particles)
	{
		particle->DataUpdate();
	}
}

void ParticleSystem::SetMeshName(std::string meshName)
{
	m_ParticleMeshName = meshName;
}

void ParticleSystem::SetRenderType(PARTICLE_RENDER_OPTION renderType)
{
	m_ParticleData->RenderType = renderType;
}

void ParticleSystem::SetMaxParticles(int maxCount)
{
	m_MaxParticle = maxCount;
	m_ParticleData->Particle_Count = maxCount;

	// Particle ����..
	AddParticle();
}

void ParticleSystem::SetDelayTime(float delay)
{
	m_DelayTime = delay;
}

void ParticleSystem::SetRateOverTime(float count)
{
	m_RateOverTimeCount = count;

	m_RateOverTime = 1.0f / count;
}

void ParticleSystem::SetShapeRadius(float radius)
{
	m_ParticleData->Area_Radius = Vector3(radius);

	m_RandomStartPosition->SetRange(Vector3(-radius, -radius, -radius), Vector3(radius, radius, radius));
}

void ParticleSystem::SetShapeRadius(float x, float y, float z)
{
	m_ParticleData->Area_Radius = Vector3(x, y, z);

	m_RandomStartPosition->SetRange(Vector3(-x, -y, -z), Vector3(x, y, z));
}

void ParticleSystem::SetStartForce(Vector3 force)
{	
	m_StartForce.SetRange(force);

	m_RandomStartForce->SetRange(force, force);
}

void ParticleSystem::SetStartForce(Vector3 minForce, Vector3 maxForce)
{
	m_StartForce.SetRange(minForce, maxForce);

	m_RandomStartForce->SetRange(minForce, maxForce);
}

void ParticleSystem::SetStartColor(Vector4 color)
{
	m_StartColor.SetRange(color);

	m_RandomStartColor->SetRange(color / 255.0f, color / 255.0f);
}

void ParticleSystem::SetStartColor(Vector4 minColor, Vector4 maxColor)
{
	m_StartColor.SetRange(minColor, maxColor);
	
	m_RandomStartColor->SetRange(minColor / 255.0f, maxColor / 255.0f);
}

void ParticleSystem::SetStartLifeTime(float time)
{
	m_StartLifeTime.SetRange(time);

	m_RandomLifeTime->SetRange(time, time);
}

void ParticleSystem::SetStartLifeTime(float minTime, float maxTime)
{
	m_StartLifeTime.SetRange(minTime, maxTime);
	
	m_RandomLifeTime->SetRange(minTime, maxTime);
}

void ParticleSystem::SetStartScale(float scale)
{
	m_StartScale.SetRange(scale);

	m_RandomStartSize->SetRange(scale, scale);
}

void ParticleSystem::SetStartScale(float minScale, float maxScale)
{
	m_StartScale.SetRange(minScale, maxScale);
	
	m_RandomStartSize->SetRange(minScale, maxScale);
}

void ParticleSystem::SetStartRotation(float rot)
{
	m_StartRotation.SetRange(rot);

	m_RandomStartRotation->SetRange(rot, rot);
}

void ParticleSystem::SetStartRotation(float minRot, float maxRot)
{
	m_StartRotation.SetRange(minRot, maxRot);
	
	m_RandomStartRotation->SetRange(minRot, maxRot);
}

void ParticleSystem::SetLifeTimeForce(Vector3 force)
{
	m_LifeTimeForce.SetRange(force);

	m_RandomLifeTimeForce->SetRange(force, force);
}

void ParticleSystem::SetLifeTimeForce(Vector3 minForce, Vector3 maxForce)
{
	m_LifeTimeForce.SetRange(minForce, maxForce);
	
	m_RandomLifeTimeForce->SetRange(minForce, maxForce);
}

void ParticleSystem::SetLifeTimeColor(Vector4 minColor, Vector4 maxColor, PARTICLE_LIFETIME_OPTION option)
{
	m_LifeTimeColor.SetRange(minColor, maxColor);

	m_SystemDesc->ColorType = option;

	m_SystemDesc->LifeTimeMinColor = minColor / 255.0f;
	m_SystemDesc->LifeTimeMaxColor = maxColor / 255.0f;
}

void ParticleSystem::SetLifeTimeScale(float minScale, float maxScale, PARTICLE_LIFETIME_OPTION option)
{
	m_LifeTimeScale.SetRange(minScale, maxScale);

	m_SystemDesc->ScaleType = option;

	if (minScale <= maxScale)
	{
		m_SystemDesc->LifeTimeMinScale = minScale;
		m_SystemDesc->LifeTimeMaxScale = maxScale;
	}
	else
	{
		m_SystemDesc->LifeTimeMinScale = maxScale;
		m_SystemDesc->LifeTimeMaxScale = minScale;
	}
}

void ParticleSystem::SetLifeTimeRotation(float rot)
{
	m_LifeTimeRotation.SetRange(rot);

	m_RandomLifeTimeRotation->SetRange(rot, rot);
}

void ParticleSystem::SetLifeTimeRotation(float minRot, float maxRot)
{
	m_LifeTimeRotation.SetRange(minRot, maxRot);
	
	m_RandomLifeTimeRotation->SetRange(minRot, maxRot);
}

void ParticleSystem::SetTextureTiling(int count_x, int count_y)
{
	m_Tiling.m_Height = count_x;
	m_Tiling.m_Height = count_y;

	m_SystemDesc->Tile_Width = count_x;
	m_SystemDesc->Tile_Height = count_y;
	m_SystemDesc->Total_Frame = count_x * count_y;
}

void ParticleSystem::SetPlayTime(float playTime, bool loop /*= false*/)
{
	m_PlayTime = playTime;
	m_Looping = loop;
}

void ParticleSystem::SetDiffuseName(std::string diffuseName)
{
	m_DiffuseName = diffuseName;
}

void ParticleSystem::Play(bool loop)
{
	if (loop) m_Looping = true;

	m_Playing = true;
	m_TickTime = 0.0f;
	m_NowPlayTime = m_PlayTime;

	// �����ð� �缳��..
	m_NowDelayTime = m_DelayTime;

	// �����ð��� �������� �ʾҴٸ� ���� ��� ��ƼŬ �Ѱ� ���..
	if (m_DelayTime == 0.0f) StartPlay();
}

void ParticleSystem::Stop()
{
	m_Playing = false;
	m_TickTime = 0.0f;
	m_NowPlayTime = 0.0f;
	m_NowDelayTime = 0.0f;
}

void ParticleSystem::SetNextParticle(ParticleSystem* particle)
{
	m_NextParticles.push_back(particle);
}

std::string ParticleSystem::GetMeshName()
{
	return m_ParticleMeshName;
}

PARTICLE_RENDER_OPTION ParticleSystem::GetRenderType()
{
	return m_ParticleData->RenderType;
}

int ParticleSystem::GetMaxParticles()
{
	return m_MaxParticle;
}

float ParticleSystem::GetDelayTime()
{
	return m_DelayTime;
}

float ParticleSystem::GetRateOverTime()
{
	return m_RateOverTimeCount;
}

float ParticleSystem::GetPlayTime()
{
	return m_PlayTime;
}

bool ParticleSystem::GetLoop()
{
	return m_Looping;
}

Vector3 ParticleSystem::GetShapeRadius()
{
	return m_ParticleData->Area_Radius;
}

Range<Vector3> ParticleSystem::GetStartForce()
{
	return m_StartForce;
}

Range<Vector4> ParticleSystem::GetStartColor()
{
	return m_StartColor;
}

Range<float> ParticleSystem::GetStartLifeTime()
{
	return m_StartLifeTime;
}

Range<float> ParticleSystem::GetStartScale()
{
	return m_StartScale;
}

Range<float> ParticleSystem::GetStartRotation()
{
	return m_StartRotation;
}

Range<Vector3> ParticleSystem::GetLifeTimeForce()
{
	return m_LifeTimeForce;
}

Range<Vector4> ParticleSystem::GetLifeTimeColor()
{
	return m_LifeTimeColor;
}

Range<float> ParticleSystem::GetLifeTimeScale()
{
	return m_LifeTimeScale;
}

Range<float> ParticleSystem::GetLifeTimeRotation()
{
	return m_LifeTimeRotation;
}

PARTICLE_LIFETIME_OPTION ParticleSystem::GetLifeTimeColorOption()
{
	return m_SystemDesc->ColorType;
}

PARTICLE_LIFETIME_OPTION ParticleSystem::GetLifeTimeScaleOption()
{
	return m_SystemDesc->ScaleType;
}

Range<int> ParticleSystem::GetTextureTiling()
{
	return m_Tiling;
}

void ParticleSystem::StartPlay()
{
	// ���۽� �Ѱ��� ���..
	m_PlayCount = 1;

	CreateParticle();
}

void ParticleSystem::AddParticle()
{
	// ParticleSystem Transform..
	Transform* systemTransform = gameobject->GetTransform();

	// �߰��� Particle ����..
	int addParticle = m_MaxParticle - (int)m_Particles.size();

	if (addParticle < 1) return;

	// �ִ� Particle ������ŭ ����� �д�..
	for (int index = 0; index < addParticle; index++)
	{
		// ���ο� Particle..
		GameObject* newObject = new GameObject();
		Particle* newParticle = newObject->AddComponent<Particle>();
		Transform* particleTransform = newObject->AddComponent<Transform>();

		// �ش� Particle Transform ����..
		newObject->transform = particleTransform;

		// �ش� Particle Data ����..
		newParticle->m_Index = index;
		newParticle->m_SystemDesc = m_SystemDesc;

		// ������ Particle�� ParticleSystem ���� ��ü�� ����..
		systemTransform->SetChild(particleTransform);
		particleTransform->SetParent(systemTransform);

		// Particle Data ����..
		OneParticle* newParticleData = new OneParticle();
		newParticle->m_ParticleData = newParticleData;

		// Particle List ����..
		m_Particles.push_back(newParticle);
		m_ParticleData->m_Particles.push_back(newParticleData);
	}
}

void ParticleSystem::CreateParticle()
{
	for (int i = 0; i < m_PlayCount; i++)
	{
		for (Particle* particle : m_Particles)
		{
			// ���� ������ ��ƼŬ�� �ƴѰ�� ���..
			if (particle->m_Playing == false)
			{
				// Particle Data ����..
				m_ParticleDesc->LifeTime = m_RandomLifeTime->GetRandomNumber();
				m_ParticleDesc->StartScale = m_RandomStartSize->GetRandomNumber();
				m_ParticleDesc->StartRot = m_RandomStartRotation->GetRandomNumber();
				m_ParticleDesc->StartColor = m_RandomStartColor->GetRandomNumber();
				m_ParticleDesc->StartForce = m_RandomStartForce->GetRandomNumber();
				m_ParticleDesc->StartPos = m_RandomStartPosition->GetRandomNumber();
				m_ParticleDesc->LifeRot = m_RandomLifeTimeRotation->GetRandomNumber();
				m_ParticleDesc->LifeForce = m_RandomLifeTimeForce->GetRandomNumber();

				particle->SetPlay(m_ParticleDesc);
				break;
			}
		}
	}

	// üũ�� ���� �ð� �ʱ�ȭ..
	m_TickTime = 0.0f;
}

void ParticleSystem::StartNextParticle()
{
	for (ParticleSystem* particle : m_NextParticles)
	{
		particle->Play();
	}
}

void ParticleSystem::Release()
{
	// Particle List ����..
	m_Particles.clear();
	m_NextParticles.clear();

	// Particle Descriptor ����..
	delete m_SystemDesc;

	// Particle Data ����..
	for (OneParticle* particle : m_ParticleData->m_Particles)
	{
		delete particle;
	}
	m_ParticleData->m_Particles.clear();

	delete m_ParticleData;
}