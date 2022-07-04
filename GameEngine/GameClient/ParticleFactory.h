#pragma once
#include <vector>
#include <unordered_map>
#include "ParticleSystem.h"
#include "ParticleController.h"

enum class PARTICLE_TYPE
{
	BossFog,
	BossMelee,
	BossPage,
	BossProjectile,
	BossProjectileExplode,
	BossProjectileFloor,
	BossPush,
	CounterAttack,
	HitSmoke,
	ManaSmoke,
	ManaTreeSmoke,
	MonsterExplode,
	PlayerAttack,
	PlayerHeal
};

class ParticleFactory
{
private:
	ParticleFactory();
	~ParticleFactory();

public:
	struct ParticleProperty
	{
		ParticleProperty(const char* _key, float _time) : key(_key), start_time(_time) {}

		const char* key;
		float start_time;
	};

	using KeyList = std::vector<ParticleProperty>;

public:
	static ParticleFactory* Get();
	static void Release();

public:
	ParticleController* CreateParticleController(PARTICLE_TYPE particle_type);

public:
	const KeyList& GetParticleKeyList(PARTICLE_TYPE particle_type);

private:
	void Initialize();
	void Reset();

private:
	std::unordered_map<PARTICLE_TYPE, KeyList> m_ParticleKeyList;
	std::vector<GameObject*> m_ControllerList;

private:
	static ParticleFactory* g_Instance;
};

