#include "Output_Header.hlsli"
#include "Function_Header.hlsli"

#define ALBEDO_MAP      0x00000001
#define NORMAL_MAP      0x00000010
#define EMISSIVE_MAP    0x00000100
#define ORM_MAP         0x00001000

cbuffer cbMaterial : register(b0)
{
    float4 gColor   : packoffset(c0);
    uint gTexID     : packoffset(c1.x);
};

#ifdef TERRAIN_MESH
Texture2D gDiffuseLayer1    : register(t0);
Texture2D gDiffuseLayer2    : register(t1);
Texture2D gNormalLayer1     : register(t2);
Texture2D gNormalLayer2     : register(t3);
Texture2D gORMLayer1        : register(t4);
Texture2D gORMLayer2        : register(t5);
#else
Texture2D gDiffuseMap       : register(t0);
Texture2D gNormalMap        : register(t1);
Texture2D gEmissiveMap      : register(t2);
Texture2D gORMMap           : register(t3);
#endif

SamplerState gSamWrapLinear      : register(s0);
SamplerState gSamWrapAnisotropic : register(s1);

[earlydepthstencil]
MeshPixelOut Deferred_PBR_PS(MeshPixelIn pin)
{
    MeshPixelOut pout;

    float4 albedo       = float4(0.0f, 0.0f, 0.0f, 1.0f);
    float4 emissive     = float4(0.0f, 0.0f, 0.0f, 1.0f);
    float3 orm          = float3(0.0f, 1.0f, 0.0f);
    float3 normalColor  = float3(0.0f, 0.0f, 0.0f);
    float3 normalW      = float3(0.0f, 0.0f, 0.0f);
    float3 normalV      = float3(0.0f, 0.0f, 0.0f);
    
#ifdef TERRAIN_MESH
    float3 mask = pin.MaskColor.rgb;
    orm = float3(0.0f,0.0f,0.0f);
    
    if (mask.r > 0.0f)
    {
        albedo.rgb += gDiffuseLayer1.Sample(gSamWrapAnisotropic, pin.Tex).rgb * mask.r;
        normalColor += gNormalLayer1.Sample(gSamWrapAnisotropic, pin.Tex).rgb * mask.r;
        orm += gORMLayer1.Sample(gSamWrapLinear, pin.Tex).rgb * mask.r;
    }
    if (mask.g > 0.0f)
    {
        albedo.rgb += gDiffuseLayer2.Sample(gSamWrapAnisotropic, pin.Tex).rgb * mask.g;
        normalColor += gNormalLayer2.Sample(gSamWrapAnisotropic, pin.Tex).rgb* mask.g;
        orm += gORMLayer2.Sample(gSamWrapLinear, pin.Tex).rgb * mask.g;
    }
    
    normalW = UnpackNormal(normalColor, pin.NormalW, pin.TangentW);
    normalV = UnpackNormal(normalColor, pin.NormalV, pin.TangentV);
#else
    if (gTexID & ALBEDO_MAP)
    {
        albedo = gDiffuseMap.Sample(gSamWrapLinear, pin.Tex);
    }
    else
    {
        albedo = gColor;
    }
    if (gTexID & NORMAL_MAP)
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
    if (gTexID & EMISSIVE_MAP)
    {
        emissive = gEmissiveMap.Sample(gSamWrapLinear, pin.Tex);
    }
    if (gTexID & ORM_MAP)
    {
        orm = gORMMap.Sample(gSamWrapLinear, pin.Tex).rgb;
    }
#endif
    
    pout.Albedo = albedo;
    pout.Emissive = emissive;
    pout.Normal = float4(normalW, orm.g);
    pout.Position = float4(pin.PosW, orm.b);
    pout.NormalDepth = float4(normalV, pin.PosV.z);
    
    return pout;
}

