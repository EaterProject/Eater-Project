#include "Output_Header.hlsli"

Texture2D gDiffuseMap : register(t0);

SamplerState gSamWrapLinear : register(s0);

float4 Particle_PS(ParticlePixelIn pin) : SV_TARGET
{
    float4 pout = gDiffuseMap.Sample(gSamWrapLinear, pin.Tex);
    
    clip(pout.a - 0.1f);
    
    // �� �ȼ��� ���ϰ� ����(2.2f)�� �����ش�..
    pout = pout * pin.Color * 2.2f;
    
    pout = saturate(pout);
    
    return pout;
}