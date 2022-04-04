#include "Input_Header.hlsli"

float4 Depth_PS(MeshDepthVertexOut pin) : SV_Target
{
    return float4(pin.Depth, pin.Depth, pin.Depth, 1);
}
