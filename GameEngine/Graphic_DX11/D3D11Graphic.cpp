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

	// ���� ü�� ������ �ʱ�ȭ..
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// swapChainDesc�� ���� �� ���۷� ����..
	swapChainDesc.BufferCount = 1;

	// �� ������ �ʺ�� ���̸� ����..
	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;

	// �� ���ۿ� �Ϲ� 32 ��Ʈ ǥ���� ����..
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// �� ������ ������� ����..
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;

	// ��ĵ ���� ���� �� ������ �������� �������� ����..
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// �� ������ ��� ����..
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT | DXGI_USAGE_UNORDERED_ACCESS;
	swapChainDesc.OutputWindow = (HWND)hwnd;

	// ��Ƽ ���ø��� ���ϴ�..
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = true;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// �߰� �÷��� ���� �� ��..
	swapChainDesc.Flags = createDeviceFlags;

	// featureLevel�� DirectX 11�� ����..
	D3D_FEATURE_LEVEL featurelevel = D3D_FEATURE_LEVEL_11_0;

	// ���� ü��, Direct3D ��ġ �� Direct3D ��ġ ���ؽ�Ʈ ����..
	/// MSDN���� SwapChain �� Device �� �ѹ��� �������ִ� �Լ��� ������..
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

	std::string name(fileName);

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

HRESULT D3D11Graphic::SaveVolumeTextureDDS(ID3D11ShaderResourceView* resource, const char* saveName, UINT pixelSize, void** ppResource)
{
	HRESULT result;

	ID3D11Resource* image_resource = nullptr;
	resource->GetResource(&image_resource);

	DirectX::ScratchImage scrimage_source;
	result = DirectX::CaptureTexture(m_Device.Get(), m_DeviceContext.Get(), image_resource, scrimage_source);

	if (FAILED(result))
	{
		PROFILE_RESULT(PROFILE_OUTPUT::LOG_FILE, result, "[ Graphic ][ Save ][ VolumeTexture DDS ] '%s' FAILED!!", saveName);
		PROFILE_RESULT(PROFILE_OUTPUT::VS_CODE, result, "[ Graphic ][ Save ][ VolumeTexture DDS ] '%s' FAILED!!", saveName);

		return result;
	}

	const DirectX::Image* image_source = scrimage_source.GetImages();
	DirectX::TexMetadata image_data = scrimage_source.GetMetadata();

	DirectX::Rect rect;
	rect.y = 0;
	rect.h = pixelSize;
	rect.w = pixelSize;

	std::vector<DirectX::Image>			image_result(pixelSize);
	std::vector<DirectX::ScratchImage>	scrimage_result(pixelSize);

	for (UINT i = 0; i < pixelSize; i++)
	{
		rect.x = pixelSize * i;

		image_data.width = pixelSize;
		image_data.height = pixelSize;
		image_data.mipLevels = 1;

		result = scrimage_result[i].Initialize(image_data);

		result = DirectX::CopyRectangle(*image_source, rect, *scrimage_result[i].GetImages(), DirectX::TEX_FILTER_FLAGS::TEX_FILTER_DEFAULT, 0, 0);

		if (FAILED(result))
		{
			PROFILE_RESULT(PROFILE_OUTPUT::LOG_FILE, result, "[ Graphic ][ Save ][ VolumeTexture DDS ] '%s' FAILED!!", saveName);
			PROFILE_RESULT(PROFILE_OUTPUT::VS_CODE, result, "[ Graphic ][ Save ][ VolumeTexture DDS ] '%s' FAILED!!", saveName);

			return result;
		}

		image_result[i] = *scrimage_result[i].GetImages();
	}

	DirectX::TexMetadata tex;
	tex.format = image_source->format;
	tex.width = pixelSize;
	tex.height = pixelSize;
	tex.depth = pixelSize;
	tex.arraySize = 1;
	tex.mipLevels = 1;
	tex.miscFlags = 0;
	tex.miscFlags2 = 0;
	tex.dimension = DirectX::TEX_DIMENSION::TEX_DIMENSION_TEXTURE3D;

	std::string savePath(saveName);
	std::wstring w_savePath(savePath.begin(), savePath.end());

	result = DirectX::SaveToDDSFile(&image_result[0], pixelSize, tex, DirectX::DDS_FLAGS::DDS_FLAGS_NONE, w_savePath.c_str());

	if (FAILED(result))
	{
		PROFILE_RESULT(PROFILE_OUTPUT::LOG_FILE, result, "[ Graphic ][ Save ][ VolumeTexture DDS ] '%s' FAILED!!", saveName);
		PROFILE_RESULT(PROFILE_OUTPUT::VS_CODE, result, "[ Graphic ][ Save ][ VolumeTexture DDS ] '%s' FAILED!!", saveName);

		return result;
	}

	if (ppResource)
	{
		DirectX::ScratchImage scrimage;
		result = scrimage.Initialize3DFromImages(&image_result[0], pixelSize);

		if (FAILED(result))
		{
			PROFILE_RESULT(PROFILE_OUTPUT::LOG_FILE, result, "[ Graphic ][ Save ][ VolumeTexture DDS ] '%s' FAILED!!", saveName);
			PROFILE_RESULT(PROFILE_OUTPUT::VS_CODE, result, "[ Graphic ][ Save ][ VolumeTexture DDS ] '%s' FAILED!!", saveName);

			return result;
		}

		ID3D11ShaderResourceView* pSRV = nullptr;
		result = DirectX::CreateShaderResourceView(m_Device.Get(), scrimage.GetImages(), pixelSize, tex, &pSRV);

		if (FAILED(result))
		{
			PROFILE_RESULT(PROFILE_OUTPUT::LOG_FILE, result, "[ Graphic ][ Save ][ VolumeTexture DDS ] '%s' FAILED!!", saveName);
			PROFILE_RESULT(PROFILE_OUTPUT::VS_CODE, result, "[ Graphic ][ Save ][ VolumeTexture DDS ] '%s' FAILED!!", saveName);

			return result;
		}

		// �ش� ���ҽ� ����..
		*ppResource = pSRV;
	}

	return result;
}

