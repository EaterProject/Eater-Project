#include "DirectDefine.h"
#include "ShaderBase.h"
#include "ComputeShader.h"

#include "ShaderResourceHashTable.h"
#include "CompilerDefine.h"
#include <sstream>
#include <fstream>

ComputeShader::ComputeShader(const char* shaderName, const char* fileName, const char* entry_point, const char* shader_model, const D3D_SHADER_MACRO* pDefines)
	:ShaderBase(SHADER_TYPE::COMPUTE_SHADER, shaderName)
{
	LoadShader(g_ShaderRoute + fileName, entry_point, shader_model, pDefines);
}

ComputeShader::~ComputeShader()
{

}

void ComputeShader::LoadShader(std::string fileName, const char* entry_point, const char* shader_model, const D3D_SHADER_MACRO* pDefines)
{
	ID3DBlob* shaderBlob = nullptr;
	ID3D11ShaderReflection* pReflector = nullptr;
	ShaderResourceHashTable* resource_table = ShaderResourceHashTable::Get();

	int cbuffer_register_slot = -1;	// ConstantBuffer Max Register Slot
	int sampler_register_slot = -1;	// Sampler Max Register Slot
	int srv_register_slot = -1;		// ShaderResourceView Max Register Slot
	int uav_register_slot = -1;		// UnorderedAccessView Max Register Slot
	size_t hash_key = 0;			// Resource Hash Code

	std::wstring wPath(fileName.begin(), fileName.end());

	// Compute HLSL Load..
	CreateShader(wPath.c_str(), pDefines, entry_point, shader_model, &shaderBlob);

	// Create Compute Shader..
	HR(g_Device->CreateComputeShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, &m_CS));
	
	// Debug Name..
	GPU_RESOURCE_DEBUG_NAME(m_CS.Get(), entry_point);
	
	// Create Reflector..
	D3DReflect(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pReflector);

	D3D11_SHADER_DESC shaderDesc;
	pReflector->GetDesc(&shaderDesc);

	/// Shader Resource Reflection
	// Shader Resource..
	for (unsigned int rsindex = 0; rsindex < shaderDesc.BoundResources; rsindex++)
	{
		D3D11_SHADER_INPUT_BIND_DESC bindDesc;
		pReflector->GetResourceBindingDesc(rsindex, &bindDesc);

		// Resource Type에 맞는 해당 List에 삽입..
		switch (bindDesc.Type)
		{
		case D3D_SIT_CBUFFER:
		{
			// Constant Buffer Reflection..
			ID3D11ShaderReflectionConstantBuffer* cBuffer = pReflector->GetConstantBufferByName(bindDesc.Name);
			D3D11_SHADER_BUFFER_DESC bufferDesc;

			cBuffer->GetDesc(&bufferDesc);

			// Constant Buffer Hash Code..
			hash_key = resource_table->FindHashCode(RESOURCE_TYPE::CB, bindDesc.Name);

			// Constant Buffer Register Slot Number..
			cbuffer_register_slot = bindDesc.BindPoint;

			// Key (Constant Buffer HashCode) && Value (Register Slot, Constant Buffer)
			m_ConstantBufferList.insert(std::make_pair(hash_key, new ConstantBuffer(bindDesc.Name, cbuffer_register_slot, bufferDesc.Size)));
		}
		break;
		case D3D_SIT_TEXTURE:
		{
			// SRV Hash Code..
			hash_key = resource_table->FindHashCode(RESOURCE_TYPE::SRV, bindDesc.Name);

			// SRV Register Slot Number..
			srv_register_slot = bindDesc.BindPoint;

			// SRV 추가..
			m_SRVList.insert(std::make_pair(hash_key, new ShaderResourceBuffer(bindDesc.Name, srv_register_slot)));
		}
		break;
		case D3D_SIT_SAMPLER:
		{
			// Sampler Hash Code..
			hash_key = resource_table->FindHashCode(RESOURCE_TYPE::SS, bindDesc.Name);

			// Sampler Register Slot Number..
			sampler_register_slot = bindDesc.BindPoint;

			// Sampler 추가..
			m_SamplerList.insert(std::make_pair(hash_key, new SamplerBuffer(bindDesc.Name, sampler_register_slot)));
		}
		break;
		case D3D_SIT_UAV_RWTYPED:
		{
			// UAV Hash Code..
			hash_key = resource_table->FindHashCode(RESOURCE_TYPE::UAV, bindDesc.Name);

			// UAV Register Slot Number..
			uav_register_slot = bindDesc.BindPoint;

			// UAV 추가..
			m_UAVList.insert(std::make_pair(hash_key, new UnorderedAccessBuffer(bindDesc.Name, uav_register_slot)));
		}
		break;
		default:
			break;
		}
	}

	// 마지막으로 Binding 된 Resource Register Index 기준으로 사이즈 설정..
	m_ConstantBuffers.resize(++cbuffer_register_slot);
	m_SamplerStates.resize(++sampler_register_slot);
	m_ShaderResourceViews.resize(++srv_register_slot);
	m_UnorderedAccessViews.resize(++uav_register_slot);

	pReflector->Release();
}

