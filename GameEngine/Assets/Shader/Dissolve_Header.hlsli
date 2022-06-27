#ifndef DISSOLVE_HEADER
#define DISSOLVE_HEADER

#include "SamplerState_Header.hlsli"

cbuffer cbDissolveOption
{
    float3 gEdgeColor   : packoffset(c0.x);
    float gThickness    : packoffset(c0.w);
    
    float gOuterEdge    : packoffset(c1.x);
    float gOuterFactor  : packoffset(c1.y);
    float gInnerEdge    : packoffset(c1.z);
    float gInnerFactor  : packoffset(c1.w);
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
        
        float inner_fade = smoothstep(0.0f, 1.0f, (gInnerEdge - factor) * gInnerFactor); // 수치 올리면 끝부분이 부드러워짐 
        
        outColor.rgb = lerp(color.rgb, gEdgeColor * grad_factor, inner_fade);
        
        outColor.a = 0.0f;
    }
    
    if (factor < gOuterEdge)
    {
        outColor.a = smoothstep(0.0f, 1.0f, 1.0f - ((gOuterEdge - factor) * gOuterFactor));
    }
    
    outColor.a *= color.a;
    
    return outColor;
}
#endif
