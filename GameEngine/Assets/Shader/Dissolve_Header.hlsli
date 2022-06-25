#ifndef DISSOLVE_HEADER
#define DISSOLVE_HEADER

cbuffer cbDessolveOption
{
    int gOctaves = 6;
    float gThickness = 0.1f;
    float gOuterEdge = 0.5f;
    float gInnerEdge = 0.6f;
    float gTime;
    
    float gStartTime;
    float gDuration;
    
    float4 gFireColor;
    float4 gAshColor;
};
    

float FBM(in float2 coord)
{
    float value = 0.0f;
    float scale = 0.5f;
    
    for (int i = 0; i < gOctaves; i++)
    {
        value += noise(coord) * scale;
        coord *= 2.0f;
        scale *= 0.5f;
    }
    
    return value;
}

float4 BurningEffect(in float4 color, in float2 uv)
{
    float noise = 0.0f;
    float scale = 0.5f;
    
    float4 outColor = color;
    
    for (int i = 0; i < gOctaves; i++)
    {
        noise += noise(uv) * scale;
        uv *= 2.0f;
        scale *= 0.5f;
    }
    
    float outer_edge = (gTime - gStartTime) / gDuration;
    float inner_edge = outer_edge + gThickness;
    
    if (noise < gInnerEdge)
    {
        float grad_factor = clamp((gInnerEdge - noise) / gThickness, 0.0f, 1.0f);
        
        float4 fire_grad = lerp(gFireColor, gAshColor, grad_factor);
        
        float inner_fade = clamp((gInnerEdge - noise) / 0.05f, 0.0f, 1.0f); // 수치 올리면 끝부분이 부드러워짐 
        
        outColor = lerp(color, fire_grad, inner_fade);
        
        outColor.a = 0.0f;
    }
    
    if (noise < gOuterEdge)
    {
        outColor.a = clamp(1.0f - (gOuterEdge - noise) / 0.03f, 0.0f, 1.0f);
    }
    
    outColor.a *= color.a;
    
    return color;
}
#endif
