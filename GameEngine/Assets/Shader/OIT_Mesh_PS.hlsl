#include "Output_Header.hlsli"
#include "OIT_Header.hlsli"
#include "Function_Header.hlsli"
#include "Define_Header.hlsli"
#include "PBR_Header.hlsli"
#include "IBL_Header.hlsli"

cbuffer cbMaterial : register(b0)
{
    float4 gAddColor : packoffset(c0);
    float gEmissiveFactor : packoffset(c1.x);
    float gRoughnessFactor : packoffset(c1.y);
    float gMetallicFactor : packoffset(c1.z);
    uint gTexID : packoffset(c1.w);
};

cbuffer cbLightSub : register(b1)
{
    float4x4 gViewProjTex : packoffset(c0);
    float3 gEyePosW : packoffset(c4.x);
    uint gInt : packoffset(c4.w);
}

cbuffer cbLight : register(b2)
{
    DirectionalLight gDirLights[DIRECTION_LIGHT_COUNT];
    PointLight gPointLights[POINT_LIGHT_COUNT];
    SpotLight gSpotLights[SPOT_LIGHT_COUNT];
    
    uint gDirLightCount;
    uint gPointLightCount;
    uint gSpotLightCount;
};

RWStructuredBuffer<FLStaticNode> gPieceLinkBuffer : register(u0);
RWByteAddressBuffer gFirstOffsetBuffer : register(u1);

Texture2D gDiffuseMap       : register(t0);
Texture2D gNormalMap        : register(t1);
Texture2D gEmissiveMap      : register(t2);
Texture2D gORMMap           : register(t3);

Texture2D gShadowMap        : register(t4);

TextureCube gIBLIrradiance  : register(t5);
TextureCube gIBLPrefilter   : register(t6);
Texture2D gBRDFlut          : register(t7);

SamplerComparisonState gSamBorderComparisonLinearPoint : register(s0);
SamplerState gSamWrapLinear : register(s1);
SamplerState gSamClampLinear : register(s2);

[earlydepthstencil]
void OIT_Mesh_PS(MeshPixelIn pin)
{
    float4 albedo = float4(0.0f, 0.0f, 0.0f, 1.0f);
    float4 emissive = float4(0.0f, 0.0f, 0.0f, 1.0f);
    float3 orm = float3(0.0f, 0.0f, 0.0f);

    float3 normalColor = float3(0.0f, 0.0f, 0.0f);
    float3 normal = float3(0.0f, 0.0f, 0.0f);
    
    float roughness = gRoughnessFactor;
    float metallic = gMetallicFactor;
    
    if (gTexID & ALBEDO_MAP)
    {
        albedo = gDiffuseMap.Sample(gSamWrapLinear, pin.Tex);
    }
    else
    {
        albedo = gAddColor;
    }
    
    clip(albedo.a - 0.1f);
    
    if (gTexID & NORMAL_MAP)
    {
        normalColor = gNormalMap.Sample(gSamWrapLinear, pin.Tex).rgb;
        normal = UnpackNormal(normalColor, pin.NormalW, pin.TangentW);
    }
    else
    {
        normal = normalize(pin.NormalW);
    }
    if (gTexID & EMISSIVE_MAP)
    {
        emissive = gEmissiveMap.Sample(gSamWrapLinear, pin.Tex) * gEmissiveFactor;
    }
    if (gTexID & ORM_MAP)
    {
        orm += gORMMap.Sample(gSamWrapLinear, pin.Tex).rgb;
        roughness += orm.g;
        metallic += orm.b;
    }
    
	// View Direction
    float3 ViewDirection = normalize(gEyePosW - pin.PosW);
    
    // 현재 픽셀의 Shadow 값..
    float4 shadowPos = mul(gDirLights[0].LightViewProj, float4(pin.PosW, 1.0f));
    float shadows = 1.0f;
	
#ifdef SHADOW
    shadows = CalcShadowFactor(gSamBorderComparisonLinearPoint, gShadowMap, float3(shadowPos.xyz));
#endif
    
    // Gamma Correction
	// Gamma Space -> Linear Space
    albedo.rgb = pow(albedo.rgb, 2.2f);
    emissive.rgb = pow(emissive.rgb, 2.2f);
    
    float4 litColor = float4(0.0f, 0.0f, 0.0f, albedo.a);
    litColor.rgb += PBR_DirectionalLight(ViewDirection, normal, gDirLights[0],
                                albedo.rgb, 1.0f, roughness, metallic, shadows);

    litColor.rgb += PBR_PointLight(ViewDirection, normal, gPointLights, gPointLightCount, pin.PosW,
                                albedo.rgb, 1.0f, roughness, metallic, shadows);
    
    litColor.rgb += PBR_SpotLight(ViewDirection, normal, gSpotLights, gSpotLightCount, pin.PosW,
                                albedo.rgb, 1.0f, roughness, metallic, shadows);
    
    float3 irradiance = gIBLIrradiance.Sample(gSamClampLinear, normal).rgb;
    float3 prefilteredColor = gIBLPrefilter.SampleLevel(gSamClampLinear, reflect(-ViewDirection, normal), roughness * MAX_REF_LOD).rgb;
    float2 brdf = gBRDFlut.Sample(gSamClampLinear, float2(max(dot(normal, ViewDirection), 0.0f), roughness)).rg;
    
    litColor.rgb += IBL_EnvironmentLight(ViewDirection, normal, irradiance, prefilteredColor, brdf,
                                        albedo.rgb, 1.0f, roughness, metallic);

    uint pixelCount = gPieceLinkBuffer.IncrementCounter();
    
    uint2 vPos = (uint2) pin.PosH.xy;
    uint startOffsetAddress = 4 * (gFrameWidth * vPos.y + vPos.x);
    uint oldStartOffset;
    gFirstOffsetBuffer.InterlockedExchange(
        startOffsetAddress, pixelCount, oldStartOffset);
    
    FLStaticNode node;
    node.Data.Color = PackColorFromFloat4(litColor);
    node.Data.Depth = pin.PosH.z;
    node.Next = oldStartOffset;
    
    gPieceLinkBuffer[pixelCount] = node;
}
