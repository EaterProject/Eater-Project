#include "DirectDefine.h"
#include "FactoryBase.h"
#include "D3D11GraphicBase.h"
#include "ResourceManagerBase.h"
#include "ShaderManagerBase.h"

ID3D11Graphic* FactoryBase::g_Graphic = nullptr;
IShaderManager* FactoryBase::g_ShaderManager = nullptr;
IGraphicResourceManager* FactoryBase::g_ResourceManager = nullptr;

void FactoryBase::Initialize(ID3D11Graphic* graphic, IGraphicResourceManager* resource, IShaderManager* shader)
{
	g_Graphic = graphic;
	g_ShaderManager = shader;
	g_ResourceManager = resource;
}

void FactoryBase::Reset()
{
	g_Graphic = nullptr;
	g_ShaderManager = nullptr;
	g_ResourceManager = nullptr;
}
