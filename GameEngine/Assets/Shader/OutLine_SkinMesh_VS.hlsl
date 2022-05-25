#ifndef SKIN_MESH
    #define SKIN_MESH
#endif

#include "Input_Header.hlsli"
#include "VertexFetch_Header.hlsli"

StructuredBuffer<BoneAnimation> gAnimationBuffer : register(t0);

cbuffer cbOutLineSkinMesh : register(b0)
{
    float4x4 gWorldViewProj     : packoffset(c0);
    float4x4 gWorld             : packoffset(c4);
    uint gPrevAnimationIndex    : packoffset(c8.x);
    uint gNextAnimationIndex    : packoffset(c8.y);
    float gFrameTime            : packoffset(c8.z);
    float gPad                  : packoffset(c8.w);
};

cbuffer cbOutLine : register(b1)
{
    float gSize : packoffset(c0.x);
}

float4 OutLine_SkinMesh_VS(MeshVertexIn vin) : SV_POSITION
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
        nowIndex = gPrevAnimationIndex + vin.BoneIndices1[i];
        nextIndex = gNextAnimationIndex + vin.BoneIndices1[i];
        
        prevBoneTM = LoadBoneMatrix(gAnimationBuffer.Load(nowIndex));
        nextBoneTM = LoadBoneMatrix(gAnimationBuffer.Load(nextIndex));

        nowBoneTM = lerp(prevBoneTM, nextBoneTM, gFrameTime);

        posL += vin.BoneWeights1[i] * mul(float4(vin.PosL, 1.0f), nowBoneTM).xyz;
        normalL += vin.BoneWeights1[i] * mul(vin.NormalL, (float3x3) nowBoneTM);
    }
    
    for (int j = 0; j < 4; ++j)
    {
        nowIndex = gPrevAnimationIndex + vin.BoneIndices2[j];
        nextIndex = gNextAnimationIndex + vin.BoneIndices2[j];
        
        prevBoneTM = LoadBoneMatrix(gAnimationBuffer.Load(nowIndex));
        nextBoneTM = LoadBoneMatrix(gAnimationBuffer.Load(nextIndex));
        
        nowBoneTM = lerp(prevBoneTM, nextBoneTM, gFrameTime);
        
        posL += vin.BoneWeights2[j] * mul(float4(vin.PosL, 1.0f), prevBoneTM).xyz;
        normalL += vin.BoneWeights2[j] * mul(vin.NormalL, (float3x3) prevBoneTM);
    }

    float4 posH = mul(gWorldViewProj, float4(posL, 1.0f));

#ifdef OUT_LINE
    posH.xyz += mul((float3x3) gWorldViewProj, normalL).xyz * gSize;
#endif
    
    return posH;
}