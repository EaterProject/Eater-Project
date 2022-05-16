#include "DirectDefine.h"
#include "D3D11Graphic.h"
#include "ToolKitDefine.h"
#include "ImageParser.h"

#include "Profiler/Profiler.h"

D3D11Graphic::D3D11Graphic(HWND hwnd, int screenWidth, int screenHeight)
{
	Initialize(hwnd, screenWidth, screenHeight);
}

D3D11Graphic::~D3D11Graphic()
{

}

void D3D11Graphic::Initialize(HWND hwnd, int screenWidth, int screenHeight)
{
	HRESULT result;
	UINT createDeviceFlags = 0;
	
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// 스왑 체인 설정을 초기화..
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// swapChainDesc을 단일 백 버퍼로 설정..
	swapChainDesc.BufferCount = 1;

	// 백 버퍼의 너비와 높이를 설정..
	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;

	// 백 버퍼에 일반 32 비트 표면을 설정..
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// 백 버퍼의 재생률을 설정..
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;

	// 스캔 라인 순서 및 배율을 지정되지 않음으로 설정..
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// 백 버퍼의 사용 설정..
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = (HWND)hwnd;

	// 멀티 샘플링을 끕니다..
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = true;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// 추가 플래그 설정 안 함..
	swapChainDesc.Flags = createDeviceFlags;

	// featureLevel을 DirectX 11로 설정..
	D3D_FEATURE_LEVEL featurelevel = D3D_FEATURE_LEVEL_11_0;

	// 스왑 체인, Direct3D 장치 및 Direct3D 장치 컨텍스트 생성..
	/// MSDN에선 SwapChain 과 Device 를 한번에 생성해주는 함수를 지향함..
	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featurelevel, 1,
		D3D11_SDK_VERSION, &swapChainDesc, &m_SwapChain, &m_Device, NULL, &m_DeviceContext);

	if (FAILED(result))
	{
		PROFILE_RESULT(PROFILE_OUTPUT::LOG_FILE, result, "[ Graphic ][ Create ][ Device ] FAILED!!");
		PROFILE_RESULT(PROFILE_OUTPUT::VS_CODE, result, "[ Graphic ][ Create ][ Device ] FAILED!!");
	}
}

void D3D11Graphic::Release()
{

}

HRESULT D3D11Graphic::SaveTextureDDS(ID3D11Resource* resource, const char* fileName)
{
	HRESULT result;
	std::string name = fileName;
	name = "../Assets/Texture/Bake/" + name + ".dds";

	DirectX::ScratchImage image;
	result = DirectX::CaptureTexture(m_Device.Get(), m_DeviceContext.Get(), resource, image);

	if (SUCCEEDED(result))
	{
		result = DirectX::SaveToDDSFile(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::DDS_FLAGS_NONE, std::wstring(name.begin(), name.end()).c_str());
	}

	if (FAILED(result))
	{
		PROFILE_RESULT(PROFILE_OUTPUT::LOG_FILE, result, "[ Graphic ][ Save ][ Texture ] '%s' FAILED!!", fileName);
		PROFILE_RESULT(PROFILE_OUTPUT::VS_CODE, result, "[ Graphic ][ Save ][ Texture ] '%s' FAILED!!", fileName);
	}

	return result;
}

HRESULT D3D11Graphic::SaveTextureDDS(ID3D11DepthStencilView* resource, const char* fileName)
{
	ID3D11Texture2D* textureInterface = nullptr;
	ID3D11Resource* textureResource;
	resource->GetResource(&textureResource);

	return SaveTextureDDS(textureResource, fileName);
}

HRESULT D3D11Graphic::SaveTextureDDS(ID3D11ShaderResourceView* resource, const char* fileName)
{
	ID3D11Texture2D* textureInterface = nullptr;
	ID3D11Resource* textureResource;
	resource->GetResource(&textureResource);
	
	return SaveTextureDDS(textureResource, fileName);
}

HRESULT D3D11Graphic::CaptureTextureDDS(const char* fileName)
{
	ID3D11RenderTargetView* rtv = nullptr;
	ID3D11Resource* resource = nullptr;

	m_DeviceContext->OMGetRenderTargets(1, &rtv, nullptr);
	rtv->GetResource(&resource);

	return SaveTextureDDS(resource, fileName);
}

