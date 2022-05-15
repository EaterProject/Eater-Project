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
	// �ش� ���� ���� �ʱ�ȭ..
	void SetRange(int min, int max);

	// �ش� ���� ĳ�� �ʱ�ȭ..
	void Reset();

	// �ش� ���� ���� ���� ����..
	int GetRandomNumber();

private:
	std::uniform_int_distribution<int> m_RandomGenerator;
};

class RandomFloat : public RandomBase
{
public:
	// �ش� ���� ���� �ʱ�ȭ..
	void SetRange(float min, float max);

	// �ش� ���� ĳ�� �ʱ�ȭ..
	void Reset();

	// �ش� ���� ���� ���� ����..
	float GetRandomNumber();

private:
	std::uniform_real_distribution<float> m_RandomGenerator;
};

class RandomVector3 : public RandomBase
{
public:
	// �ش� ���� ���� �ʱ�ȭ..
	void SetRange(DirectX::SimpleMath::Vector3 min, DirectX::SimpleMath::Vector3 max);

	// �ش� ���� ĳ�� �ʱ�ȭ..
	void Reset();

	// �ش� ���� ���� ���� ����..
	DirectX::SimpleMath::Vector3 GetRandomNumber();

private:
	std::uniform_real_distribution<float> m_RandomGenerator_X;
	std::uniform_real_distribution<float> m_RandomGenerator_Y;
	std::uniform_real_distribution<float> m_RandomGenerator_Z;
};

class RandomVector4 : public RandomBase
{
public:
	// �ش� ���� ���� �ʱ�ȭ..
	void SetRange(DirectX::SimpleMath::Vector4 min, DirectX::SimpleMath::Vector4 max);

	// �ش� ���� ĳ�� �ʱ�ȭ..
	void Reset();

	// �ش� ���� ���� ���� ����..
	DirectX::SimpleMath::Vector4 GetRandomNumber();

private:
	std::uniform_real_distribution<float> m_RandomGenerator_X;
	std::uniform_real_distribution<float> m_RandomGenerator_Y;
	std::uniform_real_distribution<float> m_RandomGenerator_Z;
	std::uniform_real_distribution<float> m_RandomGenerator_W;
};