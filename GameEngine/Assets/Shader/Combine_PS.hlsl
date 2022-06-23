#include "SamplerState_Header.hlsli"
#include "Output_Header.hlsli"
#include "Function_Header.hlsli"

#define LUT_SIZE    float3(16.0f, 16.0f, 16.0f)

cbuffer cbDrawFinal : register(b0)
{
    float gBloomFactor      : packoffset(c0.x);
    float gBaseLUTFactor    : packoffset(c0.y);
    float gBlendLUTFactor   : packoffset(c0.z);
    float gPad              : packoffset(c0.w);
}

Texture2D gOriginMap;
Texture2D gBloomMap;
//Texture2D gOutLineMap;
Texture3D gBaseLUT;
Texture3D gBlendLUT;

static float3 LUT_Scale  = (LUT_SIZE - float3(1.0f, 1.0f, 1.0f)) / LUT_SIZE;
static float3 LUT_Offset = float3(1.0f, 1.0f, 1.0f) / (float3(2.0f, 2.0f, 2.0f) * LUT_SIZE);

float4 Combine_PS(ScreenPixelIn pin) : SV_TARGET
{
#ifdef BLOOM
    // output: gOriginMap + coefficient * gBlurMap
    float4 outColor = mad(gBloomFactor, gBloomMap.Sample(gSamClampLinear, pin.Tex), gOriginMap.Sample(gSamClampLinear, pin.Tex));
#else
    float4 outColor = gOriginMap.Sample(gSamClampLinear, pin.Tex);
#endif
    
//#ifdef OUT_LINE
//    outColor.rgb += gOutLineMap.Sample(gSamClampLinear, pin.Tex).rgb;
//#endif
    
#ifdef HDR
    // Filmic ToneMapping
    outColor.rgb = ACESToneMapping(outColor.rgb);
#else
    outColor.rgb = saturate(outColor.rgb);
#endif
            
   	// Linear Space -> Gamma Space
    outColor.rgb = pow(outColor.rgb, 1.0f / 2.2f);
    
#ifdef COLOR_GRADING
    float3 color = gBaseLUT.SampleLevel(gSamWrapLinear, LUT_Scale * outColor.rgb + LUT_Offset, 0).rgb * gBaseLUTFactor;
    color += gBlendLUT.SampleLevel(gSamWrapLinear, LUT_Scale * outColor.rgb + LUT_Offset, 0).rgb * gBlendLUTFactor;
    
    outColor.rgb = color;
#endif  

    return outColor;
}