#include "DirectDefine.h"
#include "ResourceFactory.h"
#include "BakingFactory.h"
#include "FactoryManager.h"
#include "DepthStencilViewDefine.h"

FactoryManager::FactoryManager(ID3D11Graphic* graphic, IShaderManager* shader, IGraphicResourceManager* resource)
{
	FactoryBase::Initialize(graphic, resource, shader);

	m_ResourceFactory = new GraphicResourceFactory();
	m_BakingFactory = new BakingFactory();
}

FactoryManager::~FactoryManager()
{

}

void FactoryManager::Create(int width, int height)
{
	m_ResourceFactory->Create(width, height);
	m_BakingFactory->Create(width, height);
}

void FactoryManager::Start()
{
	m_ResourceFactory->Start();
	m_BakingFactory->Start();
}

void FactoryManager::Release()
{
	SAFE_RELEASE(m_ResourceFactory);
	SAFE_RELEASE(m_BakingFactory);
}

void FactoryManager::CreateTextureBuffer(std::string path, TextureBuffer** ppResource)
{
	m_ResourceFactory->CreateTextureBuffer(path, ppResource);
}

void FactoryManager::CreateMeshBuffer(ParserData::CMesh* mesh, MeshBuffer** ppResource)
{
	m_ResourceFactory->CreateMeshBuffer(mesh, ppResource);
}

void FactoryManager::BakeShadowMap(std::string fileName)
{
	m_BakingFactory->PreBakeShadowMap(fileName);
}

void FactoryManager::BakeEnvironmentMap(TextureBuffer* environment, EnvironmentBuffer** ppResource)
{
	// 货肺款 Environment Buffer 积己..
	(*ppResource) = new EnvironmentBuffer();
	(*ppResource)->Environment = environment;

	m_BakingFactory->PreBakeEnvironmentMap(*ppResource);
}

void FactoryManager::CreateImg(std::string name, Hash_Code hash_code, std::string fileName)
{
	m_ResourceFactory->CreateImage(name, hash_code, fileName);
}

void FactoryManager::CreateDS(std::string name, Hash_Code hash_code, D3D11_TEXTURE2D_DESC* texDesc, D3D11_SUBRESOURCE_DATA* subData, D3D11_DEPTH_STENCIL_VIEW_DESC* dsvDesc, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc)
{
	m_ResourceFactory->CreateDepthStencil(name, hash_code, texDesc, subData, dsvDesc, srvDesc);
}

void FactoryManager::CreateRT(std::string name, Hash_Code hash_code, D3D11_TEXTURE2D_DESC* texDesc, D3D11_SUBRESOURCE_DATA* subData, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc)
{
	m_ResourceFactory->CreateRenderTexture(name, hash_code, texDesc, subData, rtvDesc, srvDesc, uavDesc);
}

void FactoryManager::CreateRB(std::string name, Hash_Code hash_code, D3D11_BUFFER_DESC* bufferDesc, D3D11_SUBRESOURCE_DATA* subData, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc)
{
	m_ResourceFactory->CreateRenderBuffer(name, hash_code, bufferDesc, subData, rtvDesc, srvDesc, uavDesc);
}

void FactoryManager::CreateDB(std::string name, Hash_Code hash_code, UINT format, UINT topology, UINT vByteSize, UINT iByteSize, UINT vSize, UINT iCount, const void* vertices, const void* indices)
{
	m_ResourceFactory->CreateDrawBuffer(name, hash_code, format, topology, vByteSize, iByteSize, vSize, iCount, vertices, indices);
}

void FactoryManager::CreateIB(std::string name, Hash_Code hash_code, UINT byteSize, UINT instCount, const void* instance)
{
	m_ResourceFactory->CreateInstanceBuffer(name, hash_code, byteSize, instCount, instance);
}

void FactoryManager::CreateRTV(std::string name, Hash_Code hash_code, D3D11_TEXTURE2D_DESC* texDesc, D3D11_SUBRESOURCE_DATA* subData, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc)
{
	m_ResourceFactory->CreateRenderTargetView(name, hash_code, texDesc, subData, rtvDesc);
}

void FactoryManager::CreateRTV(std::string name, Hash_Code hash_code, ID3D11Texture2D* tex2D, D3D11_SUBRESOURCE_DATA* subData, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc)
{
	m_ResourceFactory->CreateRenderTargetView(name, hash_code, tex2D, subData, rtvDesc);
}

void FactoryManager::CreateSRV(std::string name, Hash_Code hash_code, D3D11_TEXTURE2D_DESC* texDesc, D3D11_SUBRESOURCE_DATA* subData, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc)
{
	m_ResourceFactory->CreateShaderResourceView(name, hash_code, texDesc, subData, srvDesc);
}

void FactoryManager::CreateSRV(std::string name, Hash_Code hash_code, ID3D11Texture2D* tex2D, D3D11_SUBRESOURCE_DATA* subData, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc)
{
	m_ResourceFactory->CreateShaderResourceView(name, hash_code, tex2D, subData, srvDesc);
}

void FactoryManager::CreateUAV(std::string name, Hash_Code hash_code, D3D11_TEXTURE2D_DESC* texDesc, D3D11_SUBRESOURCE_DATA* subData, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc)
{
	m_ResourceFactory->CreateUnorderedAccessView(name, hash_code, texDesc, subData, uavDesc);
}

void FactoryManager::CreateUAV(std::string name, Hash_Code hash_code, ID3D11Texture2D* tex2D, D3D11_SUBRESOURCE_DATA* subData, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc)
{
	m_ResourceFactory->CreateUnorderedAccessView(name, hash_code, tex2D, subData, uavDesc);
}

void FactoryManager::CreateDSS(std::string name, Hash_Code hash_code, D3D11_DEPTH_STENCIL_DESC* dssDesc)
{
	m_ResourceFactory->CreateDepthStencilState(name, hash_code, dssDesc);
}

void FactoryManager::CreateRS(std::string name, Hash_Code hash_code, D3D11_RASTERIZER_DESC* rsDesc)
{
	m_ResourceFactory->CreateRasterizerState(name, hash_code, rsDesc);
}

void FactoryManager::CreateBS(std::string name, Hash_Code hash_code, D3D11_BLEND_DESC* bsDesc)
{
	m_ResourceFactory->CreateBlendState(name, hash_code, bsDesc);
}

void FactoryManager::CreateSS(std::string name, Hash_Code hash_code, D3D11_SAMPLER_DESC* ssDesc)
{
	m_ResourceFactory->CreateSamplerState(name, hash_code, ssDesc);
}

void FactoryManager::CreateVP(std::string name, Hash_Code hash_code, float ratio_offsetX, float ratio_offsetY, float ratio_sizeX, float ratio_sizeY, float width, float height)
{
	m_ResourceFactory->CreateViewPort(name, hash_code, ratio_offsetX, ratio_offsetY, ratio_sizeX, ratio_sizeY, width, height);
}

void FactoryManager::CreateVP(std::string name, Hash_Code hash_code, float offsetX, float offsetY, float width, float height)
{
	m_ResourceFactory->CreateViewPort(name, hash_code, offsetX, offsetY, width, height);
}
