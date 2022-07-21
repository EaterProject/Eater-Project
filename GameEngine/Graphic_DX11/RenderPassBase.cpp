#include "DirectDefine.h"
#include "RenderPassBase.h"
#include "FactoryManagerBase.h"
#include "ResourceManagerBase.h"
#include "ShaderManagerBase.h"
#include "EngineData.h"
#include "RenderData.h"
#include "ShaderBase.h"
#include "GraphicResource.h"
#include "RenderTarget.h"

Microsoft::WRL::ComPtr<ID3D11Device> RenderPassBase::g_Device = nullptr;
Microsoft::WRL::ComPtr<ID3D11DeviceContext> RenderPassBase::g_Context = nullptr;
RenderTexture* RenderPassBase::g_BindingRenderTarget = nullptr;
IFactoryManager* RenderPassBase::g_Factory = nullptr;
IGraphicResourceManager* RenderPassBase::g_Resource = nullptr;
IShaderManager* RenderPassBase::g_Shader = nullptr;
GlobalData* RenderPassBase::g_GlobalData = nullptr;
RenderOption* RenderPassBase::g_RenderOption = nullptr;
RenderData* RenderPassBase::g_Picking = nullptr;

void RenderPassBase::PushShader(const char* shaderName)
{
	ShaderBase* shader = g_Shader->GetShader(shaderName);

	if (shader == nullptr) return;

	m_OptionShaderList.push_back(shader);
}

void RenderPassBase::Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, IFactoryManager* factory, IGraphicResourceManager* resourceManager, IShaderManager* shaderManager, RenderOption* renderOption)
{
	g_Device = device;
	g_Context = context;
	g_Factory = factory;
	g_Resource = resourceManager;
	g_Shader = shaderManager;

	g_RenderOption = renderOption;

	ConstantBufferManager::g_RenderOption = renderOption;
}

void RenderPassBase::SetGlobalData(GlobalData* globalData)
{
	g_GlobalData = globalData;

	ConstantBufferManager::g_GlobalData = globalData;
}

void RenderPassBase::GraphicReset()
{
	g_Context->ClearState();
}

void RenderPassBase::Reset()
{
	RESET_COM(g_Context);
	g_Factory = nullptr;
	g_Resource = nullptr;
	g_Shader = nullptr;
	g_GlobalData = nullptr;
}

void RenderPassBase::UpdateBuffer(ID3D11Buffer* buffer, void* bufferData, size_t size)
{
	if (buffer == nullptr) { return;}

	// Mapping SubResource Data..
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	// GPU Access Lock Buffer Data..
	g_Context->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	// Copy Resource Data..
	memcpy(mappedResource.pData, bufferData, size);

	// GPU Access UnLock Buffer Data..
	g_Context->Unmap(buffer, 0);
}
