#include "DirectDefine.h"
#include "GraphicState.h"

DepthStencilState::DepthStencilState(Hash_Code hashCode, ID3D11DepthStencilState* dss)
	:GraphicState(RESOURCE_TYPE::DSS, hashCode, dss)
{
}

DepthStencilState::~DepthStencilState()
{

}

void DepthStencilState::GetDesc(D3D11_DEPTH_STENCIL_DESC* desc)
{
	m_Resource->GetDesc(desc);
}

RasterizerState::RasterizerState(Hash_Code hashCode, ID3D11RasterizerState* rs)
	:GraphicState(RESOURCE_TYPE::RS, hashCode, rs)
{
}

RasterizerState::~RasterizerState()
{

}

void RasterizerState::GetDesc(D3D11_RASTERIZER_DESC* desc)
{
	m_Resource->GetDesc(desc);
}

BlendState::BlendState(Hash_Code hashCode, ID3D11BlendState* bs)
	:GraphicState(RESOURCE_TYPE::BS, hashCode, bs)
{
}

BlendState::~BlendState()
{

}

void BlendState::GetDesc(D3D11_BLEND_DESC* desc)
{
	m_Resource->GetDesc(desc);
}

SamplerState::SamplerState(Hash_Code hashCode, ID3D11SamplerState* ss)
	:GraphicState(RESOURCE_TYPE::SS, hashCode, ss)
{
}

SamplerState::~SamplerState()
{

}

void SamplerState::GetDesc(D3D11_SAMPLER_DESC* desc)
{
	m_Resource->GetDesc(desc);
}

ViewPort::ViewPort(Hash_Code hashCode, float ratio_offsetX, float ratio_offsetY, float ratio_sizeX, float ratio_sizeY, float width, float height)
	:ResourceBase(RESOURCE_TYPE::VP, hashCode), m_IsResize(true)
{
	m_OffsetX = ratio_offsetX;
	m_OffsetY = ratio_offsetY;
	m_SizeX = ratio_sizeX;
	m_SizeY = ratio_sizeY;

	m_ViewPort = new D3D11_VIEWPORT();
	m_ViewPort->TopLeftX = ratio_offsetX * width;
	m_ViewPort->TopLeftY = ratio_offsetY * height;
	m_ViewPort->Width = ratio_sizeX * width;
	m_ViewPort->Height = ratio_sizeY * height;
	m_ViewPort->MinDepth = 0.0f;
	m_ViewPort->MaxDepth = 1.0f;
}

ViewPort::ViewPort(Hash_Code hashCode, float offsetX, float offsetY, float width, float height)
	:ResourceBase(RESOURCE_TYPE::VP, hashCode), m_IsResize(true)
{
	m_ViewPort = new D3D11_VIEWPORT();
	m_ViewPort->TopLeftX = offsetX;
	m_ViewPort->TopLeftY = offsetY;
	m_ViewPort->Width = width;
	m_ViewPort->Height = height;
	m_ViewPort->MinDepth = 0.0f;
	m_ViewPort->MaxDepth = 1.0f;
}

ViewPort::~ViewPort()
{
	Release();
}

void ViewPort::Reset()
{
	SAFE_DELETE(m_ViewPort);
}

void ViewPort::Release()
{
	SAFE_DELETE(m_ViewPort);
}

void ViewPort::SetResize(bool resize)
{
	m_IsResize = resize;
}

void ViewPort::OnResize(int width, int height)
{
	m_ViewPort->TopLeftX = m_OffsetX * (float)width;
	m_ViewPort->TopLeftY = m_OffsetY * (float)height;
	m_ViewPort->Width	 = m_SizeX * (float)width;
	m_ViewPort->Height	 = m_SizeY * (float)height;
}

bool ViewPort::GetResize()
{
	return m_IsResize;
}

D3D11_VIEWPORT* ViewPort::Get()
{
	return m_ViewPort;
}
