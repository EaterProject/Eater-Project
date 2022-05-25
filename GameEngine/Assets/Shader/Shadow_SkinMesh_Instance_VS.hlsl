#ifndef SKIN_MESH
    #define SKIN_MESH
#endif

#include "Input_Header.hlsli"
#include "Instance_Header.hlsli"
#include "VertexFetch_Header.hlsli"

StructuredBuffer<BoneAnimation> gAnimationBuffer : register(t0);

cbuffer cbInstanceDepthSkinMesh : register(b0)
{
    float4x4 gViewProj           : packoffset(c0);
};

float4 Shadow_SkinMesh_Instance_VS(MeshPosVertexIn vin, MeshDepthInstanceIn instance) : SV_POSITION
{
    uint nowIndex;
    uint nextIndex;
    
    float4x4 prevBoneTM;
    float4x4 nextBoneTM;
    float4x4 nowBoneTM;
    
    float3 posL = float3(0.0f, 0.0f, 0.0f);
    float3 normalL = float3(0.0f, 0.0f, 0.0f);
    float3 tangentL = float3(0.0f, 0.0f, 0.0f);
    
    for (int i = 0; i < 4; ++i)
    {
        nowIndex = instance.PrevAnimationIndex + vin.BoneIndices1[i];
        nextIndex = instance.NextAnimationIndex + vin.BoneIndices1[i];
        
        prevBoneTM = LoadBoneMatrix(gAnimationBuffer.Load(nowIndex));
        nextBoneTM = LoadBoneMatrix(gAnimationBuffer.Load(nextIndex));

        nowBoneTM = lerp(prevBoneTM, nextBoneTM, instance.FrameTime);

        posL += vin.BoneWeights1[i] * mul(float4(vin.PosL, 1.0f), nowBoneTM).xyz;
    }
    
    for (int j = 0; j < 4; ++j)
    {
        nowIndex = instance.PrevAnimationIndex + vin.BoneIndices2[j];
        nextIndex = instance.NextAnimationIndex + vin.BoneIndices2[j];
        
        prevBoneTM = LoadBoneMatrix(gAnimationBuffer.Load(nowIndex));
        nextBoneTM = LoadBoneMatrix(gAnimationBuffer.Load(nextIndex));
        
        nowBoneTM = lerp(prevBoneTM, nextBoneTM, instance.FrameTime);
        
        posL += vin.BoneWeights2[j] * mul(float4(vin.PosL, 1.0f), prevBoneTM).xyz;
    }
    
    float3 posW = mul(instance.World, float4(posL, 1.0f)).xyz;
    float4 posH = mul(gViewProj, float4(posW, 1.0f));

    return posH;
};