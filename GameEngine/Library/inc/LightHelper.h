//***************************************************************************************
// LightHelper.h by Frank Luna (C) 2011 All Rights Reserved.
//
// Helper classes for lighting.
//***************************************************************************************

#ifndef LIGHTHELPER_H
#define LIGHTHELPER_H
#define ALIGNED_MEMORY(byte) __declspec(align(byte))

#include <Windows.h>
#include "SimpleMath.h"

// Note: Make sure structure alignment agrees with HLSL structure padding rules. 
//   Elements are packed into 4D vectors with the restriction that an element
//   cannot straddle a 4D vector boundary.

using namespace DirectX::SimpleMath;

struct DirectionalLightData
{
	DirectionalLightData() = default;

	Vector3 Diffuse = Vector3(1.0f, 1.0f, 1.0f);
	float Power = 1.0f;

	Vector3 Direction = Vector3(-0.707f, -1.0f, 0.707f);
	float Pad;

	Matrix LightViewProj = Matrix();
};

struct PointLightData
{
	PointLightData() = default;
	
	Vector3 Diffuse = Vector3(1.0f, 1.0f, 1.0f);
	float Range = 10.0f;

	Vector3 Position = Vector3(0.0f, 0.0f, 0.0f);
	float Power = 1.0f;

	Matrix LightViewProj = Matrix();
};

struct SpotLightData
{
	SpotLightData() = default;
	
	Vector3 Diffuse = Vector3(1.0f, 1.0f, 1.0f);
	float Range = 10.0f;

	Vector3 Position = Vector3(0.0f, 0.0f, 0.0f);
	float Power = 1.0f;

	Vector3 Direction = Vector3(0.0f, -1.0f, 0.0f);
	float Angle = 25.0f;

	float AttStart = 0.0f;
	float AttRange = 0.0f;
	Vector2 Pad = Vector2(0.0f, 0.0f);

	Matrix LightViewProj = Matrix();
};

#endif // LIGHTHELPER_H