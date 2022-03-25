#include "Input_Header.hlsli"
#include "Instance_Header.hlsli"

cbuffer cbInstanceShadowStaticMesh : register(b0)
{
    float4x4 gViewProj     : packoffset(c0);
};

float4 Shadow_StaticMesh_Instance_VS(MeshPosVertexIn vin, MeshInstanceIn instance) : SV_POSITION
{
    float3 posW = mul(instance.World, float4(vin.PosL, 1.0f)).xyz;

    return mul(gViewProj, float4(posW, 1.0f));
};