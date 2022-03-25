#ifndef SKIN_MESH
    #define SKIN_MESH
#endif

#include "Input_Header.hlsli"

cbuffer cbSkinMeshID : register(b0)
{
    float4 gHashColor            : packoffset(c0);
    float4x4 gWorldViewProj      : packoffset(c1);
    float4x4 gBoneTransforms[96] : packoffset(c5);
};

MeshIDVertexOut ID_SkinMesh_VS(MeshPosVertexIn vin)
{
    MeshIDVertexOut vout;
    
    float3 posL = float3(0.0f, 0.0f, 0.0f);

    for (int i = 0; i < 4; ++i)
    {
        posL += vin.BoneWeights1[i] * mul(gBoneTransforms[vin.BoneIndices1[i]], float4(vin.PosL, 1.0f)).xyz;
    }
	
    for (int j = 0; j < 4; ++j)
    {
        posL += vin.BoneWeights2[j] * mul(gBoneTransforms[vin.BoneIndices2[j]], float4(vin.PosL, 1.0f)).xyz;
    }
    
    vout.PosH = mul(gWorldViewProj, float4(posL, 1.0f));
    vout.HashColor = gHashColor;
    
    return vout;
};