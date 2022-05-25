#include "SamplerState_Header.hlsli"
#include "Output_Header.hlsli"
#include "OIT_Header.hlsli"
#include "Function_Header.hlsli"
#include "Define_Header.hlsli"
#include "PBR_Header.hlsli"
#include "IBL_Header.hlsli"
#include "Fog_Header.hlsli"

cbuffer cbMaterial : register(b0)
{
    float3 gAddColor        : packoffset(c0.x);
    uint gOption            : packoffset(c0.w);
        
    float3 gLimLightColor   : packoffset(c1.x);
    float gLimLightWidth    : packoffset(c1.w);
    
    float3 gEmissiveColor   : packoffset(c2.x);
    
    float gRoughnessFactor  : packoffset(c2.w);
    float gMetallicFactor   : packoffset(c3.x);
};

cbuffer cbLightSub : register(b1)
{
    float4x4 gViewProjTex : packoffset(c0);
    float3 gEyePosW       : packoffset(c4.x);
    float gIBLFactor      : packoffset(c4.w);
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

[earlydepthstencil]
void OIT_Mesh_PS(MeshPixelIn pin)
{
    float4 albedo       = float4(0.0f, 0.0f, 0.0f, 1.0f);
    float3 emissive     = float3(0.0f, 0.0f, 0.0f);
    float2 rm           = float2(0.0f, 0.0f);

    float3 normalColor  = float3(0.0f, 0.0f, 0.0f);
    float3 normal       = float3(0.0f, 0.0f, 0.0f);
        
	// View Direction
    float3 ViewDirection = normalize(gEyePosW - pin.PosW);
    
    if (gOption & ALBEDO_MAP)
    {
        albedo = gDiffuseMap.Sample(gSamWrapLinear, pin.Tex);
        clip(albedo.a - 0.001f);
    }
    if (gOption & NORMAL_MAP)
    {
        normalColor = gNormalMap.Sample(gSamWrapLinear, pin.Tex).rgb;
        normal = UnpackNormal(normalColor, pin.NormalW, pin.TangentW);
    }
    else
    {
        normal = normalize(pin.NormalW);
    }
    if (gOption & EMISSIVE_MAP)
    {
        emissive += gEmissiveMap.Sample(gSamWrapLinear, pin.Tex).rgb * gEmissiveColor;
    }
    if (gOption & ORM_MAP)
    {
        rm += gORMMap.Sample(gSamWrapLinear, pin.Tex).gb;
    }
    if (gOption & LIM_LIGHT)
    {
        emissive += gLimLightColor * smoothstep(1.0f - gLimLightWidth, 1.0f, 1.0f - max(0, dot(normal, ViewDirection)));
    }
    
    albedo.rgb += gAddColor.rgb;
    
    float roughness = saturate(gRoughnessFactor + rm.x);
    float metallic = saturate(gMetallicFactor + rm.y);

    // 현재 픽셀의 Shadow 값..
    float shadows = 1.0f;
	
#ifdef SHADOW
    float4 shadowPos = mul(gDirLights[0].LightViewProj, float4(pin.PosW, 1.0f));
    shadowPos.xyz /= shadowPos.w;
    shadows = CalcShadowFactor(gSamBorderComparisonLinearPoint, gShadowMap, float3(shadowPos.xyz));
#endif
    
    // Gamma Correction
	// Gamma Space -> Linear Space
    albedo.rgb = pow(albedo.rgb, 2.2f);
    emissive = pow(emissive, 2.2f);
    
    float3 litColor = float3(0.0f, 0.0f, 0.0f);
    litColor += PBR_DirectionalLight(ViewDirection, normal, gDirLights[0],
                                albedo.rgb, 1.0f, roughness, metallic, shadows);

    litColor += PBR_PointLight(ViewDirection, normal, gPointLights, gPointLightCount, pin.PosW,
                                albedo.rgb, 1.0f, roughness, metallic, shadows);
    
    litColor += PBR_SpotLight(ViewDirection, normal, gSpotLights, gSpotLightCount, pin.PosW,
                                albedo.rgb, 1.0f, roughness, metallic, shadows);
    
#ifdef IBL
    float3 irradiance = gIBLIrradiance.Sample(gSamClampLinear, normal).rgb;
    float3 prefilteredColor = gIBLPrefilter.SampleLevel(gSamClampLinear, reflect(-ViewDirection, normal), roughness * MAX_REF_LOD).rgb;
    float2 brdf = gBRDFlut.Sample(gSamClampLinear, float2(max(dot(normal, ViewDirection), 0.0f), roughness)).rg;
    
    litColor += IBL_EnvironmentLight(ViewDirection, normal, irradiance, prefilteredColor, brdf,
                                        albedo.rgb, 1.0f, roughness, metallic, gIBLFactor);
#endif

    litColor += emissive;
    
#ifdef FOG
    litColor = Fog(litColor.rgb, pin.PosW);
#endif
    
    uint pixelCount = gPieceLinkBuffer.IncrementCounter();
    
    uint2 vPos = (uint2) pin.PosH.xy;
    uint startOffsetAddress = 4 * (gFrameWidth * vPos.y + vPos.x);
    uint oldStartOffset;
    gFirstOffsetBuffer.InterlockedExchange(
        startOffsetAddress, pixelCount, oldStartOffset);
    
    float strength = length(litColor);
    float4 color = float4(litColor / strength, albedo.a);
    
    FLStaticNode node;
    node.Data.Strength = strength;
    node.Data.Color = PackColorFromFloat4(color);
    node.Data.Depth = pin.PosH.z;
    node.Next = oldStartOffset;
    
    gPieceLinkBuffer[pixelCount] = node;
}
