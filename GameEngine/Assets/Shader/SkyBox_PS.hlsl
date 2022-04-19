#include "Output_Header.hlsli"

TextureCube gSkyCube : register(t0);
SamplerState gSamWrapLinear : register(s0);

[earlydepthstencil]
float4 SkyBox_PS(SkyBoxPixelIn pin) : SV_TARGET
{
    return pow(gSkyCube.Sample(gSamWrapLinear, pin.PosL), 2.2f);
}