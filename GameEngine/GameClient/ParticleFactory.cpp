#include "ParticleFactory.h"
#include "EaterEngineAPI.h"
#include "GameObject.h"

ParticleFactory* ParticleFactory::g_Instance = nullptr;

ParticleFactory::ParticleFactory()
{

}

ParticleFactory::~ParticleFactory()
{

}

ParticleFactory* ParticleFactory::Get()
{
	if (g_Instance == nullptr)
	{
		g_Instance = new ParticleFactory();
		g_Instance->Initialize();
	}

	return g_Instance;
}

void ParticleFactory::Release()
{
	if (g_Instance != nullptr)
	{
		g_Instance->Reset();
		delete g_Instance;
	}
}

ParticleController* ParticleFactory::CreateParticleController(PARTICLE_TYPE particle_type)
{
	// �ش� ��ƼŬ Ÿ�Կ� ���� Key�� �޾ƿ���..
	auto&& itor = m_ParticleKeyList.find(particle_type);

	if (itor == m_ParticleKeyList.end()) return nullptr;

	// �ش� ��ƼŬ ��Ʈ�ѷ��� ���ϴ� ��ƼŬ Ű ����Ʈ..
	KeyList& particle_key = itor->second;

	// ���ο� ��Ʈ�ѷ� ����..
	GameObject* particle_controller = Instance_ParticleController();
	ParticleController* controller = particle_controller->GetComponent<ParticleController>();

	// ���� ������ ���� �ش� ��Ʈ�ѷ� ������Ʈ ���..
	m_ControllerList.push_back(particle_controller);

	GameObject* particle = nullptr;

	// ����ص� ��ƼŬ�� Ű���� ���鼭 ��ƼŬ ���� �� ��Ʈ�ѷ��� ���..
	for (auto& particle_data : particle_key)
	{
		particle = Instance_Particle("Particle", particle_data.key);
		controller->PushParticle(particle_data.key, particle, particle_data.start_time);
	}

	return controller;
}

const ParticleFactory::KeyList& ParticleFactory::GetParticleKeyList(PARTICLE_TYPE particle_type)
{
	auto&& itor = m_ParticleKeyList.find(particle_type);

	if (itor != m_ParticleKeyList.end())
	{
		return itor->second;
	}

	return KeyList();
}

