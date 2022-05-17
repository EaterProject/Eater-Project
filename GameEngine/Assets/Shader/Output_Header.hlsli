#ifndef OUTPUT_HEADER
#define OUTPUT_HEADER

// Mesh Pixel Shader Input & Output Data Struct
struct MeshPixelIn
{
    float4 PosH         : SV_POSITION;
    float3 PosW         : POSITION0;
    float3 PosV         : POSITION1;
    float2 Tex          : TEXCOORD;
    float3 NormalW      : NORMAL0;
    float3 NormalV      : NORMAL1;
    float3 TangentW     : TANGENT0;
    float3 TangentV     : TANGENT1;
    
#ifdef TERRAIN_MESH
    float4 MaskColor    : TEXCOORD1;
#endif
};

struct MeshIDPixelIn
{
    float4 PosH         : SV_POSITION;
    float4 HashColor     : HASHCOLOR;
};

struct MeshPixelOut
{
    float4 Albedo       : SV_Target0;
    float4 Emissive     : SV_Target1;
    float4 Normal       : SV_Target2;
    float4 Position     : SV_Target3;
    float4 NormalDepth  : SV_Target4;
};

// Particle Vertex Shader Input & Output Data Struct
struct ParticlePixelIn
{
    float4 PosH  : SV_POSITION;
    float3 PosW  : POSITION;
    float2 Tex   : TEXCOORD;
    float4 Color : COLOR;
};

// SkyBox Pixel Shader Input & Output Data Struct
struct SkyBoxPixelIn
{
    float4 PosH : SV_POSITION;
    float3 PosL : POSITION0;
    float3 PosW : POSITION1;
};

// Screen Pixel Shader Input & Output Data Struct
struct ScreenPixelIn
{
    float4 PosH : SV_POSITION;
    float2 Tex  : TEXCOORD0;
    
#ifdef SSAO
	float3 ToFarPlane : TEXCOORD1;
#endif
};

// Debug Pixel Shader Input & Output Data Struct
struct DebugLinePixelIn
{
    float4 PosH  : SV_POSITION;
    float4 Color : COLOR;
};

struct DebugTexturePixelIn
{
    float4 PosH : SV_POSITION;
    float2 Tex  : TEXCOORD;
};

#endif