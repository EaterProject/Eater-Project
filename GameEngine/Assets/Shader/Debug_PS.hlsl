#include "Output_Header.hlsli"

cbuffer cbDebugOption : register(b0)
{
    float3 gColor : packoffset(c0);
}

Texture2D gDiffuseMap : register(t0);
SamplerState gSamWrapLinear : register(s0);

float4 Debug_PS(DebugLinePixelIn pin) : SV_TARGET
{
#ifdef CUSTOM_COLOR
    return float4(gColor, 1.0f);
#else
    return pin.Color;
#endif
}

float4 Debug_Texture_PS(DebugTexturePixelIn pin) : SV_TARGET
{
    float4 texColor = gDiffuseMap.Sample(gSamWrapLinear, pin.Tex);
#ifdef RENDER_TARGET
    return float4(texColor.rgb, 1.0f);
#else
    clip(texColor.a - 0.1f);
    
    texColor.rgb *= gColor;
    
    return texColor;
#endif
}