#pragma once
#include <windows.h>
#include <string>

interface ID3D11Graphic
{
public:
	virtual void Initialize(HWND hwnd, int screenWidth, int screenHeight) abstract;
	virtual void Release() abstract;

public:
	virtual void SaveTextureDDS(ID3D11Resource* resource, const char* fileName) abstract;
	virtual void SaveTextureDDS(ID3D11ShaderResourceView* resource, const char* fileName) abstract;

public:
	virtual void CreateBuffer(D3D11_BUFFER_DESC* bufferDesc, D3D11_SUBRESOURCE_DATA* subData, ID3D11Buffer** buffer) abstract;
	virtual void CreateBuffer(D3D11_BUFFER_DESC* bufferDesc, D3D11_SUBRESOURCE_DATA* subData, D3D11_USAGE usage, ID3D11Buffer** buffer) abstract;
	virtual void CreateTextureBuffer(std::string filePath, ID3D11Resource** resource, ID3D11ShaderResourceView** srv) abstract;

public:
	virtual void CreateBackBuffer(UINT width, UINT height, ID3D11Texture2D** tex2D, ID3D11RenderTargetView** rtv, ID3D11ShaderResourceView** srv) abstract;
	virtual void CreateTexture2D(D3D11_TEXTURE2D_DESC* texDesc, D3D11_SUBRESOURCE_DATA* subData, ID3D11Texture2D** tex2D) abstract;
	virtual void CreateDepthStencilView(ID3D11Resource* resource, D3D11_DEPTH_STENCIL_VIEW_DESC* dsvDesc, ID3D11DepthStencilView** dsv) abstract;
	virtual void CreateRenderTargetView(ID3D11Resource* resource, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc, ID3D11RenderTargetView** rtv) abstract;
	virtual void CreateShaderResourceView(ID3D11Resource* resource, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc, ID3D11ShaderResourceView** srv) abstract;
	virtual void CreateUnorderedAccessView(ID3D11Resource* resource, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc, ID3D11UnorderedAccessView** uav) abstract;
	
public:
	virtual void CreateDepthStencilState(D3D11_DEPTH_STENCIL_DESC* dssDesc, ID3D11DepthStencilState** dss) abstract;
	virtual void CreateRasterizerState(D3D11_RASTERIZER_DESC* rsDesc, ID3D11RasterizerState** rs) abstract;
	virtual void CreateBlendState(D3D11_BLEND_DESC* bsDesc, ID3D11BlendState** bs) abstract;
	virtual void CreateSamplerState(D3D11_SAMPLER_DESC* ssDesc, ID3D11SamplerState** ss) abstract;

public:
	virtual Microsoft::WRL::ComPtr<ID3D11Device> GetDevice() abstract;
	virtual Microsoft::WRL::ComPtr<ID3D11DeviceContext> GetContext() abstract;
	virtual Microsoft::WRL::ComPtr<IDXGISwapChain> GetSwapChain() abstract;
};
