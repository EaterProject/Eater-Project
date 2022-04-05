#include "DirectDefine.h"
#include "ShaderBase.h"
#include "VertexShader.h"

#include "ShaderResourceHashTable.h"
#include "CompilerDefine.h"
#include <sstream>
#include <fstream>
#include "ConstantBufferDefine.h"

VertexShader::VertexShader(const char* shaderName, const char* fileName, const char* entry_point, const char* shader_model, const D3D_SHADER_MACRO* pDefines)
	:ShaderBase(SHADER_TYPE::VERTEX_SHADER, shaderName)
{
	LoadShader(g_ShaderRoute + fileName, entry_point, shader_model, pDefines);
}

VertexShader::~VertexShader()
{

}

void VertexShader::LoadShader(std::string fileName, const char* entry_point, const char* shader_model, const D3D_SHADER_MACRO* pDefines)
{
	ID3DBlob* shaderBlob = nullptr;
	ID3D11ShaderReflection* pReflector = nullptr;

	ShaderResourceHashTable* resource_table = ShaderResourceHashTable::Get();

	size_t cbuffer_register_slot = 0;	// ConstantBuffer Max Register Slot
	size_t sampler_register_slot = 0;	// Sampler Max Register Slot
	size_t srv_register_slot = 0;		// ShaderResourceView Max Register Slot
	size_t hash_key = 0;				// Resource Hash Code
	
	std::wstring wPath(fileName.begin(), fileName.end());

	// Vertex HLSL Load..
	CreateShader(wPath.c_str(), pDefines, entry_point, shader_model, &shaderBlob);

	// Create Vertex Shader..
	HR(g_Device->CreateVertexShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, &m_VS));

	// Debug Name..
	GPU_RESOURCE_DEBUG_NAME(m_VS.Get(), entry_point);

	// Create Reflector..
	D3DReflect(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pReflector);

	D3D11_SHADER_DESC shaderDesc;
	pReflector->GetDesc(&shaderDesc);

	/// Input Layout Reflection
	// Shader Input Layout..
	std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;
	for (unsigned inputIndex = 0; inputIndex < shaderDesc.InputParameters; inputIndex++)
	{
		D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
		pReflector->GetInputParameterDesc(inputIndex, &paramDesc);
		
		// Instancing Input Layout 체크..
		D3D11_INPUT_CLASSIFICATION inputSlotClass;
		UINT inputSlot;
		UINT instanceDataStepRate;
		std::string semanticName = paramDesc.SemanticName;

		if (semanticName.find("INSTANCE_") == 0)
		{
			inputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
			inputSlot = 1;
			instanceDataStepRate = 1;
		}
		else
		{
			inputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			inputSlot = 0;
			instanceDataStepRate = 0;
		}

		// Shader Input Data를 기반으로 생성..
		D3D11_INPUT_ELEMENT_DESC elementDesc;
		elementDesc.SemanticName = paramDesc.SemanticName;
		elementDesc.SemanticIndex = paramDesc.SemanticIndex;
		elementDesc.InputSlot = inputSlot;
		elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		elementDesc.InputSlotClass = inputSlotClass;
		elementDesc.InstanceDataStepRate = instanceDataStepRate;

		// Shader Data 기반으로 DXGI format 설정..
		if (paramDesc.Mask == 1)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
		}
		else if (paramDesc.Mask <= 3)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
		}
		else if (paramDesc.Mask <= 7)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		}
		else if (paramDesc.Mask <= 15)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		}
		
		// 현 InputLayout 데이터 삽입..
		inputLayoutDesc.push_back(elementDesc);
	}
	
	// Shader InputLayout 생성..
	HR(g_Device->CreateInputLayout(&inputLayoutDesc[0], (UINT)inputLayoutDesc.size(), shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), &m_InputLayout));
	
	// Debug Name..
	GPU_RESOURCE_DEBUG_NAME(m_InputLayout.Get(), (std::string(entry_point) + "_InputLayout").c_str());

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

void VertexShader::Update()
{
	// Vertex Shader 설정..
	g_DeviceContext->VSSetShader(m_VS.Get(), nullptr, 0);

	// Vertex Shader SamplerState 설정..
	if (!m_SamplerStates.empty())
		g_DeviceContext->VSSetSamplers(0, (UINT)m_SamplerStates.size(), m_SamplerStates[0].GetAddressOf());

	// Vertex Shader ConstantBuffer 설정..
	if (!m_ConstantBuffers.empty())
		g_DeviceContext->VSSetConstantBuffers(0, (UINT)m_ConstantBuffers.size(), m_ConstantBuffers[0].GetAddressOf());
	
	// Vertex Shader ShaderResourceView 설정..
	if (!m_ShaderResourceViews.empty())
		g_DeviceContext->VSSetShaderResources(0, (UINT)m_ShaderResourceViews.size(), m_ShaderResourceViews[0].GetAddressOf());

	// Shader InputLayout 설정.. 
	g_DeviceContext->IASetInputLayout(m_InputLayout.Get());
}

void VertexShader::Update(ID3D11DeviceContext* context)
{
	// Vertex Shader 설정..
	context->VSSetShader(m_VS.Get(), nullptr, 0);

	// Vertex Shader SamplerState 설정..
	if (!m_SamplerStates.empty())
		context->VSSetSamplers(0, (UINT)m_SamplerStates.size(), m_SamplerStates[0].GetAddressOf());

	// Vertex Shader ConstantBuffer 설정..
	if (!m_ConstantBuffers.empty())
		context->VSSetConstantBuffers(0, (UINT)m_ConstantBuffers.size(), m_ConstantBuffers[0].GetAddressOf());

	// Vertex Shader ShaderResourceView 설정..
	if (!m_ShaderResourceViews.empty())
		context->VSSetShaderResources(0, (UINT)m_ShaderResourceViews.size(), m_ShaderResourceViews[0].GetAddressOf());

	// Shader InputLayout 설정.. 
	context->IASetInputLayout(m_InputLayout.Get());
}

void VertexShader::Release()
{
	ShaderBase::Release();

	RESET_COM(m_VS);
	RESET_COM(m_InputLayout);
}

void VertexShader::UnBindConstantBuffer(UINT startSlot, UINT numViews)
{
	constexpr ID3D11Buffer* nullbuffer = nullptr;

	g_DeviceContext->VSSetConstantBuffers(startSlot, numViews, &nullbuffer);
}

void VertexShader::UnBindShaderResourceView(UINT startSlot, UINT numViews)
{
	constexpr ID3D11ShaderResourceView* nullSRV = nullptr;

	g_DeviceContext->VSSetShaderResources(startSlot, numViews, &nullSRV);
}
