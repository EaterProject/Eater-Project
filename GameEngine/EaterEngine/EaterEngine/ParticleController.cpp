#include "ParticleController.h"
#include "ParticleSystem.h"

#include "TimeManager.h"
#include "GameObject.h"
#include "EaterEngineAPI.h"

#include "Profiler/Profiler.h"


EATER_ENGINEDLL ParticleController::ParticleController()
	:m_ControllerState(PARTICLE_STATE::END_STATE), m_TotalPlayTime(0.0f), m_PlayTime(0.0f), m_StartTime(0.0f), m_NowParticleListSize(0), m_Pause(false)
{

}

ParticleController::~ParticleController()
{

}

void ParticleController::Update()
{
	if (m_Pause) return;

	switch (m_ControllerState)
	{
	case PARTICLE_STATE::START_STATE:
	{
		float&& dTime = mTimeManager->DeltaTime();
		
		m_ControllerState = PARTICLE_STATE::PLAY_STATE;

		m_PlayTime = dTime;

		// Particle System List Update..
		UpdateController();
	}
	break;
	case PARTICLE_STATE::PLAY_STATE:
	{
		float&& dTime = mTimeManager->DeltaTime();

		m_PlayTime += dTime;

		// Particle System List Update..
		UpdateController();
	}
	break;
	case PARTICLE_STATE::PLAY_STAY_STATE:
	{
		float&& dTime = mTimeManager->DeltaTime();
		
		m_PlayTime += dTime;

		// 모든 파티클들이 끝났을 경우 상태 변경..
		if (m_TotalPlayTime <= m_PlayTime)
		{
			m_PlayTime = 0.0f;
			m_ControllerState = PARTICLE_STATE::END_STATE;
		}
	}
		break;
	default:
		break;
	}
}

void ParticleController::PushParticle(std::string particle_key, ParticleSystem* particle, float start_time)
{
	if (particle == nullptr) return;

	ParticleNode&& node_itor = m_ParticleNodeDataList.find(particle_key);

	// 해당 키에 대한 파티클이 삽입되어 있다면..
	if (node_itor != m_ParticleNodeDataList.end()) return;

	// 하이어라키 연결..
	GameObject* object = particle->gameobject;
	object->ChoiceParent(gameobject);

	// 새로운 파티클 노드 데이터 생성..
	NodeData* node_data = new NodeData();

	// 파티클 노드 삽입..
	m_ParticleNodeDataList.insert({ particle_key, node_data });

	ParticleList&& list_itor = m_ParticleSystemList.find(start_time);

	// 파티클 리스트에서 위치하는 인덱스..
	int list_index = 0;

	ParticleData particle_data;
	particle_data.Node_Data = node_data;
	particle_data.Particle_Value = particle;

	// 해당 시간에 대한 파티클 리스트에 삽입..
	if (list_itor == m_ParticleSystemList.end())
	{
		m_ParticleSystemList.insert({ start_time, std::vector<ParticleData>(1, particle_data) });
	}
	else
	{
		list_index = list_itor->second.size();
		list_itor->second.push_back(particle_data);
	}

	// 새로운 파티클 노드 데이터 설정..
	node_data->Time_Key = start_time;
	node_data->Particle_Value = particle;
	node_data->List_Index = list_index;

	// TotalTime 재설정..
	SetTotalTime();
}

void ParticleController::PushParticle(std::string particle_key, GameObject* particle, float start_time)
{
	ParticleSystem* system = particle->GetComponent<ParticleSystem>();

	if (system == nullptr) return;

	PushParticle(particle_key, system, start_time);
}

void ParticleController::PopParticle(std::string particle_key)
{
	// 해당 노드 검색..
	ParticleNode&& node_itor = m_ParticleNodeDataList.find(particle_key);

	// 해당 키에 대한 파티클이 삽입되있지 않다면..
	if (node_itor == m_ParticleNodeDataList.end()) return;

	NodeData* node_data = node_itor->second;

	// 해당 리스트 검색..
	ParticleList&& list_itor = m_ParticleSystemList.find(node_data->Time_Key);

	// 현재 노드의 데이터..
	float time_key = node_data->Time_Key;
	int list_index = node_data->List_Index;

	// 하이어라키 제거..
	GameObject* object = node_data->Particle_Value->gameobject;
	object->DisconnectParent();

	// 해당 리스트 내부에 존재하는 파티클 제거..
	delete node_data;

	list_itor->second.erase(std::next(list_itor->second.begin(), list_index));

	// 만약 해당 리스트가 비어있다면 제거..
	if (list_itor->second.empty())
	{
		m_ParticleSystemList.erase(time_key);
	}
	else
	{
		// 해당 리스트 내부에 존재하는 노드 데이터 재설정..
		for (int i = list_index; i < list_itor->second.size(); i++)
		{
			list_itor->second[i].Node_Data->List_Index--;
		}
	}

	// 해당 노드 데이터 제거..
	m_ParticleNodeDataList.erase(particle_key);

	// TotalTime 재설정..
	SetTotalTime();
}

void ParticleController::SetScale(float& scale)
{
	for (auto& node : m_ParticleNodeDataList)
	{
		node.second->Particle_Value->SetScale(scale);
	}
}

