#include "SimpleMath.h"
#include "RandomNumber.h"

std::default_random_engine RandomBase::g_RandomEngine;

void RandomInt::SetRange(int min, int max)
{
	Swap(min, max);
	m_RandomGenerator = std::uniform_int_distribution<int>(min, max);
}

void RandomInt::Reset()
{
	m_RandomGenerator.reset();
}

int RandomInt::GetRandomNumber()
{
	return m_RandomGenerator(g_RandomEngine);
}

void RandomFloat::SetRange(float min, float max)
{
	Swap(min, max);
	m_RandomGenerator = std::uniform_real_distribution<float>(min, max);
}

void RandomFloat::Reset()
{
	m_RandomGenerator.reset();
}

float RandomFloat::GetRandomNumber()
{
	return m_RandomGenerator(g_RandomEngine);
}

void RandomVector3::SetRange(DirectX::SimpleMath::Vector3 min, DirectX::SimpleMath::Vector3 max)
{
	Swap(min.x, max.x);
	m_RandomGenerator_X = std::uniform_real_distribution<float>(min.x, max.x);
	Swap(min.y, max.y);
	m_RandomGenerator_Y = std::uniform_real_distribution<float>(min.y, max.y);
	Swap(min.z, max.z);
	m_RandomGenerator_Z = std::uniform_real_distribution<float>(min.z, max.z);
}

void RandomVector3::Reset()
{
	m_RandomGenerator_X.reset();
	m_RandomGenerator_Y.reset();
	m_RandomGenerator_Z.reset();
}

DirectX::SimpleMath::Vector3 RandomVector3::GetRandomNumber()
{
	return DirectX::SimpleMath::Vector3(m_RandomGenerator_X(g_RandomEngine),
										m_RandomGenerator_Y(g_RandomEngine),
										m_RandomGenerator_Z(g_RandomEngine));
}

void RandomVector4::SetRange(DirectX::SimpleMath::Vector4 min, DirectX::SimpleMath::Vector4 max)
{
	Swap(min.x, max.x);
	m_RandomGenerator_X = std::uniform_real_distribution<float>(min.x, max.x);

	Swap(min.y, max.y);
	m_RandomGenerator_Y = std::uniform_real_distribution<float>(min.y, max.y);

	Swap(min.z, max.z);
	m_RandomGenerator_Z = std::uniform_real_distribution<float>(min.z, max.z);

	Swap(min.w, max.w);
	m_RandomGenerator_W = std::uniform_real_distribution<float>(min.w, max.w);
}

void RandomVector4::Reset()
{
	m_RandomGenerator_X.reset();
	m_RandomGenerator_Y.reset();
	m_RandomGenerator_Z.reset();
	m_RandomGenerator_W.reset();
}

DirectX::SimpleMath::Vector4 RandomVector4::GetRandomNumber()
{
	return DirectX::SimpleMath::Vector4(m_RandomGenerator_X(g_RandomEngine),
										m_RandomGenerator_Y(g_RandomEngine),
										m_RandomGenerator_Z(g_RandomEngine), 
										m_RandomGenerator_W(g_RandomEngine));
}
