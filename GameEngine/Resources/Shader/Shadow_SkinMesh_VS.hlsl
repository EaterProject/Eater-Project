#ifndef SKIN_MESH
    #define SKIN_MESH
#endif

#include "Input_Header.hlsli"

cbuffer cbShadowSkinMesh : register(b0)
{
    float4x4 gWorldViewProj      : packoffset(c0);
    float4x4 gBoneTransforms[96] : packoffset(c4);
};

float4 Shadow_SkinMesh_VS(MeshPosVertexIn vin) : SV_POSITION
{
    float3 posL = float3(0.0f, 0.0f, 0.0f);

    for (int i = 0; i < 4; ++i)
    {
        posL += vin.BoneWeights1[i] * mul(gBoneTransforms[vin.BoneIndices1[i]], float4(vin.PosL, 1.0f)).xyz;
    }
	
    for (int j = 0; j < 4; ++j)
    {
        posL += vin.BoneWeights2[j] * mul(gBoneTransforms[vin.BoneIndices2[j]], float4(vin.PosL, 1.0f)).xyz;
    }
    
    return mul(gWorldViewProj, float4(posL, 1.0f));
};