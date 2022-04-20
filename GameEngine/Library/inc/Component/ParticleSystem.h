#pragma once
#include "EaterEngineDLL.h"
#include "Component.h"
#include <vector>

struct PARTICLE_SHARE_DESC;
struct PARTICLE_DESC;

class MeshFilter;
class Particle;
class ParticleData;
class RandomInt;
class RandomFloat;
class RandomVector3;
class RandomVector4;

template<typename T>
struct RandomRange
{
	void SetRange(T range)		{ m_Min = range; m_Max = range; }
	void SetRange(T min, T max) { m_Min = min; m_Max = max; }

	T m_Min;
	T m_Max;
};

class ParticleSystem : public Component
{
public:
	EATER_ENGINEDLL ParticleSystem();
	~ParticleSystem();

public:
	void Awake() override;
	void SetUp() override;
	void Start() override;
	void Update() override;

public:
	EATER_ENGINEDLL void DataUpdate();

public:
	EATER_ENGINEDLL void SetMeshName(std::string meshName);								// ��ƼŬ ����� �Ž� Ÿ��
	EATER_ENGINEDLL void SetRenderType(PARTICLE_RENDER_OPTION renderType);				// ��ƼŬ ����� ���� Ÿ��
	EATER_ENGINEDLL void SetMaxParticles(int maxCount);									// �ִ� ��ƼŬ ��� ����
	EATER_ENGINEDLL void SetDelayTime(float delay);										// ��ƼŬ ���� �� ���� �ð�
	EATER_ENGINEDLL void SetRateOverTime(float count);									// 1�ʿ� ����� ��ƼŬ ����
	EATER_ENGINEDLL void SetShapeRadius(float radius);									// ��ƼŬ ���� ���� ����
	EATER_ENGINEDLL void SetShapeRadius(float x, float y, float z);						// ��ƼŬ ���� ���� ����

	EATER_ENGINEDLL void SetStartForce(Vector3 force);									// ��ƼŬ ���� �ӷ�
	EATER_ENGINEDLL void SetStartForce(Vector3 minForce, Vector3 maxForce);				// ��ƼŬ ���� �ӷ�
	EATER_ENGINEDLL void SetStartColor(Vector4 color);									// ��ƼŬ ���� ����
	EATER_ENGINEDLL void SetStartColor(Vector4 minColor, Vector4 maxColor);				// ��ƼŬ ���� ����
	EATER_ENGINEDLL void SetStartLifeTime(float time);									// ��ƼŬ ���� �����ð�
	EATER_ENGINEDLL void SetStartLifeTime(float minTime, float maxTime);				// ��ƼŬ ���� �����ð�
	EATER_ENGINEDLL void SetStartScale(float scale);									// ��ƼŬ ���� ũ��
	EATER_ENGINEDLL void SetStartScale(float minScale, float maxScale);					// ��ƼŬ ���� ũ��
	EATER_ENGINEDLL void SetStartRotation(float rot);									// ��ƼŬ ���� ȸ��
	EATER_ENGINEDLL void SetStartRotation(float minRot, float maxRot);					// ��ƼŬ ���� ȸ��

	EATER_ENGINEDLL void SetLifeTimeForce(Vector3 force);								// ��ƼŬ ���� �� ���� �ӷ� ����
	EATER_ENGINEDLL void SetLifeTimeForce(Vector3 minForce, Vector3 maxForce);			// ��ƼŬ ���� �� ���� �ӷ� ����
	EATER_ENGINEDLL void SetLifeTimeColor(Vector4 minColor, Vector4 maxColor, PARTICLE_LIFETIME_OPTION option);			// ��ƼŬ ���� �� ���� ����
	EATER_ENGINEDLL void SetLifeTimeScale(float minScale, float maxScale, PARTICLE_LIFETIME_OPTION option);				// ��ƼŬ ���� �� ũ�� ����
	EATER_ENGINEDLL void SetLifeTimeRotation(float rot);								// ��ƼŬ ���� �� ���� ȸ�� ����
	EATER_ENGINEDLL void SetLifeTimeRotation(float minRot, float maxRot);				// ��ƼŬ ���� �� ���� ȸ�� ����

