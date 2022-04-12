#pragma once
#include <windows.h>
#include <assert.h>
#include "ResourcesData.h"
#include "ParserData.h"
#include "HashBase.h"

class LoadMeshData;
class TextureBuffer;
class EnvironmentBuffer;
class MeshBuffer;

interface IFactoryManager
{
public:
	virtual void Create(int width, int height) abstract;
	virtual void Start() abstract;
	virtual void Release() abstract;

public:
	// Create Output Graphic Resource..
	virtual void CreateTextureBuffer(std::string path, TextureBuffer** ppResource) abstract;
	virtual void CreateMeshBuffer(ParserData::CMesh* mesh, MeshBuffer** ppResource) abstract;

public:
	// Baking Graphic Resource..
	virtual void BakeShadowMap(std::string fileName) abstract;
	virtual void BakeEnvironmentMap(TextureBuffer* environment, EnvironmentBuffer** ppResource) abstract;

public:
	template<typename T, Enable_Check<T> = NULL> void CreateImage(std::string path);

public:
	// Create Graphic Struct Resource..
	template<typename T, Enable_Check<T> = NULL> void CreateDepthStencil(D3D11_TEXTURE2D_DESC* texDesc, D3D11_SUBRESOURCE_DATA* subData, D3D11_DEPTH_STENCIL_VIEW_DESC* dsvDesc, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc);
	template<typename T, Enable_Check<T> = NULL> void CreateRenderTexture(D3D11_TEXTURE2D_DESC* texDesc, D3D11_SUBRESOURCE_DATA* subData, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc);
	template<typename T, Enable_Check<T> = NULL> void CreateRenderBuffer(D3D11_BUFFER_DESC* bufferDesc, D3D11_SUBRESOURCE_DATA* subData, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc);
	
	// Create Graphic Draw Resource..
	template<typename T, Enable_Check<T> = NULL> void CreateDrawBuffer(UINT format, UINT topology, UINT vByteSize, UINT iByteSize, UINT vSize, UINT iCount, const void* vertices, const void* indices);
	template<typename T, Enable_Check<T> = NULL> void CreateInstanceBuffer(UINT byteSize, UINT instCount, const void* instance);

	// Create Graphic View Resource..
	template<typename T, Enable_Check<T> = NULL> void CreateRenderTargetView(D3D11_TEXTURE2D_DESC* texDesc, D3D11_SUBRESOURCE_DATA* subData, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc);
	template<typename T, Enable_Check<T> = NULL> void CreateRenderTargetView(ID3D11Texture2D* tex2D, D3D11_SUBRESOURCE_DATA* subData, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc);
	template<typename T, Enable_Check<T> = NULL> void CreateShaderResourceView(D3D11_TEXTURE2D_DESC* texDesc, D3D11_SUBRESOURCE_DATA* subData, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc);
	template<typename T, Enable_Check<T> = NULL> void CreateShaderResourceView(ID3D11Texture2D* tex2D, D3D11_SUBRESOURCE_DATA* subData, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc);
	template<typename T, Enable_Check<T> = NULL> void CreateUnorderedAccessView(D3D11_TEXTURE2D_DESC* texDesc, D3D11_SUBRESOURCE_DATA* subData, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc);
	template<typename T, Enable_Check<T> = NULL> void CreateUnorderedAccessView(ID3D11Texture2D* tex2D, D3D11_SUBRESOURCE_DATA* subData, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc);
	
	// Create Graphic State Resource..
	template<typename T, Enable_Check<T> = NULL> void CreateDepthStencilState(D3D11_DEPTH_STENCIL_DESC* dssDesc);
	template<typename T, Enable_Check<T> = NULL> void CreateRasterizerState(D3D11_RASTERIZER_DESC* rsDesc);
	template<typename T, Enable_Check<T> = NULL> void CreateBlendState(D3D11_BLEND_DESC* bsDesc);
	template<typename T, Enable_Check<T> = NULL> void CreateSamplerState(D3D11_SAMPLER_DESC* ssDesc);
	template<typename T, Enable_Check<T> = NULL> void CreateViewPort(float ratio_offsetX, float ratio_offsetY, float ratio_sizeX, float ratio_sizeY, float width, float height);
	template<typename T, Enable_Check<T> = NULL> void CreateViewPort(float offsetX, float offsetY, float width, float height);

private:
	virtual void CreateImg(std::string name, Hash_Code hash_code, std::string fileName) abstract;

	virtual void CreateDS(std::string name, Hash_Code hash_code, D3D11_TEXTURE2D_DESC* texDesc, D3D11_SUBRESOURCE_DATA* subData, D3D11_DEPTH_STENCIL_VIEW_DESC* dsvDesc, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc) abstract;
	virtual void CreateRT(std::string name, Hash_Code hash_code, D3D11_TEXTURE2D_DESC* texDesc, D3D11_SUBRESOURCE_DATA* subData, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc) abstract;
	virtual void CreateRB(std::string name, Hash_Code hash_code, D3D11_BUFFER_DESC* bufferDesc, D3D11_SUBRESOURCE_DATA* subData, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc) abstract;

