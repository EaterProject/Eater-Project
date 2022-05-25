#include "SamplerState_Header.hlsli"
#include "Output_Header.hlsli"
#include "OIT_Header.hlsli"
#include "Fog_Header.hlsli"

cbuffer cbParticleOption : register(b0)
{
    float gStrength : packoffset(c0);
};

RWStructuredBuffer<FLStaticNode> gPieceLinkBuffer : register(u0);
RWByteAddressBuffer gFirstOffsetBuffer : register(u1);

Texture2D gDiffuseMap : register(t0);

[earlydepthstencil]
void OIT_Particle_PS(ParticlePixelIn pin)
{
    float4 texColor = gDiffuseMap.Sample(gSamClampLinear, pin.Tex);
    
    clip(texColor.a - 0.001f);

    // µÎ ÇÈ¼¿À» °öÇÏ°í °¨¸¶(2.0f)¸¦ °öÇØÁØ´Ù..
    texColor.rgb = pow(texColor.rgb, 2.2f) * gStrength;
    texColor = texColor * pin.Color;
    
#ifdef FOG
    texColor.rgb = Fog(texColor.rgb, pin.PosW);
#endif
    
    uint pixelCount = gPieceLinkBuffer.IncrementCounter();
    
    uint2 vPos = (uint2) pin.PosH.xy;
    uint startOffsetAddress = 4 * (gFrameWidth * vPos.y + vPos.x);
    uint oldStartOffset;
    gFirstOffsetBuffer.InterlockedExchange(
        startOffsetAddress, pixelCount, oldStartOffset);
    
    float strength = length(texColor);
    float4 color = float4(texColor.rgb / strength, texColor.a);
    
    FLStaticNode node;
    node.Data.Strength = strength;
    node.Data.Color = PackColorFromFloat4(color);
    node.Data.Depth = pin.PosH.z;
    node.Next = oldStartOffset;
    
    gPieceLinkBuffer[pixelCount] = node;
}
