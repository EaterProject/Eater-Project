#include "DirectDefine.h"
#include "RenderPassBase.h"
#include "FactoryManagerBase.h"
#include "ResourceManagerBase.h"
#include "ShaderManagerBase.h"
#include "EngineData.h"

Microsoft::WRL::ComPtr<ID3D11DeviceContext> RenderPassBase::g_Context = nullptr;
IFactoryManager* RenderPassBase::g_Factory = nullptr;
IGraphicResourceManager* RenderPassBase::g_Resource = nullptr;
IShaderManager* RenderPassBase::g_Shader = nullptr;
GlobalData* RenderPassBase::g_GlobalData = nullptr;

void RenderPassBase::Initialize(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, IFactoryManager* factory, IGraphicResourceManager* resourceManager, IShaderManager* shaderManager)
{
	g_Context = context;
	g_Factory = factory;
	g_Resource = resourceManager;
	g_Shader = shaderManager;
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
