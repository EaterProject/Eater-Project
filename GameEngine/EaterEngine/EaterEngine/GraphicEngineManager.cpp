#include "SimpleMath.h"
#include "GraphicEngineManager.h"
#include "GraphicsEngine.h"
#include "EngineData.h"
#include "ParserData.h"
#include "ObjectManager.h"
#include "GlobalDataManager.h"

GraphicEngineManager::GraphicEngineManager()
{
	GEngine = nullptr;
}

GraphicEngineManager::~GraphicEngineManager()
{

}

void GraphicEngineManager::Initialize(HWND Hwnd, int WinSizeWidth, int WinSizeHeight)
{
	// Graphic Engine Create..
	GEngine = GraphicEngine::Get();

	// Graphic Engine Initialize..
	GEngine->Initialize(Hwnd, WinSizeWidth, WinSizeHeight);

	// Graphic Engine RenderSetting..
	GEngine->SetGlobalData(GlobalDataManager::g_GlobalData);
}

void GraphicEngineManager::OnReSize(int Change_Width, int Change_Height)
{
	//荤捞令 函版
	GEngine->OnReSize(Change_Width, Change_Height);
}

void GraphicEngineManager::Release()
{
	GEngine->Release();
}

void GraphicEngineManager::RenderSetting(RenderOption* renderOption)
{
	GEngine->RenderSetting(renderOption);
}

void GraphicEngineManager::PushInstance(MeshData* mesh)
{
	GEngine->PushInstance(mesh);
}

void GraphicEngineManager::PushMesh(MeshBuffer* meshData)
{
	GEngine->PushMesh(meshData);
}

void GraphicEngineManager::PushMaterial(MaterialBuffer* materialData)
{
	GEngine->PushMaterial(materialData);
}

void GraphicEngineManager::PushChangeInstance(MeshData* mesh)
{
	GEngine->PushChangeInstance(mesh);
}

void GraphicEngineManager::DeleteInstance(MeshData* mesh)
{
	GEngine->DeleteInstance(mesh);
}

void GraphicEngineManager::Render()
{
	GEngine->Render();
}

void* GraphicEngineManager::PickingRender(int x, int y)
{
	return GEngine->PickingRender(x, y);
}

void GraphicEngineManager::AddOccluder(MeshBuffer* occluder)
{
	GlobalDataManager::g_GlobalData->OccluderList.push_back(occluder);
}

void GraphicEngineManager::CreateMeshBuffer(ParserData::CMesh* model, MeshBuffer** ppResource)
{
	// Mesh 积己..
	GEngine->CreateMeshBuffer(model, ppResource);
}

void GraphicEngineManager::CreateTextureBuffer(std::string Name, TextureBuffer** ppResource)
{
	// Texture 积己..
	GEngine->CreateTextureBuffer(Name, ppResource);
}

void GraphicEngineManager::CreateAnimationBuffer(ModelData* model, ModelAnimationData* animation, AnimationBuffer** ppResource)
{
	GEngine->CreateAnimationBuffer(model, animation, ppResource);
}

void GraphicEngineManager::BakeShadowMap(std::string Path)
{
	GEngine->BakeShadowMap(Path);
}

void GraphicEngineManager::BakeEnvironmentMap(TextureBuffer* environment, EnvironmentBuffer** ppResource)
{
	// Texture 积己..
	GEngine->BakeEnvironmentMap(environment, ppResource);
}

void GraphicEngineManager::SetShadowMap(TextureBuffer* resource)
{
	GEngine->SetShadowMap(resource);
}

void GraphicEngineManager::SetEnvironmentMap(EnvironmentBuffer* resource)
{
	GEngine->SetEnvironmentMap(resource);
}