HRESULT D3D11Graphic::SaveVolumeTextureDDS(const char* fileName, const char* saveName, UINT pixelSize)
{
	HRESULT result;
	std::string filePath(fileName);
	std::wstring w_filePath(filePath.begin(), filePath.end());

	std::size_t Start = filePath.rfind('.') + 1;
	std::size_t End = filePath.length() - Start;
	std::string Type = filePath.substr(Start, End);

	DirectX::ScratchImage scrimage_source;

	// Ȯ���ڿ� ���� �ؽ�ó ���� �ε� ���..
	if (Type == "dds" || Type == "DDS")
	{
		result = DirectX::LoadFromDDSFile(w_filePath.c_str(), DirectX::DDS_FLAGS::DDS_FLAGS_NONE, nullptr, scrimage_source);
	}
	else
	{
		result = DirectX::LoadFromWICFile(w_filePath.c_str(), DirectX::WIC_FLAGS::WIC_FLAGS_NONE, nullptr, scrimage_source);
	}

	if (FAILED(result))
	{
		PROFILE_RESULT(PROFILE_OUTPUT::LOG_FILE, result, "[ Graphic ][ Save ][ SaveVolumeTextureDDS ] '%s' FAILED!!", filePath.c_str());
		PROFILE_RESULT(PROFILE_OUTPUT::VS_CODE, result, "[ Graphic ][ Save ][ SaveVolumeTextureDDS ] '%s' FAILED!!", filePath.c_str());
		return result;
	}

	const DirectX::Image* image_source = scrimage_source.GetImages();
	DirectX::TexMetadata image_data = scrimage_source.GetMetadata();

	DirectX::Rect rect;
	rect.y = 0;
	rect.h = pixelSize;
	rect.w = pixelSize;

	std::vector<DirectX::Image>			image_result(pixelSize);
	std::vector<DirectX::ScratchImage>	scrimage_result(pixelSize);

	for (UINT i = 0; i < pixelSize; i++)
	{
		rect.x = pixelSize * i;

		image_data.width = pixelSize;
		image_data.height = pixelSize;

		scrimage_result[i].Initialize(image_data);

		result = DirectX::CopyRectangle(*image_source, rect, *scrimage_result[i].GetImages(), DirectX::TEX_FILTER_FLAGS::TEX_FILTER_DEFAULT, 0, 0);

		if (FAILED(result))
		{
			PROFILE_RESULT(PROFILE_OUTPUT::LOG_FILE, result, "[ Graphic ][ Save ][ SaveVolumeTextureDDS ] '%s' FAILED!!", filePath.c_str());
			PROFILE_RESULT(PROFILE_OUTPUT::VS_CODE, result, "[ Graphic ][ Save ][ SaveVolumeTextureDDS ] '%s' FAILED!!", filePath.c_str());
			return result;
		}

		image_result[i] = *scrimage_result[i].GetImages();
	}

	DirectX::TexMetadata tex;
	tex.format = image_source->format;
	tex.width = pixelSize;
	tex.height = pixelSize;
	tex.depth = pixelSize;
	tex.arraySize = 1;
	tex.mipLevels = 1;
	tex.miscFlags = 0;
	tex.dimension = DirectX::TEX_DIMENSION::TEX_DIMENSION_TEXTURE3D;

	std::string savePath(saveName);
	std::wstring w_savePath(savePath.begin(), savePath.end());
	result = DirectX::SaveToDDSFile(&image_result[0], 16, tex, DirectX::DDS_FLAGS::DDS_FLAGS_NONE, w_savePath.c_str());

	if (FAILED(result))
	{
		PROFILE_RESULT(PROFILE_OUTPUT::LOG_FILE, result, "[ Graphic ][ Save ][ SaveVolumeTextureDDS ] '%s' FAILED!!", filePath.c_str());
		PROFILE_RESULT(PROFILE_OUTPUT::VS_CODE, result, "[ Graphic ][ Save ][ SaveVolumeTextureDDS ] '%s' FAILED!!", filePath.c_str());
	}

	return result;
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

	// Buffer Resource ����..
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

	// Buffer Resource ����..
	return m_Device->CreateBuffer(bufferDesc, subData, buffer);
}

