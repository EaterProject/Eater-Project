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

		// ��� ��ƼŬ���� ������ ��� ���� ����..
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

	// �ش� Ű�� ���� ��ƼŬ�� ���ԵǾ� �ִٸ�..
	if (node_itor != m_ParticleNodeDataList.end()) return;

	// ���̾��Ű ����..
	GameObject* object = particle->gameobject;
	object->ChoiceParent(gameobject);

	// ���ο� ��ƼŬ ��� ������ ����..
	NodeData* node_data = new NodeData();

	// ��ƼŬ ��� ����..
	m_ParticleNodeDataList.insert({ particle_key, node_data });

	ParticleList&& list_itor = m_ParticleSystemList.find(start_time);

	// ��ƼŬ ����Ʈ���� ��ġ�ϴ� �ε���..
	int list_index = 0;

	ParticleData particle_data;
	particle_data.Node_Data = node_data;
	particle_data.Particle_Value = particle;

	// �ش� �ð��� ���� ��ƼŬ ����Ʈ�� ����..
	if (list_itor == m_ParticleSystemList.end())
	{
		m_ParticleSystemList.insert({ start_time, std::vector<ParticleData>(1, particle_data) });
	}
	else
	{
		list_index = list_itor->second.size();
		list_itor->second.push_back(particle_data);
	}

	// ���ο� ��ƼŬ ��� ������ ����..
	node_data->Time_Key = start_time;
	node_data->Particle_Value = particle;
	node_data->List_Index = list_index;

	// TotalTime �缳��..
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
	// �ش� ��� �˻�..
	ParticleNode&& node_itor = m_ParticleNodeDataList.find(particle_key);

	// �ش� Ű�� ���� ��ƼŬ�� ���Ե����� �ʴٸ�..
	if (node_itor == m_ParticleNodeDataList.end()) return;

	NodeData* node_data = node_itor->second;

	// �ش� ����Ʈ �˻�..
	ParticleList&& list_itor = m_ParticleSystemList.find(node_data->Time_Key);

	// ���� ����� ������..
	float time_key = node_data->Time_Key;
	int list_index = node_data->List_Index;

	// ���̾��Ű ����..
	GameObject* object = node_data->Particle_Value->gameobject;
	object->DisconnectParent();

	// �ش� ����Ʈ ���ο� �����ϴ� ��ƼŬ ����..
	delete node_data;

	list_itor->second.erase(std::next(list_itor->second.begin(), list_index));

	// ���� �ش� ����Ʈ�� ����ִٸ� ����..
	if (list_itor->second.empty())
	{
		m_ParticleSystemList.erase(time_key);
	}
	else
	{
		// �ش� ����Ʈ ���ο� �����ϴ� ��� ������ �缳��..
		for (int i = list_index; i < list_itor->second.size(); i++)
		{
			list_itor->second[i].Node_Data->List_Index--;
		}
	}

	// �ش� ��� ������ ����..
	m_ParticleNodeDataList.erase(particle_key);

	// TotalTime �缳��..
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
	// �Ͻ����� ���̶�� �ٽ� �簳..
	if (m_Pause)
	{
		Resume();
		return;
	}

	if (m_ControllerState != PARTICLE_STATE::END_STATE) return;

	m_ControllerState = PARTICLE_STATE::START_STATE;

	// ù��° ��Һ��� ����..
	m_NowParticleList = m_ParticleSystemList.begin();

	// ���� ���Ե� ��ƼŬ�� ���ٸ�..
	if (m_NowParticleList == m_ParticleSystemList.end()) return;

	// �÷��� �ð� �ʱ�ȭ..
	m_PlayTime = 0.0f;

	// ���� ��ƼŬ ����Ʈ�� ���� ������ ����..
	SetNowParticleList();
}

void ParticleController::Pause()
{
	if (m_NowParticleList == m_ParticleSystemList.end()) return;

	ParticleList play_particle = m_NowParticleList;

	// ���� ������� ��� ��ƼŬ �Ͻ� ����..
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

	// ���� ������� ��� ��ƼŬ ����..
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

	// ���� ������� ��� ��ƼŬ ����..
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
	// �ش� ��� �˻�..
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

		// ���� ��ƼŬ ����Ʈ�� ����..
		m_NowParticleList++;

		// ���� ��� ��ƼŬ ����Ʈ�� ����� ���¶��..
		if (m_NowParticleList == m_ParticleSystemList.end())
		{
			m_NowParticleList = --m_ParticleSystemList.end();
			m_ControllerState = PARTICLE_STATE::PLAY_STAY_STATE;
		}
		else
		{
			// ���� ��ƼŬ ����Ʈ�� ���� ������ ����..
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

		// ���������� ������ �ð� ����..
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