HRESULT D3D11Graphic::CreateBuffer(D3D11_BUFFER_DESC* bufferDesc, D3D11_SUBRESOURCE_DATA* subData, ID3D11Buffer** buffer)
{
	if (bufferDesc == nullptr) return E_INVALIDARG;

	// Buffer Resource 생성..
	return m_Device->CreateBuffer(bufferDesc, subData, buffer);
}

HRESULT D3D11Graphic::CreateBuffer(D3D11_BUFFER_DESC* bufferDesc, D3D11_SUBRESOURCE_DATA* subData, D3D11_USAGE usage, ID3D11Buffer** buffer)
{
	if (bufferDesc == nullptr) return E_INVALIDARG;

	switch (usage)
	{
	case D3D11_USAGE_DEFAULT:
		bufferDesc->Usage = D3D11_USAGE_DEFAULT;
		break;
	case D3D11_USAGE_IMMUTABLE:
		bufferDesc->Usage = D3D11_USAGE_IMMUTABLE;
		break;
	case D3D11_USAGE_DYNAMIC:
		bufferDesc->Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc->CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		break;
	case D3D11_USAGE_STAGING:
		bufferDesc->Usage = D3D11_USAGE_STAGING;
		break;
	default:
		break;
	}

	// Buffer Resource 생성..
	return m_Device->CreateBuffer(bufferDesc, subData, buffer);
}

HRESULT D3D11Graphic::CreateTexture2D(D3D11_TEXTURE2D_DESC* texDesc, D3D11_SUBRESOURCE_DATA* subData, ID3D11Texture2D** tex2D)
{
	if (texDesc == nullptr) return E_INVALIDARG;

	// Texture2D Resource 생성..
	return m_Device->CreateTexture2D(texDesc, subData, tex2D);
}

HRESULT D3D11Graphic::CreateTextureBuffer(std::string filePath, ID3D11Resource** resource, ID3D11ShaderResourceView** srv)
{
	HRESULT result;
	std::wstring wPath(filePath.begin(), filePath.end());

	std::size_t Start = filePath.rfind('.') + 1;
	std::size_t End = filePath.length() - Start;
	std::string Type = filePath.substr(Start, End);

	// 확장자에 따른 텍스처 파일 로드 방식..
	if (Type == "dds" || Type == "DDS")
	{
		result = DirectX::CreateDDSTextureFromFile(m_Device.Get(), m_DeviceContext.Get(), wPath.c_str(), resource, srv);
	}
	else if (Type == "hdr" || Type == "HDR")
	{
		DirectX::TexMetadata tex;
		DirectX::ScratchImage image;

		result = DirectX::LoadFromHDRFile(wPath.c_str(), &tex, image);
		result = DirectX::CreateShaderResourceView(m_Device.Get(), image.GetImages(), image.GetImageCount(), tex, srv);

		(*srv)->GetResource(resource);
	}
	else
	{
		result = DirectX::CreateWICTextureFromFile(m_Device.Get(), m_DeviceContext.Get(), wPath.c_str(), resource, srv);
	}

	if (FAILED(result))
	{
		PROFILE_RESULT(PROFILE_OUTPUT::LOG_FILE, result, "[ Graphic ][ Create ][ ShaderResourceView ] '%s' FAILED!!", filePath.c_str());
		PROFILE_RESULT(PROFILE_OUTPUT::VS_CODE, result, "[ Graphic ][ Create ][ ShaderResourceView ] '%s' FAILED!!", filePath.c_str());
	}

	return result;
}

