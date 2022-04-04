#ifndef SKIN_MESH
    #define SKIN_MESH
#endif

#include "Input_Header.hlsli"
#include "Instance_Header.hlsli"

cbuffer cbInstanceSkinMesh
{
    float4x4 gView               : packoffset(c0);
    float4x4 gProj               : packoffset(c4);
    float4x4 gTexTransform       : packoffset(c8);
    float4x4 gBoneTransforms[96] : packoffset(c12);
};

MeshVertexOut SkinMesh_Instance_VS(MeshVertexIn vin, MeshInstanceIn instance)
{
    MeshVertexOut vout;
    
    float3 posL = float3(0.0f, 0.0f, 0.0f);
    float3 normalL = float3(0.0f, 0.0f, 0.0f);
    float3 tangentL = float3(0.0f, 0.0f, 0.0f);
    
    for (int i = 0; i < 4; ++i)
    {
        posL += vin.BoneWeights1[i] * mul(gBoneTransforms[vin.BoneIndices1[i]], float4(vin.PosL, 1.0f)).xyz;
        normalL += vin.BoneWeights1[i] * mul((float3x3) gBoneTransforms[vin.BoneIndices1[i]], vin.NormalL);
        tangentL += vin.BoneWeights1[i] * mul((float3x3) gBoneTransforms[vin.BoneIndices1[i]], vin.TangentL);
    }
    
    for (int j = 0; j < 4; ++j)
    {
        posL += vin.BoneWeights2[j] * mul(gBoneTransforms[vin.BoneIndices2[j]], float4(vin.PosL, 1.0f)).xyz;
        normalL += vin.BoneWeights2[j] * mul((float3x3) gBoneTransforms[vin.BoneIndices2[j]], vin.NormalL);
        tangentL += vin.BoneWeights2[j] * mul((float3x3) gBoneTransforms[vin.BoneIndices2[j]], vin.TangentL);
    }
    
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
