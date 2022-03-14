#include "Output_Header.hlsli"
#include "OIT_Header.hlsli"

RWStructuredBuffer<FLStaticNode> gPieceLinkBuffer : register(u0);
RWByteAddressBuffer gFirstOffsetBuffer : register(u1);

Texture2D gDiffuseMap : register(t0);
SamplerState gSamWrapLinear : register(s0);

[earlydepthstencil]
void OIT_Particle_PS(ParticlePixelIn pin)
{
    float4 texColor = gDiffuseMap.Sample(gSamWrapLinear, pin.Tex);
    
    clip(texColor.a - 0.1f);

    // µÎ ÇÈ¼¿À» °öÇÏ°í °¨¸¶(2.0f)¸¦ °öÇØÁØ´Ù..
    texColor.rgb = pow(texColor.rgb, 2.2f) * 4.0f;
    texColor = texColor * pin.Color;
    texColor = saturate(texColor);
    
    uint pixelCount = gPieceLinkBuffer.IncrementCounter();
    
    uint2 vPos = (uint2) pin.PosH.xy;
    uint startOffsetAddress = 4 * (gFrameWidth * vPos.y + vPos.x);
    uint oldStartOffset;
    gFirstOffsetBuffer.InterlockedExchange(
        startOffsetAddress, pixelCount, oldStartOffset);
    
    FLStaticNode node;
    node.Data.Color = PackColorFromFloat4(texColor);
    node.Data.Depth = pin.PosH.z;
    node.Next = oldStartOffset;
    
    gPieceLinkBuffer[pixelCount] = node;
}
