#include "Light_Header.hlsli"

#define ALBEDO_MAP 0x00000001
#define NORMAL_MAP 0x00000010

cbuffer cbLightSub : register(b0)
{
    float3 gEyePosW         : packoffset(c0);
    float4x4 gViewProjTex   : packoffset(c3);
};

cbuffer cbLight : register(b1)
{
    DirectionalLight gDirLights;
    PointLight gPointLights[5];
    SpotLight gSpotLights[5];
    Material gMaterials[5];

    uint gPointLightCount;
    uint gSpotLightCount;
};

cbuffer cbMaterial : register(b2)
{
    float4 gColor : packoffset(c0);
    uint gMatID : packoffset(c1.x);
    uint gTexID : packoffset(c1.y);
};

Texture2D gDiffuseMap   : register(t0);
Texture2D gNormalMap    : register(t1);
Texture2D gShadowMap    : register(t2);

SamplerComparisonState gSamBorderComparisonLinearPoint : register(s0);
SamplerState gSamWrapLinear : register(s1);

struct PixelIn
{
    float4 PosH : SV_POSITION;
    float3 PosW : POSITIONW;
    float3 PosV : POSITIONV;
    float3 NormalW : NORMALW;
    float3 NormalV : NORMALV;
    float2 Tex : TEXCOORD;
    float3 ShadowPosH : POS_SHADOW;
    
    float3x3 TBN : TANGENT;
};

float4 Forward_PS(PixelIn pin) : SV_Target0
{
    float4 albedo = gColor;
    float3 normal = pin.NormalW;
    
    if (gTexID & ALBEDO_MAP)
    {
        albedo = gDiffuseMap.Sample(gSamWrapLinear, pin.Tex);
    }
    
    if (gTexID & NORMAL_MAP)
    {
        float3 normalMapSample = 2.0f * gNormalMap.Sample(gSamWrapLinear, pin.Tex).rgb - 1.0f;
        normal = mul(normalMapSample, pin.TBN);
    }
    
    // Gamma Correction
	// Gamma Space -> Linear Space
	// 모든 라이팅 연산은 선형 공간에서 이루어져야 한다..
    albedo.rgb = pow(albedo.rgb, 2.2);

    // Shadow
    float shadows = CalcShadowFactor(gSamBorderComparisonLinearPoint, gShadowMap, pin.ShadowPosH);
    
    // View Direction
    float3 ViewDirection = gEyePosW - pin.PosW;
    ViewDirection = normalize(ViewDirection);
    
    // Start with a sum of zero.
    float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    float4 A, D, S;
    
	float4 litColor = albedo;
    
    // Directional Light
    ComputeDirectionalLight(gMaterials[gMatID], gDirLights, normal, ViewDirection,
	                        A, D, S);
    
    ambient += A;
    diffuse += shadows * D;
    spec += shadows * S;
    
    // Point Light
    if (gPointLightCount > 0)
    {
			[unroll]
        for (uint i = 0; i < gPointLightCount; ++i)
        {
            ComputePointLight(gMaterials[gMatID], gPointLights[i], pin.PosW.xyz, normal, ViewDirection,
			                  A, D, S);
    
            ambient += A;
            diffuse += D;
            spec += S;
        }
    }
    
    // Spot Light
    if (gSpotLightCount > 0)
    {
		[unroll]
        for (uint i = 0; i < gSpotLightCount; ++i)
        {
            ComputeSpotLight(gMaterials[gMatID], gSpotLights[i], pin.PosW.xyz, normal, ViewDirection,
					         A, D, S);
    
            ambient += A;
            diffuse += D;
            spec += S;
        }
    }
    
    // Modulate with late add.
    litColor = albedo * (ambient + diffuse) + spec;
    
    // Common to take alpha from diffuse material and texture.
    litColor.a = gMaterials[gMatID].Diffuse.a * albedo.a;
    
    if (gTexID & NORMAL_MAP)
    {
        return litColor;  
    }
    else
    {
        litColor.rgb = pow(litColor.rgb, 1.0f / 2.2f);
        return litColor;  
    }
}