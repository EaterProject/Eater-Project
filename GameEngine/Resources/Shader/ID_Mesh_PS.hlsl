#include "Output_Header.hlsli"

float4 ID_Mesh_PS(MeshIDPixelIn pin) : SV_TARGET
{
    return pin.HashColor;
}