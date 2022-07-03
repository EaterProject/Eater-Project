#include "EngineData.h"
#include "Transform.h"
#include "ParticleSystem.h"
#include "Particle.h"

#include "Material.h"
#include "GameObject.h"
#include "TimeManager.h"
#include "LoadManager.h"
#include "MeshFilter.h"
#include "Utility.h"

ParticleSystem::ParticleSystem()
{
	m_ParticleState				= PARTICLE_STATE::END_STATE;

	m_SystemDesc				= new PARTICLE_SHARE_DESC();
	m_ParticleDesc				= new PARTICLE_DESC();

	m_ParticleData				= new ParticleData();

	m_RandomLifeTime			= new Eater::RandomFloat();
	m_RandomStartColor			= new Eater::RandomVector4();
	m_RandomStartForce			= new Eater::RandomVector3();
	m_RandomStartPosition		= new Eater::RandomVector3();
	m_RandomStartSize			= new Eater::RandomFloat();
	m_RandomStartRotation		= new Eater::RandomFloat();

	m_RandomLifeTimeForce		= new Eater::RandomVector3();
	m_RandomLifeTimeRotation	= new Eater::RandomFloat();

	m_Play = false;
	m_Pause = false;

	// 초기 강도 설정..
	SetStrength(1.0f);
}

ParticleSystem::~ParticleSystem()
{
	Release();
}

void ParticleSystem::Awake()
{
	// Awake에선 GetComponent로 필요한 Component 미리 얻어온다..
	m_MeshFilter = gameobject->GetComponent<MeshFilter>();
}

void ParticleSystem::SetUp()
{
	// Particle DiffuseMap 설정..
	m_MeshFilter->SetDiffuseTextureName(m_DiffuseName);

	// Particle Mesh 설정..
	m_MeshFilter->SetModelName(m_ParticleMeshName);
}

void ParticleSystem::Start()
{
	// Particle System Data 설정..
	gameobject->OneMeshData->Object_Data->ObjType = OBJECT_TYPE::PARTICLE_SYSTEM;
	gameobject->OneMeshData->Particle_Data = m_ParticleData;

	mTimeManager->ResetTime();
}

