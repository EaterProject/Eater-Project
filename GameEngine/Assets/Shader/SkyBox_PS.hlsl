#include "SamplerState_Header.hlsli"
#include "Output_Header.hlsli"
#include "Function_Header.hlsli"
#include "Fog_Header.hlsli"

TextureCube gSkyCube : register(t0);

[earlydepthstencil]
float4 SkyBox_PS(SkyBoxPixelIn pin) : SV_TARGET
{
#ifdef HDR
    float4 texColor = gSkyCube.Sample(gSamWrapLinear, pin.PosL);
#else
    float4 texColor = pow(gSkyCube.Sample(gSamWrapLinear, pin.PosL), 2.2f);
#endif
    
#ifdef FOG
    texColor.rgb = Fog(texColor.rgb, pin.PosW);
#endif
    
    return texColor;
}