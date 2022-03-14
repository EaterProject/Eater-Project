//***************************************************************************************
// LightHelper.h by Frank Luna (C) 2011 All Rights Reserved.
//
// Helper classes for lighting.
//***************************************************************************************

#ifndef LIGHTHELPER_H
#define LIGHTHELPER_H

#include <Windows.h>
#include "SimpleMath.h"

// Note: Make sure structure alignment agrees with HLSL structure padding rules. 
//   Elements are packed into 4D vectors with the restriction that an element
//   cannot straddle a 4D vector boundary.

using namespace DirectX::SimpleMath;

struct DirectionalLightData
{
	DirectionalLightData() = default;

	Vector4 Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	Vector4 Ambient = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	Vector4 Specular = Vector4(0.8f, 0.8f, 0.8f, 1.0f);
	Vector3 Direction = Vector3(-0.707f, -1.0f, 0.707f);
	float Index; // Pad the last float so we can set an array of lights if we wanted.

	Matrix LightViewProj = Matrix();
};

struct PointLightData
{
	PointLightData() = default;
	
	Vector4 Ambient = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	Vector4 Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	Vector4 Specular = Vector4(0.7f, 0.7f, 0.7f, 1.0f);
	Vector3 Position = Vector3(0.0f, 0.0f, 0.0f);
	float Range = 100.0f;

	Matrix LightViewProj = Matrix();
};

struct SpotLightData
{
	SpotLightData() = default;
	
	Vector4 Ambient = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	Vector4 Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	Vector4 Specular = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	Vector3 Position = Vector3(0.0f, 0.0f, 0.0f);
	float Range = 1000.0f;

	Vector3 Direction = Vector3(0.0f, -1.0f, 0.0f);
	float Spot = 9.6f;

	Vector3 Att = Vector3(0.9f, 0.9f, 0.0f);
	float Index; // Pad the last float so we can set an array of lights if we wanted.
	
	Matrix LightViewProj = Matrix();
};

struct MaterialOption
{
	MaterialOption() = default;

	Vector4 Ambient	= Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	Vector4 Diffuse	= Vector4(0.8f, 0.8f, 0.8f, 1.0f);
	Vector4 Specular = Vector4(0.4f, 0.4f, 0.4f, 1.0f); // w = SpecPower
	Vector4 Reflect	= Vector4(0.4f, 0.4f, 0.4f, 1.0f);

	bool operator==(const MaterialOption& mat)
	{
		return (Ambient == mat.Ambient && Diffuse == mat.Diffuse && Specular == mat.Specular && Reflect == mat.Reflect);
	}
};
#endif // LIGHTHELPER_H