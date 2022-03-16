#include "EngineData.h"
#include "Component.h"
#include "Transform.h"
#include "ParticleSystem.h"
#include "Particle.h"

#include "Material.h"
#include "GameObject.h"
#include "BaseManager.h"
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
	m_MeshFilter->SetMeshName(m_ParticleMeshName);
}

void ParticleSystem::Start()
{
	// Particle System Data ����..
	gameobject->OneMeshData->Object_Data->ObjType = OBJECT_TYPE::PARTICLE_SYSTEM;
	gameobject->OneMeshData->Particle_Data = m_ParticleData;
	
	// Particle ����..
	AddParticle();

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
}

void ParticleSystem::SetDelayTime(float delay)
{
	m_DelayTime = delay;
}

void ParticleSystem::SetRateOverTime(float count)
{
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
	m_RandomStartForce->SetRange(force, force);
}

void ParticleSystem::SetStartForce(Vector3 minForce, Vector3 maxForce)
{
	m_RandomStartForce->SetRange(minForce, maxForce);
}

void ParticleSystem::SetStartColor(Vector4 color)
{
	m_RandomStartColor->SetRange(color / 255.0f, color / 255.0f);
}

void ParticleSystem::SetStartColor(Vector4 minColor, Vector4 maxColor)
{
	m_RandomStartColor->SetRange(minColor / 255.0f, maxColor / 255.0f);
}

void ParticleSystem::SetStartLifeTime(float time)
{
	m_RandomLifeTime->SetRange(time, time);
}

void ParticleSystem::SetStartLifeTime(float minTime, float maxTime)
{
	m_RandomLifeTime->SetRange(minTime, maxTime);
}

void ParticleSystem::SetStartScale(float scale)
{
	m_RandomStartSize->SetRange(scale, scale);
}

void ParticleSystem::SetStartScale(float minScale, float maxScale)
{
	m_RandomStartSize->SetRange(minScale, maxScale);
}

void ParticleSystem::SetStartRotation(float rot)
{
	m_RandomStartRotation->SetRange(rot, rot);
}

void ParticleSystem::SetStartRotation(float minRot, float maxRot)
{
	m_RandomStartRotation->SetRange(minRot, maxRot);
}

void ParticleSystem::SetLifeTimeForce(Vector3 force)
{
	m_RandomLifeTimeForce->SetRange(force, force);
}

void ParticleSystem::SetLifeTimeForce(Vector3 minForce, Vector3 maxForce)
{
	m_RandomLifeTimeForce->SetRange(minForce, maxForce);
}

void ParticleSystem::SetLifeTimeColor(Vector4 minColor, Vector4 maxColor, PARTICLE_LIFETIME_OPTION option)
{
	m_SystemDesc->ColorType = option;

	m_SystemDesc->LifeTimeMinColor = minColor / 255.0f;
	m_SystemDesc->LifeTimeMaxColor = maxColor / 255.0f;
}

void ParticleSystem::SetLifeTimeScale(float minScale, float maxScale, PARTICLE_LIFETIME_OPTION option)
{
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
	m_RandomLifeTimeRotation->SetRange(rot, rot);
}

void ParticleSystem::SetLifeTimeRotation(float minRot, float maxRot)
{
	m_RandomLifeTimeRotation->SetRange(minRot, maxRot);
}

void ParticleSystem::SetTextureTiling(int count_x, int count_y)
{
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

	// �ִ� Particle ������ŭ ����� �д�..
	for (int index = 0; index < m_MaxParticle; index++)
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