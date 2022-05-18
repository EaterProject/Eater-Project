#pragma once
#include <string>
#include <typeinfo>
#include "ShaderResourceHashTable.h"

/* 
# HashClass Create Define
* ClassName		-> Struct Name
* ResourceType	-> Struct Resource Type (RESOURCE_TYPE)
*/
#define CREATE_HASH_CLASS(ClassName, ResourceType)	\
struct ClassName : public HashClass<ClassName>	\
{	\
	static RESOURCE_TYPE GetType() { return ResourceType; }	\
};

/* 
# Resource Push HashTable Define
* ClassName		-> Struct Name
* ResourceType	-> Struct Resource Type (RESOURCE_TYPE)
*/
#define RESOURCE_PUSH(ClassName, ResourceType) static bool push_##ClassName = ShaderResourceHashTable::Get()->Push(ResourceType, #ClassName, typeid(ClassName).hash_code());

/* 
# Resource Push HashTable Overlap Check Define
* ResourceMask	-> Resource Define Mask
*/
#define RESOURCE_DEFINE(ResourceMask) static bool overlap_##ResourceMask = ShaderResourceHashTable::Get()->DefineCheck(ResourceMask);

///
/// 2021/11/07 23:02
/// SeoKyuHwang
/// 
/// # HashClass Class
/// 
/// - 모든 Graphic Resource의 기반이 되는 클래스
/// - Shader Reflection을 통해 Resource의 이름을 비교해야 하기 때문에 이름대신 Hash Code로 비교하기 위해 추가를 하였는데
///	  나머지 Graphic Resource도 해당 Struct를 지정해두면 실수가 줄어들것 같아 모든 Resource를 Hash Code를 통해 가져오기 위함
/// 
template <typename T>
struct HashClass
{
	static std::string GetName()
	{
		std::string name = typeid(T).name();
		size_t spacePosition = name.find_first_of(" ");
		if (spacePosition != std::string::npos)
			return name.substr(spacePosition + 1, name.length());
		else
			return name;
	}
	
	static size_t GetHashCode()
	{
		return typeid(T).hash_code();
	}
};

// Template을 통해 들어오는 Class가 HashClass를 상속 받았는지 체크..
template<typename T>
using Enable_Check = typename std::enable_if<std::is_base_of<HashClass<T>, T>::value, bool>::type;