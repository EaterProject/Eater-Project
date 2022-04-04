#include "Output_Header.hlsli"
#include "Function_Header.hlsli"

cbuffer cbBloomFinal : register(b0)
{
    float gCoefficient;
}

Texture2D gOriginMap : register(t0);
Texture2D gBloomMap : register(t1);

SamplerState gSamClampLinear : register(s0);

float4 ToneMapping_PS(ScreenPixelIn pin) : SV_TARGET
{
#ifdef BLOOM
    // output: gOriginMap + coefficient * gBlurMap
    float4 outColor = mad(gCoefficient, gBloomMap.Sample(gSamClampLinear, pin.Tex), gOriginMap.Sample(gSamClampLinear, pin.Tex));
#else
    float4 outColor = gOriginMap.Sample(gSamClampLinear, pin.Tex);
#endif
    
#ifdef HDR
    // Filmic ToneMapping
    outColor.rgb = ACESToneMapping(outColor.rgb);
#endif
            
   	// Linear Space -> Gamma Space
    outColor.rgb = pow(outColor.rgb, 1.0f / 2.2f);
    
    return outColor;
}