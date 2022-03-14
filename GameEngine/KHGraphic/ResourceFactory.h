#pragma once
#include "ResourceFactoryBase.h"
#include "HashBase.h"

class ImageParser;

class GraphicResourceFactory : public IGraphicResourceFactory
{
public:
	GraphicResourceFactory();
	~GraphicResourceFactory();

public:
	void Create(int width, int height) override;
	void Start() override;
	void Release() override;

public:
	friend class FactoryManager;

public:
	TextureBuffer* CreateTextureBuffer(std::string path) override;
	void CreateMeshBuffer(ParserData::Mesh* mesh, LoadMeshData* meshData) override;
	void CreateEnvironmentMap(std::string path) override;

private:
	void CreateImage(std::string name, Hash_Code hash_code, std::string fileName) override;

private:
	void CreateDepthStencil(std::string name, Hash_Code hash_code, D3D11_TEXTURE2D_DESC* texDesc, D3D11_SUBRESOURCE_DATA* subData, D3D11_DEPTH_STENCIL_VIEW_DESC* dsvDesc, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc) override;
	void CreateRenderTexture(std::string name, Hash_Code hash_code, D3D11_TEXTURE2D_DESC* texDesc, D3D11_SUBRESOURCE_DATA* subData, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc) override;
	void CreateRenderBuffer(std::string name, Hash_Code hash_code, D3D11_BUFFER_DESC* bufferDesc, D3D11_SUBRESOURCE_DATA* subData, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc) override;
	void CreateDrawBuffer(std::string name, Hash_Code hash_code, UINT format, UINT topology, UINT vByteSize, UINT iByteSize, UINT vSize, UINT iCount, const void* vertices, const void* indices) override;
	void CreateDrawBuffer(std::string name, Hash_Code hash_code, UINT format, UINT topology, UINT vByteSize, UINT iByteSize, UINT vSize, UINT iCount, UINT vUsage, UINT iUsage, const void* vertices, const void* indices) override;
	void CreateInstanceBuffer(std::string name, Hash_Code hash_code, UINT byteSize, UINT instCount, const void* instance) override;

private:
	void CreateRenderTargetView(std::string name, Hash_Code hash_code, D3D11_TEXTURE2D_DESC* texDesc, D3D11_SUBRESOURCE_DATA* subData, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc) override;
	void CreateRenderTargetView(std::string name, Hash_Code hash_code, ID3D11Texture2D* tex2D, D3D11_SUBRESOURCE_DATA* subData, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc) override;
	void CreateShaderResourceView(std::string name, Hash_Code hash_code, D3D11_TEXTURE2D_DESC* texDesc, D3D11_SUBRESOURCE_DATA* subData, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc) override;
	void CreateShaderResourceView(std::string name, Hash_Code hash_code, ID3D11Texture2D* tex2D, D3D11_SUBRESOURCE_DATA* subData, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc) override;
	void CreateUnorderedAccessView(std::string name, Hash_Code hash_code, D3D11_TEXTURE2D_DESC* texDesc, D3D11_SUBRESOURCE_DATA* subData, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc) override;
	void CreateUnorderedAccessView(std::string name, Hash_Code hash_code, ID3D11Texture2D* tex2D, D3D11_SUBRESOURCE_DATA* subData, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc) override;

private:
	void CreateDepthStencilState(std::string name, Hash_Code hash_code, D3D11_DEPTH_STENCIL_DESC* dssDesc) override;
	void CreateRasterizerState(std::string name, Hash_Code hash_code, D3D11_RASTERIZER_DESC* rsDesc) override;
	void CreateBlendState(std::string name, Hash_Code hash_code, D3D11_BLEND_DESC* bsDesc) override;
	void CreateSamplerState(std::string name, Hash_Code hash_code, D3D11_SAMPLER_DESC* ssDesc) override;
	void CreateViewPort(std::string name, Hash_Code hash_code, float ratio_offsetX, float ratio_offsetY, float ratio_sizeX, float ratio_sizeY, float width, float height) override;
	void CreateViewPort(std::string name, Hash_Code hash_code, float offsetX, float offsetY, float width, float height) override;

private:
	template<typename ResourceClass, typename Resource>
	ResourceClass* RegisterResource(Hash_Code hash_code, Resource* resource);

private:
	template<typename VertexData>
	void CreateLoadBuffer(ParserData::Mesh* mesh, LoadMeshData* meshData);

	void CreateMainRenderTarget(Hash_Code hash_code, UINT width, UINT height);
	void CreateTextureRenderTarget(UINT width, UINT height);

	void CreateDepthStencilStates();
	void CreateRasterizerStates();
	void CreateSamplerStates();
	void CreateBlendStates();

	void CreateDepthStencilViews(int width, int height);
	void CreateViewPorts(int width, int height);

	void CreateInstanceBuffers();

	void CreateQuadBuffer();
	void CreateSSAOQuadBuffer();
	void CreateBoxBuffer();
	void CreateSphereBuffer();

	void CreateLineRayBuffer();
	void CreateLineQuadBuffer();
	void CreateLineAxisBuffer();
	void CreateLineBoxBuffer();
	void CreateLineCircleBuffer();
	void CreateLineGridBuffer();

private:
	ImageParser* m_Parser;

	std::string m_TextureRoute;
};

class DepthStencilView;
class RenderTargetView;
class ShaderResourceView;
class UnorderedAccessView;

#pragma region Register Resource Template Function
template<typename ResourceClass, typename Resource>
inline ResourceClass* GraphicResourceFactory::RegisterResource(Hash_Code hash_code, Resource* resource) { return nullptr; }

template<>
inline DepthStencilView* GraphicResourceFactory::RegisterResource(Hash_Code hash_code, ID3D11DepthStencilView* resource);

template<>
inline RenderTargetView* GraphicResourceFactory::RegisterResource(Hash_Code hash_code, ID3D11RenderTargetView* resource);

template<>
inline ShaderResourceView* GraphicResourceFactory::RegisterResource(Hash_Code hash_code, ID3D11ShaderResourceView* resource);

template<>
inline UnorderedAccessView* GraphicResourceFactory::RegisterResource(Hash_Code hash_code, ID3D11UnorderedAccessView* resource);
#pragma endregion Register Resource Template Function

namespace VertexInput
{
	struct MeshVertex;
	struct SkinVertex;
	struct TerrainVertex;
	struct PosTexVertex;
}

#pragma region Create Index & Vertex Buffer Template Function
template<typename VertexData>
inline void GraphicResourceFactory::CreateLoadBuffer(ParserData::Mesh* mesh, LoadMeshData* meshData) {}

template<>
inline void GraphicResourceFactory::CreateLoadBuffer<VertexInput::MeshVertex>(ParserData::Mesh* mesh, LoadMeshData* meshData);

template<>
inline void GraphicResourceFactory::CreateLoadBuffer<VertexInput::SkinVertex>(ParserData::Mesh* mesh, LoadMeshData* meshData);

template<>
inline void GraphicResourceFactory::CreateLoadBuffer<VertexInput::TerrainVertex>(ParserData::Mesh* mesh, LoadMeshData* meshData);

template<>
inline void GraphicResourceFactory::CreateLoadBuffer<VertexInput::PosTexVertex>(ParserData::Mesh* mesh, LoadMeshData* meshData);
#pragma endregion Create Index & Vertex Buffer Template Function
