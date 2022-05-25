#include "SamplerState_Header.hlsli"
#include "Output_Header.hlsli"

Texture2D gDiffuseMap : register(t0);

cbuffer cbUIOption
{
    float4 gColor : packoffset(c0);
};

float4 UI_PS(UIPixelIn pin) : SV_TARGET
{
    return gDiffuseMap.Sample(gSamWrapLinear, pin.Tex);
}
