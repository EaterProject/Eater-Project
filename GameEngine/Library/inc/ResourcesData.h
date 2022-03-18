#pragma once
#include <DirectXMath.h>
#include <windows.h>
#include <string>
//���ҽ� �ֻ��� Ŭ����
class  Resources
{
public:
	Resources() {};
	virtual ~Resources() {};
};

//�ε��� ���۸� ���� Ŭ����
class IndexBuffer : Resources
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
class VertexBuffer : Resources
{
public:
	virtual ~VertexBuffer()
	{
		delete pVertexBuf;
	};

	UINT Stride = 0;	//stride�� ���ؽ����۸� ���� �ڷ����� ũ��

	void* pVertexBuf = nullptr;
};

//�ؽ��ĸ� ���� Ŭ����
class TextureBuffer : Resources
{
public:
	virtual ~TextureBuffer()
	{
		delete pTextureBuf;
	};

	std::string Name;

	void* pTextureBuf = nullptr;
};