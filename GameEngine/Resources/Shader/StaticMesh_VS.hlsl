#include "Input_Header.hlsli"

cbuffer cbStaticMesh
{
    float4x4 gWorld         : packoffset(c0);
    float4x4 gView          : packoffset(c4);
    float4x4 gProj          : packoffset(c8);
    float4x4 gTexTransform  : packoffset(c12);
};

MeshVertexOut StaticMesh_VS(MeshVertexIn vin)
{
    MeshVertexOut vout;

    vout.PosW = mul(gWorld, float4(vin.PosL, 1.0f)).xyz;
    vout.PosV = mul(gView, float4(vout.PosW, 1.0f)).xyz;
    vout.PosH = mul(gProj, float4(vout.PosV, 1.0f));

    vout.NormalW = mul((float3x3) gWorld, vin.NormalL);
    vout.NormalV = mul((float3x3) gView, vout.NormalW);
    
    vout.TangentW = mul((float3x3) gWorld, vin.TangentL);
    vout.TangentV = mul((float3x3) gView, vout.TangentW);
    
	// Output vertex attributes for interpolation across triangle.
    vout.Tex = mul(gTexTransform, float4(vin.Tex, 0.0f, 1.0f)).xy;

    return vout;
}