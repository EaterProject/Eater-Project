#pragma once
#include <DirectXMath.h>
#include <windows.h>
#include <string>
//���ҽ� �ֻ��� Ŭ����
class Resources
{
public:
	Resources() {};
	virtual ~Resources() {};

public:
	std::string Name;
};

//�ε��� ���۸� ���� Ŭ����
class IndexBuffer : public Resources
{
public:
	virtual ~IndexBuffer()
	{
		delete pIndexBuf;
	};
	
	UINT Count	= 0;	//�ε��� ����

	void* pIndexBuf = nullptr;
};

//���ؽ� ���۸� ���� Ŭ����
class VertexBuffer : public Resources
{
public:
	virtual ~VertexBuffer()
	{
		delete pVertexBuf;
	};

	UINT Stride = 0;	//stride�� ���ؽ����۸� ���� �ڷ����� ũ��
	UINT Offset = 0;

	void* pVertexBuf = nullptr;
};

//�ؽ��ĸ� ���� Ŭ����
class TextureBuffer : public Resources
{
public:
	virtual ~TextureBuffer()
	{
		delete pTextureBuf;
	};

	void* pTextureBuf = nullptr;
};

//ȯ����� ���� Ŭ����
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