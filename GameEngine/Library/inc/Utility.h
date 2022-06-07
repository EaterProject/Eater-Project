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
		// �ش� ���� ���� �ʱ�ȭ..
		inline void SetRange(int min, int max)
		{
			Swap(min, max);
			m_RandomGenerator = std::uniform_int_distribution<int>(min, max);
		}

		// �ش� ���� ĳ�� �ʱ�ȭ..
		inline void Reset()
		{
			m_RandomGenerator.reset();
		}

		// �ش� ���� ���� ���� ����..
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
		// �ش� ���� ���� �ʱ�ȭ..
		inline void SetRange(float min, float max)
		{
			Swap(min, max);
			m_RandomGenerator = std::uniform_real_distribution<float>(min, max);
		}

		// �ش� ���� ĳ�� �ʱ�ȭ..
		inline void Reset()
		{
			m_RandomGenerator.reset();
		}

		// �ش� ���� ���� ���� ����..
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
		// �ش� ���� ���� �ʱ�ȭ..
		inline void SetRange(DirectX::SimpleMath::Vector3 min, DirectX::SimpleMath::Vector3 max)
		{
			Swap(min.x, max.x);
			m_RandomGenerator_X = std::uniform_real_distribution<float>(min.x, max.x);
			Swap(min.y, max.y);
			m_RandomGenerator_Y = std::uniform_real_distribution<float>(min.y, max.y);
			Swap(min.z, max.z);
			m_RandomGenerator_Z = std::uniform_real_distribution<float>(min.z, max.z);
		}

		// �ش� ���� ĳ�� �ʱ�ȭ..
		inline void Reset()
		{
			m_RandomGenerator_X.reset();
			m_RandomGenerator_Y.reset();
			m_RandomGenerator_Z.reset();
		}

		// �ش� ���� ���� ���� ����..
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
		// �ش� ���� ���� �ʱ�ȭ..
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

		// �ش� ���� ĳ�� �ʱ�ȭ..
		inline void Reset()
		{
			m_RandomGenerator_X.reset();
			m_RandomGenerator_Y.reset();
			m_RandomGenerator_Z.reset();
			m_RandomGenerator_W.reset();
		}

		// �ش� ���� ���� ���� ����..
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
		void Push(Function&& _func);		// Event Function �߰�..
		void Pop(Function&& _func);			// Event Function ����..
		void Reset();						// Event Function �ʱ�ȭ..

	public:
		void operator+=(Function&& _func);	// Event Function �߰�..
		void operator-=(Function&& _func);	// Event Function ����..
		void operator()(_Args... _types);	// Event Function List ȣ��..

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

	// Override �� �Լ��� ���� ��� �ش� �θ��� �Լ� ȣ���ϴ� �������� �Լ�..
	// ��� ���迡�� Overriding üũ�� ��� ���ܰ� �����⿡ �߰�..
	template<typename R, typename T, typename B, typename... Args, Base_Check<T, B> = NULL>
	constexpr auto Bind(R(T::* f)(Args...), B* p)
	{
		return [p, f](Args... args) ->R { (static_cast<T*>(p)->*f)(args...); };
	};

	// �⺻���� Member Function�� Lambda Function���� �����ִ� �Լ�..
	template<typename R, typename T, typename... Args>
	constexpr auto Bind(R(T::* f)(Args...), T* p)
	{
		return [p, f](Args... args) ->R { return (p->*f)(args...); };
	};

	// �⺻���� Static Function�� Lambda Function���� �����ִ� �Լ�..
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

		// ���� �Լ��� �����ϰ� �Լ��� �־ ���
		1) Global Function Binding Ȱ�� ����
		
			void Test_1();
			void Test_2(int _1, float _2, User* _3);
			int Test_3(int _1);
			User* Test_4(int _1, User* _3);

			auto lam_1 = Eater::Bind(Test_1);
			auto lam_2 = Eater::Bind(Test_2);
			auto lam_3 = Eater::Bind(Test_3);
			auto lam_4 = Eater::Bind(Test_4);

		// Ŭ���� ���ο� �ִ� �Լ��� �ش� Ŭ������ �Լ��� ��ü�� �־ ���
		2) Member Function Binding Ȱ�� ����
			
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

		// Ŭ���� ���ο� �ִ� �Լ��� ���� ��Ȳ �� �������̵��� ���� ���� �Լ��� Bind �õ��� ��� �������Ǵµ�
		// ��� ���ܻ�Ȳ�� ó���ϴ� �ͺ��� ��� ���踦 �˻��Ͽ� �ش� ���� ��ü�� �Լ��� Bind �ϴ� �Լ��� ��ȯ
		3) Overriding Exception Function Binding Ȱ�� ����
			
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

		# Eater Delegate Ư��
		1. Event �Լ��� Ȱ���ϱ� ���� ��������Ƿ� ��ȯ���� void�� �����Ѵ�.
		2. �⺻���� �Լ��� std::function�� Ȱ���� �����Ѵ�.
		3. �ش� Event�� ���� ������ Push & Pop �Լ��� Ȱ���ϰų�, += & -= �����ڸ� Ȱ���Ѵ�.
		4. �ش� Delegate Class�� �������� ���ø����� �̷���� �����Ƿ� ������ ����� �ݵ�� ���Խ����־���Ѵ�.

		1) No Parameter Delegate Ȱ�� ����

			void Test_1();
			void Test_2();
			
			Delegate<> _delegate;

			// ����
			_delegate.Push(Test_1);
			_delegate += Test_2;

			// ����
			_delegate.Pop(Test_1);
			_delegate -= Test_2;

			// ȣ��
			_delegate();

		2) Variable Parameter Delegate Ȱ�� ����
			
			void Test_1(int _1, float _2, User* _3);
			void Test_2(int _1, float _2, User* _3);

			Delegate<int, float, User*> _delegate;

			// ����
			_delegate.Push(Test_1);
			_delegate += Test_2;

			// ����
			_delegate.Pop(Test_1);
			_delegate -= Test_2;

			// ȣ��
			_delegate((int)value_1, (float)value_2, (User*)value_3);
	*/
}