HRESULT D3D11Graphic::CreateTexture2D(D3D11_TEXTURE2D_DESC* texDesc, D3D11_SUBRESOURCE_DATA* subData, ID3D11Texture2D** tex2D)
{
	if (texDesc == nullptr) return E_INVALIDARG;

	// Texture2D Resource ����..
	return m_Device->CreateTexture2D(texDesc, subData, tex2D);
}

HRESULT D3D11Graphic::CreateTextureBuffer(std::string filePath, ID3D11Resource** resource, ID3D11ShaderResourceView** srv)
{
	HRESULT result;
	std::wstring wPath(filePath.begin(), filePath.end());

	std::size_t Start = filePath.rfind('.') + 1;
	std::size_t End = filePath.length() - Start;
	std::string Type = filePath.substr(Start, End);

	// Ȯ���ڿ� ���� �ؽ�ó ���� �ε� ���..
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

HRESULT D3D11Graphic::CreateBackBuffer(UINT width, UINT height, ID3D11Texture2D** tex2D, ID3D11RenderTargetView** rtv, ID3D11ShaderResourceView** srv, ID3D11UnorderedAccessView** uav)
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

	result = m_Device->CreateShaderResourceView(*tex2D, nullptr, srv);

	if (FAILED(result))
	{
		PROFILE_RESULT(PROFILE_OUTPUT::LOG_FILE, result, "[ Graphic ][ Create ][ RenderTargetView ] 'BackBuffer' FAILED!!");
		PROFILE_RESULT(PROFILE_OUTPUT::VS_CODE, result, "[ Graphic ][ Create ][ RenderTargetView ] 'BackBuffer' FAILED!!");
	}

	result = m_Device->CreateUnorderedAccessView(*tex2D, nullptr, uav);

	if (FAILED(result))
	{
		PROFILE_RESULT(PROFILE_OUTPUT::LOG_FILE, result, "[ Graphic ][ Create ][ UnorderedAccessView ] 'BackBuffer' FAILED!!");
		PROFILE_RESULT(PROFILE_OUTPUT::VS_CODE, result, "[ Graphic ][ Create ][ UnorderedAccessView ] 'BackBuffer' FAILED!!");
	}

	return result;
}

