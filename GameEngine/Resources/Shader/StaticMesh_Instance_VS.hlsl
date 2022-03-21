#include "Input_Header.hlsli"
#include "Instance_Header.hlsli"

cbuffer cbInstanceStaticMesh
{
    float4x4 gView          : packoffset(c0);
    float4x4 gProj          : packoffset(c4);
    float4x4 gTexTransform  : packoffset(c8);
};

MeshVertexOut StaticMesh_Instance_VS(MeshVertexIn vin, MeshInstanceIn instance)
{
    MeshVertexOut vout;
    
    vout.PosW = mul(instance.World, float4(vin.PosL, 1.0f)).xyz;
    vout.PosV = mul(gView, float4(vout.PosW, 1.0f)).xyz;
    vout.PosH = mul(gProj, float4(vout.PosV, 1.0f));

    vout.NormalW = mul((float3x3) instance.World, vin.NormalL);
    vout.NormalV = mul((float3x3) gView, vout.NormalW);
    
    vout.TangentW = mul((float3x3) instance.World, vin.TangentL);
    vout.TangentV = mul((float3x3) gView, vout.TangentW);
    
	// Output vertex attributes for interpolation across triangle.
    vout.Tex = mul(gTexTransform, float4(vin.Tex, 0.0f, 1.0f)).xy;

    return vout;
}