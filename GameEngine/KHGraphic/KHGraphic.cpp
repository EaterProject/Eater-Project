#include "DirectDefine.h"
#include "D3D11Graphic.h"
#include "KHGraphic.h"

#include "ShaderBase.h"
#include "ShaderManager.h"
#include "ResourceManager.h"
#include "RenderManager.h"
#include "FactoryManager.h"

KHGraphic::KHGraphic()
	:m_FactoryManager(nullptr), m_RenderManager(nullptr)
{

}

KHGraphic::~KHGraphic()
{

}

void KHGraphic::Initialize(HWND hwnd, int screenWidth, int screenHeight)
{
	// DirectX11 Device 持失..
	D3D11Graphic* graphic = new D3D11Graphic(hwnd, screenWidth, screenHeight);

	// Shader Manager 持失..
	ShaderManager* shader = new ShaderManager(graphic);

	// Resource Manager 持失..
	GraphicResourceManager* resource = new GraphicResourceManager(graphic, shader);

	// Resource Factory 持失..
	FactoryManager* factory = new FactoryManager(graphic, shader, resource);

	// Render Manager 持失..
	RenderManager* renderer = new RenderManager(graphic, factory, resource, shader);

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
}

void KHGraphic::OnReSize(int screenWidth, int screenHeight)
{
	m_RenderManager->OnResize(screenWidth, screenHeight);
}

void KHGraphic::Release()
{

}

void KHGraphic::RenderSetting(RenderOption* renderOption)
{
	m_RenderManager->RenderSetting(renderOption);
}

void KHGraphic::SetGlobalData(GlobalData* globalData)
{
	m_RenderManager->SetGlobalData(globalData);
}

void KHGraphic::SetEnvironmentMap(bool enable)
{
	m_RenderManager->SetEnvironmentMap(enable);
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

void KHGraphic::Render()
{
	m_RenderManager->Render();
}

void KHGraphic::CreateTextureBuffer(std::string path, TextureBuffer** ppResource)
{
	m_FactoryManager->CreateTextureBuffer(path, ppResource);
}

void KHGraphic::CreateMeshBuffer(ParserData::CMesh* mesh, MeshBuffer** ppResource)
{
	m_FactoryManager->CreateMeshBuffer(mesh, ppResource);
}

void KHGraphic::CreateEnvironmentMap(std::string path)
{
	m_FactoryManager->CreateEnvironmentMap(path);
}
