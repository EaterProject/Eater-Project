#pragma once
#include "ShaderManagerBase.h"
#include "HashBase.h"

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

	void AddConstantBuffer();
	void AddConstantBufferUsage();

	template<typename T>
	void PushConstantBufferUsage(UINT usage);

	ShaderBase* GetBaseShader(std::string shaderName);
	VertexShader* GetVertexShader(std::string shaderName);
	PixelShader* GetPixelShader(std::string shaderName);
	ComputeShader* GetComputeShader(std::string shaderName);

private:
	// Shader List
	std::unordered_map<std::string, ShaderBase*> m_ShaderList;
	std::unordered_map<Hash_Code, UINT> m_ConstantBufferUsageList;
};

template<typename T>
void ShaderManager::PushConstantBufferUsage(UINT usage)
{
	// Template Struct Resource Type Check..
	assert(T::GetType() == RESOURCE_TYPE::CB);

	// 해당 Constant Buffer Hash Code..
	Hash_Code hashCode = T::GetHashCode();

	// 해당 Value 찾기..
	std::unordered_map<Hash_Code, UINT>::iterator it = m_ConstantBufferUsageList.find(hashCode);

	// 해당 Constant Buffer가 등록되어 있다면 삽입하지 않는다..
	if (it != m_ConstantBufferUsageList.end()) return;

	m_ConstantBufferUsageList.insert(std::pair<Hash_Code, UINT>(hashCode, usage));
}
