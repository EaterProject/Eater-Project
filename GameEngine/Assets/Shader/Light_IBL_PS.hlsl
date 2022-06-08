#include "SamplerState_Header.hlsli"
#include "Output_Header.hlsli"
#include "Function_Header.hlsli"
#include "PBR_Header.hlsli"
#include "IBL_Header.hlsli"
#include "Fog_Header.hlsli"

cbuffer cbLightSub : register(b0)
{
    float4x4 gViewProjTex   : packoffset(c0);
    float3 gEyePosW         : packoffset(c4.x);
    float gIBLFactor        : packoffset(c4.w);
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

Texture2D gBRDFlut          : register(t6);
TextureCube gIrradiance_0   : register(t7);
TextureCube gPrefilter_0    : register(t8);
TextureCube gIrradiance_1   : register(t9);
TextureCube gPrefilter_1    : register(t10);

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
    
    uint sky = emissiveRT.w;
    float roughness = normalRT.w;
    float metallic = positionRT.w;
    
    if (albedoRT.a <= 0.0001f)
        discard;
    
	// View Direction
    float3 ViewDirection = normalize(gEyePosW - positionRT.xyz);

    // 현재 픽셀의 Shadow 값..
    float shadows = 1.0f;
	
#ifdef SHADOW
    shadowRT.xyz /= shadowRT.w;
    shadows = CalcShadowFactor(gSamBorderComparisonLinearPoint, gShadowMap, float3(shadowRT.xyz));
#endif	
	
	// 현재 픽셀의 SSAO 값..
    float ao = 1.0f;
	
#ifdef SSAO
    ssaoRT.xy /= ssaoRT.w;
    ao = gSsaoMap.SampleLevel(gSamClampLinear, ssaoRT.xy, 0.0f).r;
#endif	
        
    // Gamma Correction
	// Gamma Space -> Linear Space
    albedo = pow(albedo, 2.2f);
    emissive = pow(emissive, 2.2f);
    
    float3 litColor = emissive;
    litColor += PBR_DirectionalLight(ViewDirection, normal, gDirLights[0],
                                albedo, ao, roughness, metallic, shadows);

    litColor += PBR_PointLight(ViewDirection, normal, gPointLights, gPointLightCount, positionRT.xyz,
                                albedo, ao, roughness, metallic, shadows);
    
    litColor += PBR_SpotLight(ViewDirection, normal, gSpotLights, gSpotLightCount, positionRT.xyz,
                                albedo, ao, roughness, metallic, shadows);
    
    float2 brdf = gBRDFlut.Sample(gSamClampLinear, float2(max(dot(normal, ViewDirection), 0.0f), roughness * roughness)).rg;

    switch (sky)
    {
    case 0:
    {
        float3 irradiance = gIrradiance_0.Sample(gSamClampLinear, normal).rgb;
        float3 prefilteredColor = gPrefilter_0.SampleLevel(gSamClampLinear, reflect(-ViewDirection, normal), roughness * roughness * MAX_REF_LOD).rgb;
        
        litColor += IBL_EnvironmentLight(ViewDirection, normal, irradiance, prefilteredColor, brdf,
                                albedo, ao, roughness, metallic, gIBLFactor);
                        
#ifdef FOG
        litColor = Fog(litColor, positionRT.xyz);
#endif
    }
    break;
    case 1:
    {
        float3 irradiance = gIrradiance_1.Sample(gSamClampLinear, normal).rgb;
        float3 prefilteredColor = gPrefilter_1.SampleLevel(gSamClampLinear, reflect(-ViewDirection, normal), roughness * roughness * MAX_REF_LOD).rgb;
        
        litColor += IBL_EnvironmentLight(ViewDirection, normal, irradiance, prefilteredColor, brdf,
                                albedo, ao, roughness, metallic, gIBLFactor);
    }
    break;
    default:
        break;
    }

    return float4(litColor, 1.0f);
}