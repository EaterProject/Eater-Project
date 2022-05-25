#include "Output_Header.hlsli"

cbuffer cbOutLineOption : register(b0)
{
    float3 gOutLineColor : packoffset(c0.x);
    float gPad           : packoffset(c0.w);
}

float4 OutLine_PS(float4 posH : SV_Position) : SV_Target
{
    return float4(gOutLineColor, 1.0f);
}