void ParticleFactory::Initialize()
{
	/// BossFog
	KeyList BossFog_Key;
	BossFog_Key.push_back({ "BossFog_fog",		0.0f });

	/// BossMelee
	KeyList BossMelee_Key;
	BossMelee_Key.push_back({ "BossMelee_dot",		0.0f });
	BossMelee_Key.push_back({ "BossMelee_lighting", 0.0f });
	BossMelee_Key.push_back({ "BossMelee_spark",	0.0f });

	/// BossPage
	KeyList BossPage_Key;
	BossPage_Key.push_back({ "BossPage_energy",		0.0f });
	BossPage_Key.push_back({ "BossPage_lightning",	0.0f });

	/// BossProjectile
	KeyList BossProjectile_Key;
	BossProjectile_Key.push_back({ "BossProjectile_aura",	0.0f });
	BossProjectile_Key.push_back({ "BossProjectile_circle", 0.0f });
	BossProjectile_Key.push_back({ "BossProjectile_dot",	0.0f });

	/// BossProjectileExplode
	KeyList BossProjectileExplode_Key;
	BossProjectileExplode_Key.push_back({ "BossProjectileExplode_dot",		0.0f });
	BossProjectileExplode_Key.push_back({ "BossProjectileExplode_fire",		0.0f });
	BossProjectileExplode_Key.push_back({ "BossProjectileExplode_flare",	0.0f });
	
	/// BossProjectileFloor
	KeyList BossProjectileFloor_Key;
	BossProjectileFloor_Key.push_back({ "BossProjectileFloor_circle",	0.0f });
	BossProjectileFloor_Key.push_back({ "BossProjectileFloor_fire",		0.0f });
	BossProjectileFloor_Key.push_back({ "BossProjectileFloor_flare",	0.0f });

	/// BossPush
	KeyList BossPush_Key;
	BossPush_Key.push_back({ "BossPush_circle",		0.0f });
	BossPush_Key.push_back({ "BossPush_magical",	0.0f });

	// �ι� ����
	/// CounterAttack
	KeyList CounterAttack_Key;
	CounterAttack_Key.push_back({ "CounterAttack_circle",	0.0f });
	CounterAttack_Key.push_back({ "CounterAttack_dot",		0.0f });
	CounterAttack_Key.push_back({ "CounterAttack_flare",	0.0f });

	/// HitSmoke
	KeyList HitSmoke_Key;
	HitSmoke_Key.push_back({ "HitSmoke_smoke",	0.0f });
	HitSmoke_Key.push_back({ "HitSmoke_spark",	0.0f });

	/// ManaSmoke
	KeyList ManaSmoke_Key;
	ManaSmoke_Key.push_back({ "Mana_smoke",	0.0f });
	ManaSmoke_Key.push_back({ "Mana_spark",	0.0f });

	/// ManaTreeSmoke
	KeyList ManaTreeSmoke_Key;
	ManaTreeSmoke_Key.push_back({ "ManTree_smokeUP",	0.0f });

	// �ð� ����
	/// MonsterExplode
	KeyList MonsterExplode_Key;
	MonsterExplode_Key.push_back({ "MonsterExplode_smokeFloor",		0.0f });
	MonsterExplode_Key.push_back({ "MonsterExplode_dot",			0.25f });
	MonsterExplode_Key.push_back({ "MonsterExplode_smokeExplode",	0.25f });

	// �ι� ����
	/// PlayerAttack
	KeyList PlayerAttack_Key;
	PlayerAttack_Key.push_back({ "PlayerAttack_circle",	0.0f });
	PlayerAttack_Key.push_back({ "PlayerAttack_dot",	0.0f });

	// ���� �̻���
	/// PlayerHeal
	KeyList PlayerHeal_Key;
	PlayerHeal_Key.push_back({ "PlayerHeal_circle",	0.0f });
	PlayerHeal_Key.push_back({ "PlayerHeal_dot",	0.0f });
	PlayerHeal_Key.push_back({ "PlayerHeal_health",	0.0f });
	PlayerHeal_Key.push_back({ "PlayerHeal_spark",	0.0f });
	 
	m_ParticleKeyList.insert({ PARTICLE_TYPE::BossFog,					BossFog_Key });
	m_ParticleKeyList.insert({ PARTICLE_TYPE::BossMelee,				BossMelee_Key });
	m_ParticleKeyList.insert({ PARTICLE_TYPE::BossProjectile,			BossProjectile_Key });
	m_ParticleKeyList.insert({ PARTICLE_TYPE::BossProjectileExplode,	BossProjectileExplode_Key });
	m_ParticleKeyList.insert({ PARTICLE_TYPE::BossProjectileFloor,		BossProjectileFloor_Key });
	m_ParticleKeyList.insert({ PARTICLE_TYPE::BossPush,					BossPush_Key });
	m_ParticleKeyList.insert({ PARTICLE_TYPE::CounterAttack,			CounterAttack_Key });
	m_ParticleKeyList.insert({ PARTICLE_TYPE::HitSmoke,					HitSmoke_Key });
	m_ParticleKeyList.insert({ PARTICLE_TYPE::ManaSmoke,				ManaSmoke_Key });
	m_ParticleKeyList.insert({ PARTICLE_TYPE::ManaTreeSmoke,			ManaTreeSmoke_Key });
	m_ParticleKeyList.insert({ PARTICLE_TYPE::MonsterExplode,			MonsterExplode_Key });
	m_ParticleKeyList.insert({ PARTICLE_TYPE::PlayerAttack,				PlayerAttack_Key });
	m_ParticleKeyList.insert({ PARTICLE_TYPE::PlayerHeal,				PlayerHeal_Key });
}

void ParticleFactory::Reset()
{
	for (GameObject* obj : m_ControllerList)
	{
		Destroy(obj);
	}
}
