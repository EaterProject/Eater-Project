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
		using Function = std::function<void(_Args...)>;

	public:
		void Push(Function&& _func);		// Event Function 추가..
		void Pop(Function&& _func);			// Event Function 제거..
		void Reset();						// Event Function 초기화..

	public:
		void operator+=(Function&& _func);	// Event Function 추가..
		void operator-=(Function&& _func);	// Event Function 제거..
		void operator()(_Args... _types);	// Event Function List 호출..

	private:
		std::vector<Function> pFunctionList;
	};

	template<typename ..._Args>
	inline void Delegate<_Args...>::Push(Function&& _func)
	{
		pFunctionList.push_back(_func);
	}

	template<typename ..._Args>
	inline void Delegate<_Args...>::Pop(Function&& _func)
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
	inline void Delegate< _Args... >::Reset()
	{
		pFunctionList.clear();
	}

	template<typename ..._Args>
	inline void Delegate<_Args...>::operator+=(Function&& _func)
	{
		pFunctionList.push_back(_func);
	}

	template<typename ..._Args>
	inline void Delegate<_Args...>::operator-=(Function&& _func)
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
	inline void Delegate<_Args...>::operator()(_Args... _types)
	{
		for (auto& func : pFunctionList)
		{
			func(std::forward<_Args>(_types)...);
		}
	}

	//***************************************************************************************
	// 
	// Bind Function Template
	// 
	//***************************************************************************************
	template<typename B, typename T>
	using Base_Check = typename std::enable_if<std::is_base_of<B, T>::value, bool>::type;

	// Override 된 함수가 없는 경우 해당 부모의 함수 호출하는 예외적인 함수..
	// 상속 관계에서 Overriding 체크할 경우 예외가 나오기에 추가..
	template<typename R, typename T, typename B, typename... Args, Base_Check<T, B> = NULL>
	constexpr auto Bind(R(T::* f)(Args...), B* p)
	{
		return [p, f](Args... args) ->R { (static_cast<T*>(p)->*f)(args...); };
	};

	// 기본적인 Member Function을 Lambda Function으로 묶어주는 함수..
	template<typename R, typename T, typename... Args>
	constexpr auto Bind(R(T::* f)(Args...), T* p)
	{
		return [p, f](Args... args) ->R { return (p->*f)(args...); };
	};

	// 기본적인 Static Function을 Lambda Function으로 묶어주는 함수..
	template<typename R, typename... Args>
	constexpr auto Bind(R(*f)(Args...))
	{
		return [f](Args... args) ->R { return (*f)(args...); };
	};

	//***************************************************************************************
	// 
	// Delegate & Bind Function Example
	// 
	//***************************************************************************************
	/*
		- Bind Function Example -

		// 전역 함수는 간단하게 함수만 넣어서 사용
		1) Global Function Binding 활용 예시
		
			void Test_1();
			void Test_2(int _1, float _2, User* _3);
			int Test_3(int _1);
			User* Test_4(int _1, User* _3);

			auto lam_1 = Eater::Bind(Test_1);
			auto lam_2 = Eater::Bind(Test_2);
			auto lam_3 = Eater::Bind(Test_3);
			auto lam_4 = Eater::Bind(Test_4);

		// 클래스 내부에 있는 함수는 해당 클래스의 함수와 객체를 넣어서 사용
		2) Member Function Binding 활용 예시
			
			class Base
			{
			public:
				virtual void Test_1() {};
				virtual void Test_2() abstract;
			};

			class Child : public Base
			{
			public:
				void Test_1() override {};
				void Test_2() override {};
			};

			Child* instance = new Child();

			auto lam_1 = Eater::Bind(&Child::Test_1, instance);
			auto lam_2 = Eater::Bind(&Child::Test_2, instance);

		// 클래스 내부에 있는 함수의 예외 상황 중 오버라이딩이 되지 않은 함수를 Bind 시도할 경우 문제가되는데
		// 모든 예외상황을 처리하는 것보다 상속 관계를 검사하여 해당 상위 객체의 함수로 Bind 하는 함수로 변환
		3) Overriding Exception Function Binding 활용 예시
			
			class Base
			{
			public:
				void Test_1() {};
				virtual void Test_2() {};
			};

			class Child : public Base {};

			Child* instance = new Child();

			auto lam_1 = Eater::Bind(&Child::Test_1, instance);	// Convert -> Eater::Bind(&Base::Test_1, instance);
			auto lam_2 = Eater::Bind(&Child::Test_2, instance);	// Convert -> Eater::Bind(&Base::Test_2, instance);

		- Delegate Example -

		# Eater Delegate 특성
		1. Event 함수로 활용하기 위해 만들었으므로 반환형은 void로 고정한다.
		2. 기본적인 함수를 std::function을 활용해 관리한다.
		3. 해당 Event의 삽입 삭제는 Push & Pop 함수를 활용하거나, += & -= 연산자를 활용한다.
		4. 해당 Delegate Class는 가변인자 템플릿으로 이루어져 있으므로 선언한 헤더에 반드시 포함시켜주어야한다.

		1) No Parameter Delegate 활용 예시

			void Test_1();
			void Test_2();
			
			Delegate<> _delegate;

			// 삽입
			_delegate.Push(Test_1);
			_delegate += Test_2;

			// 삭제
			_delegate.Pop(Test_1);
			_delegate -= Test_2;

			// 호출
			_delegate();

		2) Variable Parameter Delegate 활용 예시
			
			void Test_1(int _1, float _2, User* _3);
			void Test_2(int _1, float _2, User* _3);

			Delegate<int, float, User*> _delegate;

			// 삽입
			_delegate.Push(Test_1);
			_delegate += Test_2;

			// 삭제
			_delegate.Pop(Test_1);
			_delegate -= Test_2;

			// 호출
			_delegate((int)value_1, (float)value_2, (User*)value_3);
	*/
}
