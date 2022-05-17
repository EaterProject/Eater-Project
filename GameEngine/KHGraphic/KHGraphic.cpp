#include "DirectDefine.h"
#include "D3D11Graphic.h"
#include "KHGraphic.h"

#include "ShaderBase.h"
#include "ShaderManager.h"
#include "ResourceManager.h"
#include "RenderManager.h"
#include "FactoryManager.h"
#include "RenderDataConverter.h"
#include "atlbase.h"

KHGraphic::KHGraphic()
	:m_FactoryManager(nullptr), m_RenderManager(nullptr)
{

}

KHGraphic::~KHGraphic()
{

}

void KHGraphic::Initialize(HWND hwnd, int screenWidth, int screenHeight, RenderOption* renderOption)
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

void KHGraphic::OnReSize(int screenWidth, int screenHeight)
{
	m_RenderManager->OnResize(screenWidth, screenHeight);
}

void KHGraphic::Release()
{

}

void KHGraphic::RenderSetting()
{
	m_RenderManager->RenderSetting();
}

void KHGraphic::RenderSetting(RenderOption* renderOption)
{
	m_RenderManager->RenderSetting(renderOption);
}

void KHGraphic::SetGlobalData(GlobalData* globalData)
{
	m_RenderManager->SetGlobalData(globalData);
}

void KHGraphic::SetEnvironment(TextureBuffer* resource)
{
	m_RenderManager->SetEnvironment(resource);
}

void KHGraphic::SetSkyLight(SkyLightBuffer* resource)
{
	m_RenderManager->SetSkyLight(resource);
}

void KHGraphic::PushInstance(MeshData* instance)
{
	m_RenderManager->PushInstance(instance);
}

void KHGraphic::PushMesh(MeshBuffer* mesh)
{
	m_RenderManager->PushMesh(mesh);
}

void KHGraphic::PushMaterial(MaterialBuffer* material)
{
	m_RenderManager->PushMaterial(material);
}

void KHGraphic::PushAnimation(AnimationBuffer* animation)
{
	m_RenderManager->PushAnimation(animation);
}

void KHGraphic::PushChangeInstance(MeshData* instance)
{
	m_RenderManager->PushChangeInstance(instance);
}

void KHGraphic::PushChangeMesh(MeshBuffer* mesh)
{
	m_RenderManager->PushChangeMesh(mesh);
}

void KHGraphic::PushChangeMaterial(MaterialBuffer* material)
{
	m_RenderManager->PushChangeMaterial(material);
}

void KHGraphic::PushChangeAnimation(AnimationBuffer* animation)
{
	m_RenderManager->PushChangeAnimation(animation);
}

void KHGraphic::DeleteInstance(MeshData* meshData)
{
	m_RenderManager->DeleteInstance(meshData);
}

void KHGraphic::DeleteMesh(MeshBuffer* mesh)
{
	m_RenderManager->DeleteMesh(mesh);
}

void KHGraphic::DeleteMaterial(MaterialBuffer* material)
{
	m_RenderManager->DeleteMaterial(material);
}

void KHGraphic::DeleteAnimation(AnimationBuffer* animation)
{
	m_RenderManager->DeleteAnimation(animation);
}

void KHGraphic::DeleteTexture(TextureBuffer* resource)
{
	ID3D11ShaderResourceView* texture = (ID3D11ShaderResourceView*)resource->pTextureBuf;

	RELEASE_COM(texture);

	resource->pTextureBuf = nullptr;
}

void KHGraphic::DeleteSkyLight(SkyLightBuffer* resource)
{
	ID3D11ShaderResourceView* irradiance = (ID3D11ShaderResourceView*)resource->Irradiance->pTextureBuf;
	ID3D11ShaderResourceView* prefilter = (ID3D11ShaderResourceView*)resource->Prefilter->pTextureBuf;
	
	RELEASE_COM(irradiance);
	RELEASE_COM(prefilter);

	resource->Irradiance->pTextureBuf = nullptr;
	resource->Prefilter->pTextureBuf = nullptr;
}

void KHGraphic::Render()
{
	m_RenderManager->Render();
}

void* KHGraphic::PickingRender(int x, int y)
{
	return m_RenderManager->PickingRender(x, y);
}

void KHGraphic::CreateTextureBuffer(std::string path, TextureBuffer** ppResource)
{
	m_FactoryManager->CreateTextureBuffer(path, ppResource);
}

void KHGraphic::CreateMeshBuffer(ParserData::CMesh* mesh, MeshBuffer** ppResource)
{
	m_FactoryManager->CreateMeshBuffer(mesh, ppResource);
}

void KHGraphic::CreateAnimationBuffer(ModelData* model, ModelAnimationData* animation, AnimationBuffer** ppResource)
{
	m_FactoryManager->CreateAnimationBuffer(model, animation, ppResource);
}

void KHGraphic::BakeSkyLightMap(TextureBuffer* environment, bool hdri, SkyLightBuffer** ppResource)
{
	m_FactoryManager->BakeSkyLightMap(environment, hdri, ppResource);
}

void KHGraphic::BakeConvertCubeMap(TextureBuffer* resource, float angle, bool save_file, bool hdri, TextureBuffer** ppResource)
{
	m_FactoryManager->BakeConvertCubeMap(resource, angle, save_file, hdri, ppResource);
}
