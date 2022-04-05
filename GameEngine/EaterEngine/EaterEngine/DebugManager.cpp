#include "SimpleMath.h"
#include "EngineData.h"
#include "DebugManager.h"

DebugManager::DebugManager()
{

}

DebugManager::~DebugManager()
{

}

void DebugManager::DebugDrawLine(DirectX::SimpleMath::Vector3 start, DirectX::SimpleMath::Vector3 end, DirectX::SimpleMath::Vector3 color)
{
	DebugData debugData;
	debugData.DebugType = DEBUG_MESH_TYPE::DEBUG_MESH_RAY;
	debugData.RayStart = start;
	debugData.RayEnd = end;
	debugData.Color = color;

	Global->Debug_Data.push(debugData);
}

void DebugManager::DebugDrawLine(DirectX::SimpleMath::Vector3 start, DirectX::SimpleMath::Vector3 dir, float distance, DirectX::SimpleMath::Vector3 color)
{
	DebugData debugData;
	debugData.DebugType = DEBUG_MESH_TYPE::DEBUG_MESH_RAY;
	debugData.RayStart = start;
	debugData.RayEnd = start + (dir * distance);
	debugData.Color = color;

	Global->Debug_Data.push(debugData);
}

void DebugManager::DebugDrawCircle(float scale, DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 rot, DirectX::SimpleMath::Vector3 color)
{
	float pitch = rot.x * 3.141592f / 180.0f;
	float yaw = rot.y * 3.141592f / 180.0f;
	float roll = rot.z * 3.141592f / 180.0f;

	DebugData debugData;
	debugData.DebugType = DEBUG_MESH_TYPE::DEBUG_MESH_CIRCLE;
	debugData.World = Matrix::CreateScale(scale) * Matrix::CreateFromYawPitchRoll(yaw, pitch, roll) * Matrix::CreateTranslation(pos);
	debugData.Color = color;

	Global->Debug_Data.push(debugData);
}

void DebugManager::DebugDrawCircle(float scale, DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Quaternion rot, DirectX::SimpleMath::Vector3 color)
{
	DebugData debugData;
	debugData.DebugType = DEBUG_MESH_TYPE::DEBUG_MESH_CIRCLE;
	debugData.World = Matrix::CreateScale(scale) * Matrix::CreateFromQuaternion(rot) * Matrix::CreateTranslation(pos);
	debugData.Color = color;

	Global->Debug_Data.push(debugData);
}

void DebugManager::DebugDrawBox(DirectX::SimpleMath::Vector3 scale, DirectX::SimpleMath::Vector3 rot, DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 color)
{
	float pitch = rot.x * 3.141592f / 180.0f;
	float yaw = rot.y * 3.141592f / 180.0f;
	float roll = rot.z * 3.141592f / 180.0f;

	DebugData debugData;
	debugData.DebugType = DEBUG_MESH_TYPE::DEBUG_MESH_BOX;
	debugData.World = Matrix::CreateScale(scale) * Matrix::CreateFromYawPitchRoll(yaw, pitch, roll) * Matrix::CreateTranslation(pos);
	debugData.Color = color;

	Global->Debug_Data.push(debugData);
}

void DebugManager::DebugDrawBox(DirectX::SimpleMath::Vector3 scale, DirectX::SimpleMath::Quaternion rot, DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 color)
{
	DebugData debugData;
	debugData.DebugType = DEBUG_MESH_TYPE::DEBUG_MESH_BOX;
	debugData.World = Matrix::CreateScale(scale) * Matrix::CreateFromQuaternion(rot) * Matrix::CreateTranslation(pos);
	debugData.Color = color;

	Global->Debug_Data.push(debugData);
}

void DebugManager::DebugDrawSphere(float scale, DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 color)
{
	DebugData debugData;
	debugData.DebugType = DEBUG_MESH_TYPE::DEBUG_MESH_SPHERE;
	debugData.World = Matrix::CreateScale(scale) * Matrix::CreateTranslation(pos);
	debugData.Color = color;

	Global->Debug_Data.push(debugData);
}
