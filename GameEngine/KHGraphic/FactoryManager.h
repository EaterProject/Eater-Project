#pragma once
#include "FactoryManagerBase.h"

interface ID3D11Graphic;
interface IShaderManager;
interface IGraphicResourceManager;

interface IGraphicResourceFactory;
interface IBakingFactory;

class FactoryManager : public IFactoryManager
{
public:
	FactoryManager(ID3D11Graphic* graphic, IShaderManager* shader, IGraphicResourceManager* resource);
	virtual ~FactoryManager();

public:
	void Create(int width, int height) override;
	void Start() override;
	void Release() override;

public:
	void CreateTextureBuffer(std::string path, TextureBuffer** ppResource) override;
	void CreateMeshBuffer(ParserData::CMesh* mesh, MeshBuffer** ppResource) override;
	void CreateAnimationBuffer(ModelData* model, ModelAnimationData* animation, AnimationBuffer** ppResource) override;

public:
	void BakeShadowMap(std::string fileName) override;
	void BakeSkyLightMap(TextureBuffer* skyLight, bool hdri, SkyLightBuffer** ppResource) override;
	void BakeConvertCubeMap(TextureBuffer* resource, float angle, bool save_file, bool hdri, TextureBuffer** ppResource) override;

private:
	void CreateImg(std::string name, Hash_Code hash_code, std::string fileName) override;

	void CreateDS(std::string name, Hash_Code hash_code, D3D11_TEXTURE2D_DESC* texDesc, D3D11_SUBRESOURCE_DATA* subData, D3D11_DEPTH_STENCIL_VIEW_DESC* dsvDesc, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc) override;
	void CreateRT(std::string name, Hash_Code hash_code, D3D11_TEXTURE2D_DESC* texDesc, D3D11_SUBRESOURCE_DATA* subData, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc) override;
	void CreateRB(std::string name, Hash_Code hash_code, D3D11_BUFFER_DESC* bufferDesc, D3D11_SUBRESOURCE_DATA* subData, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc) override;

	void CreateDB(std::string name, Hash_Code hash_code, UINT format, UINT topology, UINT vByteSize, UINT iByteSize, UINT vSize, UINT iCount, const void* vertices, const void* indices) override;
	void CreateIB(std::string name, Hash_Code hash_code, UINT byteSize, UINT instCount, const void* instance) override;

	void CreateRTV(std::string name, Hash_Code hash_code, D3D11_TEXTURE2D_DESC* texDesc, D3D11_SUBRESOURCE_DATA* subData, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc) override;
	void CreateRTV(std::string name, Hash_Code hash_code, ID3D11Texture2D* tex2D, D3D11_SUBRESOURCE_DATA* subData, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc) override;
	void CreateSRV(std::string name, Hash_Code hash_code, D3D11_TEXTURE2D_DESC* texDesc, D3D11_SUBRESOURCE_DATA* subData, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc) override;
	void CreateSRV(std::string name, Hash_Code hash_code, ID3D11Texture2D* tex2D, D3D11_SUBRESOURCE_DATA* subData, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc) override;
	void CreateUAV(std::string name, Hash_Code hash_code, D3D11_TEXTURE2D_DESC* texDesc, D3D11_SUBRESOURCE_DATA* subData, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc) override;
	void CreateUAV(std::string name, Hash_Code hash_code, ID3D11Texture2D* tex2D, D3D11_SUBRESOURCE_DATA* subData, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc) override;
	
	void CreateDSS(std::string name, Hash_Code hash_code, D3D11_DEPTH_STENCIL_DESC* dssDesc) override;
	void CreateRS(std::string name, Hash_Code hash_code, D3D11_RASTERIZER_DESC* rsDesc) override;
	void CreateBS(std::string name, Hash_Code hash_code, D3D11_BLEND_DESC* bsDesc) override;
	void CreateSS(std::string name, Hash_Code hash_code, D3D11_SAMPLER_DESC* ssDesc) override;
	void CreateVP(std::string name, Hash_Code hash_code, float ratio_offsetX, float ratio_offsetY, float ratio_sizeX, float ratio_sizeY, float width, float height) override;
	void CreateVP(std::string name, Hash_Code hash_code, float offsetX, float offsetY, float width, float height) override;

private:
	IGraphicResourceFactory* m_ResourceFactory;
	IBakingFactory* m_BakingFactory;
};

