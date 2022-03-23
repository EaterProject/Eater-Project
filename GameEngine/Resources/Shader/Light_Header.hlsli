//***************************************************************************************
// LightHelper.fx by Frank Luna (C) 2011 All Rights Reserved.
//
// Structures and functions for lighting calculations.
//***************************************************************************************

#define DIRECTION_LIGHT_COUNT	1
#define POINT_LIGHT_COUNT		5
#define SPOT_LIGHT_COUNT		5
#define MATERIAL_COUNT			5

struct DirectionalLight
{
    float3 Diffuse;
    float Power;
    
    float3 Direction;
    float Pad;
    
    float4x4 LightViewProj;
};

struct PointLight
{
    float3 Diffuse;
    float Range;
    
    float3 Position;
    float Power;
    
    float4x4 LightViewProj;
};

struct SpotLight
{
    float3 Diffuse;
    float Range;
    
    float3 Position;
    float Power;
    
    float3 Direction;
    float Angle;
    
    float AttStart;
    float AttRange;
    float2 Pad;
    
    float4x4 LightViewProj;
};

struct Material
{
    float4 BaseColor;
    float4 AddColor;
    float EmissiveFactor;
    float RoughnessFactor;
    float MetallicFactor;
};


//---------------------------------------------------------------------------------------
// Transforms a normal map sample to world space.
//---------------------------------------------------------------------------------------
float3 NormalSampleToWorldSpace(float3 normalMapSample, float3 unitNormalW, float3 tangentW)
{
	// Uncompress each component from [0,1] to [-1,1].
    float3 normalT = 2.0f * normalMapSample - 1.0f;

	// Build orthonormal basis.
    float3 N = unitNormalW;
    float3 T = normalize(tangentW - dot(tangentW, N) * N);
    float3 B = cross(N, T);

    float3x3 TBN = float3x3(T, B, N);

	// Transform from tangent space to world space.
    float3 bumpedNormalW = mul(normalT, TBN);

    return bumpedNormalW;
}

//---------------------------------------------------------------------------------------
// Performs shadowmap test to determine if a pixel is in shadow.
//---------------------------------------------------------------------------------------

//static const float SMAP_SIZE = 2048.0f;
//static const float SMAP_DX = 1.0f / SMAP_SIZE;

float CalcShadowFactor(SamplerComparisonState samShadow,
	Texture2D shadowMap,
	float4 shadowPosH)
{
	// Complete projection by doing division by w.
    shadowPosH.xyz /= shadowPosH.w;

	// Depth in NDC space.
    float depth = shadowPosH.z;

	// Texel size.
    //const float dx = SMAP_DX;
    const float dx = 1.0f / 2048.0f;

    float percentLit = 0.0f;
	
    const float2 offsets[9] =
    {
        float2(-dx, -dx), float2(0.0f, -dx), float2(dx, -dx),
		float2(-dx, 0.0f), float2(0.0f, 0.0f), float2(dx, 0.0f),
		float2(-dx, +dx), float2(0.0f, +dx), float2(dx, +dx)
    };

	[unroll]
    for (int i = 0; i < 9; ++i)
    {
        percentLit += shadowMap.SampleCmpLevelZero(samShadow,
			shadowPosH.xy + offsets[i], depth).r;
    }

    return percentLit /= 9.0f;
}
     
static const int2 offset[9] =
{
    int2(-1, -1), int2(0, -1), int2(1, -1),
		int2(-1, 0), int2(0, 0), int2(1, 0),
		int2(-1, +1), int2(0, +1), int2(1, +1)
};

float CalcShadowFactor(SamplerComparisonState samShadow,
	Texture2D shadowMap,
	float3 shadowPosH)
{
	// Depth in NDC space.
    float depth = shadowPosH.z;

    float percentLit = 0.0f;

	[unroll]
    for (int i = 0; i < 9; ++i)
    {
        percentLit += shadowMap.SampleCmpLevelZero(samShadow,
			shadowPosH.xy, depth, offset[i]).r;
    }

    percentLit /= 9.0f;
    
    return percentLit;
}