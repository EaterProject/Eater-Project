#define GAUSSIAN_RADIUS 4
#include "Output_Header.hlsli"

Texture2D gOriginMap : register(t0);
Texture2D gDownMap : register(t1);
SamplerState gSamClampLinear : register(s0);
SamplerState gSamBorderLinear : register(s1);

cbuffer cbBloomBright : register(b0)
{
    float gThreshold : packoffset(c0.x);
}

cbuffer cbBloomBlurOrder : register(b1)
{
    float2 gBlurTexelSize	: packoffset(c0.x);
    float2 gBlurOrder       : packoffset(c0.z);
}

static float gWeights[9] = { 0.0135f, 0.0476f, 0.1172f, 0.2011f, 0.2408f, 0.2011f, 0.1172f, 0.0476f, 0.0135f };

float GetBloomCurve(float x, float threshold)
{
    float result = x;
    x *= 2.0f;

    // Bloom Curve 1
	//result = x * 0.05 + max(0, x - threshold) * 0.5; // default threshold = 1.26

    // Bloom Curve 2
    //result = x * x / 3.2;
    
    // Bloom Curve 3
    result = max(0, x - threshold); // default threshold = 1.0
	result *= result;

    return result * 0.5f;
}


float4 DownSampling_Bright_PS(ScreenPixelIn pin) : SV_TARGET
{
    float3 outColor = float3(0.0f, 0.0f, 0.0f);
    
    for (uint y = 0; y < 4; y++)
    {
        for (uint x = 0; x < 4; x++)
        {
            // Compute the sum of color values
            outColor += gOriginMap.Sample(gSamBorderLinear, pin.Tex, int2(x, y)).rgb;
        }
    }
    
    outColor /= 16;
    
    float intensity = max(dot(outColor, float3(0.3f, 0.3f, 0.3f)), 0.000001f);
    //float intensity = max(dot(outColor, float3(0.3f, 0.59f, 0.11f)), 0.000001f);
    
    float bloom_intensity = GetBloomCurve(intensity, gThreshold);
    float3 bloom_color = outColor * bloom_intensity / intensity;
    
    return float4(bloom_color, 1.0f);
}

float4 DownSampling_PS(ScreenPixelIn pin) : SV_TARGET
{
    float3 outColor = float3(0.0f, 0.0f, 0.0f);
    
    for (uint y = 0; y < 6; y++)
    {
        for (uint x = 0; x < 6; x++)
        {
            // Compute the sum of color values
            outColor += gOriginMap.Sample(gSamBorderLinear, pin.Tex, int2(x, y)).rgb;
        }
    }
    
    outColor /= 36;
    
    return float4(outColor, 1.0f);
}

float4 UpSampling_PS(ScreenPixelIn pin) : SV_TARGET
{
    float3 outColor = float3(0.0f, 0.0f, 0.0f);
    
    outColor += gOriginMap.Sample(gSamClampLinear, pin.Tex).rgb;
    outColor += gDownMap.Sample(gSamClampLinear, pin.Tex).rgb;
    
    return float4(outColor, 1.0f);
}

float4 BloomBlur_PS(ScreenPixelIn pin) : SV_TARGET
{
    float3 outColor = float3(0.0f, 0.0f, 0.0f);
    
    float2 texOffset = gBlurOrder * gBlurTexelSize;
    float3 color = float3(0.0f, 0.0f, 0.0f);
    float weight = 0.0f;
    
    for (int i = -GAUSSIAN_RADIUS; i <= GAUSSIAN_RADIUS; ++i)
    {
        outColor += gWeights[i + GAUSSIAN_RADIUS] * gOriginMap.Sample(gSamBorderLinear, pin.Tex + i * texOffset).rgb;
    }
    
    return float4(outColor, 1.0f);
}

float4 DrawScreen_PS(ScreenPixelIn pin) : SV_TARGET
{
    return gOriginMap.Sample(gSamClampLinear, pin.Tex);
}