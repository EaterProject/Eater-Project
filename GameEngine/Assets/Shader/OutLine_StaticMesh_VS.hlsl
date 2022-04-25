#include "Input_Header.hlsli"

cbuffer cbOutLineStaticMesh : register(b0)
{
    float4x4 gWorldViewProj : packoffset(c0);
    float4x4 gInvWorld      : packoffset(c4);
};

cbuffer cbOutLine : register(b1)
{
    float gSize : packoffset(c0.x);
}

float4 OutLine_StaticMesh_VS(MeshVertexIn vin) : SV_POSITION
{
#ifdef OUT_LINE
    vin.PosL += mul((float3x3) gInvWorld, vin.NormalL) * gSize;
#endif
    
    float4 posH = mul(gWorldViewProj, float4(vin.PosL, 1.0f));

    return posH;
}