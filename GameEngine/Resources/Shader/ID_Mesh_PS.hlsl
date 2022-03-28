#include "Output_Header.hlsli"

[earlydepthstencil]
float4 ID_Mesh_PS(MeshIDPixelIn pin) : SV_Target
{
    return pin.HashColor / 255.0f;
}