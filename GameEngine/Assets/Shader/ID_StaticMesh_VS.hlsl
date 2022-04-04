#include "Input_Header.hlsli"

cbuffer cbStaticMeshID : register(b0)
{
    float4 gHashColor           : packoffset(c0);
    float4x4 gWorldViewProj     : packoffset(c1);
};

MeshIDVertexOut ID_StaticMesh_VS(MeshPosVertexIn vin)
{
    MeshIDVertexOut vout;
    
    vout.PosH = mul(gWorldViewProj, float4(vin.PosL, 1.0f));
    vout.HashColor = gHashColor;
    
    return vout;
};