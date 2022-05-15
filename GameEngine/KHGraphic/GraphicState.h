#pragma once
#include "ResourceBase.h"

// DirectX 11 DepthStencilState Class
class DepthStencilState : public GraphicState<ID3D11DepthStencilState>
{
public:
	DepthStencilState(Hash_Code hashCode, ID3D11DepthStencilState* dss);
	~DepthStencilState();

public:
	void GetDesc(D3D11_DEPTH_STENCIL_DESC* desc);
};

// DirectX 11 RasterizerState Class
class RasterizerState : public GraphicState<ID3D11RasterizerState>
{
public:
	RasterizerState(Hash_Code hashCode, ID3D11RasterizerState* rs);
	~RasterizerState();

public:
	void GetDesc(D3D11_RASTERIZER_DESC* desc);
};

// DirectX 11 BlendState Class
class BlendState : public GraphicState<ID3D11BlendState>
{
public:
	BlendState(Hash_Code hashCode, ID3D11BlendState* bs);
	~BlendState();

public:
	void GetDesc(D3D11_BLEND_DESC* desc);
};

// DirectX 11 SamplerState Class
class SamplerState : public GraphicState<ID3D11SamplerState>
{
public:
	SamplerState(Hash_Code hashCode, ID3D11SamplerState* ss);
	~SamplerState();

public:
	void GetDesc(D3D11_SAMPLER_DESC* desc);
};

// DirectX 11 ViewPort Class
class ViewPort : public ResourceBase
{
public:
	ViewPort(Hash_Code hashCode, float ratio_offsetX, float ratio_offsetY, float ratio_sizeX, float ratio_sizeY, float width, float height);
	ViewPort(Hash_Code hashCode, float offsetX, float offsetY, float width, float height);
	~ViewPort();

public:
	void Reset() override;
	void Release() override;

public:
	void SetResize(bool resize);
	void OnResize(int width, int height);

public:
	bool GetResize();
	D3D11_VIEWPORT* Get();

private:
	D3D11_VIEWPORT* m_ViewPort;
	bool m_IsResize;

	float m_OffsetX;
	float m_OffsetY;
	float m_SizeX;
	float m_SizeY;
};