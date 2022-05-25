#ifndef SKIN_MESH
    #define SKIN_MESH
#endif

#include "Input_Header.hlsli"
#include "VertexFetch_Header.hlsli"

StructuredBuffer<BoneAnimation> gAnimationBuffer : register(t0);

cbuffer cbSkinMesh
{
    float4x4 gWorld              : packoffset(c0);
    float4x4 gInvWorld           : packoffset(c4);
    float4x4 gView               : packoffset(c8);
    float4x4 gProj               : packoffset(c12);
    uint gPrevAnimationIndex     : packoffset(c16.x);
    uint gNextAnimationIndex     : packoffset(c16.y);
    float gFrameTime             : packoffset(c16.z);
    float gPad                   : packoffset(c16.w);
};

MeshVertexOut SkinMesh_VS(MeshVertexIn vin)
{
    MeshVertexOut vout;
    
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
        tangentL += vin.BoneWeights1[i] * mul(vin.TangentL, (float3x3) nowBoneTM);
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
        tangentL += vin.BoneWeights2[j] * mul(vin.TangentL, (float3x3) prevBoneTM);
    }
    
    vout.PosW = mul(gWorld, float4(posL, 1.0f)).xyz;
    vout.PosV = mul(gView, float4(vout.PosW, 1.0f)).xyz;
    vout.PosH = mul(gProj, float4(vout.PosV, 1.0f));

    vout.NormalW = mul((float3x3) gInvWorld, normalL);
    vout.NormalV = mul((float3x3) gView, vout.NormalW);
    
    vout.TangentW = mul((float3x3) gWorld, tangentL);
    vout.TangentV = mul((float3x3) gView, vout.TangentW);
    
	// Output vertex attributes for interpolation across triangle.
    vout.Tex = vin.Tex;

    return vout;
}