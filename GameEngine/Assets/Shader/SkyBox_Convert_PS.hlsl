#include "SamplerState_Header.hlsli"
#include "Output_Header.hlsli"
#include "Function_Header.hlsli"

#ifdef HDRI
Texture2D gSkyCube : register(t0);
#else
TextureCube gSkyCube : register(t0);
#endif

float4 SkyBox_Convert_PS(SkyBoxPixelIn pin) : SV_TARGET
{
#ifdef HDRI
    float2 UV = SampleSphericalMap(normalize(pin.PosW));
    float4 texColor = gSkyCube.Sample(gSamWrapLinear, UV);
#else
    float3 normal = normalize(float4(pin.PosW, 1.0f));
    float4 texColor = gSkyCube.SampleLevel(gSamWrapLinear, normal, 0.0f);
#endif
    
    return texColor;
}