	virtual void CreateDB(std::string name, Hash_Code hash_code, UINT format, UINT topology, UINT vByteSize, UINT iByteSize, UINT vSize, UINT iCount, const void* vertices, const void* indices) abstract;
	virtual void CreateIB(std::string name, Hash_Code hash_code, UINT byteSize, UINT instCount, const void* instance) abstract;

	virtual void CreateDSS(std::string name, Hash_Code hash_code, D3D11_DEPTH_STENCIL_DESC* dssDesc) abstract;
	virtual void CreateRS(std::string name, Hash_Code hash_code, D3D11_RASTERIZER_DESC* rsDesc) abstract;
	virtual void CreateBS(std::string name, Hash_Code hash_code, D3D11_BLEND_DESC* bsDesc) abstract;
	virtual void CreateSS(std::string name, Hash_Code hash_code, D3D11_SAMPLER_DESC* ssDesc) abstract;
	virtual void CreateVP(std::string name, Hash_Code hash_code, float ratio_offsetX, float ratio_offsetY, float ratio_sizeX, float ratio_sizeY, float width, float height) abstract;
	virtual void CreateVP(std::string name, Hash_Code hash_code, float offsetX, float offsetY, float width, float height) abstract;

	virtual void CreateRTV(std::string name, Hash_Code hash_code, D3D11_TEXTURE2D_DESC* texDesc, D3D11_SUBRESOURCE_DATA* subData, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc) abstract;
	virtual void CreateRTV(std::string name, Hash_Code hash_code, ID3D11Texture2D* tex2D, D3D11_SUBRESOURCE_DATA* subData, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc) abstract;
	virtual void CreateSRV(std::string name, Hash_Code hash_code, D3D11_TEXTURE2D_DESC* texDesc, D3D11_SUBRESOURCE_DATA* subData, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc) abstract;
	virtual void CreateSRV(std::string name, Hash_Code hash_code, ID3D11Texture2D* tex2D, D3D11_SUBRESOURCE_DATA* subData, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc) abstract;
	virtual void CreateUAV(std::string name, Hash_Code hash_code, D3D11_TEXTURE2D_DESC* texDesc, D3D11_SUBRESOURCE_DATA* subData, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc) abstract;
	virtual void CreateUAV(std::string name, Hash_Code hash_code, ID3D11Texture2D* tex2D, D3D11_SUBRESOURCE_DATA* subData, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc) abstract;
};


template<typename T, Enable_Check<T>>
inline void IFactoryManager::CreateImage(std::string fileName)
 {
	// Template Struct Resource Type Check..
	assert(T::GetType() == RESOURCE_TYPE::SRV);

	CreateImg(T::GetName(), T::GetHashCode(), fileName);
}

template<typename T, Enable_Check<T>>
inline void IFactoryManager::CreateDepthStencil(D3D11_TEXTURE2D_DESC* texDesc, D3D11_SUBRESOURCE_DATA* subData, D3D11_DEPTH_STENCIL_VIEW_DESC* dsvDesc, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc)
{
	// Template Struct Resource Type Check..
	assert(T::GetType() == RESOURCE_TYPE::DS);

	CreateDS(T::GetName(), T::GetHashCode(), texDesc, subData, dsvDesc, srvDesc);
}

template<typename T, Enable_Check<T>>
inline void IFactoryManager::CreateRenderTexture(D3D11_TEXTURE2D_DESC* texDesc, D3D11_SUBRESOURCE_DATA* subData, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc)
{
	// Template Struct Resource Type Check..
	assert(T::GetType() == RESOURCE_TYPE::RT);

	CreateRT(T::GetName(), T::GetHashCode(), texDesc, subData, rtvDesc, srvDesc, uavDesc);
}

template<typename T, Enable_Check<T>>
inline void IFactoryManager::CreateRenderBuffer(D3D11_BUFFER_DESC* bufferDesc, D3D11_SUBRESOURCE_DATA* subData, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc /*= nullptr*/, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc /*= nullptr*/)
{
	// Template Struct Resource Type Check..
	assert(T::GetType() == RESOURCE_TYPE::RB);

	CreateRB(T::GetName(), T::GetHashCode(), bufferDesc, subData, rtvDesc, srvDesc, uavDesc);
}

template<typename T, Enable_Check<T>>
void IFactoryManager::CreateDrawBuffer(UINT format, UINT topology, UINT vByteSize, UINT iByteSize, UINT vSize, UINT iCount, const void* vertices, const void* indices)
{
	// Template Struct Resource Type Check..
	assert(T::GetType() == RESOURCE_TYPE::DB);

	CreateDB(T::GetName(), T::GetHashCode(), format, topology, vByteSize, iByteSize, vSize, iCount, vertices, indices);
}

template<typename T, Enable_Check<T>>
void IFactoryManager::CreateInstanceBuffer(UINT byteSize, UINT instCount, const void* instance)
{
	// Template Struct Resource Type Check..
	assert(T::GetType() == RESOURCE_TYPE::IB);

	CreateIB(T::GetName(), T::GetHashCode(), byteSize, instCount, instance);
}

