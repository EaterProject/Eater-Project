#include "Input_Header.hlsli"
#include "Instance_Header.hlsli"

cbuffer cbInstanceSkinMeshID : register(b0)
{
    float4x4 gViewProj     : packoffset(c0);
};

MeshIDVertexOut ID_StaticMesh_Instance_VS(MeshPosVertexIn vin, MeshIDInstanceIn instance)
{
    MeshIDVertexOut vout;
   
    float3 posW = mul(instance.World, float4(vin.PosL, 1.0f)).xyz;

    vout.PosH = mul(gViewProj, float4(posW, 1.0f));
    vout.HashColor = instance.HashColor;
    
    return vout;
};