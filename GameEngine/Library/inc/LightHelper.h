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

ALIGNED_MEMORY(16) struct DirectionalLightData
{
	DirectionalLightData() = default;

	Vector3 Diffuse = Vector3(1.0f, 1.0f, 1.0f);
	float Power = 1.0f;

	Vector3 Direction = Vector3(-0.707f, -1.0f, 0.707f);
	float Pad;

	Matrix LightViewProj = Matrix();
};

ALIGNED_MEMORY(16) struct PointLightData
{
	PointLightData() = default;
	
	Vector3 Diffuse = Vector3(1.0f, 1.0f, 1.0f);
	float Range = 100.0f;

	Vector3 Position = Vector3(0.0f, 0.0f, 0.0f);
	float Power = 1.0f;

	Matrix LightViewProj = Matrix();
};

ALIGNED_MEMORY(16) struct SpotLightData
{
	SpotLightData() = default;
	
	Vector3 Diffuse = Vector3(1.0f, 1.0f, 1.0f);
	float Range = 100.0f;

	Vector3 Position = Vector3(0.0f, 0.0f, 0.0f);
	float OuterCone = 45.0f;

	Vector3 Direction = Vector3(0.0f, -1.0f, 0.0f);
	float AttRange = 55.0f;

	Vector3 Pad;
	float Power = 1.0f;

	Matrix LightViewProj = Matrix();
};

struct MaterialSubData
{
	MaterialSubData() = default;

	Vector4 AddColor = Vector4(0.0f, 0.0f, 0.0, 1.0f);	// Add Color

	float EmissiveFactor = 1.0f;		// Emissive 강도
	float RoughnessFactor = 1.0f;		// Roughness 강도
	float MetallicFactor = 1.0f;		// Metallic 강도

	bool Alpha = false;					// Alpha Mesh
	Matrix TexTM;						// Material의 텍스쳐 행렬
};
#endif // LIGHTHELPER_H