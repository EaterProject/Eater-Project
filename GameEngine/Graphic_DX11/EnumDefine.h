#pragma once

// ResourceType..
typedef enum class RESOURCE_TYPE
{
	TEX2D,		// Texture 2D
	BUFFER,		// Buffer
	SRV,		// Shader Resource View
	UAV,		// Unordered Access View
	RTV,		// Render Target View
	DSV,		// Depth Stencil View
	DSS,		// Depth Stencil State
	SS,			// Sampler State
	RS,			// Rasterizer State
	BS,			// Blend State
	DS,			// Depth Stencil
	RT,			// Render Target
	RB,			// Render Buffer
	DB,			// Draw Buffer
	IB,			// Instance Buffer
	CB,			// Constant Buffer
	VP,			// ViewPort
}RESOURCE_TYPE;

// Buffer Type..
typedef enum BUFFER_TYPE
{
	VERTEX_BUFFER,
	INDEX_BUFFER,
	CONSTANT_BUFFER,
	STRUCTURED_BUFFER,
	RAW_BUFFER
}BUFFER_TYPE;

// Shader Type..
typedef enum SHADER_TYPE
{
	VERTEX_SHADER,
	PIXEL_SHADER,
	COMPUTE_SHADER
}SHADER_TYPE;
