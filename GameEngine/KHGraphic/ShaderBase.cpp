#include "DirectDefine.h"
#include "ShaderBase.h"
#include "CompilerDefine.h"

Microsoft::WRL::ComPtr<ID3D11Device> IShader::g_Device = nullptr;
Microsoft::WRL::ComPtr<ID3D11DeviceContext> IShader::g_DeviceContext = nullptr;
std::string IShader::g_ShaderRoute;

void IShader::Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
{
	g_Device = device;
	g_DeviceContext = context;
}

void IShader::Reset()
{
	g_Device.Reset();
	g_DeviceContext.Reset();
}

void IShader::SetShaderRoute(std::string fileRoute)
{
	g_ShaderRoute = fileRoute;
}

void ShaderBase::Release()
{
	for (Microsoft::WRL::ComPtr<ID3D11Buffer> cBuffer : m_ConstantBuffers)
	{
		RESET_COM(cBuffer);
	}

	for (Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler : m_SamplerStates)
	{
		RESET_COM(sampler);
	}

	for (Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv : m_ShaderResourceViews)
	{
		RESET_COM(srv);
	}

	for (std::pair<Hash_Code, ConstantBuffer*> cBuffer : m_ConstantBufferList)
	{
		SAFE_DELETE(cBuffer.second);
	}

	for (std::pair<Hash_Code, SamplerBuffer*> sampler : m_SamplerList)
	{
		SAFE_DELETE(sampler.second);
	}

	for (std::pair<Hash_Code, ShaderResourceBuffer*> srv : m_SRVList)
	{
		SAFE_DELETE(srv.second);
	}

	m_ConstantBuffers.clear();
	m_SamplerStates.clear();
	m_ShaderResourceViews.clear();
	m_SamplerList.clear();
	m_ConstantBufferList.clear();
	m_SRVList.clear();
}

void ShaderBase::CreateShader(const wchar_t* wPath, const D3D_SHADER_MACRO* pDefines, LPCSTR entry_point, LPCSTR shader_model, ID3DBlob** ppShader)
{
	//플래그 설정
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	dwShaderFlags |= D3DCOMPILE_DEBUG;
	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif 
	//dwShaderFlags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;

	HR(D3DCompileFromFile(wPath, pDefines, D3D_COMPILE_STANDARD_FILE_INCLUDE, entry_point, shader_model, dwShaderFlags, NULL, ppShader, NULL));
}

void ShaderBase::SetSamplerState(Hash_Code hash_code, ID3D11SamplerState* sampler)
{
	// 해당 Value 찾기..
	std::unordered_map<Hash_Code, SamplerBuffer*>::iterator it = m_SamplerList.find(hash_code);

	// 해당 Key에 대한 Value가 없다면..
	if (it == m_SamplerList.end()) return;
	
	// 해당 Register Slot에 삽입..
	m_SamplerStates[it->second->register_number] = sampler;
}

void ShaderBase::SetConstantBuffer(Hash_Code hash_code, CBUFFER_USAGE usage)
{
	// 해당 Value 찾기..
	std::unordered_map<Hash_Code, ConstantBuffer*>::iterator it = m_ConstantBufferList.find(hash_code);

	// 해당 Key에 대한 Value가 없다면..
	if (it == m_ConstantBufferList.end()) return;

	// 해당 Constant Buffer Usage 설정..
	ConstantBuffer* cBuffer = it->second;
	cBuffer->cUsage = usage;

	D3D11_USAGE cBufferUsage;
	UINT cBufferFlag = 0;

	// Usage에 따른 옵션 설정..
	switch (usage)
	{
	case CBUFFER_USAGE::DEFAULT:
		cBufferUsage = D3D11_USAGE_DEFAULT;
		break;
	case CBUFFER_USAGE::IMMUTABLE:
		cBufferUsage = D3D11_USAGE_IMMUTABLE;
		break;
	case CBUFFER_USAGE::DYNAMIC:
		cBufferUsage = D3D11_USAGE_DYNAMIC;
		cBufferFlag = D3D11_CPU_ACCESS_WRITE;
		break;
	case CBUFFER_USAGE::STAGING:
		cBufferUsage = D3D11_USAGE_STAGING;
		cBufferFlag = D3D11_CPU_ACCESS_READ;
		break;
	default:
		break;
	}

	// 새로 생성할 Constant Buffer..
	ID3D11Buffer* cBuf = nullptr;

	// 해당 Constant Buffer 생성..
	CD3D11_BUFFER_DESC cBufferDesc((UINT)cBuffer->cSize, D3D11_BIND_CONSTANT_BUFFER, cBufferUsage, cBufferFlag);

	HR(g_Device->CreateBuffer(&cBufferDesc, nullptr, &cBuf));

	// Debug Name..
	GPU_RESOURCE_DEBUG_NAME(cBuf, cBuffer->buffer_name.c_str());

	// 해당 Constant Buffer 삽입..
	m_ConstantBuffers[cBuffer->register_number] = cBuf;
}

SHADER_TYPE ShaderBase::GetType()
{
	return m_ShaderType;
}
