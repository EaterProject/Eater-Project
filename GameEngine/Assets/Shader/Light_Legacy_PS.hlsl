#include "Output_Header.hlsli"
#include "Function_Header.hlsli"
#include "Light_Header.hlsli"

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
    Material            gMaterials[MATERIAL_COUNT];
    
    uint gDirLightCount;
    uint gPointLightCount;
    uint gSpotLightCount;
};

Texture2D gAlbedoRT     : register(t0);
Texture2D gEmissiveRT   : register(t1);
Texture2D gNormalRT     : register(t2);
Texture2D gPositionRT   : register(t3);

Texture2D gSsaoMap      : register(t4);
Texture2D gShadowMap    : register(t5);

SamplerComparisonState gSamBorderComparisonLinearPoint : register(s0);
SamplerState gSamClampLinear : register(s1);

float4 Light_Legacy_PS(ScreenPixelIn pin) : SV_TARGET
{
    float4 albedoRT = gAlbedoRT.Sample(gSamClampLinear, pin.Tex);
    float4 emissiveRT = gEmissiveRT.Sample(gSamClampLinear, pin.Tex);
    float4 normalRT = gNormalRT.Sample(gSamClampLinear, pin.Tex);
    float4 positionRT = gPositionRT.Sample(gSamClampLinear, pin.Tex);
    float4 shadowRT = mul(gDirLights[0].LightViewProj, float4(positionRT.xyz, 1.0f));
    float4 ssaoRT = mul(gViewProjTex, float4(positionRT.xyz, 1.0f));
	
    
    // Start with a sum of zero.
    float4 albedo = albedoRT;
    float4 emissive = emissiveRT;
    float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);
        
	// View Direction
    float3 ViewDirection = gEyePosW - positionRT.xyz;
    ViewDirection = normalize(ViewDirection);

    // 현재 픽셀의 Shadow 값..
    float shadows = 1.0f;
	
#ifdef SHADOW
    shadowRT.xyz /= shadowRT.w;
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
    albedo = pow(albedo, 2.0f);
    emissive = pow(emissive, 2.0f);
    
	// 현재 픽셀의 Material ID..
    uint matID = round(positionRT.w);

    float4 A, D, S;

	// Directional Light
    [unroll]
    for (uint d = 0; d < gDirLightCount; ++d)
    {
        ComputeDirectionalLight(gMaterials[matID], gDirLights[d], float3(normalRT.xyz), ViewDirection,
			    A, D, S);

        ambient += ao * A;
        diffuse += shadows * D;
        spec += shadows * S;
    }

	// Point Light
    [unroll]
    for (uint p = 0; p < gPointLightCount; ++p)
    {
        ComputePointLight(gMaterials[matID], gPointLights[p], float3(positionRT.xyz), float3(normalRT.xyz), ViewDirection,
				A, D, S);

        ambient += A;
        diffuse += D;
        spec += S;
    }

	// Spot Light
	[unroll]
    for (uint s = 0; s < gSpotLightCount; ++s)
    {
        ComputeSpotLight(gMaterials[matID], gSpotLights[s], float3(positionRT.xyz), float3(normalRT.xyz), ViewDirection,
			A, D, S);

        ambient += A;
        diffuse += D;
        spec += S;
    }

	// Modulate with late add.
    float4 litColor = albedo * (ambient + diffuse) + spec;

    litColor.rgb += emissive.rgb;
	
    // Gamma Correction
	// Linear Space -> Gamma Space
    litColor.rgb = litColor.rgb / (litColor.rgb + float3(1.0f, 1.0f, 1.0f));
    litColor.rgb = pow(litColor.rgb, 0.5f);
    
	// Common to take alpha from diffuse material and texture.
    litColor.a = gMaterials[matID].Diffuse.a * albedo.a;

    return litColor;
}