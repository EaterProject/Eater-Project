#include "SamplerState_Header.hlsli"
#include "Output_Header.hlsli"

TextureCube gSkyCube : register(t0);

float4 SkyBox_Convert_PS(SkyBoxPixelIn pin) : SV_TARGET
{
    float3 normal = normalize(float4(pin.PosW, 1.0f));

    return gSkyCube.SampleLevel(gSamWrapLinear, normal, 0.0f);
}