#include "Input_Header.hlsli"

cbuffer cbOutLineStaticMesh : register(b0)
{
    float4x4 gWorldViewProj : packoffset(c0);
    float4x4 gWorld : packoffset(c4);
};

cbuffer cbOutLine : register(b1)
{
    float gSize : packoffset(c0.x);
}

float4 OutLine_StaticMesh_VS(MeshVertexIn vin) : SV_POSITION
{
    float4 posH = mul(gWorldViewProj, float4(vin.PosL, 1.0f));

#ifdef OUT_LINE
    posH.xyz += mul((float3x3) gWorld, vin.NormalL).xyz * 0.25f * gSize;
#endif
    
    return posH;
}