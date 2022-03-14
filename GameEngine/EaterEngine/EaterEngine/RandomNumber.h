#pragma once
#include <random>

class RandomBase
{
protected:
	static std::default_random_engine g_RandomEngine;

	template<typename T>
	static void Swap(T& minNum, T& maxNum);
};

template<typename T>
void RandomBase::Swap(T& minNum, T& maxNum)
{
	if (minNum != maxNum)
	{
		if (minNum > maxNum)
		{
			T temp = minNum;
			minNum = maxNum;
			maxNum = temp;
		}
	}
}

class RandomInt : public RandomBase
{
public:
	// 해당 난수 범위 초기화..
	void SetRange(int min, int max);

	// 해당 난수 캐시 초기화..
	void Reset();

	// 해당 범위 내의 난수 추출..
	int GetRandomNumber();

private:
	std::uniform_int_distribution<int> m_RandomGenerator;
};

class RandomFloat : public RandomBase
{
public:
	// 해당 난수 범위 초기화..
	void SetRange(float min, float max);

	// 해당 난수 캐시 초기화..
	void Reset();

	// 해당 범위 내의 난수 추출..
	float GetRandomNumber();

private:
	std::uniform_real_distribution<float> m_RandomGenerator;
};

class RandomVector3 : public RandomBase
{
public:
	// 해당 난수 범위 초기화..
	void SetRange(DirectX::SimpleMath::Vector3 min, DirectX::SimpleMath::Vector3 max);

	// 해당 난수 캐시 초기화..
	void Reset();

	// 해당 범위 내의 난수 추출..
	DirectX::SimpleMath::Vector3 GetRandomNumber();

private:
	std::uniform_real_distribution<float> m_RandomGenerator_X;
	std::uniform_real_distribution<float> m_RandomGenerator_Y;
	std::uniform_real_distribution<float> m_RandomGenerator_Z;
};

class RandomVector4 : public RandomBase
{
public:
	// 해당 난수 범위 초기화..
	void SetRange(DirectX::SimpleMath::Vector4 min, DirectX::SimpleMath::Vector4 max);

	// 해당 난수 캐시 초기화..
	void Reset();

	// 해당 범위 내의 난수 추출..
	DirectX::SimpleMath::Vector4 GetRandomNumber();

private:
	std::uniform_real_distribution<float> m_RandomGenerator_X;
	std::uniform_real_distribution<float> m_RandomGenerator_Y;
	std::uniform_real_distribution<float> m_RandomGenerator_Z;
	std::uniform_real_distribution<float> m_RandomGenerator_W;
};