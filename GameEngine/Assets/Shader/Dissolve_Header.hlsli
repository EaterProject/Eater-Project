#ifndef DISSOLVE_HEADER
#define DISSOLVE_HEADER

#include "SamplerState_Header.hlsli"

cbuffer cbDissolveOption
{
    float gOuterEdge    : packoffset(c0.x);
    float gOuterFactor  : packoffset(c0.y);
    float gInnerEdge    : packoffset(c0.z);
    float gInnerFactor  : packoffset(c0.w);
    
    float3 gInnerColor  : packoffset(c1.x);
    float gThickness    : packoffset(c1.w);
    
    float3 gOuterColor  : packoffset(c2.x);
};
    
Texture2D gNoiseTexture;

float4 DissolveEffect(in float4 color, in float2 uv)
{
    float4 outColor = color;
    float factor = gNoiseTexture.Sample(gSamWrapLinear, uv).r;
    
    factor *= factor;
    
    if (factor < gInnerEdge)
    {
        float grad_factor = smoothstep(0.0f, 1.0f, (gInnerEdge - factor) * gThickness);
        
        float3 burn_grad = lerp(gInnerColor, gOuterColor, grad_factor);
        
        float inner_fade = smoothstep(0.0f, 1.0f, (gInnerEdge - factor) * gInnerFactor); // 수치 올리면 끝부분이 부드러워짐 
        
        outColor.rgb = lerp(color.rgb, burn_grad, inner_fade);
        
        outColor.a = 0.0f;
    }
    
    if (factor < gOuterEdge)
    {
        outColor.a = smoothstep(0.0f, 1.0f, 1.0f - (gOuterEdge - factor) * gOuterFactor);
    }
    
    outColor.a *= color.a;
    
    return outColor;
}
#endif
