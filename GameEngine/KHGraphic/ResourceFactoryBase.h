#pragma once
#include <string>
#include "FactoryBase.h"
#include "ParserData.h"

class ModelData;
class ModelAnimationData;

class TextureBuffer;
class AnimationBuffer;
class EnvironmentBuffer;
class MeshBuffer;

///
/// 2021/11/07 22:59
/// SeoKyuHwang
///
/// # GraphicResourceFactory Interface Class
///
/// - GraphicResource를 생성해주는 Factory Class
/// - 내부적으로 Resource Manager에서 Resource 관리
/// 
interface IGraphicResourceFactory : public FactoryBase
{
public:
	friend class FactoryManager;

public:
	// Create Output Graphic Resource..
	virtual void CreateTextureBuffer(std::string path, TextureBuffer** ppResource) abstract;
	virtual void CreateMeshBuffer(ParserData::CMesh* mesh, MeshBuffer** ppResource) abstract;
	virtual void CreateAnimationBuffer(ModelData* model, ModelAnimationData* animation, AnimationBuffer** ppResource) abstract;

private:
	// Create Graphic Engine Resource..
	virtual void CreateImage(std::string name, Hash_Code hash_code, std::string fileName) abstract;

private:
	// Create Graphic Struct Resource..
	virtual void CreateDepthStencil(std::string name, Hash_Code hash_code, D3D11_TEXTURE2D_DESC* texDesc, D3D11_SUBRESOURCE_DATA* subData, D3D11_DEPTH_STENCIL_VIEW_DESC* dsvDesc, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc) abstract;
	virtual void CreateRenderTexture(std::string name, Hash_Code hash_code, D3D11_TEXTURE2D_DESC* texDesc, D3D11_SUBRESOURCE_DATA* subData, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc) abstract;
	virtual void CreateRenderBuffer(std::string name, Hash_Code hash_code, D3D11_BUFFER_DESC* bufferDesc, D3D11_SUBRESOURCE_DATA* subData, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc) abstract;
	
private:
	// Create Graphic Draw Resource..
	virtual void CreateDrawBuffer(std::string name, Hash_Code hash_code, UINT format, UINT topology, UINT vByteSize, UINT iByteSize, UINT vSize, UINT iCount, const void* vertices, const void* indices) abstract;
	virtual void CreateDrawBuffer(std::string name, Hash_Code hash_code, UINT format, UINT topology, UINT vByteSize, UINT iByteSize, UINT vSize, UINT iCount, UINT vUsage, UINT iUsage, const void* vertices, const void* indices) abstract;
	virtual void CreateInstanceBuffer(std::string name, Hash_Code hash_code, UINT byteSize, UINT instCount, const void* instance) abstract;

private:
	// Create Graphic View Resource..
	virtual void CreateRenderTargetView(std::string name, Hash_Code hash_code, D3D11_TEXTURE2D_DESC* texDesc, D3D11_SUBRESOURCE_DATA* subData, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc) abstract;
	virtual void CreateRenderTargetView(std::string name, Hash_Code hash_code, ID3D11Texture2D* tex2D, D3D11_SUBRESOURCE_DATA* subData, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc) abstract;
	virtual void CreateShaderResourceView(std::string name, Hash_Code hash_code, D3D11_TEXTURE2D_DESC* texDesc, D3D11_SUBRESOURCE_DATA* subData, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc) abstract;
	virtual void CreateShaderResourceView(std::string name, Hash_Code hash_code, ID3D11Texture2D* tex2D, D3D11_SUBRESOURCE_DATA* subData, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc) abstract;
	virtual void CreateUnorderedAccessView(std::string name, Hash_Code hash_code, D3D11_TEXTURE2D_DESC* texDesc, D3D11_SUBRESOURCE_DATA* subData, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc) abstract;
	virtual void CreateUnorderedAccessView(std::string name, Hash_Code hash_code, ID3D11Texture2D* tex2D, D3D11_SUBRESOURCE_DATA* subData, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc) abstract;

private:
	// Create Graphic State Resource..
	virtual void CreateDepthStencilState(std::string name, Hash_Code hash_code, D3D11_DEPTH_STENCIL_DESC* dssDesc) abstract;
	virtual void CreateRasterizerState(std::string name, Hash_Code hash_code, D3D11_RASTERIZER_DESC* rsDesc) abstract;
	virtual void CreateBlendState(std::string name, Hash_Code hash_code, D3D11_BLEND_DESC* bsDesc) abstract;
	virtual void CreateSamplerState(std::string name, Hash_Code hash_code, D3D11_SAMPLER_DESC* ssDesc) abstract;
	virtual void CreateViewPort(std::string name, Hash_Code hash_code, float ratio_offsetX, float ratio_offsetY, float ratio_sizeX, float ratio_sizeY, float width, float height) abstract;
	virtual void CreateViewPort(std::string name, Hash_Code hash_code, float offsetX, float offsetY, float width, float height) abstract;
};