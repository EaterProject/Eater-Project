#include "Output_Header.hlsli"
#include "Function_Header.hlsli"

cbuffer cbDrawFinal : register(b0)
{
    float3 gOutLineColor : packoffset(c0.x);
    float gBloomFactor   : packoffset(c0.w);
}

Texture2D gOriginMap : register(t0);
Texture2D gBloomMap : register(t1);
Texture2D gOutLineMap : register(t2);

SamplerState gSamClampLinear : register(s0);

float4 Combine_PS(ScreenPixelIn pin) : SV_TARGET
{
#ifdef BLOOM
    // output: gOriginMap + coefficient * gBlurMap
    float4 outColor = mad(gBloomFactor, gBloomMap.Sample(gSamClampLinear, pin.Tex), gOriginMap.Sample(gSamClampLinear, pin.Tex));
#else
    float4 outColor = gOriginMap.Sample(gSamClampLinear, pin.Tex);
#endif
    
#ifdef OUT_LINE
    outColor.rgb += gOutLineMap.Sample(gSamClampLinear, pin.Tex).rgb;
#endif
    
#ifdef HDR
    // Filmic ToneMapping
    outColor.rgb = ACESToneMapping(outColor.rgb);
#endif
            
   	// Linear Space -> Gamma Space
    outColor.rgb = pow(outColor.rgb, 1.0f / 2.2f);
    
    return outColor;
}