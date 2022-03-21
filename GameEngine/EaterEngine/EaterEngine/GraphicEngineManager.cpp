#include "SimpleMath.h"
#include "BaseManager.h"
#include "GraphicEngineManager.h"
#include "DebugManager.h"
#include "GraphicsEngine.h"
#include "EngineData.h"
#include "ParserData.h"
#include "ObjectManager.h"

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
	GEngine->SetGlobalData(Global);
}

void GraphicEngineManager::OnReSize(int Change_Width, int Change_Height)
{
	//사이즈 변경
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

void GraphicEngineManager::SetEnvironment(bool enable)
{
	GEngine->SetEnvironmentMap(enable);
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

void GraphicEngineManager::LoadEnvironment(std::string mPath)
{
	GEngine->CreateEnvironmentMap(mPath);
}

void GraphicEngineManager::DebugDrawLine(DirectX::SimpleMath::Vector3 start, DirectX::SimpleMath::Vector3 end, DirectX::SimpleMath::Vector4 color)
{
	RayCastData ray;
	ray.RayStart = start;
	ray.RayEnd = end;
	ray.RayColor = color;

	Global->RayCastDebugData.push(ray);
}

void GraphicEngineManager::DebugDrawLine(DirectX::SimpleMath::Vector3 start, DirectX::SimpleMath::Vector3 dir, float distance, DirectX::SimpleMath::Vector4 color)
{
	RayCastData ray;
	ray.RayStart = start;
	ray.RayEnd = start + (dir * distance);
	ray.RayColor = color;

	Global->RayCastDebugData.push(ray);
}

void GraphicEngineManager::CreateMeshBuffer(ParserData::CMesh* model, MeshBuffer** ppResource)
{
	// Mesh 생성..
	GEngine->CreateMeshBuffer(model, ppResource);
}

void GraphicEngineManager::CreateTextureBuffer(std::string Name, TextureBuffer** ppResource)
{
	// Texture 생성..
	GEngine->CreateTextureBuffer(Name, ppResource);
}
