#pragma once
typedef size_t	register_slot;
typedef size_t	Hash_Code;

// Constant Buffer Usage Type..
typedef enum CBUFFER_USAGE : UINT
{
	DEFAULT,
	IMMUTABLE,
	DYNAMIC,
	STAGING
}CBUFFER_USAGE;

// 모든 Shader Resource들의 Base Class..
// 해당 Resource Buffer Name & Binding Register Index 보유..
class ShaderResourceBase
{
public:
	ShaderResourceBase(std::string name, register_slot rNum) : buffer_name(name), register_number(rNum) {}

public:
	// Buffer Name
	std::string buffer_name;

	// Register Slot Number
	register_slot register_number;
};

// ConstantBuffer Resource Data Class
class ConstantBuffer : public ShaderResourceBase
{
public:
	ConstantBuffer(std::string name, register_slot rNum, size_t size) : ShaderResourceBase(name, rNum), cSize(size) {}

public:
	CBUFFER_USAGE cUsage;
	size_t cSize;
};

// SamplerState Resource Data Class
class SamplerBuffer : public ShaderResourceBase
{
public:
	SamplerBuffer(std::string name, register_slot rNum) : ShaderResourceBase(name, rNum) {}
};

// ShaderResourceView Resource Data Class
class ShaderResourceBuffer : public ShaderResourceBase
{
public:
	ShaderResourceBuffer(std::string name, register_slot rNum) : ShaderResourceBase(name, rNum) {}
};

// UnorderedAccessView Resource Data Class
class UnorderedAccessBuffer : public ShaderResourceBase
{
public:
	UnorderedAccessBuffer(std::string name, register_slot rNum) : ShaderResourceBase(name, rNum) {}
};
