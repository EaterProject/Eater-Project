#include "Input_Header.hlsli"

cbuffer cbDebugObject : register(b0)
{
    float4x4 gWorldViewProj : packoffset(c0);
};

DebugLineVertexOut Debug_VS(DebugLineVertexIn vin)
{
    DebugLineVertexOut vout;

	// Transform to homogeneous clip space.
    vout.PosH = mul(gWorldViewProj, float4(vin.PosL, 1.0f));

    vout.Color = vin.Color;
    
    return vout;
}

DebugTextureVertexOut Debug_Texture_VS(DebugTextureVertexIn vin)
{
    DebugTextureVertexOut vout;

	// Transform to homogeneous clip space.
#ifdef RENDER_TARGET
    vout.PosH = float4(vin.PosL, 1.0f);
#else
    vout.PosH = mul(gWorldViewProj, float4(vin.PosL, 1.0f));
#endif
    
    vout.Tex = vin.Tex;
    
    return vout;
}