#include "Output_Header.hlsli"
#include "Function_Header.hlsli"
#include "PBR_Header.hlsli"
#include "IBL_Header.hlsli"

cbuffer cbLightSub : register(b0)
{
    float4x4 gViewProjTex   : packoffset(c0);
    float3 gEyePosW         : packoffset(c4.x);
    uint gInt               : packoffset(c4.w);
}

cbuffer cbLight : register(b1)
{
    DirectionalLight    gDirLights[DIRECTION_LIGHT_COUNT];
    PointLight          gPointLights[POINT_LIGHT_COUNT];
    SpotLight           gSpotLights[SPOT_LIGHT_COUNT];
    
    uint gDirLightCount;
    uint gPointLightCount;
    uint gSpotLightCount;
};

Texture2D gAlbedoRT         : register(t0);
Texture2D gEmissiveRT       : register(t1);
Texture2D gNormalRT         : register(t2);
Texture2D gPositionRT       : register(t3);

Texture2D gSsaoMap          : register(t4);
Texture2D gShadowMap        : register(t5);

TextureCube gIBLIrradiance  : register(t6);
TextureCube gIBLPrefilter   : register(t7);
Texture2D gBRDFlut          : register(t8);

SamplerComparisonState gSamBorderComparisonLinearPoint : register(s0);
SamplerState gSamClampLinear : register(s1);

float4 Light_IBL_PS(ScreenPixelIn pin) : SV_TARGET
{
    float4 albedoRT = gAlbedoRT.Sample(gSamClampLinear, pin.Tex);
    float4 emissiveRT = gEmissiveRT.Sample(gSamClampLinear, pin.Tex);
    float4 normalRT = gNormalRT.Sample(gSamClampLinear, pin.Tex);
    float4 positionRT = gPositionRT.Sample(gSamClampLinear, pin.Tex);
    float4 shadowRT = mul(gDirLights[0].LightViewProj, float4(positionRT.xyz, 1.0f));
    float4 ssaoRT = mul(gViewProjTex, float4(positionRT.xyz, 1.0f));
	
    float3 albedo = albedoRT.rgb;
    float3 emissive = emissiveRT.rgb;
    float3 normal = normalRT.xyz;
    
    float roughness = normalRT.w;
    float metallic = positionRT.w;
    
    if (any(normal) == false)
        discard;
    
	// View Direction
    float3 ViewDirection = normalize(gEyePosW - positionRT.xyz);

    // 현재 픽셀의 Shadow 값..
    float shadows = 1.0f;
	
#ifdef SHADOW
    shadows = CalcShadowFactor(gSamBorderComparisonLinearPoint, gShadowMap, float3(shadowRT.xyz));
#endif	
	
	// 현재 픽셀의 SSAO 값..
    float ao = 1.0f;
	
#ifdef SSAO
    ssaoRT /= ssaoRT.w;
    ao = gSsaoMap.SampleLevel(gSamClampLinear, ssaoRT.xy, 0.0f).r;
#endif	
        
    // Gamma Correction
	// Gamma Space -> Linear Space
    albedo = pow(albedo, 2.2f);
    emissive = pow(emissive, 2.2f);
    
    float3 litColor = float3(0.0f, 0.0f, 0.0f);
    litColor += PBR_DirectionalLight(ViewDirection, normal, gDirLights[0],
                                albedo, ao, roughness, metallic, shadows);

    litColor += PBR_PointLight(ViewDirection, normal, gPointLights, gPointLightCount, positionRT.xyz,
                                albedo, ao, roughness, metallic, shadows);
    
    litColor += PBR_SpotLight(ViewDirection, normal, gSpotLights, gSpotLightCount, positionRT.xyz,
                                albedo, ao, roughness, metallic, shadows);
    
    float3 irradiance = gIBLIrradiance.Sample(gSamClampLinear, normal).rgb;
    float3 prefilteredColor = gIBLPrefilter.SampleLevel(gSamClampLinear, reflect(-ViewDirection, normal), roughness * MAX_REF_LOD).rgb;
    float2 brdf = gBRDFlut.Sample(gSamClampLinear, float2(max(dot(normal, ViewDirection), 0.0f), roughness)).rg;
    
    litColor += IBL_EnvironmentLight(ViewDirection, normal, irradiance, prefilteredColor, brdf, 
                                        albedo, ao, roughness, metallic);

    litColor += emissive * 7.0f;
    
    return float4(litColor, 1.0f);
}