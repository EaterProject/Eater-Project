#pragma once
#include <DirectXMath.h>
#include <windows.h>
#include <string>
//리소스 최상위 클래스
class Resources
{
public:
	Resources() {};
	virtual ~Resources() {};

public:
	std::string Name;
};

//인덱스 버퍼를 받을 클래스
class IndexBuffer : public Resources
{
public:
	virtual ~IndexBuffer()
	{
		delete pIndexBuf;
	};
	
	UINT Count	= 0;	//인덱스 개수

	void* pIndexBuf = nullptr;
};

//버텍스 버퍼를 받을 클래스
class VertexBuffer : public Resources
{
public:
	virtual ~VertexBuffer()
	{
		delete pVertexBuf;
	};

	UINT Stride = 0;	//stride값 버텍스버퍼를 만든 자료형의 크기
	UINT Offset = 0;

	void* pVertexBuf = nullptr;
};

//텍스쳐를 받을 클래스
class TextureBuffer : public Resources
{
public:
	virtual ~TextureBuffer()
	{
		delete pTextureBuf;
	};

	void* pTextureBuf = nullptr;
};

//환경맵을 받을 클래스
class EnvironmentBuffer : public Resources
{
public:
	virtual ~EnvironmentBuffer()
	{
		delete Environment;
		delete Irradiance;
		delete Prefilter;
	};

	TextureBuffer* Environment = nullptr;
	TextureBuffer* Irradiance = nullptr;
	TextureBuffer* Prefilter = nullptr;
};