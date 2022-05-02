#include "Output_Header.hlsli"

TextureCube gSkyCube : register(t0);
SamplerState gSamWrapLinear : register(s0);

[earlydepthstencil]
SkyBoxPixelOut SkyBox_PS(SkyBoxPixelIn pin)
{
    SkyBoxPixelOut pout;
    pout.Albedo = pow(gSkyCube.Sample(gSamWrapLinear, pin.PosL), 2.2f);
    pout.Position = float4(pin.PosW, 1.0f);
    
    return pout;
}