void ParticleController::SetScale(float&& scale)
{
	for (auto& node : m_ParticleNodeDataList)
	{
		node.second->Particle_Value->SetScale(scale);
	}
}

void ParticleController::SetScale(float& x, float& y)
{
	for (auto& node : m_ParticleNodeDataList)
	{
		node.second->Particle_Value->SetScale(x, y);
	}
}

void ParticleController::SetScale(float&& x, float&& y)
{
	for (auto& node : m_ParticleNodeDataList)
	{
		node.second->Particle_Value->SetScale(x, y);
	}
}

void ParticleController::SetScale(DirectX::SimpleMath::Vector2 scale)
{
	for (auto& node : m_ParticleNodeDataList)
	{
		node.second->Particle_Value->SetScale(scale);
	}
}

void ParticleController::Play()
{
	// 일시정지 중이라면 다시 재개..
	if (m_Pause)
	{
		Resume();
		return;
	}

	if (m_ControllerState != PARTICLE_STATE::END_STATE) return;

	m_ControllerState = PARTICLE_STATE::START_STATE;

	// 첫번째 요소부터 시작..
	m_NowParticleList = m_ParticleSystemList.begin();

	// 현재 삽입된 파티클이 없다면..
	if (m_NowParticleList == m_ParticleSystemList.end()) return;

	// 플레이 시간 초기화..
	m_PlayTime = 0.0f;

	// 현재 파티클 리스트에 대한 데이터 설정..
	SetNowParticleList();
}

void ParticleController::Pause()
{
	if (m_NowParticleList == m_ParticleSystemList.end()) return;

	ParticleList play_particle = m_NowParticleList;

	// 현재 재생중인 모든 파티클 일시 정지..
	while (play_particle != m_ParticleSystemList.end())
	{
		for (auto& particle : play_particle->second)
		{
			particle.Particle_Value->Pause();
		}

		play_particle--;
	}

	m_Pause = true;
}

void ParticleController::Stop()
{
	if (m_NowParticleList == m_ParticleSystemList.end()) return;

	ParticleList play_particle = m_NowParticleList;

	// 현재 재생중인 모든 파티클 정지..
	while (play_particle != m_ParticleSystemList.end())
	{
		for (auto& particle : play_particle->second)
		{
			particle.Particle_Value->Stop();
		}

		play_particle--;
	}

	m_ControllerState = PARTICLE_STATE::END_STATE;
	m_Pause = false;
}

void ParticleController::Resume()
{
	if (m_NowParticleList == m_ParticleSystemList.end()) return;

	ParticleList play_particle = m_NowParticleList;

	// 현재 재생중인 모든 파티클 정지..
	while (play_particle != m_ParticleSystemList.end())
	{
		for (auto& particle : play_particle->second)
		{
			particle.Particle_Value->Play();
		}

		play_particle--;
	}

	m_Pause = false;
}

ParticleSystem* ParticleController::GetParticle(std::string particle_key)
{
	// 해당 노드 검색..
	ParticleNode&& node_itor = m_ParticleNodeDataList.find(particle_key);

	if (node_itor != m_ParticleNodeDataList.end())
	{
		return node_itor->second->Particle_Value;
	}

	return nullptr;
}

void ParticleController::UpdateController()
{
	if (m_StartTime <= m_PlayTime)
	{
		for (int i = 0; i < m_NowParticleListSize; i++)
		{
			m_NowParticleList->second[i].Particle_Value->Play();
		}

		// 다음 파티클 리스트로 설정..
		m_NowParticleList++;

		// 만약 모든 파티클 리스트가 재생된 상태라면..
		if (m_NowParticleList == m_ParticleSystemList.end())
		{
			m_NowParticleList = --m_ParticleSystemList.end();
			m_ControllerState = PARTICLE_STATE::PLAY_STAY_STATE;
		}
		else
		{
			// 현재 파티클 리스트에 대한 데이터 설정..
			SetNowParticleList();
		}

		PROFILE_LOG(PROFILE_OUTPUT::VS_CODE, "NextTime : %.3f\nParticleCount : %d", m_StartTime, m_NowParticleListSize);
	}
}

void ParticleController::SetTotalTime()
{
	if (m_ParticleSystemList.empty())
	{
		m_TotalPlayTime = 0.0f;
	}
	else
	{
		float particleTime = 0.0f;
		float startTime = 0.0f;
		float maxTime = 0.0f;

		for (auto& particle_list : m_ParticleSystemList)
		{
			startTime = particle_list.first;

			for (auto& particle : particle_list.second)
			{
				particleTime = startTime + particle.Particle_Value->GetTotalPlayTime();

				if (particleTime > maxTime)
				{
					maxTime = particleTime;
				}
			}
		}

		// 최종적으로 끝나는 시간 삽입..
		m_TotalPlayTime = maxTime;
	}
}

void ParticleController::SetNowParticleList()
{
	m_NowParticleListSize = m_NowParticleList->second.size();
	m_StartTime = m_NowParticleList->first;
}

PARTICLE_STATE ParticleController::GetState()
{
	return m_ControllerState;
}

float ParticleController::GetTotalPlayTime()
{
	return m_TotalPlayTime;
}