	EATER_ENGINEDLL void SetTextureTiling(int count_x, int count_y);					// ��ƼŬ �ؽ��� ���� ����
	EATER_ENGINEDLL void SetPlayTime(float playTime, bool loop = false);				// ��ƼŬ �ý��� �÷��� �ð� �� �ݺ� ����
	
	EATER_ENGINEDLL void SetDiffuseName(std::string diffuseName);

	EATER_ENGINEDLL void Play(bool loop = false);										// ��ƼŬ �ý��� �÷��� �ð� �� �ݺ� ����
	EATER_ENGINEDLL void Stop();														// ��ƼŬ �ý��� ����

	EATER_ENGINEDLL void SetNextParticle(ParticleSystem* particle);

public:
	EATER_ENGINEDLL std::string GetMeshName();
	EATER_ENGINEDLL PARTICLE_RENDER_OPTION GetRenderType();
	EATER_ENGINEDLL int GetMaxParticles();
	EATER_ENGINEDLL float GetDelayTime();
	EATER_ENGINEDLL float GetRateOverTime();
	EATER_ENGINEDLL Vector3 GetShapeRadius();
	EATER_ENGINEDLL RandomRange<Vector3> GetStartForce();
	EATER_ENGINEDLL RandomRange<Vector4> GetStartColor();
	EATER_ENGINEDLL RandomRange<float> GetStartLifeTime();
	EATER_ENGINEDLL RandomRange<float> GetStartScale();
	EATER_ENGINEDLL RandomRange<float> GetStartRotation();
	EATER_ENGINEDLL RandomRange<Vector3> GetLifeTimeForce();
	EATER_ENGINEDLL RandomRange<Vector4> GetLifeTimeColor();
	EATER_ENGINEDLL RandomRange<float> GetLifeTimeScale();
	EATER_ENGINEDLL RandomRange<float> GetLifeTimeRotation();
	EATER_ENGINEDLL PARTICLE_LIFETIME_OPTION GetLifeTimeColorOption();
	EATER_ENGINEDLL PARTICLE_LIFETIME_OPTION GetLifeTimeScaleOption();

public:
	void Release();

private:
	void StartPlay();
	void AddParticle();

	void CreateParticle();
	void StartNextParticle();

private:
	MeshFilter* m_MeshFilter;

private:
	ParticleData* m_ParticleData;
	PARTICLE_SHARE_DESC* m_SystemDesc;
	PARTICLE_DESC* m_ParticleDesc;

	std::vector<Particle*> m_Particles;
	std::vector<ParticleSystem*> m_NextParticles;

	std::string m_ParticleMeshName;
	std::string m_DiffuseName;

	bool m_Looping;				// �ݺ� ��� ����
	bool m_Playing;				// ��� ����
	int m_MaxParticle;			// �ִ� ��ƼŬ ����
	int m_PlayCount;			// �̹��� ������ ��ƼŬ ����

	float m_PlayTime;			// ���� ���� �ð�
	float m_NowPlayTime;		// ���� ���� �ð�
	float m_DelayTime;			// ���� ���� �ð�
	float m_NowDelayTime;		// ���� ���� �ð�
	float m_TickTime;			// ���� ���� �ð�
	float m_RateOverTime;		// �ʴ� ����� ��ƼŬ ����
	float m_RateOverTimeCount;	// �ʴ� ����� ��ƼŬ ����

	RandomRange<Vector3>	m_StartForce;
	RandomRange<Vector4>	m_StartColor;
	RandomRange<float>		m_StartLifeTime;
	RandomRange<float>		m_StartScale;
	RandomRange<float>		m_StartRotation;

	RandomRange<Vector3>	m_LifeTimeForce;
	RandomRange<Vector4>	m_LifeTimeColor;
	RandomRange<float>		m_LifeTimeScale;
	RandomRange<float>		m_LifeTimeRotation;

	RandomFloat*	m_RandomLifeTime;
	RandomVector4*	m_RandomStartColor;
	RandomVector3*	m_RandomStartPosition;
	RandomVector3*	m_RandomStartForce;
	RandomVector3*	m_RandomLifeTimeForce;

	RandomFloat*	m_RandomStartSize;
	RandomFloat*	m_RandomStartRotation;
	RandomFloat*	m_RandomLifeTimeRotation;
};