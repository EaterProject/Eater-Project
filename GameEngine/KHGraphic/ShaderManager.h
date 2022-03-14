#pragma once
#include "ShaderManagerBase.h"

interface ID3D11Graphic;

class ShaderManager : public IShaderManager
{
public:
	ShaderManager(ID3D11Graphic* graphic);
	~ShaderManager();

public:
	friend class OriginalShader;

public:
	void Create() override;
	void Start() override;
	void Release() override;

public:
	void AddSampler(Hash_Code hash_code, ID3D11SamplerState* sampler) override;

public:
	ShaderBase* LoadShader(SHADER_TYPE shaderType, const char* fileName, const char* entry_point, const char* shaderName, const D3D_SHADER_MACRO* pDefines = nullptr);
	OriginalShader GetShader(std::string shaderName) override;

private:
	void CreateShader();

	VertexShader* GetVertexShader(std::string shaderName);
	PixelShader* GetPixelShader(std::string shaderName);
	ComputeShader* GetComputeShader(std::string shaderName);

private:
	// Shader List
	std::unordered_map<std::string, ShaderBase*> m_ShaderList;
};
