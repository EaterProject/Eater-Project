#ifndef SKIN_MESH
    #define SKIN_MESH
#endif

#include "Input_Header.hlsli"
#include "VertexFetch_Header.hlsli"

StructuredBuffer<BoneAnimation> gAnimationBuffer : register(t0);

cbuffer cbSkinMeshID : register(b0)
{
    float4x4 gWorldViewProj      : packoffset(c0);
    float4 gHashColor            : packoffset(c4);
    uint gPrevAnimationIndex     : packoffset(c5.x);
    uint gNextAnimationIndex     : packoffset(c5.y);
    float gFrameTime             : packoffset(c5.z);
    float gPad                   : packoffset(c5.w);
};

MeshIDVertexOut ID_SkinMesh_VS(MeshPosVertexIn vin)
{
    MeshIDVertexOut vout;
    
    uint nowIndex;
    uint nextIndex;
    
    float4x4 prevBoneTM;
    float4x4 nextBoneTM;
    float4x4 nowBoneTM;
    
    float3 posL = float3(0.0f, 0.0f, 0.0f);
    
    for (int i = 0; i < 4; ++i)
    {
        nowIndex = gPrevAnimationIndex + vin.BoneIndices1[i];
        nextIndex = gNextAnimationIndex + vin.BoneIndices1[i];
        
        prevBoneTM = LoadBoneMatrix(gAnimationBuffer.Load(nowIndex));
        nextBoneTM = LoadBoneMatrix(gAnimationBuffer.Load(nextIndex));

        nowBoneTM = lerp(prevBoneTM, nextBoneTM, gFrameTime);

        posL += vin.BoneWeights1[i] * mul(float4(vin.PosL, 1.0f), nowBoneTM).xyz;
    }
    
    for (int j = 0; j < 4; ++j)
    {
        nowIndex = gPrevAnimationIndex + vin.BoneIndices2[j];
        nextIndex = gNextAnimationIndex + vin.BoneIndices2[j];
        
        prevBoneTM = LoadBoneMatrix(gAnimationBuffer.Load(nowIndex));
        nextBoneTM = LoadBoneMatrix(gAnimationBuffer.Load(nextIndex));
        
        nowBoneTM = lerp(prevBoneTM, nextBoneTM, gFrameTime);
        
        posL += vin.BoneWeights2[j] * mul(float4(vin.PosL, 1.0f), prevBoneTM).xyz;
    }
    
    vout.PosH = mul(gWorldViewProj, float4(posL, 1.0f));
    vout.HashColor = gHashColor;
    
    return vout;
};