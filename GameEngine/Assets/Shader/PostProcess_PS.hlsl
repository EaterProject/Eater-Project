#include "SamplerState_Header.hlsli"
#include "Output_Header.hlsli"

Texture2D gOriginMap : register(t0);

// Pixel shader: monochrome.
float4 Monochrome_PS(ScreenPixelIn pin) : SV_TARGET
{
    float4 color = gOriginMap.Sample(gSamClampLinear, pin.Tex);
    float3 grayscale = float3(0.2125f, 0.7154f, 0.0721f);
    float3 output = dot(color.rgb, grayscale);
    return float4(output, color.a);
}

// Pixel shader: sepia.
float4 Sepia_PS(ScreenPixelIn pin) : SV_TARGET
{
    float4 color = gOriginMap.Sample(gSamClampLinear, pin.Tex);

    float3 red = float3(0.393f, 0.769f, 0.189f);
    float3 green = float3(0.349f, 0.686f, 0.168f);
    float3 blue = float3(0.272f, 0.534f, 0.131f);

    float3 output;
    output.r = dot(color.rgb, red);
    output.g = dot(color.rgb, green);
    output.b = dot(color.rgb, blue);
    return float4(output, color.a);
}
