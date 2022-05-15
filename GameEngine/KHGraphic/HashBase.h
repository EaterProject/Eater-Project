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
/// - ��� Graphic Resource�� ����� �Ǵ� Ŭ����
/// - Shader Reflection�� ���� Resource�� �̸��� ���ؾ� �ϱ� ������ �̸���� Hash Code�� ���ϱ� ���� �߰��� �Ͽ��µ�
///	  ������ Graphic Resource�� �ش� Struct�� �����صθ� �Ǽ��� �پ��� ���� ��� Resource�� Hash Code�� ���� �������� ����
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

// Template�� ���� ������ Class�� HashClass�� ��� �޾Ҵ��� üũ..
template<typename T>
using Enable_Check = typename std::enable_if<std::is_base_of<HashClass<T>, T>::value, bool>::type;