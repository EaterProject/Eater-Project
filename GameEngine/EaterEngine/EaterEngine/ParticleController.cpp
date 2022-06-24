#include "ParticleController.h"
#include "ParticleSystem.h"

#include "TimeManager.h"
#include "EaterEngineAPI.h"
EATER_ENGINEDLL ParticleController::ParticleController()
	:m_ControllerState(PARTICLE_STATE::END_STATE), m_TotalPlayTime(0.0f), m_PlayTime(0.0f), m_StartTime(0.0f), m_NowParticleListSize(0)
{

}

ParticleController::~ParticleController()
{

}

void ParticleController::Update()
{
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
			m_ControllerState = PARTICLE_STATE::END_STATE;
		}
	}
		break;
	default:
		break;
	}
}

void ParticleController::PushParticle(std::string particle_name, ParticleSystem* particle, float start_time)
{
	if (particle == nullptr) return;

	ParticleNode node_itor = m_ParticleNodeDataList.find(particle_name);

	// �ش� Ű�� ���� ��ƼŬ�� ���ԵǾ� �ִٸ�..
	if (node_itor != m_ParticleNodeDataList.end()) return;

	// ���ο� ��ƼŬ ��� ������ ����..
	NodeData* node_data = new NodeData();

	// ��ƼŬ ��� ����..
	m_ParticleNodeDataList.insert({ particle_name, node_data });

	ParticleList list_itor = m_ParticleSystemList.find(start_time);

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

void ParticleController::PopParticle(std::string particle_name)
{
	// �ش� ��� �˻�..
	ParticleNode&& node_itor = m_ParticleNodeDataList.find(particle_name);

	// �ش� Ű�� ���� ��ƼŬ�� ���Ե����� �ʴٸ�..
	if (node_itor == m_ParticleNodeDataList.end()) return;

	NodeData* node_data = node_itor->second;

	// �ش� ����Ʈ �˻�..
	ParticleList list_itor = m_ParticleSystemList.find(node_data->Time_Key);

	// ���� ����� ������..
	float time_key = node_data->Time_Key;
	int list_index = node_data->List_Index;

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
	m_ParticleNodeDataList.erase(particle_name);

	// TotalTime �缳��..
	SetTotalTime();
}

void ParticleController::Play()
{
	m_ControllerState = PARTICLE_STATE::START_STATE;

	// ù��° ��Һ��� ����..
	m_NowParticleList = m_ParticleSystemList.begin();

	// ���� ���Ե� ��ƼŬ�� ���ٸ�..
	if (m_NowParticleList == m_ParticleSystemList.end()) return;

	// ���� ��ƼŬ ����Ʈ�� ���� ������ ����..
	SetNowParticleList();
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
			m_ControllerState = PARTICLE_STATE::PLAY_STAY_STATE;
		}
		else
		{
			// ���� ��ƼŬ ����Ʈ�� ���� ������ ����..
			SetNowParticleList();
		}
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
				particleTime = startTime + particle.Particle_Value->GetPlayTime() + particle.Particle_Value->GetDelayTime();

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
