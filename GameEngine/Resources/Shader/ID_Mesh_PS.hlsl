#include "Output_Header.hlsli"

float4 ID_Mesh_PS(MeshIDPixelIn pin) : SV_Target
{
    return pin.HashColor;
}