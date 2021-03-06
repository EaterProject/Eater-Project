#ifndef INPUT_HEADER
#define INPUT_HEADER

// Mesh Position Vertex Shader Input & Output Data Struct
struct MeshPosVertexIn
{
#ifdef SKIN_MESH
    uint4 BoneIndices1  : BONEINDICES1;
    uint4 BoneIndices2  : BONEINDICES2;
    float4 BoneWeights1 : WEIGHTS1;
    float4 BoneWeights2 : WEIGHTS2;
#endif
    
    float3 PosL : POSITION;
};

// Mesh Vertex Shader Input & Output Data Struct
struct MeshVertexIn
{
#ifdef SKIN_MESH
    uint4 BoneIndices1  : BONEINDICES1;
    uint4 BoneIndices2  : BONEINDICES2;
    float4 BoneWeights1 : WEIGHTS1;
    float4 BoneWeights2 : WEIGHTS2;
#endif
    
    float3 PosL         : POSITION;
    float2 Tex          : TEXCOORD;
    float3 NormalL      : NORMAL;
    float3 TangentL     : TANGENT;
    
#ifdef TERRAIN_MESH
    float4 Mask         : TEXCOORD1;
#endif
};

struct MeshVertexOut
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

// Mesh Depth Vertex Shader Output Data Struct
struct MeshDepthVertexOut
{
    float4 PosH : SV_POSITION;
    float Depth : ZDEPTH;
};

// Mesh ID Vertex Shader Output Data Struct
struct MeshIDVertexOut
{
    float4 PosH         : SV_POSITION;
    float4 HashColor    : COLOR;
};

// Particle Vertex Shader Input & Output Data Struct
struct ParticleVertexIn
{
    float3 PosL : POSITION;
    float2 Tex  : TEXCOORD;
};

struct ParticleVertexOut
{
    float4 PosH  : SV_POSITION;
    float3 PosW  : POSITION;
    float2 Tex : TEXCOORD;
    float4 Color : COLOR;
};

// SkyBox Vertex Shader Input & Output Data Struct
struct SkyBoxVertexIn
{
    float3 PosL : POSITION;
};

struct SkyBoxVertexOut
{
    float4 PosH : SV_POSITION;
    float3 PosL : POSITION0;
    float3 PosW : POSITION1;
};

// Screen Vertex Shader Input & Output Data Struct
struct ScreenVertexIn
{
	float3 PosL     : POSITION;
	float2 Tex      : TEXCOORD;
    
#ifdef SSAO
	float3 ToFarPlaneIndex : NORMAL;
#endif
};

struct ScreenVertexOut
{
	float4 PosH		  : SV_POSITION;
	float2 Tex		  : TEXCOORD0;
    
#ifdef SSAO
	float3 ToFarPlane : TEXCOORD1;
#endif
};

// Debug Vertex Shader Input & Output Data Struct
struct DebugLineVertexIn
{
    float3 PosL : POSITION;
    float4 Color : COLOR;
};

struct DebugTextureVertexIn
{
    float3 PosL : POSITION;
    float2 Tex : TEXCOORD;
};

struct DebugLineVertexOut
{
    float4 PosH : SV_POSITION;
    float4 Color : COLOR;
};

struct DebugTextureVertexOut
{
    float4 PosH : SV_POSITION;
    float2 Tex : TEXCOORD;
};

// UI Vertex Shader Input & Output Data Struct
struct UIVertexIn
{
    float3 PosL : POSITION;
    float2 Tex : TEXCOORD;
};

struct UIVertexOut
{
    float4 PosH : SV_POSITION;
    float2 Tex : TEXCOORD;
};

#endif