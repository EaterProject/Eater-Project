#pragma once
#include <string>
#include "ShaderTypes.h"

///
/// 2021/11/07 23:00
/// SeoKyuHwang
///
/// ShaderManager Interface Class
///
/// - Shader Resource를 관리하는 Class

typedef size_t Hash_Code;

interface IShaderManager
{
public:
	virtual void Create() abstract;
	virtual void Start() abstract;
	virtual void Release() abstract;
	
public:
	virtual void AddSampler(Hash_Code hash_code, ID3D11SamplerState* sampler) abstract;

public:
	// GetShader Original Pointer..
	virtual OriginalShader GetShader(std::string shaderName) abstract;
};