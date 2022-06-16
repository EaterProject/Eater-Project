#include "SamplerState_Header.hlsli"
#include "Output_Header.hlsli"

Texture2D gDiffuseMap : register(t0);

cbuffer cbUIOption
{
    float4 gColor : packoffset(c0);
};

cbuffer cbUIFill
{
    float2 gFill  : packoffset(c0);
};

float4 UI_PS(UIPixelIn pin) : SV_TARGET
{
    return gDiffuseMap.Sample(gSamBorderLinear, pin.Tex) * gColor;
}

float4 UI_Fill_PS(UIPixelIn pin) : SV_TARGET
{
    return (pin.Tex.x <= gFill.x && pin.Tex.y <= gFill.y) ? gDiffuseMap.Sample(gSamBorderLinear, pin.Tex) * gColor : float4(0.0f, 0.0f, 0.0f, 0.0f);
}

float4 UI_Fill_Reverse_PS(UIPixelIn pin) : SV_TARGET
{
    return (pin.Tex.x >= gFill.x && pin.Tex.y >= gFill.y) ? gDiffuseMap.Sample(gSamBorderLinear, pin.Tex) * gColor : float4(0.0f, 0.0f, 0.0f, 0.0f);
}
