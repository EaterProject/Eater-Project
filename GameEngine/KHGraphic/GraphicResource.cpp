#include "DirectDefine.h"
#include "GraphicResource.h"

Texture2D::Texture2D(Hash_Code hashCode, ID3D11Texture2D* resource)
	:GraphicResource(RESOURCE_TYPE::TEX2D, hashCode, resource), m_Width_Ratio(1.0f), m_Height_Ratio(1.0f)
{
	D3D11_TEXTURE2D_DESC desc;
	resource->GetDesc(&desc);

	m_Width = desc.Width;
	m_Height = desc.Height;
}

Texture2D::~Texture2D()
{
	Release();
}

void Texture2D::OnResize(int width, int height)
{
	m_Width = (UINT)(width * m_Width_Ratio);
	m_Height = (UINT)(height * m_Height_Ratio);
}

void Texture2D::SetRatio(float width_ratio, float height_ratio)
{
	m_Width_Ratio = width_ratio;
	m_Height_Ratio = height_ratio;
}

void Texture2D::GetDesc(D3D11_TEXTURE2D_DESC* desc)
{
	m_Resource->GetDesc(desc);

	desc->Width = m_Width;
	desc->Height = m_Height;
}

Buffer::Buffer(Hash_Code hashCode, ID3D11Buffer* resource)
	:GraphicResource(RESOURCE_TYPE::BUFFER, hashCode, resource)
{
	D3D11_BUFFER_DESC desc;
	resource->GetDesc(&desc);

	m_ByteWidth = desc.ByteWidth;
}

Buffer::~Buffer()
{
	Release();
}

void Buffer::OnResize(UINT byteWidth)
{
	m_ByteWidth = byteWidth;
}

void Buffer::GetDesc(D3D11_BUFFER_DESC* desc)
{
	m_Resource->GetDesc(desc);

	desc->ByteWidth = m_ByteWidth;
}
