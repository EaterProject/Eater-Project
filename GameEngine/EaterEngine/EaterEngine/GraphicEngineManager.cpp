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

void GraphicEngineManager::Initialize(HWND Hwnd, int WinSizeWidth, int WinSizeHeight, RenderOption* renderOption)
{
	// Graphic Engine Create..
	GEngine = GraphicEngine::Get();

	// Graphic Engine Initialize..
	GEngine->Initialize(Hwnd, WinSizeWidth, WinSizeHeight);

	// Graphic Engine Global Data Setting..
	GEngine->SetGlobalData(GlobalDataManager::g_GlobalData);

	// Graphic Engine Render Setting..
	GEngine->RenderSetting(renderOption);
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

void GraphicEngineManager::RenderSetting()
{
	GEngine->RenderSetting();
}

void GraphicEngineManager::PushInstance(MeshData* meshData)
{
	GEngine->PushInstance(meshData);
}

void GraphicEngineManager::PushMesh(MeshBuffer* mesh)
{
	GEngine->PushMesh(mesh);
}

void GraphicEngineManager::PushMaterial(MaterialBuffer* material)
{
	GEngine->PushMaterial(material);
}

void GraphicEngineManager::PushAnimation(AnimationBuffer* animation)
{
	GEngine->PushAnimation(animation);
}

void GraphicEngineManager::PushChangeInstance(MeshData* meshData)
{
	GEngine->PushChangeInstance(meshData);
}

void GraphicEngineManager::PushChangeMesh(MeshBuffer* mesh)
{
	GEngine->PushChangeMesh(mesh);
}

void GraphicEngineManager::PushChangeMaterial(MaterialBuffer* material)
{
	GEngine->PushChangeMaterial(material);
}

void GraphicEngineManager::PushChangeAnimation(AnimationBuffer* animation)
{
	GEngine->PushChangeAnimation(animation);
}

void GraphicEngineManager::DeleteInstance(MeshData* meshData)
{
	GEngine->DeleteInstance(meshData);
}

void GraphicEngineManager::DeleteMesh(MeshBuffer* mesh)
{
	GEngine->DeleteMesh(mesh);
}

void GraphicEngineManager::DeleteMaterial(MaterialBuffer* material)
{
	GEngine->DeleteMaterial(material);
}

void GraphicEngineManager::DeleteAnimation(AnimationBuffer* animation)
{
	GEngine->DeleteAnimation(animation);
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

void GraphicEngineManager::BakeEnvironmentMap(TextureBuffer* environment, EnvironmentBuffer** ppResource)
{
	// Texture 积己..
	GEngine->BakeEnvironmentMap(environment, ppResource);
}

void GraphicEngineManager::SetEnvironmentMap(EnvironmentBuffer* resource)
{
	GEngine->SetEnvironmentMap(resource);
}
