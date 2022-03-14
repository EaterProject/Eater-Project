#pragma once
#include "ResourceBase.h"

/// 2021/11/04 12:20
/// SeoKyuHwang
///
/// # Texture2D Class
///
/// - Rendering에 필요한 Resource Texture2D Class
/// 

class Texture2D : public GraphicResource<ID3D11Texture2D>
{
public:
	Texture2D(Hash_Code hashCode, ID3D11Texture2D* resource);
	~Texture2D();

public:
	void OnResize(int width, int height);
	void SetRatio(float width_ratio, float height_ratio);

public:
	void GetDesc(D3D11_TEXTURE2D_DESC* desc);

private:
	UINT m_Width;
	UINT m_Height;

	float m_Width_Ratio;
	float m_Height_Ratio;
};

/// 2022/01/04 19:20
/// SeoKyuHwang
///
/// # Buffer Class
///
/// - Rendering에 필요한 Resource Buffer Class
/// 

class Buffer : public GraphicResource<ID3D11Buffer>
{
public:
	Buffer(Hash_Code hashCode, ID3D11Buffer* buffer);
	~Buffer();

public:
	void OnResize(UINT byteWidth);

public:
	void GetDesc(D3D11_BUFFER_DESC* desc);

private:
	UINT m_ByteWidth;
};