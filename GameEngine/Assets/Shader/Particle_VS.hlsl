#include "Input_Header.hlsli"

cbuffer cbParticleMesh
{
    float4x4 gWorld         : packoffset(c0);
    float4x4 gViewProj      : packoffset(c4);
    float4x4 gTexTransform  : packoffset(c8);
    float4 gColor           : packoffset(c12);
};

ParticleVertexOut Particle_VS(ParticleVertexIn vin)
{
    ParticleVertexOut vout;
    
    float3 posW = mul(gWorld, float4(vin.PosL, 1.0f));
    vout.PosH = mul(gViewProj, float4(posW, 1.0f));
    vout.Tex = mul(gTexTransform, float4(vin.Tex, 0.0f, 1.0f)).xy;
    vout.Color = gColor;
    return vout;
}