HRESULT D3D11Graphic::CreateBackBuffer(UINT width, UINT height, ID3D11Texture2D** tex2D, ID3D11RenderTargetView** rtv, ID3D11ShaderResourceView** srv)
{
	HRESULT result;

	// BackBuffer Resize..
	result = m_SwapChain->ResizeBuffers(1, (UINT)width, (UINT)height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

	if (FAILED(result))
	{
		PROFILE_RESULT(PROFILE_OUTPUT::LOG_FILE, result, "[ Graphic ][ Create ][ Buffer ] 'BackBuffer' FAILED!!");
		PROFILE_RESULT(PROFILE_OUTPUT::VS_CODE, result, "[ Graphic ][ Create ][ Buffer ] 'BackBuffer' FAILED!!");
	}

	// Get Swap Chain Back Buffer Pointer..
	result = m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(tex2D));

	if (FAILED(result))
	{
		PROFILE_RESULT(PROFILE_OUTPUT::LOG_FILE, result, "[ Graphic ][ Get ][ Buffer ] 'BackBuffer' FAILED!!");
		PROFILE_RESULT(PROFILE_OUTPUT::VS_CODE, result, "[ Graphic ][ Get ][ Buffer ] 'BackBuffer' FAILED!!");
	}

	// BackBuffer Resource Resize..
	result = m_Device->CreateRenderTargetView(*tex2D, nullptr, rtv);

	if (FAILED(result))
	{
		PROFILE_RESULT(PROFILE_OUTPUT::LOG_FILE, result, "[ Graphic ][ Create ][ RenderTargetView ] 'BackBuffer' FAILED!!");
		PROFILE_RESULT(PROFILE_OUTPUT::VS_CODE, result, "[ Graphic ][ Create ][ RenderTargetView ] 'BackBuffer' FAILED!!");
	}

	return result;
}

HRESULT D3D11Graphic::CreateDepthStencilView(ID3D11Resource* resource, D3D11_DEPTH_STENCIL_VIEW_DESC* dsvDesc, ID3D11DepthStencilView** dsv)
{
	// RenderTargetView Resource 생성..
	return m_Device->CreateDepthStencilView(resource, dsvDesc, dsv);
}

HRESULT D3D11Graphic::CreateRenderTargetView(ID3D11Resource* resource, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc, ID3D11RenderTargetView** rtv)
{
	// RenderTargetView Resource 생성..
	return m_Device->CreateRenderTargetView(resource, rtvDesc, rtv);
}

HRESULT D3D11Graphic::CreateShaderResourceView(ID3D11Resource* resource, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc, ID3D11ShaderResourceView** srv)
{
	// ShaderResourceView Resource 생성..
	return m_Device->CreateShaderResourceView(resource, srvDesc, srv);
}

HRESULT D3D11Graphic::CreateUnorderedAccessView(ID3D11Resource* resource, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc, ID3D11UnorderedAccessView** uav)
{
	// UnorderedAccessView Resource 생성..
	return m_Device->CreateUnorderedAccessView(resource, uavDesc, uav);
}

HRESULT D3D11Graphic::CreateDepthStencilState(D3D11_DEPTH_STENCIL_DESC* dssDesc, ID3D11DepthStencilState** dss)
{
	if (dssDesc == nullptr) return E_INVALIDARG;

	// DepthStencilState Resource 생성..
	return m_Device->CreateDepthStencilState(dssDesc, dss);
}

HRESULT D3D11Graphic::CreateRasterizerState(D3D11_RASTERIZER_DESC* rsDesc, ID3D11RasterizerState** rs)
{
	if (rsDesc == nullptr) return E_INVALIDARG;

	// RasterizerState Resource 생성..
	return m_Device->CreateRasterizerState(rsDesc, rs);
}

HRESULT D3D11Graphic::CreateBlendState(D3D11_BLEND_DESC* bsDesc, ID3D11BlendState** bs)
{
	if (bsDesc == nullptr) return E_INVALIDARG;
	
	// BlendState Resource 생성..
	return m_Device->CreateBlendState(bsDesc, bs);
}

HRESULT D3D11Graphic::CreateSamplerState(D3D11_SAMPLER_DESC* ssDesc, ID3D11SamplerState** ss)
{
	if (ssDesc == nullptr) return E_INVALIDARG;

	// SamplerState Resource 생성..
	return m_Device->CreateSamplerState(ssDesc, ss);
}

Microsoft::WRL::ComPtr<ID3D11Device> D3D11Graphic::GetDevice()
{
	return m_Device;
}

Microsoft::WRL::ComPtr<ID3D11DeviceContext> D3D11Graphic::GetContext()
{
	return m_DeviceContext;
}

Microsoft::WRL::ComPtr<IDXGISwapChain> D3D11Graphic::GetSwapChain()
{
	return m_SwapChain;
}

