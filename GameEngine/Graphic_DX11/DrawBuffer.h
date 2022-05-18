#pragma once
#include "ResourceBase.h"

/// 2021/12/05 21:17
/// SeoKyuHwang
///
/// # DrawBuffer Class
///
/// - Rendering에 필요한 기본 Vertex, Index Buffer Class
///

class DrawBuffer : public ResourceBase
{
public:
	DrawBuffer(Hash_Code hashCode, Buffer* vb, Buffer* ib);
	~DrawBuffer();

public:
	void Reset() override;
	void Release() override;

public:
	Buffer* VertexBuf;		// Vertex Buffer
	Buffer* IndexBuf;		// Index Buffer

	DXGI_FORMAT Format;					// Format Type
	D3D_PRIMITIVE_TOPOLOGY Topology;	// Topology Type

	UINT IndexCount;		// Index Count
	UINT Stride;			// Vertex Data Size
	UINT Offset;			// Offset
};

class InstanceBuffer : public ResourceBase
{
public:
	InstanceBuffer(Hash_Code hashCode, Buffer* instbuf);
	~InstanceBuffer();

public:
	void Reset() override;
	void Release() override;

public:
	Buffer* InstanceBuf;	// Instance Buffer

	UINT DataSize;			// Maximum Instance Data Size
	UINT Stride;			// Instance Data Size
	UINT Offset;			// Offset
};