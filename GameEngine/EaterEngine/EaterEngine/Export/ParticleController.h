#pragma once
#include "EaterEngineDLL.h"
#include "Component.h"
#include <map>
#include <vector>

class ParticleSystem;
class Transform;
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
	void Awake() override;

public:
	EATER_ENGINEDLL void PushParticle(std::string particle_key, ParticleSystem* particle, float start_time);
	EATER_ENGINEDLL void PushParticle(std::string particle_key, GameObject* particle, float start_time);
	EATER_ENGINEDLL void PopParticle(std::string particle_key);

	EATER_ENGINEDLL void SetScale(float& scale);
	EATER_ENGINEDLL void SetScale(float&& scale);
	EATER_ENGINEDLL void SetScale(float& x, float& y);
	EATER_ENGINEDLL void SetScale(float&& x, float&& y);
	EATER_ENGINEDLL void SetScale(DirectX::SimpleMath::Vector2 scale);
	
	EATER_ENGINEDLL void Play();
	EATER_ENGINEDLL void Pause();
	EATER_ENGINEDLL void Stop();

	//현재 들어있는 파티클들을 움직여줌
	EATER_ENGINEDLL void SetPosition(float x, float y, float z);
	EATER_ENGINEDLL void SetPosition(Vector3 Pos);
public:
	EATER_ENGINEDLL ParticleSystem* GetParticle(std::string particle_key);
	EATER_ENGINEDLL PARTICLE_STATE GetState();
	EATER_ENGINEDLL float GetTotalPlayTime();

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
	Transform* mTransform;

	std::map<float, std::vector<ParticleData>> m_ParticleSystemList;
	std::unordered_map<std::string, NodeData*> m_ParticleNodeDataList;
};

