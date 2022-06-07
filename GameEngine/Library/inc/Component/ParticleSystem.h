#pragma once
#include "EaterEngineDLL.h"
#include "Component.h"
#include <vector>

struct PARTICLE_SHARE_DESC;
struct PARTICLE_DESC;

class MeshFilter;
class Particle;
class ParticleData;

namespace Eater
{
	class RandomInt;
	class RandomFloat;
	class RandomVector3;
	class RandomVector4;
}

template<typename T>
struct Range
{
	Range() {}

	void SetRange(T range)		{ m_Min = range; m_Max = range; }
	void SetRange(T min, T max) { m_Min = min; m_Max = max; }
	void SetSize(T size)		{ m_Width = size; m_Height = size; }
	void SetSize(T x, T y)		{ m_Width = x; m_Height = y; }

	union
	{
		T m_Min;
		T m_Width;
	};
	union
	{
		T m_Max;
		T m_Height;
	};
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
	EATER_ENGINEDLL void SetMeshName(std::string meshName);								// ��ƼŬ ����� �Ž� Ÿ��
	EATER_ENGINEDLL void SetRenderType(PARTICLE_RENDER_OPTION renderType);				// ��ƼŬ ����� ���� Ÿ��
	EATER_ENGINEDLL void SetMaxParticles(int maxCount);									// �ִ� ��ƼŬ ��� ����
	EATER_ENGINEDLL void SetDelayTime(float delay);										// ��ƼŬ ���� �� ���� �ð�
	EATER_ENGINEDLL void SetRateOverTime(float count);									// 1�ʿ� ����� ��ƼŬ ����
	EATER_ENGINEDLL void SetStrength(float strength);									// ��ƼŬ ��� ����
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
	EATER_ENGINEDLL float GetStrength();
	EATER_ENGINEDLL float GetPlayTime();
	EATER_ENGINEDLL bool GetLoop();
	EATER_ENGINEDLL Vector3 GetShapeRadius();
	EATER_ENGINEDLL Range<Vector3> GetStartForce();
	EATER_ENGINEDLL Range<Vector4> GetStartColor();
	EATER_ENGINEDLL Range<float> GetStartLifeTime();
	EATER_ENGINEDLL Range<float> GetStartScale();
	EATER_ENGINEDLL Range<float> GetStartRotation();
	EATER_ENGINEDLL Range<Vector3> GetLifeTimeForce();
	EATER_ENGINEDLL Range<Vector4> GetLifeTimeColor();
	EATER_ENGINEDLL Range<float> GetLifeTimeScale();
	EATER_ENGINEDLL Range<float> GetLifeTimeRotation();
	EATER_ENGINEDLL PARTICLE_LIFETIME_OPTION GetLifeTimeColorOption();
	EATER_ENGINEDLL PARTICLE_LIFETIME_OPTION GetLifeTimeScaleOption();
	EATER_ENGINEDLL Range<int> GetTextureTiling();
	EATER_ENGINEDLL std::string GetTextureName();

public:
	void Release();

private:
	void DataUpdate();

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

	float m_Strength;			// ��ƼŬ ��� ����

	float m_PlayTime;			// ���� ���� �ð�
	float m_NowPlayTime;		// ���� ���� �ð�
	float m_DelayTime;			// ���� ���� �ð�
	float m_NowDelayTime;		// ���� ���� �ð�
	float m_TickTime;			// ���� ���� �ð�
	float m_RateOverTime;		// �ʴ� ����� ��ƼŬ ����
	float m_RateOverTimeCount;	// �ʴ� ����� ��ƼŬ ����

	Range<Vector3>		m_StartForce;
	Range<Vector4>		m_StartColor;
	Range<float>		m_StartLifeTime;
	Range<float>		m_StartScale;
	Range<float>		m_StartRotation;

	Range<Vector3>		m_LifeTimeForce;
	Range<Vector4>		m_LifeTimeColor;
	Range<float>		m_LifeTimeScale;
	Range<float>		m_LifeTimeRotation;
	Range<int>			m_Tiling;

	Eater::RandomFloat*	m_RandomLifeTime;
	Eater::RandomVector4*	m_RandomStartColor;
	Eater::RandomVector3*	m_RandomStartPosition;
	Eater::RandomVector3*	m_RandomStartForce;
	Eater::RandomVector3*	m_RandomLifeTimeForce;

	Eater::RandomFloat*	m_RandomStartSize;
	Eater::RandomFloat*	m_RandomStartRotation;
	Eater::RandomFloat*	m_RandomLifeTimeRotation;
};