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
	EATER_ENGINEDLL void SetMeshName(std::string meshName);								// 파티클 출력할 매쉬 타입
	EATER_ENGINEDLL void SetRenderType(PARTICLE_RENDER_OPTION renderType);				// 파티클 출력할 랜더 타입
	EATER_ENGINEDLL void SetMaxParticles(int maxCount);									// 최대 파티클 출력 개수
	EATER_ENGINEDLL void SetDelayTime(float delay);										// 파티클 실행 전 지연 시간
	EATER_ENGINEDLL void SetRateOverTime(float count);									// 1초에 출력할 파티클 개수
	EATER_ENGINEDLL void SetStrength(float strength);									// 파티클 밝기 강도
	EATER_ENGINEDLL void SetShapeRadius(float radius);									// 파티클 랜덤 생성 범위
	EATER_ENGINEDLL void SetShapeRadius(float x, float y, float z);						// 파티클 랜덤 생성 범위

	EATER_ENGINEDLL void SetStartForce(Vector3 force);									// 파티클 고정 속력
	EATER_ENGINEDLL void SetStartForce(Vector3 minForce, Vector3 maxForce);				// 파티클 랜덤 속력
	EATER_ENGINEDLL void SetStartColor(Vector4 color);									// 파티클 고정 색상
	EATER_ENGINEDLL void SetStartColor(Vector4 minColor, Vector4 maxColor);				// 파티클 랜덤 색상
	EATER_ENGINEDLL void SetStartLifeTime(float time);									// 파티클 고정 유지시간
	EATER_ENGINEDLL void SetStartLifeTime(float minTime, float maxTime);				// 파티클 랜덤 유지시간
	EATER_ENGINEDLL void SetStartScale(float scale);									// 파티클 고정 크기
	EATER_ENGINEDLL void SetStartScale(float minScale, float maxScale);					// 파티클 랜덤 크기
	EATER_ENGINEDLL void SetStartRotation(float rot);									// 파티클 고정 회전
	EATER_ENGINEDLL void SetStartRotation(float minRot, float maxRot);					// 파티클 랜덤 회전

	EATER_ENGINEDLL void SetLifeTimeForce(Vector3 force);								// 파티클 생성 후 고정 속력 범위
	EATER_ENGINEDLL void SetLifeTimeForce(Vector3 minForce, Vector3 maxForce);			// 파티클 생성 후 랜덤 속력 범위
	EATER_ENGINEDLL void SetLifeTimeColor(Vector4 minColor, Vector4 maxColor, PARTICLE_LIFETIME_OPTION option);			// 파티클 생성 후 색상 범위
	EATER_ENGINEDLL void SetLifeTimeScale(float minScale, float maxScale, PARTICLE_LIFETIME_OPTION option);				// 파티클 생성 후 크기 범위
	EATER_ENGINEDLL void SetLifeTimeRotation(float rot);								// 파티클 생성 후 고정 회전 범위
	EATER_ENGINEDLL void SetLifeTimeRotation(float minRot, float maxRot);				// 파티클 생성 후 랜덤 회전 범위

	EATER_ENGINEDLL void SetTextureTiling(int count_x, int count_y);					// 파티클 텍스쳐 나눌 개수
	EATER_ENGINEDLL void SetPlayTime(float playTime, bool loop = false);				// 파티클 시스템 플레이 시간 및 반복 여부
	
	EATER_ENGINEDLL void SetDiffuseName(std::string diffuseName);

	EATER_ENGINEDLL void Play(bool loop = false);										// 파티클 시스템 플레이 시간 및 반복 여부
	EATER_ENGINEDLL void Stop();														// 파티클 시스템 중지

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

	bool m_Looping;				// 반복 재생 여부
	bool m_Playing;				// 재생 여부
	int m_MaxParticle;			// 최대 파티클 개수
	int m_PlayCount;			// 이번에 실행할 파티클 개수

	float m_Strength;			// 파티클 밝기 강도

	float m_PlayTime;			// 실행 지속 시간
	float m_NowPlayTime;		// 현재 지속 시간
	float m_DelayTime;			// 실행 지연 시간
	float m_NowDelayTime;		// 현재 지연 시간
	float m_TickTime;			// 현재 진행 시간
	float m_RateOverTime;		// 초당 출력할 파티클 개수
	float m_RateOverTimeCount;	// 초당 출력할 파티클 개수

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