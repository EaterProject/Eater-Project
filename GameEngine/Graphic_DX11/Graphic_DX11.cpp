#include "DirectDefine.h"
#include "D3D11Graphic.h"
#include "Graphic_DX11.h"

#include "ShaderBase.h"
#include "ShaderManager.h"
#include "ResourceManager.h"
#include "RenderManager.h"
#include "FactoryManager.h"
#include "RenderDataConverter.h"
#include "Utility.h"
#include "atlbase.h"

Graphic_DX11::Graphic_DX11()
	:m_FactoryManager(nullptr), m_RenderManager(nullptr)
{

}

Graphic_DX11::~Graphic_DX11()
{

}

void Graphic_DX11::Initialize(HWND hwnd, int screenWidth, int screenHeight, RenderOption* renderOption)
{
	// DirectX11 Device 持失..
	D3D11Graphic* graphic = new D3D11Graphic(hwnd, screenWidth, screenHeight);

	// Shader Manager 持失..
	ShaderManager* shader = new ShaderManager(graphic);

	// Resource Manager 持失..
	GraphicResourceManager* resource = new GraphicResourceManager(graphic, shader);

	// Resource Factory 持失..
	FactoryManager* factory = new FactoryManager(graphic, shader, resource);

	// Render Data Converter 持失..
	RenderDataConverter* converter = new RenderDataConverter();

	// Render Manager 持失..
	RenderManager* renderer = new RenderManager(graphic, factory, resource, shader, converter, renderOption);

	// Initialize..
	factory->Create(screenWidth, screenHeight);
	shader->Create();
	resource->Create();
	renderer->Create(screenWidth, screenHeight);

	// Start..
	factory->Start();
	shader->Start();
	resource->Start();
	renderer->Start(screenWidth, screenHeight);

	// Pointer 竺舛..
	m_FactoryManager = factory;
	m_RenderManager = renderer;
	m_Converter = converter;
}

void Graphic_DX11::OnReSize(int screenWidth, int screenHeight)
{
	m_RenderManager->OnResize(screenWidth, screenHeight);
}

void Graphic_DX11::Release()
{

}

void Graphic_DX11::RenderSetting()
{
	m_RenderManager->RenderSetting();
}

void Graphic_DX11::RenderSetting(RenderOption* renderOption)
{
	m_RenderManager->RenderSetting(renderOption);
}

void Graphic_DX11::SetGlobalData(GlobalData* globalData)
{
	m_RenderManager->SetGlobalData(globalData);
}

void Graphic_DX11::SetFullScreenBlur(bool enable, UINT blur_count)
{
	m_RenderManager->SetFullScreenBlur(enable, blur_count);
}

void Graphic_DX11::SetSkyCube(TextureBuffer* resource)
{
	m_RenderManager->SetSkyCube(resource);
}

void Graphic_DX11::SetSkyLight(SkyLightBuffer* resource, UINT index)
{
	m_RenderManager->SetSkyLight(resource, index);
}

void Graphic_DX11::SetColorGradingBaseTexture(TextureBuffer* lut_resource)
{
	m_RenderManager->SetColorGradingBaseTexture(lut_resource);
}

void Graphic_DX11::SetColorGradingBlendTexture(TextureBuffer* lut_resource)
{
	m_RenderManager->SetColorGradingBlendTexture(lut_resource);
}

void Graphic_DX11::SetColorGradingBlendFactor(float blend_factor)
{
	m_RenderManager->SetColorGradingBlendFactor(blend_factor);
}

void Graphic_DX11::PushInstance(MeshData* instance)
{
	m_RenderManager->PushInstance(instance);
}

void Graphic_DX11::PushMesh(MeshBuffer* mesh)
{
	m_RenderManager->PushMesh(mesh);
}

void Graphic_DX11::PushMaterial(MaterialBuffer* material)
{
	m_RenderManager->PushMaterial(material);
}

void Graphic_DX11::PushAnimation(AnimationBuffer* animation)
{
	m_RenderManager->PushAnimation(animation);
}

void Graphic_DX11::PushChangeInstance(MeshData* instance)
{
	m_RenderManager->PushChangeInstance(instance);
}

