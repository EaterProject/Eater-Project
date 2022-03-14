#pragma once
#include <DirectXMath.h>
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
	
	void* pIndexBuf = nullptr;
	unsigned int Count	= 0;	//�ε��� ����
};

//���ؽ� ���۸� ���� Ŭ����
class VertexBuffer : Resources
{
public:
	virtual ~VertexBuffer()
	{
		delete pVertexBuf;
	};

	void* pVertexBuf = nullptr;

	unsigned int Stride = 0;	//stride�� ���ؽ����۸� ���� �ڷ����� ũ��
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