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
		pIndexBuf = nullptr;
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
		pVertexBuf = nullptr;
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
		pTextureBuf = nullptr;
	};

	void* pTextureBuf = nullptr;
};

// Animation Array Texture Data Buffer
class AnimationBuffer : public Resources
{
public:
	virtual ~AnimationBuffer()
	{

	}

	UINT BufferIndex = 0;						// Animation Buffer Index

	UINT AnimationCount = 0;
	UINT FrameOffset = 0;
	std::vector<UINT> AnimationOffset;

	void* pAnimationBuf = nullptr;
};
