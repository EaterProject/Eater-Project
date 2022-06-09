#include "SamplerState_Header.hlsli"
#include "Output_Header.hlsli"
#include "Function_Header.hlsli"
#include "Define_Header.hlsli"

cbuffer cbMaterial : register(b0)
{
    float3 gAddColor        : packoffset(c0.x);
    uint gOption            : packoffset(c0.w);
        
    float3 gLimLightColor   : packoffset(c1.x);
    float gLimLightWidth    : packoffset(c1.w);
    
    float3 gEmissiveColor   : packoffset(c2.x);
    
    float gRoughnessFactor  : packoffset(c2.w);
    float gMetallicFactor   : packoffset(c3.x);
    
    uint gSkyLightIndex     : packoffset(c3.y);
};

cbuffer cbCamera : register(b1)
{
    float3 gEyePos : packoffset(c0.x);
    float gPad     : packoffset(c0.w);
}

#ifdef TERRAIN_MESH
Texture2D gDiffuseLayer1    : register(t0);
Texture2D gDiffuseLayer2    : register(t1);
Texture2D gDiffuseLayer3    : register(t2);
Texture2D gDiffuseLayer4    : register(t3);
Texture2D gNormalLayer1     : register(t4);
Texture2D gNormalLayer2     : register(t5);
Texture2D gNormalLayer3     : register(t6);
Texture2D gNormalLayer4     : register(t7);
Texture2D gORMLayer1        : register(t8);
Texture2D gORMLayer2        : register(t9);
Texture2D gORMLayer3        : register(t10);
Texture2D gORMLayer4        : register(t11);
#else
Texture2D gDiffuseMap       : register(t0);
Texture2D gNormalMap        : register(t1);
Texture2D gEmissiveMap      : register(t2);
Texture2D gORMMap           : register(t3);
#endif

[earlydepthstencil]
MeshPixelOut Deferred_PBR_PS(MeshPixelIn pin)
{
    MeshPixelOut pout;
    
    float4 albedo       = float4(0.0f, 0.0f, 0.0f, 1.0f);
    float3 emissive     = float3(0.0f, 0.0f, 0.0f);
    float2 rm           = float2(0.0f, 0.0f);
    
    float3 normalColor  = float3(0.0f, 0.0f, 0.0f);
    float3 normalW      = float3(0.0f, 0.0f, 0.0f);
    float3 normalV      = float3(0.0f, 0.0f, 0.0f);
    
#ifdef TERRAIN_MESH
    float4 mask = pin.MaskColor;
    float2 tex;
    
    if (mask.r > 0.0f)
    {
        tex = pin.Tex * 1.5f;
        albedo.rgb += gDiffuseLayer1.Sample(gSamWrapAnisotropic, tex).rgb * mask.r;
        normalColor += gNormalLayer1.Sample(gSamWrapAnisotropic, tex).rgb * mask.r;
        rm += gORMLayer1.Sample(gSamWrapLinear, tex).gb * mask.r;
    }
    if (mask.g > 0.0f)
    {
        tex = pin.Tex * 5.0f;
        albedo.rgb += gDiffuseLayer2.Sample(gSamWrapAnisotropic, tex).rgb * mask.g;
        normalColor += gNormalLayer2.Sample(gSamWrapAnisotropic, tex).rgb * mask.g;
        rm += gORMLayer2.Sample(gSamWrapLinear, tex).gb * mask.g;
    }
    if (mask.b > 0.0f)
    {
        tex = pin.Tex * 6.0f;
        albedo.rgb += gDiffuseLayer3.Sample(gSamWrapAnisotropic, tex).rgb * mask.b;
        normalColor += gNormalLayer3.Sample(gSamWrapAnisotropic, tex).rgb * mask.b;
        rm += gORMLayer3.Sample(gSamWrapLinear, tex).gb * mask.b;
    }
    if (mask.a > 0.0f)
    {
        tex = pin.Tex * 3.0f;
        albedo.rgb += gDiffuseLayer4.Sample(gSamWrapAnisotropic, tex).rgb * mask.a;
        normalColor += gNormalLayer4.Sample(gSamWrapAnisotropic, tex).rgb * mask.a;
        rm += gORMLayer4.Sample(gSamWrapLinear, tex).gb * mask.a;
    }
    
    normalW = UnpackNormal(normalColor, pin.NormalW, pin.TangentW);
    normalV = UnpackNormal(normalColor, pin.NormalV, pin.TangentV);
#else
    if (gOption & ALBEDO_MAP)
    {
        albedo = gDiffuseMap.Sample(gSamWrapLinear, pin.Tex);

        clip(albedo.a - 0.1f);
    }
    if (gOption & NORMAL_MAP)
    {
        normalColor = gNormalMap.Sample(gSamWrapLinear, pin.Tex).rgb;
        normalW = UnpackNormal(normalColor, pin.NormalW, pin.TangentW);
        normalV = UnpackNormal(normalColor, pin.NormalV, pin.TangentV);
    }
    else
    {
        normalW = normalize(pin.NormalW);
        normalV = normalize(pin.NormalV);
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
        emissive += gLimLightColor * smoothstep(1.0f - gLimLightWidth, 1.0f, 1.0f - max(0, dot(normalW, normalize(gEyePos - pin.PosW))));
    }
#endif
    
    albedo.rgb += gAddColor.rgb;
    rm.x += gRoughnessFactor;
    rm.y += gMetallicFactor;
    
    pout.Albedo = albedo;
    pout.Emissive = float4(emissive, gSkyLightIndex);
    pout.Normal = float4(normalW, saturate(rm.x));
    pout.Position = float4(pin.PosW, saturate(rm.y));
    pout.NormalDepth = float4(normalV, pin.PosV.z);
    
    return pout;
}

