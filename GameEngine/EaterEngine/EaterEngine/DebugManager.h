#pragma once
#include <windows.h>
#include <string>

class DebugManager
{
public:
	DebugManager();
	~DebugManager();

public:
	friend class GameEngine;

public:
	static void DebugDrawLine(DirectX::SimpleMath::Vector3 start, DirectX::SimpleMath::Vector3 end, DirectX::SimpleMath::Vector3 color);
	static void DebugDrawLine(DirectX::SimpleMath::Vector3 start, DirectX::SimpleMath::Vector3 dir, float distance, DirectX::SimpleMath::Vector3 color);

	static void DebugDrawCircle(float scale, DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 rot, DirectX::SimpleMath::Vector3 color);
	static void DebugDrawCircle(float scale, DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Quaternion rot, DirectX::SimpleMath::Vector3 color);

	static void DebugDrawBox(DirectX::SimpleMath::Vector3 scale, DirectX::SimpleMath::Vector3 rot, DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 color);
	static void DebugDrawBox(DirectX::SimpleMath::Vector3 scale, DirectX::SimpleMath::Quaternion rot, DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 color);

	static void DebugDrawSphere(float scale, DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 color);

private:
	static void Initialize();
	static void RenderOptionUpdate(RenderOption* option);

public:
	static bool IsDebug;
};