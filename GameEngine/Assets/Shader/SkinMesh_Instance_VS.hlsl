#ifndef SKIN_MESH
    #define SKIN_MESH
#endif

#include "Input_Header.hlsli"
#include "Instance_Header.hlsli"

StructuredBuffer<Bone> gAnimationBuffer : register(t0);
//Buffer<float4> gAnimationBuffer : register(t0);

cbuffer cbInstanceSkinMesh
{
    float4x4 gView               : packoffset(c0);
    float4x4 gProj               : packoffset(c4);
    float4x4 gTexTransform       : packoffset(c8);
};

float4x4 LoadBoneMatrix(in Bone bone)
{
    return float4x4( float4(bone.Row1.xyz, 0.0f),
                     float4(bone.Row2.xyz, 0.0f), 
                     float4(bone.Row3.xyz, 0.0f), 
                     float4(bone.Row1.w, bone.Row2.w, bone.Row3.w, 1.0f) );
}

MeshVertexOut SkinMesh_Instance_VS(MeshVertexIn vin, MeshInstanceIn instance)
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
    
    //for (int i = 0; i < 4; ++i)
    //{
    //    nowIndex = instance.PrevAnimationIndex + vin.BoneIndices1[i];
    //    nowIndex = instance.NextAnimationIndex + vin.BoneIndices1[i];
    //    
    //    prevBoneTM = LoadBoneMatrix(gAnimationBuffer.Load(nowIndex));
    //    nextBoneTM = LoadBoneMatrix(gAnimationBuffer.Load(nextIndex));
    //    
    //    nowBoneTM = lerp(nowBoneTM, nextBoneTM, instance.FrameTime);
    //    
    //    posL += vin.BoneWeights1[i] * mul(nowBoneTM, float4(vin.PosL, 1.0f)).xyz;
    //    normalL += vin.BoneWeights1[i] * mul((float3x3) nowBoneTM, vin.NormalL);
    //    tangentL += vin.BoneWeights1[i] * mul((float3x3) nowBoneTM, vin.TangentL);
    //}
    //
    //for (int j = 0; j < 4; ++j)
    //{
    //    nowIndex = instance.PrevAnimationIndex + vin.BoneIndices2[j];
    //    nowIndex = instance.NextAnimationIndex + vin.BoneIndices2[j];
    //    
    //    prevBoneTM = LoadBoneMatrix(gAnimationBuffer.Load(nowIndex));
    //    nextBoneTM = LoadBoneMatrix(gAnimationBuffer.Load(nextIndex));
    //    
    //    nowBoneTM = lerp(nowBoneTM, nextBoneTM, instance.FrameTime);
    //    
    //    posL += vin.BoneWeights2[j] * mul(nowBoneTM, float4(vin.PosL, 1.0f)).xyz;
    //    normalL += vin.BoneWeights2[j] * mul((float3x3) nowBoneTM, vin.NormalL);
    //    tangentL += vin.BoneWeights2[j] * mul((float3x3) nowBoneTM, vin.TangentL);
    //}
    
    vout.PosW = mul(instance.World, float4(posL, 1.0f)).xyz;
    vout.PosV = mul(gView, float4(vout.PosW, 1.0f)).xyz;
    vout.PosH = mul(gProj, float4(vout.PosV, 1.0f));

    vout.NormalW = mul((float3x3) instance.World, normalL);
    vout.NormalV = mul((float3x3) gView, vout.NormalW);
    
    vout.TangentW = mul((float3x3) instance.World, tangentL);
    vout.TangentV = mul((float3x3) gView, vout.TangentW);
    
	// Output vertex attributes for interpolation across triangle.
    vout.Tex = mul(gTexTransform, float4(vin.Tex, 0.0f, 1.0f)).xy;

    return vout;
}
