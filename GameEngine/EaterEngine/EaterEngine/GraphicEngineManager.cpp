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

void GraphicEngineManager::Initialize(HWND Hwnd, int WinSizeWidth, int WinSizeHeight, ObjectManager* GM)
{
	ObjManager = GM;

	// Graphic Engine Create..
	GEngine = GraphicEngine::Create();

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

void GraphicEngineManager::AddMeshData(MeshData* mesh)
{
	GEngine->AddMeshData(mesh);
}

void GraphicEngineManager::DeleteMeshData(MeshData* mesh)
{
	GEngine->DeleteMeshData(mesh);
}

void GraphicEngineManager::Render()
{
	if (ObjManager->InitFuncUpdate)
	{
		ObjManager->InitFuncUpdate = false;
		GEngine->ConvertMeshData();
	}

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

	Global->mRayCastDebugData.push(ray);
}

void GraphicEngineManager::DebugDrawLine(DirectX::SimpleMath::Vector3 start, DirectX::SimpleMath::Vector3 dir, float distance, DirectX::SimpleMath::Vector4 color)
{
	RayCastData ray;
	ray.RayStart = start;
	ray.RayEnd = start + (dir * distance);
	ray.RayColor = color;

	Global->mRayCastDebugData.push(ray);
}

void GraphicEngineManager::CreateMeshBuffer(ParserData::Mesh* mModel, LoadMeshData* meshData)
{
	//매쉬 생성
	GEngine->CreateMeshBuffer(mModel, meshData);
}

TextureBuffer* GraphicEngineManager::CreateTextureBuffer(std::string Name)
{
	//텍스쳐 생성
	return GEngine->CreateTextureBuffer(Name);
}
