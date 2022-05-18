#pragma once
#include "ResourceBase.h"

// DirectX 11 DepthStencilView Class
class DepthStencilView : public GraphicView<ID3D11DepthStencilView>
{
public:
	DepthStencilView(Hash_Code hashCode, ID3D11DepthStencilView* dsv);
	~DepthStencilView();

public:
	void GetDesc(D3D11_DEPTH_STENCIL_VIEW_DESC* desc);
};

// DirectX 11 RenderTargetView Class
class RenderTargetView : public GraphicView<ID3D11RenderTargetView>
{
public:
	RenderTargetView(Hash_Code hashCode, ID3D11RenderTargetView* rtv);
	~RenderTargetView();

public:
	void GetDesc(D3D11_RENDER_TARGET_VIEW_DESC* desc);
};

// DirectX 11 ShaderResourceView Class
class ShaderResourceView : public GraphicView<ID3D11ShaderResourceView>
{
public:
	ShaderResourceView(Hash_Code hashCode, ID3D11ShaderResourceView* srv);
	~ShaderResourceView();

public:
	void GetDesc(D3D11_SHADER_RESOURCE_VIEW_DESC* desc);
};

// DirectX 11 UnorderedAccessView Class
class UnorderedAccessView : public GraphicView<ID3D11UnorderedAccessView>
{
public:
	UnorderedAccessView(Hash_Code hashCode, ID3D11UnorderedAccessView* uav);
	~UnorderedAccessView();

public:
	void GetDesc(D3D11_UNORDERED_ACCESS_VIEW_DESC* desc);
};

