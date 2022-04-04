#include "Input_Header.hlsli"

cbuffer cbCubeObject : register(b0)
{
    float4x4 gCubeWorldViewProj;
};

SkyBoxVertexOut SkyBox_VS(SkyBoxVertexIn vin)
{
    SkyBoxVertexOut vout;

    vout.PosH = mul(gCubeWorldViewProj, float4(vin.PosL, 1.0f)).xyww;

    vout.PosL = vin.PosL;
    
    return vout;
}