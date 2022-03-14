#include "Input_Header.hlsli"

cbuffer cbShadowStaticMesh : register(b0)
{
    float4x4 gWorld        : packoffset(c0);
    float4x4 gViewProj     : packoffset(c4);
};

float4 Shadow_StaticMesh_VS(ShadowVertexIn vin) : SV_POSITION
{
    float3 posW = mul(gWorld, float4(vin.PosL, 1.0f)).xyz;
    float4 posH = mul(gViewProj, float4(posW, 1.0f));

    return posH;
};