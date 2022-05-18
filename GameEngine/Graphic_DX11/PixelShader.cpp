#include "DirectDefine.h"
#include "ShaderBase.h"
#include "PixelShader.h"

#include "ShaderResourceHashTable.h"
#include "CompilerDefine.h"
#include <sstream>
#include <fstream>
#include "ConstantBufferDefine.h"

PixelShader::PixelShader(const char* shaderName, const char* fileName, const char* entry_point, const char* shader_model, const D3D_SHADER_MACRO* pDefines)
	:ShaderBase(SHADER_TYPE::PIXEL_SHADER, shaderName)
{
	LoadShader(g_ShaderRoute + fileName, entry_point, shader_model, pDefines);
}

PixelShader::~PixelShader()
{

}

void PixelShader::LoadShader(std::string fileName, const char* entry_point, const char* shader_model, const D3D_SHADER_MACRO* pDefines)
{
	ID3DBlob* shaderBlob = nullptr;
	ID3D11ShaderReflection* pReflector = nullptr;
	ShaderResourceHashTable* resource_table = ShaderResourceHashTable::Get();
	
	int cbuffer_register_slot = -1;	// ConstantBuffer Max Register Slot
	int sampler_register_slot = -1;	// Sampler Max Register SlotD
	int srv_register_slot = -1;		// ShaderResourceView Max Register Slot
	size_t hash_key = 0;			// Resource Hash Code

	std::wstring wPath(fileName.begin(), fileName.end());

	// Pixel HLSL Load..
	CreateShader(wPath.c_str(), pDefines, entry_point, shader_model, &shaderBlob);

	// Create Pixel Shader..
	HR(g_Device->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, &m_PS));

	// Debug Name..
	GPU_RESOURCE_DEBUG_NAME(m_PS.Get(), entry_point);

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
		case D3D_SIT_BYTEADDRESS:
		{
			if (bindDesc.Dimension == D3D_SRV_DIMENSION_BUFFER)
			{
				// SRV Hash Code..
				hash_key = resource_table->FindHashCode(RESOURCE_TYPE::SRV, bindDesc.Name);

				// SRV Register Slot Number..
				srv_register_slot = bindDesc.BindPoint;

				// SRV 추가..
				m_SRVList.insert(std::make_pair(hash_key, new ShaderResourceBuffer(bindDesc.Name, srv_register_slot)));
			}
		}
			break;
		case D3D_SIT_STRUCTURED:
		{
			if (bindDesc.Dimension == D3D_SRV_DIMENSION_BUFFER)
			{
				// SRV Hash Code..
				hash_key = resource_table->FindHashCode(RESOURCE_TYPE::SRV, bindDesc.Name);

				// SRV Register Slot Number..
				srv_register_slot = bindDesc.BindPoint;

				// SRV 추가..
				m_SRVList.insert(std::make_pair(hash_key, new ShaderResourceBuffer(bindDesc.Name, srv_register_slot)));
			}
		}
			break;
		case D3D_SIT_UAV_RWSTRUCTURED_WITH_COUNTER:
		{
		}
			break;
		case D3D_SIT_UAV_RWBYTEADDRESS:
		{

		}
			break;
		case D3D_SIT_UAV_RWSTRUCTURED:
			break;
		default:
			break;
		}
	}

	// 마지막으로 Binding 된 Resource Register Index 기준으로 사이즈 설정..
	m_ConstantBuffers.resize(++cbuffer_register_slot);
	m_SamplerStates.resize(++sampler_register_slot);
	m_ShaderResourceViews.resize(++srv_register_slot);

	pReflector->Release();
}

void PixelShader::Update()
{
	// Pixel Shader 연결..
	g_DeviceContext->PSSetShader(m_PS.Get(), nullptr, 0);

	// Pixel Shader SamplerState 설정..
	if (!m_SamplerStates.empty())
		g_DeviceContext->PSSetSamplers(0, (UINT)m_SamplerStates.size(), &m_SamplerStates[0]);

	// Pixel Shader ConstantBuffer 설정..
	if (!m_ConstantBuffers.empty())
		g_DeviceContext->PSSetConstantBuffers(0, (UINT)m_ConstantBuffers.size(), &m_ConstantBuffers[0]);

	// Pixel Shader ShaderResourceView 설정..
	if (!m_ShaderResourceViews.empty())
		g_DeviceContext->PSSetShaderResources(0, (UINT)m_ShaderResourceViews.size(), &m_ShaderResourceViews[0]);
}

void PixelShader::Update(ID3D11DeviceContext* context)
{
	// Pixel Shader 연결..
	context->PSSetShader(m_PS.Get(), nullptr, 0);

	// Pixel Shader SamplerState 설정..
	if (!m_SamplerStates.empty())
		context->PSSetSamplers(0, (UINT)m_SamplerStates.size(), &m_SamplerStates[0]);

	// Pixel Shader ConstantBuffer 설정..
	if (!m_ConstantBuffers.empty())
		context->PSSetConstantBuffers(0, (UINT)m_ConstantBuffers.size(), &m_ConstantBuffers[0]);

	// Pixel Shader ShaderResourceView 설정..
	if (!m_ShaderResourceViews.empty())
		context->PSSetShaderResources(0, (UINT)m_ShaderResourceViews.size(), &m_ShaderResourceViews[0]);
}

void PixelShader::Release()
{
	ShaderBase::Release();

	RESET_COM(m_PS);
}

void PixelShader::UnBindConstantBuffer(UINT startSlot, UINT numViews)
{
	constexpr ID3D11Buffer* nullbuffer = nullptr;

	g_DeviceContext->PSSetConstantBuffers(startSlot, numViews, &nullbuffer);
}

void PixelShader::UnBindShaderResourceView(UINT startSlot, UINT numViews)
{
	constexpr ID3D11ShaderResourceView* nullSRV = nullptr;

	g_DeviceContext->PSSetShaderResources(startSlot, numViews, &nullSRV);
}