HRESULT D3D11Graphic::CreateDepthStencilView(ID3D11Resource* resource, D3D11_DEPTH_STENCIL_VIEW_DESC* dsvDesc, ID3D11DepthStencilView** dsv)
{
	// RenderTargetView Resource ����..
	return m_Device->CreateDepthStencilView(resource, dsvDesc, dsv);
}

HRESULT D3D11Graphic::CreateRenderTargetView(ID3D11Resource* resource, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc, ID3D11RenderTargetView** rtv)
{
	// RenderTargetView Resource ����..
	return m_Device->CreateRenderTargetView(resource, rtvDesc, rtv);
}

HRESULT D3D11Graphic::CreateShaderResourceView(ID3D11Resource* resource, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc, ID3D11ShaderResourceView** srv)
{
	// ShaderResourceView Resource ����..
	return m_Device->CreateShaderResourceView(resource, srvDesc, srv);
}

HRESULT D3D11Graphic::CreateUnorderedAccessView(ID3D11Resource* resource, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc, ID3D11UnorderedAccessView** uav)
{
	// UnorderedAccessView Resource ����..
	return m_Device->CreateUnorderedAccessView(resource, uavDesc, uav);
}

HRESULT D3D11Graphic::CreateDepthStencilState(D3D11_DEPTH_STENCIL_DESC* dssDesc, ID3D11DepthStencilState** dss)
{
	if (dssDesc == nullptr) return E_INVALIDARG;

	// DepthStencilState Resource ����..
	return m_Device->CreateDepthStencilState(dssDesc, dss);
}

HRESULT D3D11Graphic::CreateRasterizerState(D3D11_RASTERIZER_DESC* rsDesc, ID3D11RasterizerState** rs)
{
	if (rsDesc == nullptr) return E_INVALIDARG;

	// RasterizerState Resource ����..
	return m_Device->CreateRasterizerState(rsDesc, rs);
}

HRESULT D3D11Graphic::CreateBlendState(D3D11_BLEND_DESC* bsDesc, ID3D11BlendState** bs)
{
	if (bsDesc == nullptr) return E_INVALIDARG;
	
	// BlendState Resource ����..
	return m_Device->CreateBlendState(bsDesc, bs);
}

HRESULT D3D11Graphic::CreateSamplerState(D3D11_SAMPLER_DESC* ssDesc, ID3D11SamplerState** ss)
{
	if (ssDesc == nullptr) return E_INVALIDARG;

	// SamplerState Resource ����..
	return m_Device->CreateSamplerState(ssDesc, ss);
}

void D3D11Graphic::GetImageSize(std::string filePath, UINT* width, UINT* height)
{
	HRESULT result;
	std::wstring wPath(filePath.begin(), filePath.end());

	std::size_t Start = filePath.rfind('.') + 1;
	std::size_t End = filePath.length() - Start;
	std::string Type = filePath.substr(Start, End);

	DirectX::TexMetadata tex;
	DirectX::ScratchImage image;

	// Ȯ���ڿ� ���� �ؽ�ó ���� �ε� ���..
	if (Type == "dds" || Type == "DDS")
	{
		result = DirectX::LoadFromDDSFile(wPath.c_str(), DirectX::DDS_FLAGS::DDS_FLAGS_NONE, &tex, image);
	}
	else if (Type == "hdr" || Type == "HDR")
	{
		result = DirectX::LoadFromHDRFile(wPath.c_str(), &tex, image);
	}
	else
	{
		result = DirectX::LoadFromWICFile(wPath.c_str(), DirectX::WIC_FLAGS::WIC_FLAGS_NONE, &tex, image);
	}

	if (FAILED(result))
	{
		PROFILE_RESULT(PROFILE_OUTPUT::LOG_FILE, result, "[ Graphic ][ Get ][ ImageSize ] '%s' FAILED!!", filePath.c_str());
		PROFILE_RESULT(PROFILE_OUTPUT::VS_CODE, result, "[ Graphic ][ Get ][ ImageSize ] '%s' FAILED!!", filePath.c_str());
	}
	else
	{
		*width = (UINT)tex.width;
		*height = (UINT)tex.height;
	}
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

