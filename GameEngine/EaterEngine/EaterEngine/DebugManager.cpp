#include "SimpleMath.h"
#include "EngineData.h"
#include "DebugManager.h"
#include "GlobalDataManager.h"
#include "GameEngine.h"

bool DebugManager::IsDebug = false;

DebugManager::DebugManager()
{

}

DebugManager::~DebugManager()
{

}

void DebugManager::Initialize()
{
	GameEngine::RenderOptionFunction += Eater::Bind(RenderOptionUpdate);
}

void DebugManager::DebugDrawLine(DirectX::SimpleMath::Vector3 start, DirectX::SimpleMath::Vector3 end, DirectX::SimpleMath::Vector3 color)
{
	if (IsDebug == false) return;

	DebugData debugData;
	debugData.DebugType = DEBUG_MESH_TYPE::DEBUG_MESH_RAY;
	debugData.RayStart = start;
	debugData.RayEnd = end;
	debugData.Color = color;


	GlobalDataManager::g_GlobalData->Debug_Data.push(debugData);
}

void DebugManager::DebugDrawLine(DirectX::SimpleMath::Vector3 start, DirectX::SimpleMath::Vector3 dir, float distance, DirectX::SimpleMath::Vector3 color)
{
	if (IsDebug == false) return;
	
	DebugData debugData;
	debugData.DebugType = DEBUG_MESH_TYPE::DEBUG_MESH_RAY;
	debugData.RayStart = start;
	debugData.RayEnd = start + (dir * distance);
	debugData.Color = color;

	GlobalDataManager::g_GlobalData->Debug_Data.push(debugData);
}

void DebugManager::DebugDrawCircle(float scale, DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 rot, DirectX::SimpleMath::Vector3 color)
{
	if (IsDebug == false) return;
	
	float pitch = rot.x * 3.141592f / 180.0f;
	float yaw = rot.y * 3.141592f / 180.0f;
	float roll = rot.z * 3.141592f / 180.0f;

	DebugData debugData;
	debugData.DebugType = DEBUG_MESH_TYPE::DEBUG_MESH_CIRCLE;
	debugData.World = Matrix::CreateScale(scale) * Matrix::CreateFromYawPitchRoll(yaw, pitch, roll) * Matrix::CreateTranslation(pos);
	debugData.Color = color;

	GlobalDataManager::g_GlobalData->Debug_Data.push(debugData);
}

void DebugManager::DebugDrawCircle(float scale, DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Quaternion rot, DirectX::SimpleMath::Vector3 color)
{
	if (IsDebug == false) return;
	
	DebugData debugData;
	debugData.DebugType = DEBUG_MESH_TYPE::DEBUG_MESH_CIRCLE;
	debugData.World = Matrix::CreateScale(scale) * Matrix::CreateFromQuaternion(rot) * Matrix::CreateTranslation(pos);
	debugData.Color = color;

	GlobalDataManager::g_GlobalData->Debug_Data.push(debugData);
}

void DebugManager::DebugDrawBox(DirectX::SimpleMath::Vector3 scale, DirectX::SimpleMath::Vector3 rot, DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 color)
{
	if (IsDebug == false) return;
	
	float pitch = rot.x * 3.141592f / 180.0f;
	float yaw = rot.y * 3.141592f / 180.0f;
	float roll = rot.z * 3.141592f / 180.0f;

	DebugData debugData;
	debugData.DebugType = DEBUG_MESH_TYPE::DEBUG_MESH_BOX;
	debugData.World = Matrix::CreateScale(scale) * Matrix::CreateFromYawPitchRoll(yaw, pitch, roll) * Matrix::CreateTranslation(pos);
	debugData.Color = color;

	GlobalDataManager::g_GlobalData->Debug_Data.push(debugData);
}

void DebugManager::DebugDrawBox(DirectX::SimpleMath::Vector3 scale, DirectX::SimpleMath::Quaternion rot, DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 color)
{
	if (IsDebug == false) return;

	DebugData debugData;
	debugData.DebugType = DEBUG_MESH_TYPE::DEBUG_MESH_BOX;
	debugData.World = Matrix::CreateScale(scale) * Matrix::CreateFromQuaternion(rot) * Matrix::CreateTranslation(pos);
	debugData.Color = color;

	GlobalDataManager::g_GlobalData->Debug_Data.push(debugData);
}

void DebugManager::DebugDrawSphere(float scale, DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 color)
{
	if (IsDebug == false) return;
	
	DebugData debugData;
	debugData.DebugType = DEBUG_MESH_TYPE::DEBUG_MESH_SPHERE;
	debugData.World = Matrix::CreateScale(scale) * Matrix::CreateTranslation(pos);
	debugData.Color = color;

	GlobalDataManager::g_GlobalData->Debug_Data.push(debugData);
}

void DebugManager::RenderOptionUpdate(RenderOption* option)
{
	if (option->DebugOption & DEBUG_OPTION::DEBUG_MODE)
	{
		IsDebug = true;
	}
	else
	{
		IsDebug = false;
	}
}
