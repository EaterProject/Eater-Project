#pragma once
#include <DirectXMath.h>
#include <string>
//리소스 최상위 클래스
class  Resources
{
public:
	Resources() {};
	virtual ~Resources() {};
};

//인덱스 버퍼를 받을 클래스
class IndexBuffer : Resources
{
public:
	virtual ~IndexBuffer()
	{
		delete pIndexBuf;
	};
	
	void* pIndexBuf = nullptr;
	unsigned int Count	= 0;			//인덱스 개수
};


//버텍스 버퍼를 받을 클래스
class VertexBuffer : Resources
{
public:
	virtual ~VertexBuffer()
	{
		delete pVertexBuf;
	};

	void* pVertexBuf = nullptr;

	unsigned int VectexDataSize = 0;	//버텍스 
	unsigned int Count	= 0;			//버텍스 개수
};

//텍스쳐를 받을 클래스
class TextureBuffer : Resources
{
public:
	virtual ~TextureBuffer()
	{
		delete pTextureBuf;
	};

	void* pTextureBuf = nullptr;
};








