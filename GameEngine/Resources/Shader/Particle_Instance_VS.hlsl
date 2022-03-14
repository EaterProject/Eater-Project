#include "Input_Header.hlsli"
#include "Instance_Header.hlsli"

cbuffer cbInstanceParticleMesh
{
    float4x4 gViewProj : packoffset(c0);
};

ParticleVertexOut Particle_Instance_VS(ParticleVertexIn vin, ParticleInstanceIn instance)
{
    ParticleVertexOut vout;
    
    float3 posW = mul(instance.World, float4(vin.PosL, 1.0f)).xyz;
    vout.PosH = mul(gViewProj, float4(posW, 1.0f));
    vout.Tex = mul(instance.TexTransform, float4(vin.Tex, 0.0f, 1.0f)).xy;
    vout.Color = instance.Color;
    
    return vout;
}