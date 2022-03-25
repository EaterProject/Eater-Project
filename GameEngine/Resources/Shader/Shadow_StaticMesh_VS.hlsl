#include "Input_Header.hlsli"

cbuffer cbShadowStaticMesh : register(b0)
{
    float4x4 gWorldViewProj     : packoffset(c0);
};

float4 Shadow_StaticMesh_VS(MeshPosVertexIn vin) : SV_POSITION
{
    return mul(gWorldViewProj, float4(vin.PosL, 1.0f));
};