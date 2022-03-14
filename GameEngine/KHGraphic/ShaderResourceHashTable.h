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
/// - ShaderResource Struct ���� Hash Code Ű������ �����ϱ� ���� Class..
/// - Shader Reflect Load �� Description�� ���ϱ� ���� �ʿ�..
/// - Resource Struct ����� ���ÿ� Hash Table�� �ø��� ���� Define Ȱ��..
/// - Define�� ���� �ش� Resource Struct ����� ���ÿ� Hash Table�� ���..

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
	// Hash Code Push �Լ�..
	bool Push(RESOURCE_TYPE type, std::string name, Hash_Code hash_code);

	// �ش� Hash Code ��ȯ �Լ�..
	size_t FindHashCode(RESOURCE_TYPE type, std::string cBufName);

	bool DefineCheck(UINT nowDefine);

	// Hash Table Reset �Լ�..
	void Destroy();

private:
	// Hash Code Push Check �Լ�..
	bool CheckHashCode(std::unordered_map<std::string, Hash_Code>& table, std::string name, Hash_Code hash_code);

	UINT DEFINE_MASK = 0x00000000;
};
