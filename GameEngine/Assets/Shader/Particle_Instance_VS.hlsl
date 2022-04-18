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
    vout.Tex.x = instance.TexScale.x * vin.Tex.x + instance.TexPos.x;
    vout.Tex.y = instance.TexScale.y * vin.Tex.y + instance.TexPos.y;
    vout.Color = instance.Color;
    
    return vout;
}