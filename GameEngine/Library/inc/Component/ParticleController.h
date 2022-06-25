#pragma once
#include "EaterEngineDLL.h"
#include "Component.h"
#include <map>

class ParticleSystem;
class ParticleController : public Component
{
private:
	struct NodeData
	{
		float Time_Key;
		int List_Index;
		ParticleSystem* Particle_Value;
	};

	struct ParticleData
	{
		ParticleSystem* Particle_Value;
		NodeData* Node_Data;
	};

	using ParticleList = std::map<float, std::vector<ParticleData>>::iterator;
	using ParticleNode = std::unordered_map<std::string, NodeData*>::iterator;

public:
	EATER_ENGINEDLL ParticleController();
	~ParticleController();

public:
	void Update() override;

public:
	EATER_ENGINEDLL void PushParticle(std::string particle_key, ParticleSystem* particle, float start_time);
	EATER_ENGINEDLL void PopParticle(std::string particle_key);

	EATER_ENGINEDLL void Play();
	EATER_ENGINEDLL void Pause();
	EATER_ENGINEDLL void Stop();

public:
	EATER_ENGINEDLL ParticleSystem* GetParticle(std::string particle_key);
	EATER_ENGINEDLL PARTICLE_STATE GetState();

private:
	void UpdateController();

	void SetTotalTime();
	void SetNowParticleList();

	void Resume();

private:
	PARTICLE_STATE m_ControllerState;

	bool m_Pause;

	float m_TotalPlayTime;
	float m_PlayTime;
	float m_StartTime;

	ParticleList m_NowParticleList;

	int m_NowParticleListSize;

	std::map<float, std::vector<ParticleData>> m_ParticleSystemList;
	std::unordered_map<std::string, NodeData*> m_ParticleNodeDataList;
};

