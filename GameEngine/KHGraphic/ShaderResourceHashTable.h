#pragma once
#include <unordered_map>
#include "EnumDefine.h"

#define DEFINE_CB				0x00000001
#define DEFINE_SS				0x00000010
#define DEFINE_SRV				0x00000100
#define DEFINE_UAV				0x00001000

typedef size_t Hash_Code;

///
/// 2021/11/08 1:22
/// SeoKyuHwang
/// 
/// # ShaderResourceHashTable Class
/// 
/// - ShaderResource Struct 기준 Hash Code 키값으로 설정하기 위한 Class..
/// - Shader Reflect Load 시 Description과 비교하기 위해 필요..
/// - Resource Struct 선언과 동시에 Hash Table에 올리기 위해 Define 활용..
/// - Define을 통해 해당 Resource Struct 선언과 동시에 Hash Table에 등록..

class ShaderResourceHashTable
{
public:
	static ShaderResourceHashTable* Get();

	static ShaderResourceHashTable* instance;

public:
	// ConstantBuffer Hash Table..
	std::unordered_map<std::string, Hash_Code> g_CBuffer_HashTable;
	// Sampler Hash Table..
	std::unordered_map<std::string, Hash_Code> g_Sampler_HashTable;
	// ShaderResourceView Hash Table..
	std::unordered_map<std::string, Hash_Code> g_SRV_HashTable;
	// UnorderedAccessView Hash Table..
	std::unordered_map<std::string, Hash_Code> g_UAV_HashTable;

public:
	// Hash Code Push 함수..
	bool Push(RESOURCE_TYPE type, std::string name, Hash_Code hash_code);

	// 해당 Hash Code 반환 함수..
	size_t FindHashCode(RESOURCE_TYPE type, std::string cBufName);

	bool DefineCheck(UINT nowDefine);

	// Hash Table Reset 함수..
	void Destroy();

private:
	// Hash Code Push Check 함수..
	bool CheckHashCode(std::unordered_map<std::string, Hash_Code>& table, std::string name, Hash_Code hash_code);

	UINT DEFINE_MASK = 0x00000000;
};
