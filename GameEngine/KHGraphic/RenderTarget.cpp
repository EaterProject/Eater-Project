#include "DirectDefine.h"
#include "GraphicView.h"
#include "GraphicResource.h"
#include "RenderTarget.h"

RenderTarget::RenderTarget(RESOURCE_TYPE type, Hash_Code hashCode, RenderTargetView* rtv, ShaderResourceView* srv, UnorderedAccessView* uav)
	:ResourceBase(type, hashCode), m_RTV(rtv), m_SRV(srv), m_UAV(uav), m_IsResize(true)
{

}

RenderTarget::~RenderTarget()
{

}

void RenderTarget::Reset()
{
	SAFE_RESET(m_RTV);
	SAFE_RESET(m_SRV);
	SAFE_RESET(m_UAV);
}

void RenderTarget::Release()
{
	SAFE_RELEASE(m_RTV);
	SAFE_RELEASE(m_SRV);
	SAFE_RELEASE(m_UAV);
}

void RenderTarget::SetNumElements(UINT numElements)
{
	if (m_RTV) m_RTV->SetNumElements(numElements);
	if (m_SRV) m_SRV->SetNumElements(numElements);
	if (m_UAV) m_UAV->SetNumElements(numElements);
}

void RenderTarget::SetResize(bool resize)
{
	m_IsResize = resize;
}

bool RenderTarget::GetResize()
{
	return m_IsResize;
}

RenderTargetView* RenderTarget::GetRTV()
{
	return m_RTV;
}

ShaderResourceView* RenderTarget::GetSRV()
{
	return m_SRV;
}

UnorderedAccessView* RenderTarget::GetUAV()
{
	return m_UAV;
}

RenderTexture::RenderTexture(Hash_Code hashCode, Texture2D* tex2D, RenderTargetView* rtv, ShaderResourceView* srv, UnorderedAccessView* uav)
	:RenderTarget(RESOURCE_TYPE::RT, hashCode, rtv, srv, uav), m_Tex2D(tex2D)
{

}

RenderTexture::~RenderTexture()
{
	Release();
}

void RenderTexture::Reset()
{
	SAFE_RESET(m_Tex2D);

	RenderTarget::Reset();
}

void RenderTexture::Release()
{
	SAFE_RELEASE(m_Tex2D);

	RenderTarget::Release();
}

void RenderTexture::OnResize(int width, int height)
{
	// Texture Resize..
	m_Tex2D->OnResize(width, height);
	if (m_RTV) m_RTV->SetViewSize(width, height);
	if (m_SRV) m_SRV->SetViewSize(width, height);
	if (m_UAV) m_UAV->SetViewSize(width, height);
}

void RenderTexture::SetRatio(float ratio)
{
	m_Tex2D->SetRatio(ratio, ratio);
}

void RenderTexture::SetRatio(float width_ratio, float height_ratio)
{
	m_Tex2D->SetRatio(width_ratio, height_ratio);
}

Texture2D* RenderTexture::GetTex2D()
{
	return m_Tex2D;
}

RenderBuffer::RenderBuffer(Hash_Code hashCode, Buffer* buffer, RenderTargetView* rtv, ShaderResourceView* srv, UnorderedAccessView* uav)
	:RenderTarget(RESOURCE_TYPE::RB, hashCode, rtv, srv, uav), m_Buffer(buffer)
{

}

RenderBuffer::~RenderBuffer()
{
	Release();
}

void RenderBuffer::Reset()
{
	SAFE_RESET(m_Buffer);

	RenderTarget::Reset();
}

void RenderBuffer::Release()
{
	SAFE_RELEASE(m_Buffer);

	RenderTarget::Release();
}

void RenderBuffer::OnResize(UINT byteWidth)
{
	// Buffer & Bind Resource Resize..
	m_Buffer->OnResize(byteWidth);
}

Buffer* RenderBuffer::GetBuffer()
{
	return m_Buffer;
}