void Graphic_DX11::PushChangeMesh(MeshBuffer* mesh)
{
	m_RenderManager->PushChangeMesh(mesh);
}

void Graphic_DX11::PushChangeMaterial(MaterialBuffer* material)
{
	m_RenderManager->PushChangeMaterial(material);
}

void Graphic_DX11::PushChangeAnimation(AnimationBuffer* animation)
{
	m_RenderManager->PushChangeAnimation(animation);
}

void Graphic_DX11::DeleteInstance(MeshData* meshData)
{
	m_RenderManager->DeleteInstance(meshData);
}

void Graphic_DX11::DeleteMesh(MeshBuffer* mesh)
{
	m_RenderManager->DeleteMesh(mesh);
}

void Graphic_DX11::DeleteMaterial(MaterialBuffer* material)
{
	m_RenderManager->DeleteMaterial(material);
}

void Graphic_DX11::DeleteAnimation(AnimationBuffer* animation)
{
	m_RenderManager->DeleteAnimation(animation);
}

void Graphic_DX11::PushMaterialBlockInstance(MeshData* instance)
{
	m_RenderManager->PushMaterialBlockInstance(instance);
}

void Graphic_DX11::PopMaterialBlockInstance(MeshData* instance)
{
	m_RenderManager->PopMaterialBlockInstance(instance);
}

void Graphic_DX11::DeleteTexture(TextureBuffer* resource)
{
	ID3D11ShaderResourceView* texture = (ID3D11ShaderResourceView*)resource->pTextureBuf;

	RELEASE_COM(texture);

	resource->pTextureBuf = nullptr;
}

void Graphic_DX11::DeleteSkyLight(SkyLightBuffer* resource)
{
	ID3D11ShaderResourceView* irradiance = (ID3D11ShaderResourceView*)resource->Irradiance->pTextureBuf;
	ID3D11ShaderResourceView* prefilter = (ID3D11ShaderResourceView*)resource->Prefilter->pTextureBuf;
	
	RELEASE_COM(irradiance);
	RELEASE_COM(prefilter);

	resource->Irradiance->pTextureBuf = nullptr;
	resource->Prefilter->pTextureBuf = nullptr;
}

void Graphic_DX11::Render()
{
	m_RenderManager->Render();
}

void* Graphic_DX11::PickingRender(int x, int y)
{
	return m_RenderManager->PickingRender(x, y);
}

void Graphic_DX11::CreateTextureBuffer(std::string path, TextureBuffer** ppResource)
{
	m_FactoryManager->CreateTextureBuffer(path, ppResource);
}

void Graphic_DX11::CreateMeshBuffer(ParserData::CMesh* mesh, MeshBuffer** ppResource)
{
	m_FactoryManager->CreateMeshBuffer(mesh, ppResource);
}

void Graphic_DX11::CreateAnimationBuffer(ModelData* model, ModelAnimationData* animation, AnimationBuffer** ppResource)
{
	m_FactoryManager->CreateAnimationBuffer(model, animation, ppResource);
}

void Graphic_DX11::BakeSkyLightMap(TextureBuffer* environment, bool hdri, SkyLightBuffer** ppResource)
{
	m_FactoryManager->BakeSkyLightMap(environment, hdri, ppResource);
}

void Graphic_DX11::BakeConvertCubeMap(TextureBuffer* resource, float angle, float threshold, bool hdri, TextureBuffer** ppResource)
{
	m_FactoryManager->BakeConvertCubeMap(resource, angle, threshold, hdri, ppResource);
}

void Graphic_DX11::SaveConvertCubeMap(TextureBuffer* resource, std::string SaveName)
{
	m_FactoryManager->SaveConvertCubeMap(resource, SaveName);
}

void Graphic_DX11::SaveSpriteToVolumeTexture(TextureBuffer* resource, std::string saveName, UINT pixelSize, TextureBuffer** ppResource)
{
	m_FactoryManager->SaveSpriteToVolumeTexture(resource, saveName, pixelSize, ppResource);
}

void Graphic_DX11::SaveSpriteToVolumeTexture(std::string fileName, std::string saveName, UINT pixelSize)
{
	m_FactoryManager->SaveSpriteToVolumeTexture(fileName, saveName, pixelSize);
}
