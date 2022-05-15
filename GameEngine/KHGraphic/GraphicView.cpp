#include "DirectDefine.h"
#include "GraphicView.h"

DepthStencilView::DepthStencilView(Hash_Code hashCode, ID3D11DepthStencilView* dsv)
	:GraphicView(RESOURCE_TYPE::DSV, hashCode, dsv)
{

}

DepthStencilView::~DepthStencilView()
{

}

void DepthStencilView::GetDesc(D3D11_DEPTH_STENCIL_VIEW_DESC* desc)
{
	assert(m_Resource);

	// Description 추출..
	m_Resource->GetDesc(desc);
}

RenderTargetView::RenderTargetView(Hash_Code hashCode, ID3D11RenderTargetView* rtv)
	:GraphicView(RESOURCE_TYPE::RTV, hashCode, rtv)
{
	D3D11_RENDER_TARGET_VIEW_DESC desc;
	rtv->GetDesc(&desc);

	m_NumElements = desc.Buffer.NumElements;
}

RenderTargetView::~RenderTargetView()
{

}

void RenderTargetView::GetDesc(D3D11_RENDER_TARGET_VIEW_DESC* desc)
{
	assert(m_Resource);

	// Description 추출..
	m_Resource->GetDesc(desc);

	desc->Buffer.NumElements = m_NumElements;
}

ShaderResourceView::ShaderResourceView(Hash_Code hashCode, ID3D11ShaderResourceView* srv)
	:GraphicView(RESOURCE_TYPE::SRV, hashCode, srv)
{
	D3D11_SHADER_RESOURCE_VIEW_DESC desc;
	srv->GetDesc(&desc);

	m_NumElements = desc.Buffer.NumElements;
}

ShaderResourceView::~ShaderResourceView()
{

}

void ShaderResourceView::GetDesc(D3D11_SHADER_RESOURCE_VIEW_DESC* desc)
{
	assert(m_Resource);

	// Description 추출..
	m_Resource->GetDesc(desc);

	desc->Buffer.NumElements = m_NumElements;
}

UnorderedAccessView::UnorderedAccessView(Hash_Code hashCode, ID3D11UnorderedAccessView* uav)
	:GraphicView(RESOURCE_TYPE::UAV, hashCode, uav)
{
	D3D11_UNORDERED_ACCESS_VIEW_DESC desc;
	uav->GetDesc(&desc);

	m_NumElements = desc.Buffer.NumElements;
}

UnorderedAccessView::~UnorderedAccessView()
{

}

void UnorderedAccessView::GetDesc(D3D11_UNORDERED_ACCESS_VIEW_DESC* desc)
{
	assert(m_Resource);

	// Description 추출..
	m_Resource->GetDesc(desc);

	desc->Buffer.NumElements = m_NumElements;
}
