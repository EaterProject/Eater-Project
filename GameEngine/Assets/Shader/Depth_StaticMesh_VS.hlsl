#include "Input_Header.hlsli"

cbuffer cbDepthStaticMesh : register(b0)
{
    float4x4 gWorldViewProj : packoffset(c0);
};

MeshDepthVertexOut Depth_StaticMesh_VS(MeshPosVertexIn vin)
{
    MeshDepthVertexOut vout;
	
    // Transform the position into screen space.
    vout.PosH = mul(gWorldViewProj, float4(vin.PosL, 1.0f));

    // Record the depth value to pass to the pixel shader
    vout.Depth = vout.PosH.z / vout.PosH.w;
	
    return vout;
}