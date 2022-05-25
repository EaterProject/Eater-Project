#include "Input_Header.hlsli"

cbuffer cbUIObject : register(b0)
{
    float4x4 gWorldViewProj : packoffset(c0);
};

UIVertexOut UI_VS(UIVertexIn vin)
{
    UIVertexOut vout;

	// Transform to homogeneous clip space.
    vout.PosH = mul(gWorldViewProj, float4(vin.PosL, 1.0f));

    vout.Tex = vin.Tex;
    
    return vout;
}