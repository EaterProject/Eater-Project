#pragma once
//***************************************************************************************
// 
// Random Class ( int, float, vector2, vector3, vector4 )
// 
//***************************************************************************************

template<typename T>
inline void Eater::RandomBase::Swap(T& minNum, T& maxNum)
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

inline void Eater::RandomInt::SetRange(int min, int max)
{
	Swap(min, max);
	m_RandomGenerator = std::uniform_int_distribution<int>(min, max);
}

inline void Eater::RandomInt::Reset()
{
	m_RandomGenerator.reset();
}

inline int Eater::RandomInt::GetRandomNumber()
{
	return m_RandomGenerator(g_RandomEngine);
}

inline void Eater::RandomFloat::SetRange(float min, float max)
{
	Swap(min, max);
	m_RandomGenerator = std::uniform_real_distribution<float>(min, max);
}

inline void Eater::RandomFloat::Reset()
{
	m_RandomGenerator.reset();

}

inline float Eater::RandomFloat::GetRandomNumber()
{
	return m_RandomGenerator(g_RandomEngine);
}

inline void Eater::RandomVector3::SetRange(DirectX::SimpleMath::Vector3 min, DirectX::SimpleMath::Vector3 max)
{
	Swap(min.x, max.x);
	m_RandomGenerator_X = std::uniform_real_distribution<float>(min.x, max.x);
	Swap(min.y, max.y);
	m_RandomGenerator_Y = std::uniform_real_distribution<float>(min.y, max.y);
	Swap(min.z, max.z);
	m_RandomGenerator_Z = std::uniform_real_distribution<float>(min.z, max.z);
}

inline void Eater::RandomVector3::Reset()
{
	m_RandomGenerator_X.reset();
	m_RandomGenerator_Y.reset();
	m_RandomGenerator_Z.reset();
}

inline DirectX::SimpleMath::Vector3 Eater::RandomVector3::GetRandomNumber()
{
	return DirectX::SimpleMath::Vector3(m_RandomGenerator_X(g_RandomEngine),
										m_RandomGenerator_Y(g_RandomEngine),
										m_RandomGenerator_Z(g_RandomEngine));
}

inline void Eater::RandomVector4::SetRange(DirectX::SimpleMath::Vector4 min, DirectX::SimpleMath::Vector4 max)
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

inline void Eater::RandomVector4::Reset()
{
	m_RandomGenerator_X.reset();
	m_RandomGenerator_Y.reset();
	m_RandomGenerator_Z.reset();
	m_RandomGenerator_W.reset();
}

inline DirectX::SimpleMath::Vector4 Eater::RandomVector4::GetRandomNumber()
{
	return DirectX::SimpleMath::Vector4(m_RandomGenerator_X(g_RandomEngine),
										m_RandomGenerator_Y(g_RandomEngine),
										m_RandomGenerator_Z(g_RandomEngine),
										m_RandomGenerator_W(g_RandomEngine));
}

//***************************************************************************************
// 
// Delegate Class
// 
//***************************************************************************************
template<typename ..._Args>
using Function = std::function<void(_Args...)>;

template<typename ..._Args>
inline void Eater::Delegate<_Args...>::Push(Function&& _func)
{
	pFunctionList.push_back(_func);
}

template<typename ..._Args>
inline void Eater::Delegate<_Args...>::Pop(Function&& _func)
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
inline void Eater::Delegate< _Args... >::Reset()
{
	pFunctionList.clear();
}

template<typename ..._Args>
inline void Eater::Delegate<_Args...>::operator+=(Function&& _func)
{
	pFunctionList.push_back(_func);
}

template<typename ..._Args>
inline void Eater::Delegate<_Args...>::operator-=(Function&& _func)
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
inline void Eater::Delegate<_Args...>::operator=(Function&& _func)
{
	pFunctionList.clear();
	pFunctionList.push_back(_func);
}

template<typename ..._Args>
inline void Eater::Delegate<_Args...>::operator()(_Args... _types)
{
	for (auto& func : pFunctionList)
	{
		func(std::forward<_Args>(_types)...);
	}
}
