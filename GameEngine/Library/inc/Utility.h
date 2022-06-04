#pragma once
#include <random>
#include <vector>
#include <iostream>
#include <functional>
#include <type_traits>
#include <utility>
#include "SimpleMath.h"

namespace Eater
{
	//***************************************************************************************
	// 
	// Random Class ( int, float, vector2, vector3, vector4 )
	// 
	//***************************************************************************************

	class RandomBase
	{
	protected:
		static inline std::default_random_engine g_RandomEngine;

		template<typename T>
		static void Swap(T& minNum, T& maxNum);
	};

	template<typename T>
	inline void RandomBase::Swap(T& minNum, T& maxNum)
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
		inline void SetRange(int min, int max)
		{
			Swap(min, max);
			m_RandomGenerator = std::uniform_int_distribution<int>(min, max);
		}

		// 해당 난수 캐시 초기화..
		inline void Reset()
		{
			m_RandomGenerator.reset();
		}

		// 해당 범위 내의 난수 추출..
		inline int GetRandomNumber()
		{
			return m_RandomGenerator(g_RandomEngine);
		}

	private:
		std::uniform_int_distribution<int> m_RandomGenerator;
	};

	class RandomFloat : public RandomBase
	{
	public:
		// 해당 난수 범위 초기화..
		inline void SetRange(float min, float max)
		{
			Swap(min, max);
			m_RandomGenerator = std::uniform_real_distribution<float>(min, max);
		}

		// 해당 난수 캐시 초기화..
		inline void Reset()
		{
			m_RandomGenerator.reset();
		}

		// 해당 범위 내의 난수 추출..
		inline float GetRandomNumber()
		{
			return m_RandomGenerator(g_RandomEngine);
		}

	private:
		std::uniform_real_distribution<float> m_RandomGenerator;
	};

	class RandomVector3 : public RandomBase
	{
	public:
		// 해당 난수 범위 초기화..
		inline void SetRange(DirectX::SimpleMath::Vector3 min, DirectX::SimpleMath::Vector3 max)
		{
			Swap(min.x, max.x);
			m_RandomGenerator_X = std::uniform_real_distribution<float>(min.x, max.x);
			Swap(min.y, max.y);
			m_RandomGenerator_Y = std::uniform_real_distribution<float>(min.y, max.y);
			Swap(min.z, max.z);
			m_RandomGenerator_Z = std::uniform_real_distribution<float>(min.z, max.z);
		}

		// 해당 난수 캐시 초기화..
		inline void Reset()
		{
			m_RandomGenerator_X.reset();
			m_RandomGenerator_Y.reset();
			m_RandomGenerator_Z.reset();
		}

		// 해당 범위 내의 난수 추출..
		inline DirectX::SimpleMath::Vector3 GetRandomNumber()
		{
			return DirectX::SimpleMath::Vector3(m_RandomGenerator_X(g_RandomEngine),
												m_RandomGenerator_Y(g_RandomEngine),
												m_RandomGenerator_Z(g_RandomEngine));
		}

	private:
		std::uniform_real_distribution<float> m_RandomGenerator_X;
		std::uniform_real_distribution<float> m_RandomGenerator_Y;
		std::uniform_real_distribution<float> m_RandomGenerator_Z;
	};

	class RandomVector4 : public RandomBase
	{
	public:
		// 해당 난수 범위 초기화..
		inline void SetRange(DirectX::SimpleMath::Vector4 min, DirectX::SimpleMath::Vector4 max)
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

		// 해당 난수 캐시 초기화..
		inline void Reset()
		{
			m_RandomGenerator_X.reset();
			m_RandomGenerator_Y.reset();
			m_RandomGenerator_Z.reset();
			m_RandomGenerator_W.reset();
		}

		// 해당 범위 내의 난수 추출..
		inline DirectX::SimpleMath::Vector4 GetRandomNumber()
		{
			return DirectX::SimpleMath::Vector4(m_RandomGenerator_X(g_RandomEngine),
												m_RandomGenerator_Y(g_RandomEngine),
												m_RandomGenerator_Z(g_RandomEngine),
												m_RandomGenerator_W(g_RandomEngine));
		}

	private:
		std::uniform_real_distribution<float> m_RandomGenerator_X;
		std::uniform_real_distribution<float> m_RandomGenerator_Y;
		std::uniform_real_distribution<float> m_RandomGenerator_Z;
		std::uniform_real_distribution<float> m_RandomGenerator_W;
	};

	//***************************************************************************************
	// 
	// Delegate Class
	// 
	//***************************************************************************************
	template<typename ..._Args>
	class Delegate
	{
	public:
		//using Function = std::function<void(_Args...)>;

	public:
		void Push(std::function<void(_Args...)>&& _func);
		void Pop(std::function<void(_Args...)>&& _func);

	public:
		void operator-=(std::function<void(_Args...)>&& _func);
		void operator+=(std::function<void(_Args...)>&& _func);
		void operator()(_Args... _types);

	private:
		std::vector<std::function<void(_Args...)>> pFunctionList;
	};

	template<typename ..._Args>
	inline void Delegate<_Args...>::Push(std::function<void(_Args...)>&& _func)
	{
		pFunctionList.push_back(_func);
	}

	template<typename ..._Args>
	inline void Delegate<_Args...>::Pop(std::function<void(_Args...)>&& _func)
	{
		for (int index = 0; index < pFunctionList.size(); index++)
		{
			if (pFunctionList[index].target_type() == _func.target_type())
			{
				pFunctionList.erase(std::next(pFunctionList.begin(), index));
				break;
			}
		}
	}

	template<typename ..._Args>
	inline void Delegate<_Args...>::operator-=(std::function<void(_Args...)>&& _func)
	{
		for (int index = 0; index < pFunctionList.size(); index++)
		{
			if (pFunctionList[index].target_type() == _func.target_type())
			{
				pFunctionList.erase(std::next(pFunctionList.begin(), index));
				break;
			}
		}
	}

	template<typename ..._Args>
	inline void Delegate<_Args...>::operator+=(std::function<void(_Args...)>&& _func)
	{
		pFunctionList.push_back(_func);
	}

	template<typename ..._Args>
	inline void Delegate<_Args...>::operator()(_Args... _types)
	{
		for (auto& func : pFunctionList)
		{
			func(std::forward<_Args>(_types)...);
		}
	}
}
