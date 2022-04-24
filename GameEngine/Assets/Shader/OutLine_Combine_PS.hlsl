#include "Output_Header.hlsli"

Texture2D gOriginMap : register(t0);
Texture2D gOutLineMap : register(t1);

SamplerState gSamClampLinear : register(s0);

cbuffer cbOutLineOption : register(b1)
{
    float3 gOutLineColor : packoffset(c0.x);
    float gPad           : packoffset(c0.w);
}

float4 OutLine_Combine_PS(ScreenPixelIn pin)
{
    float4 origin = gOriginMap.Sample(gSamClampLinear, pin.Tex);

    float4 outline = gOutLineMap.Sample(gSamClampLinear, pin.Tex);

    return float4(origin.rgb + outline.rgb, origin.a);
}