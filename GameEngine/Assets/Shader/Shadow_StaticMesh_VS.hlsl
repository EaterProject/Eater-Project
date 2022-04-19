#include "Input_Header.hlsli"

cbuffer cbDepthStaticMesh : register(b0)
{
    float4x4 gWorldViewProj     : packoffset(c0);
};

float4 Shadow_StaticMesh_VS(MeshPosVertexIn vin) : SV_POSITION
{
    float4 posH = mul(gWorldViewProj, float4(vin.PosL, 1.0f));
    
    return posH;
};