template<typename T, Enable_Check<T>>
inline void IFactoryManager::CreateRenderTargetView(D3D11_TEXTURE2D_DESC* texDesc, D3D11_SUBRESOURCE_DATA* subData, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc)
{
	// Template Struct Resource Type Check..
	assert(T::GetType() == RESOURCE_TYPE::RTV);

	CreateRTV(T::GetName(), T::GetHashCode(), texDesc, subData, rtvDesc);
}

template<typename T, Enable_Check<T>>
inline void IFactoryManager::CreateRenderTargetView(ID3D11Texture2D* tex2D, D3D11_SUBRESOURCE_DATA* subData, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc /*= nullptr*/)
{
	// Template Struct Resource Type Check..
	assert(T::GetType() == RESOURCE_TYPE::RTV);

	CreateRTV(T::GetName(), T::GetHashCode(), tex2D, subData, rtvDesc);
}

template<typename T, Enable_Check<T>>
inline void IFactoryManager::CreateShaderResourceView(D3D11_TEXTURE2D_DESC* texDesc, D3D11_SUBRESOURCE_DATA* subData, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc)
{
	// Template Struct Resource Type Check..
	assert(T::GetType() == RESOURCE_TYPE::SRV);

	CreateSRV(T::GetName(), T::GetHashCode(), texDesc, subData, srvDesc);
}

template<typename T, Enable_Check<T>>
inline void IFactoryManager::CreateShaderResourceView(ID3D11Texture2D* tex2D, D3D11_SUBRESOURCE_DATA* subData, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc /*= nullptr*/)
{
	// Template Struct Resource Type Check..
	assert(T::GetType() == RESOURCE_TYPE::SRV);

	CreateSRV(T::GetName(), T::GetHashCode(), tex2D, subData, srvDesc);
}

template<typename T, Enable_Check<T>>
inline void IFactoryManager::CreateUnorderedAccessView(D3D11_TEXTURE2D_DESC* texDesc, D3D11_SUBRESOURCE_DATA* subData, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc)
{
	// Template Struct Resource Type Check..
	assert(T::GetType() == RESOURCE_TYPE::UAV);

	CreateUAV(T::GetName(), T::GetHashCode(), texDesc, subData, uavDesc);
}

template<typename T, Enable_Check<T>>
inline void IFactoryManager::CreateUnorderedAccessView(ID3D11Texture2D* tex2D, D3D11_SUBRESOURCE_DATA* subData, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc /*= nullptr*/)
{
	// Template Struct Resource Type Check..
	assert(T::GetType() == RESOURCE_TYPE::UAV);

	CreateUAV(T::GetName(), T::GetHashCode(), tex2D, subData, uavDesc);
}

template<typename T, Enable_Check<T>>
inline void IFactoryManager::CreateDepthStencilState(D3D11_DEPTH_STENCIL_DESC* dssDesc)
{
	// Template Struct Resource Type Check..
	assert(T::GetType() == RESOURCE_TYPE::DSS);

	CreateDSS(T::GetName(), T::GetHashCode(), dssDesc);
}

template<typename T, Enable_Check<T>>
inline void IFactoryManager::CreateRasterizerState(D3D11_RASTERIZER_DESC* rsDesc)
{
	// Template Struct Resource Type Check..
	assert(T::GetType() == RESOURCE_TYPE::RS);

	CreateRS(T::GetName(), T::GetHashCode(), rsDesc);
}

template<typename T, Enable_Check<T>>
inline void IFactoryManager::CreateBlendState(D3D11_BLEND_DESC* bsDesc)
{
	// Template Struct Resource Type Check..
	assert(T::GetType() == RESOURCE_TYPE::BS);

	CreateBS(T::GetName(), T::GetHashCode(), bsDesc);
}

template<typename T, Enable_Check<T>>
inline void IFactoryManager::CreateSamplerState(D3D11_SAMPLER_DESC* ssDesc)
{
	// Template Struct Resource Type Check..
	assert(T::GetType() == RESOURCE_TYPE::SS);

	CreateSS(T::GetName(), T::GetHashCode(), ssDesc);
}

template<typename T, Enable_Check<T>>
inline void IFactoryManager::CreateViewPort(float ratio_offsetX, float ratio_offsetY, float ratio_sizeX, float ratio_sizeY, float width, float height)
{
	// Template Struct Resource Type Check..
	assert(T::GetType() == RESOURCE_TYPE::VP);

	CreateVP(T::GetName(), T::GetHashCode(), ratio_offsetX, ratio_offsetY, ratio_sizeX, ratio_sizeY, width, height);
}

template<typename T, Enable_Check<T>>
void IFactoryManager::CreateViewPort(float offsetX , float offsetY, float width, float height)
{
	// Template Struct Resource Type Check..
	assert(T::GetType() == RESOURCE_TYPE::VP);

	CreateVP(T::GetName(), T::GetHashCode(), offsetX, offsetY, width, height);
}