void ComputeShader::Update()
{
	// Compute Shader 연결..
	g_DeviceContext->CSSetShader(m_CS.Get(), nullptr, 0);

	// Compute Shader ShaderSampler 설정..
	if (!m_SamplerStates.empty())
		g_DeviceContext->CSSetSamplers(0, (UINT)m_SamplerStates.size(), m_SamplerStates[0].GetAddressOf());

	// Compute Shader ConstantBuffer 설정..
	if (!m_ConstantBuffers.empty())
		g_DeviceContext->CSSetConstantBuffers(0, (UINT)m_ConstantBuffers.size(), m_ConstantBuffers[0].GetAddressOf());

	// Compute Shader ShaderResourceView 설정..
	if (!m_ShaderResourceViews.empty())
		g_DeviceContext->CSSetShaderResources(0, (UINT)m_ShaderResourceViews.size(), m_ShaderResourceViews[0].GetAddressOf());

	// Compute Shader UnorderedAccessView 설정..
	if (!m_UnorderedAccessViews.empty())
		g_DeviceContext->CSSetUnorderedAccessViews(0, (UINT)m_UnorderedAccessViews.size(), m_UnorderedAccessViews[0].GetAddressOf(), 0);
}

void ComputeShader::Update(ID3D11DeviceContext* context)
{
	// Compute Shader 연결..
	context->CSSetShader(m_CS.Get(), nullptr, 0);

	// Compute Shader ShaderSampler 설정..
	if (!m_SamplerStates.empty())
		context->CSSetSamplers(0, (UINT)m_SamplerStates.size(), m_SamplerStates[0].GetAddressOf());

	// Compute Shader ConstantBuffer 설정..
	if (!m_ConstantBuffers.empty())
		context->CSSetConstantBuffers(0, (UINT)m_ConstantBuffers.size(), m_ConstantBuffers[0].GetAddressOf());

	// Compute Shader ShaderResourceView 설정..
	if (!m_ShaderResourceViews.empty())
		context->CSSetShaderResources(0, (UINT)m_ShaderResourceViews.size(), m_ShaderResourceViews[0].GetAddressOf());

	// Compute Shader UnorderedAccessView 설정..
	if (!m_UnorderedAccessViews.empty())
		context->CSSetUnorderedAccessViews(0, (UINT)m_UnorderedAccessViews.size(), m_UnorderedAccessViews[0].GetAddressOf(), 0);
}

void ComputeShader::Release()
{
	ShaderBase::Release();

	RESET_COM(m_CS);

	for (Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> uav : m_UnorderedAccessViews)
	{
		RESET_COM(uav);
	}

	for (std::pair<Hash_Code, UnorderedAccessBuffer*> uav : m_UAVList)
	{
		SAFE_DELETE(uav.second);
	}

	m_UnorderedAccessViews.clear();
	m_UAVList.clear();
}

void ComputeShader::UnBindConstantBuffer(UINT startSlot, UINT numViews)
{
	constexpr ID3D11Buffer* nullbuffer = nullptr;

	g_DeviceContext->CSSetConstantBuffers(startSlot, numViews, &nullbuffer);
}

void ComputeShader::UnBindShaderResourceView(UINT startSlot, UINT numViews)
{
	constexpr ID3D11ShaderResourceView* nullSRV = nullptr;

	g_DeviceContext->CSSetShaderResources(startSlot, numViews, &nullSRV);
}

void ComputeShader::UnBindUnorderedAccessView(UINT startSlot, UINT numViews)
{
	constexpr ID3D11UnorderedAccessView* nullUAV = nullptr;
	constexpr UINT nullOffset = -1;

	g_DeviceContext->CSSetUnorderedAccessViews(startSlot, numViews, &nullUAV, &nullOffset);
}