void ParticleSystem::Update()
{
	if (m_Pause == true || m_Play == false) return;

	float&& dTime = mTimeManager->DeltaTime();

	if (m_NowDelayTime > 0.0f)
	{
		m_TickTime += dTime;

		if (m_TickTime >= m_NowDelayTime)
		{
			// 지연시간이 끝나면 최초 실행..
			m_ParticleState = PARTICLE_STATE::START_STATE;
			m_NowDelayTime = 0.0f;
		}
		return;
	}

	if (m_NowPlayTime > 0.0f)
	{
		if (m_ParticleState == PARTICLE_STATE::PLAY_STATE)
		{
			m_ParticleState = PARTICLE_STATE::PLAY_STATE;
		}

		m_TickTime += dTime;
		m_NowPlayTime -= dTime;

		// 현재 파티클 출력할 시간이 지낫다면 실행..
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
			m_TickTime += dTime;
			
			if (m_RateOverTime <= m_TickTime)
			{
				m_PlayCount = (int)(m_TickTime / m_RateOverTime);
				CreateParticle();
			}

			m_ParticleState = PARTICLE_STATE::START_STATE;
			m_NowPlayTime = 10000.0f;
		}
		else
		{
			// 후 실행 파티클 실행..
			//StartNextParticle();

			// 초기화..
			m_ParticleState = PARTICLE_STATE::END_STATE;
			m_Play = false;
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

	// Particle 생성..
	AddParticle();

	DataUpdate();
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

void ParticleSystem::SetStrength(float strength)
{
	m_Strength = strength;

	m_ParticleData->Particle_Strength = strength;
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

	if (force != Zero_3)
	{
		m_SystemDesc->AniType |= POSITION_ANI;
	}
	else
	{
		if (m_LifeTimeForce.m_Min == Zero_3 && m_LifeTimeForce.m_Max == Zero_3)
		{
			if (m_SystemDesc->AniType & POSITION_ANI)
			{
				m_SystemDesc->AniType ^= POSITION_ANI;
			}
		}
	}
}

void ParticleSystem::SetStartForce(Vector3 minForce, Vector3 maxForce)
{
	m_StartForce.SetRange(minForce, maxForce);

	m_RandomStartForce->SetRange(minForce, maxForce);

	if (minForce != Zero_3 || maxForce != Zero_3)
	{
		m_SystemDesc->AniType |= POSITION_ANI;
	}
	else
	{
		if (m_LifeTimeForce.m_Min == Zero_3 && m_LifeTimeForce.m_Max == Zero_3)
		{
			if (m_SystemDesc->AniType & POSITION_ANI)
			{
				m_SystemDesc->AniType ^= POSITION_ANI;
			}
		}
	}
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

	if (force != Zero_3)
	{
		m_SystemDesc->AniType |= POSITION_ANI;
	}
	else
	{
		if (m_StartForce.m_Min == Zero_3 && m_StartForce.m_Max == Zero_3)
		{
			if (m_SystemDesc->AniType & POSITION_ANI)
			{
				m_SystemDesc->AniType ^= POSITION_ANI;
			}
		}
	}
}

void ParticleSystem::SetLifeTimeForce(Vector3 minForce, Vector3 maxForce)
{
	m_LifeTimeForce.SetRange(minForce, maxForce);
	
	m_RandomLifeTimeForce->SetRange(minForce, maxForce);

	if (minForce != Zero_3 || maxForce != Zero_3)
	{
		m_SystemDesc->AniType |= POSITION_ANI;
	}
	else
	{
		if (m_StartForce.m_Min == Zero_3 && m_StartForce.m_Max == Zero_3)
		{
			if (m_SystemDesc->AniType & POSITION_ANI)
			{
				m_SystemDesc->AniType ^= POSITION_ANI;
			}
		}
	}
}

void ParticleSystem::SetLifeTimeColor(Vector4 minColor, Vector4 maxColor, PARTICLE_LIFETIME_OPTION option)
{
	m_LifeTimeColor.SetRange(minColor, maxColor);

	m_SystemDesc->ColorType = option;

	m_SystemDesc->LifeTimeMinColor = minColor / 255.0f;
	m_SystemDesc->LifeTimeMaxColor = maxColor / 255.0f;

	if (minColor == maxColor)
	{
		if (m_SystemDesc->AniType & COLOR_ANI)
		{
			m_SystemDesc->AniType ^= COLOR_ANI;
		}
	}
	else
	{
		m_SystemDesc->AniType |= COLOR_ANI;
	}

	DataUpdate();
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

	if (minScale == maxScale)
	{
		if (m_SystemDesc->AniType & SCALE_ANI)
		{
			m_SystemDesc->AniType ^= SCALE_ANI;
		}
	}
	else
	{
		m_SystemDesc->AniType |= SCALE_ANI;
	}

	DataUpdate();
}

void ParticleSystem::SetLifeTimeRotation(float rot)
{
	m_LifeTimeRotation.SetRange(rot);

	m_RandomLifeTimeRotation->SetRange(rot, rot);

	if (m_SystemDesc->AniType & ROTATION_ANI)
	{
		m_SystemDesc->AniType ^= ROTATION_ANI;
	}
}

void ParticleSystem::SetLifeTimeRotation(float minRot, float maxRot)
{
	m_LifeTimeRotation.SetRange(minRot, maxRot);
	
	m_RandomLifeTimeRotation->SetRange(minRot, maxRot);

	if (minRot == maxRot)
	{
		if (m_SystemDesc->AniType & ROTATION_ANI)
		{
			m_SystemDesc->AniType ^= ROTATION_ANI;
		}
	}
	else
	{
		m_SystemDesc->AniType |= ROTATION_ANI;
	}
}

void ParticleSystem::SetTextureTiling(int count_x, int count_y)
{
	m_Tiling.m_Width  = count_x;
	m_Tiling.m_Height = count_y;

	m_SystemDesc->Tile_Width = count_x;
	m_SystemDesc->Tile_Height = count_y;
	m_SystemDesc->Total_Frame = count_x * count_y;

	if (count_x > 1 || count_y > 1)
	{
		m_SystemDesc->AniType |= TEXTURE_ANI;
	}
	else
	{
		if (m_SystemDesc->AniType & TEXTURE_ANI)
		{
			m_SystemDesc->AniType ^= TEXTURE_ANI;
		}
	}

	DataUpdate();
}

void ParticleSystem::SetPlayTime(float playTime)
{
	m_PlayTime = playTime;
}

void ParticleSystem::SetLoop(bool loop)
{
	m_Looping = loop;
}

void ParticleSystem::SetDiffuseName(std::string diffuseName)
{
	m_DiffuseName = diffuseName;
}

void ParticleSystem::Play()
{
	// 일시정지 중이라면 다시 재개..
	if (m_Pause)
	{
		Resume();
		return;
	};

	if (m_Play) return;

	// 초기값 설정..
	m_Play = true;
	m_TickTime = 0.0f;
	m_NowPlayTime = m_PlayTime;

	// 지연시간 재설정..
	m_NowDelayTime = m_DelayTime;
}

void ParticleSystem::Pause()
{
	m_Pause = true;

	for (int i = 0; i < m_Particles.size(); i++)
	{
		m_Particles[i]->Pause();
	}
}

void ParticleSystem::Stop()
{
	m_Play = false;
	m_Pause = false;
	m_TickTime = 0.0f;
	m_NowPlayTime = 0.0f;
	m_NowDelayTime = 0.0f;
}

void ParticleSystem::SetNextParticle(ParticleSystem* particle)
{
	m_NextParticles.push_back(particle);
}

void ParticleSystem::SetScale(float& scale)
{
	m_SystemDesc->Scale = { scale, scale };
}

void ParticleSystem::SetScale(float&& scale)
{
	m_SystemDesc->Scale = { scale, scale };
}

void ParticleSystem::SetScale(float& x, float& y)
{
	m_SystemDesc->Scale = { x,y };
}

void ParticleSystem::SetScale(float&& x, float&& y)
{
	m_SystemDesc->Scale = { x,y };
}

void ParticleSystem::SetScale(DirectX::SimpleMath::Vector2 scale)
{
	m_SystemDesc->Scale = scale;
}

std::string ParticleSystem::GetMeshName()
{
	return m_ParticleMeshName;
}

PARTICLE_STATE ParticleSystem::GetState()
{
	return m_ParticleState;
}

PARTICLE_RENDER_OPTION ParticleSystem::GetRenderType()
{
	return m_ParticleData->RenderType;
}

int ParticleSystem::GetMaxParticles()
{
	return m_MaxParticle;
}

float ParticleSystem::GetTotalPlayTime()
{
	return m_PlayTime + m_DelayTime + m_StartLifeTime.m_Max;
}

float ParticleSystem::GetDelayTime()
{
	return m_DelayTime;
}

float ParticleSystem::GetRateOverTime()
{
	return m_RateOverTimeCount;
}

float ParticleSystem::GetStrength()
{
	return m_Strength;
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

std::string ParticleSystem::GetTextureName()
{
	if (m_MeshFilter != nullptr)
	{
		return m_MeshFilter->GetDiffuseTextureName();
	}
	return m_DiffuseName;
}

void ParticleSystem::Resume()
{
	for (int i = 0; i < m_Particles.size(); i++)
	{
		m_Particles[i]->Resume();
	}

	m_Pause = false;
}

void ParticleSystem::DataUpdate()
{
	// Data 변경시 호출하면 Particle Data Update..
	for (Particle* particle : m_Particles)
	{
		particle->DataUpdate();
	}
}

void ParticleSystem::AddParticle()
{
	// ParticleSystem Transform..
	Transform* systemTransform = gameobject->GetTransform();

	// 추가할 Particle 개수..
	int addParticle = m_MaxParticle - (int)m_Particles.size();

	if (addParticle < 1) return;

	// 최대 Particle 개수만큼 만들어 둔다..
	for (int index = 0; index < addParticle; index++)
	{
		// 새로운 Particle..
		GameObject* newObject = new GameObject();
		Particle* newParticle = newObject->AddComponent<Particle>();
		Transform* particleTransform = newObject->AddComponent<Transform>();

		// 해당 Particle Transform 설정..
		newObject->transform = particleTransform;

		// 해당 Particle Data 설정..
		newParticle->m_Index = index;
		newParticle->m_SystemDesc = m_SystemDesc;

		// 생성한 Particle을 ParticleSystem 하위 객체로 연결..
		systemTransform->SetChild(particleTransform);
		particleTransform->SetParent(systemTransform);

		// Particle Data 설정..
		OneParticle* newParticleData = new OneParticle();
		newParticle->m_ParticleData = newParticleData;
		newParticle->Stop();

		// Particle List 삽입..
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
			// 실행 상태의 파티클이 아닌경우 출력..
			if (particle->m_Play == false)
			{
				// Particle Data 설정..
				m_ParticleDesc->LifeTime = m_RandomLifeTime->GetRandomNumber();
				m_ParticleDesc->StartScale = m_RandomStartSize->GetRandomNumber();
				m_ParticleDesc->StartRot = m_RandomStartRotation->GetRandomNumber();
				m_ParticleDesc->StartColor = m_RandomStartColor->GetRandomNumber();
				m_ParticleDesc->StartForce = m_RandomStartForce->GetRandomNumber();
				m_ParticleDesc->StartPos = m_RandomStartPosition->GetRandomNumber();
				m_ParticleDesc->LifeRot = m_RandomLifeTimeRotation->GetRandomNumber();
				m_ParticleDesc->LifeForce = m_RandomLifeTimeForce->GetRandomNumber();

				particle->Play(m_ParticleDesc);
				m_CreateCount++;
				break;
			}
		}
	}

	// 체크용 현재 시간 초기화..
	m_TickTime -= m_CreateCount * m_RateOverTime;
	m_CreateCount = 0;
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
	// Particle List 삭제..
	m_Particles.clear();
	m_NextParticles.clear();

	// Particle Descriptor 해제..
	delete m_SystemDesc;

	// Particle Data 해제..
	for (OneParticle* particle : m_ParticleData->m_Particles)
	{
		delete particle;
	}
	m_ParticleData->m_Particles.clear();

	delete m_ParticleData;
}