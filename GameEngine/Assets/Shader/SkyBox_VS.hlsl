#include "Input_Header.hlsli"

cbuffer cbCubeObject : register(b0)
{
    float4x4 gCubeWorldViewProj;
    float4x4 gCubeWorld;
};

SkyBoxVertexOut SkyBox_VS(SkyBoxVertexIn vin)
{
    SkyBoxVertexOut vout;

    vout.PosL = vin.PosL;
    vout.PosW = mul(gCubeWorld, float4(vin.PosL, 1.0f)).xyz;
    
    vout.PosH = mul(gCubeWorldViewProj, float4(vin.PosL, 1.0f)).xyww;
    
    return vout;
}