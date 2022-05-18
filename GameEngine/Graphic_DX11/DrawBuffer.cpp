#include "DirectDefine.h"
#include "ResourceBase.h"
#include "GraphicResource.h"
#include "DrawBuffer.h"

DrawBuffer::DrawBuffer(Hash_Code hashCode, Buffer* vb, Buffer* ib)
	: ResourceBase(RESOURCE_TYPE::DB, hashCode), VertexBuf(vb), IndexBuf(ib), IndexCount(0), Stride(0), Offset(0)
{

}

DrawBuffer::~DrawBuffer()
{
	Release();
}

void DrawBuffer::Reset()
{
	VertexBuf->Reset();
	IndexBuf->Reset();
}

void DrawBuffer::Release()
{
	RELEASE_COM(VertexBuf);
	RELEASE_COM(IndexBuf);
}

InstanceBuffer::InstanceBuffer(Hash_Code hashCode, Buffer* instbuf)
	: ResourceBase(RESOURCE_TYPE::IB, hashCode), InstanceBuf(instbuf), Stride(0), Offset(0)
{

}

InstanceBuffer::~InstanceBuffer()
{
	Release();
}

void InstanceBuffer::Reset()
{
	InstanceBuf->Reset();
}

void InstanceBuffer::Release()
{
	RELEASE_COM(InstanceBuf);
}
