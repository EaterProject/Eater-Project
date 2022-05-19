#include "DirectDefine.h"
#include "D3D11GraphicBase.h"
#include "ResourceManagerBase.h"
#include "ShaderManagerBase.h"
#include "GraphicState.h"
#include "GraphicView.h"
#include "GraphicResource.h"
#include "DrawBuffer.h"
#include "ShaderBase.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "BakingFactory.h"

#include "MathDefine.h"
#include "RasterizerStateDefine.h"
#include "DepthStencilStateDefine.h"
#include "ShaderResourceViewDefine.h"
#include "ConstantBufferDefine.h"
#include "DrawBufferDefine.h"
#include "DepthStencilViewDefine.h"
#include "ResourceManager.h"
#include "RenderTargetDefine.h"

using namespace DirectX;

BakingFactory::BakingFactory()
{

}

BakingFactory::~BakingFactory()
{

}

void BakingFactory::Create(int width, int height)
{

}

void BakingFactory::Start()
{
	// BRDF Illuminance Map Pre Baking..
	PreBakeBRDFMap();
}

void BakingFactory::Release()
{

}

void BakingFactory::PreBakeShadowMap(std::string fileName)
{
	g_Graphic->CaptureTextureDDS(fileName.c_str());
}

void BakingFactory::PreBakeBRDFMap()
{
	// 货肺款 Resource Pointer 积己..
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context = g_Graphic->GetContext();
	ID3D11Texture2D* brdflutTex2D = nullptr;
	ID3D11RenderTargetView* brdflutRTV = nullptr;
	ID3D11ShaderResourceView* brdflutSRV = nullptr;
	
	DrawBuffer* quadDB = g_ResourceManager->GetDrawBuffer<DB_Quad>();
	VertexShader* screenVS = g_ShaderManager->GetShader("Screen_VS");
	PixelShader* brdflutPS = g_ShaderManager->GetShader("IBL_IntegrateBRDF_PS");

	D3D11_TEXTURE2D_DESC brdflutDesc;
	ZeroMemory(&brdflutDesc, sizeof(brdflutDesc));
	brdflutDesc.Width = 512;
	brdflutDesc.Height = 512;
	brdflutDesc.MipLevels = 1;
	brdflutDesc.ArraySize = 1;
	brdflutDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
	brdflutDesc.Usage = D3D11_USAGE_DEFAULT;
	brdflutDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	brdflutDesc.CPUAccessFlags = 0;
	brdflutDesc.MiscFlags = 0;
	brdflutDesc.SampleDesc.Count = 1;
	brdflutDesc.SampleDesc.Quality = 0;

	D3D11_RENDER_TARGET_VIEW_DESC brdflutRTVDesc;
	ZeroMemory(&brdflutRTVDesc, sizeof(brdflutRTVDesc));
	brdflutRTVDesc.Format = brdflutDesc.Format;
	brdflutRTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

	D3D11_SHADER_RESOURCE_VIEW_DESC brdflutSRVDesc;
	ZeroMemory(&brdflutSRVDesc, sizeof(brdflutSRVDesc));
	brdflutSRVDesc.Format = brdflutDesc.Format;
	brdflutSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	brdflutSRVDesc.TextureCube.MostDetailedMip = 0;
	brdflutSRVDesc.TextureCube.MipLevels = 1;

	D3D11_VIEWPORT brdflutViewport;
	brdflutViewport.Width = 512;
	brdflutViewport.Height = 512;
	brdflutViewport.MinDepth = 0.0f;
	brdflutViewport.MaxDepth = 1.0f;
	brdflutViewport.TopLeftX = 0.0f;
	brdflutViewport.TopLeftY = 0.0f;

	g_Graphic->CreateTexture2D(&brdflutDesc, nullptr, &brdflutTex2D);
	g_Graphic->CreateRenderTargetView(brdflutTex2D, &brdflutRTVDesc, &brdflutRTV);
	g_Graphic->CreateShaderResourceView(brdflutTex2D, &brdflutSRVDesc, &brdflutSRV);

	// ShaderResourceView 积己..
	ShaderResourceView* newResource = new ShaderResourceView(gBRDFlut::GetHashCode(), brdflutSRV);

	// Resource 殿废..
	g_ResourceManager->AddResource<gBRDFlut>(newResource);

	// BRDF LUT Texture Create..
	context->OMSetRenderTargets(1, &brdflutRTV, 0);
	context->RSSetViewports(1, &brdflutViewport);
	context->ClearRenderTargetView(brdflutRTV, reinterpret_cast<const float*>(&DXColors::DeepDarkGray));

	screenVS->Update();

	brdflutPS->Update();

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->IASetVertexBuffers(0, 1, quadDB->VertexBuf->GetAddress(), &quadDB->Stride, &quadDB->Offset);
	context->IASetIndexBuffer(quadDB->IndexBuf->Get(), DXGI_FORMAT_R32_UINT, 0);

	context->DrawIndexed(quadDB->IndexCount, 0, 0);

	// Save Texture..
	//g_Graphic->SaveTextureDDS(brdflutSRV, "BRDF_LUT");

	// Debug Name..
	CPU_RESOURCE_DEBUG_NAME(newResource, "gBRDFlut");

	GPU_RESOURCE_DEBUG_NAME(brdflutSRV, "gBRDFlut");

	RESET_COM(context);
	RELEASE_COM(brdflutRTV);
	RELEASE_COM(brdflutTex2D);
}

void BakingFactory::BakeSkyLightMap(TextureBuffer* resource, bool hdri, SkyLightBuffer* pResource)
{
	if (resource == nullptr) return;

	// 货肺款 Resource Pointer 积己..
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context = g_Graphic->GetContext();
	ID3D11Texture2D* irradianceTex2D = nullptr;
	ID3D11RenderTargetView* irradianceRTV = nullptr;
	ID3D11ShaderResourceView* irradianceSRV = nullptr;
	ID3D11Texture2D* prefilterTex2D = nullptr;
	ID3D11RenderTargetView* prefilterRTV = nullptr;
	ID3D11ShaderResourceView* prefilterSRV = nullptr;

	D3D11_TEXTURE2D_DESC irradianceDesc;
	ZeroMemory(&irradianceDesc, sizeof(irradianceDesc));
	irradianceDesc.Width = 64;
	irradianceDesc.Height = 64;
	irradianceDesc.MipLevels = 1;
	irradianceDesc.ArraySize = 6;
	irradianceDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	irradianceDesc.Usage = D3D11_USAGE_DEFAULT;
	irradianceDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	irradianceDesc.CPUAccessFlags = 0;
	irradianceDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE | D3D11_RESOURCE_MISC_GENERATE_MIPS;
	irradianceDesc.SampleDesc.Count = 1;
	irradianceDesc.SampleDesc.Quality = 0;

	D3D11_RENDER_TARGET_VIEW_DESC irradianceRTVDesc;
	ZeroMemory(&irradianceRTVDesc, sizeof(irradianceRTVDesc));
	irradianceRTVDesc.Format = irradianceDesc.Format;
	irradianceRTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
	irradianceRTVDesc.Texture2DArray.ArraySize = 1;
	irradianceRTVDesc.Texture2DArray.MipSlice = 0;

	D3D11_SHADER_RESOURCE_VIEW_DESC irradianceSRVDesc;
	ZeroMemory(&irradianceSRVDesc, sizeof(irradianceSRVDesc));
	irradianceSRVDesc.Format = irradianceDesc.Format;
	irradianceSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	irradianceSRVDesc.TextureCube.MostDetailedMip = 0;
	irradianceSRVDesc.TextureCube.MipLevels = 1;

	D3D11_VIEWPORT irradianceViewport;
	irradianceViewport.Width = 64;
	irradianceViewport.Height = 64;
	irradianceViewport.MinDepth = 0.0f;
	irradianceViewport.MaxDepth = 1.0f;
	irradianceViewport.TopLeftX = 0.0f;
	irradianceViewport.TopLeftY = 0.0f;

	unsigned int maxMipLevels = 5;
	D3D11_TEXTURE2D_DESC prefilterDesc;
	ZeroMemory(&prefilterDesc, sizeof(prefilterDesc));
	prefilterDesc.Width = 512;
	prefilterDesc.Height = 512;
	prefilterDesc.MipLevels = maxMipLevels;
	prefilterDesc.ArraySize = 6;
	prefilterDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	prefilterDesc.Usage = D3D11_USAGE_DEFAULT;
	prefilterDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	prefilterDesc.CPUAccessFlags = 0;
	prefilterDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE | D3D11_RESOURCE_MISC_GENERATE_MIPS;
	prefilterDesc.SampleDesc.Count = 1;
	prefilterDesc.SampleDesc.Quality = 0;

	D3D11_SHADER_RESOURCE_VIEW_DESC prefilterSRVDesc;
	ZeroMemory(&prefilterSRVDesc, sizeof(prefilterSRVDesc));
	prefilterSRVDesc.Format = prefilterDesc.Format;
	prefilterSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	prefilterSRVDesc.TextureCube.MostDetailedMip = 0;
	prefilterSRVDesc.TextureCube.MipLevels = maxMipLevels;

	D3D11_RENDER_TARGET_VIEW_DESC prefilterRTVDesc;
	ZeroMemory(&prefilterRTVDesc, sizeof(prefilterRTVDesc));
	prefilterRTVDesc.Format = prefilterDesc.Format;
	prefilterRTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
	prefilterRTVDesc.Texture2DArray.ArraySize = 1;
	prefilterRTVDesc.Texture2DArray.MipSlice = 0;

	D3D11_VIEWPORT prefilterViewport;
	prefilterViewport.Width = 0.0f;
	prefilterViewport.Height = 0.0f;
	prefilterViewport.MinDepth = 0.0f;
	prefilterViewport.MaxDepth = 1.0f;
	prefilterViewport.TopLeftX = 0.0f;
	prefilterViewport.TopLeftY = 0.0f;

	g_Graphic->CreateTexture2D(&irradianceDesc, nullptr, &irradianceTex2D);
	g_Graphic->CreateTexture2D(&prefilterDesc, nullptr, &prefilterTex2D);
	g_Graphic->CreateShaderResourceView(irradianceTex2D, &irradianceSRVDesc, &irradianceSRV);
	g_Graphic->CreateShaderResourceView(prefilterTex2D, &prefilterSRVDesc, &prefilterSRV);

	XMVECTOR tar[] = { XMVectorSet(1, 0, 0, 0), XMVectorSet(-1, 0, 0, 0), XMVectorSet(0, 1, 0, 0), XMVectorSet(0, -1, 0, 0), XMVectorSet(0, 0, 1, 0), XMVectorSet(0, 0, -1, 0) };
	XMVECTOR up[] = { XMVectorSet(0, 1, 0, 0), XMVectorSet(0, 1, 0, 0), XMVectorSet(0, 0, -1, 0), XMVectorSet(0, 0, 1, 0), XMVectorSet(0, 1, 0, 0), XMVectorSet(0, 1, 0, 0) };

	ID3D11ShaderResourceView* skycube		= (ID3D11ShaderResourceView*)resource->pTextureBuf;
	ID3D11RasterizerState* cubemapRS		= g_ResourceManager->GetRasterizerState<RS_CubeMap>()->Get();
	ID3D11DepthStencilState* cubemapDSS		= g_ResourceManager->GetDepthStencilState<DSS_CubeMap>()->Get();

	DrawBuffer* boxDB = g_ResourceManager->GetDrawBuffer<DB_Box>();

	VertexShader* cubmapVS		= g_ShaderManager->GetShader("SkyBox_VS");
	PixelShader* irradiancePS	= nullptr;
	PixelShader* prefilterPS	= nullptr;

	if (hdri)
	{
		irradiancePS = g_ShaderManager->GetShader("IBL_Convolution_HDRI_PS");
		prefilterPS = g_ShaderManager->GetShader("IBL_PrefilterMap_HDRI_PS");
	}
	else
	{
		irradiancePS = g_ShaderManager->GetShader("IBL_Convolution_PS");
		prefilterPS = g_ShaderManager->GetShader("IBL_PrefilterMap_PS");
	}

	context->OMSetDepthStencilState(cubemapDSS, 0);
	context->RSSetState(cubemapRS);
	context->GenerateMips(prefilterSRV);

	// Irradiance IBL EnvMap Create..
	for (unsigned int i = 0; i < 6; i++)
	{
		irradianceRTVDesc.Texture2DArray.FirstArraySlice = i;
		g_Graphic->CreateRenderTargetView(irradianceTex2D, &irradianceRTVDesc, &irradianceRTV);

		XMVECTOR dir = XMVector3Rotate(tar[i], XMQuaternionIdentity());

		XMMATRIX view = DirectX::XMMatrixLookToLH(XMVectorSet(0, 0, 0, 0), dir, up[i]);
		XMMATRIX proj = DirectX::XMMatrixPerspectiveFovLH(0.5f * XM_PI, 1.0f, 0.1f, 4000.0f);

		context->OMSetRenderTargets(1, &irradianceRTV, 0);
		context->RSSetViewports(1, &irradianceViewport);
		context->ClearRenderTargetView(irradianceRTV, reinterpret_cast<const float*>(&DXColors::DeepDarkGray));

		CB_CubeObject cubeBuf;
		cubeBuf.gCubeWorldViewProj = view * proj;

		cubmapVS->ConstantBufferUpdate(&cubeBuf);
		cubmapVS->Update();

		irradiancePS->SetShaderResourceView<gSkyCube>(skycube);
		irradiancePS->Update();

		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		context->IASetVertexBuffers(0, 1, boxDB->VertexBuf->GetAddress(), &boxDB->Stride, &boxDB->Offset);
		context->IASetIndexBuffer(boxDB->IndexBuf->Get(), DXGI_FORMAT_R32_UINT, 0);

		context->DrawIndexed(boxDB->IndexCount, 0, 0);

		RELEASE_COM(irradianceRTV);
	}

	// Prefilter IBL EnvMap Create..
	for (unsigned int mip = 0; mip < maxMipLevels; mip++)
	{
		prefilterRTVDesc.Texture2DArray.MipSlice = mip;

		prefilterViewport.Width = 512.0f * powf(0.5f, (float)mip);
		prefilterViewport.Height = 512.0f * powf(0.5f, (float)mip);

		float roughness = (float)mip / (float)(maxMipLevels - 1);
		for (int i = 0; i < 6; i++)
		{
			prefilterRTVDesc.Texture2DArray.FirstArraySlice = i;
			g_Graphic->CreateRenderTargetView(prefilterTex2D, &prefilterRTVDesc, &prefilterRTV);

			XMVECTOR dir = XMVector3Rotate(tar[i], XMQuaternionIdentity());

			XMMATRIX view = DirectX::XMMatrixLookToLH(XMVectorSet(0, 0, 0, 0), dir, up[i]);
			XMMATRIX proj = DirectX::XMMatrixPerspectiveFovLH(0.5f * XM_PI, 1.0f, 0.1f, 4000.0f);

			context->OMSetRenderTargets(1, &prefilterRTV, 0);
			context->RSSetViewports(1, &prefilterViewport);
			context->ClearRenderTargetView(prefilterRTV, reinterpret_cast<const float*>(&DXColors::DeepDarkGray));

			CB_CubeObject cubeBuf;
			cubeBuf.gCubeWorldViewProj = view * proj;

			cubmapVS->ConstantBufferUpdate(&cubeBuf);
			cubmapVS->Update();

			CB_ExternalData prefilterBuf;
			prefilterBuf.gRoughness = roughness;

			prefilterPS->ConstantBufferUpdate(&prefilterBuf);
			prefilterPS->SetShaderResourceView<gSkyCube>(skycube);
			prefilterPS->Update();

			context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			context->IASetVertexBuffers(0, 1, boxDB->VertexBuf->GetAddress(), &boxDB->Stride, &boxDB->Offset);
			context->IASetIndexBuffer(boxDB->IndexBuf->Get(), DXGI_FORMAT_R32_UINT, 0);

			context->DrawIndexed(boxDB->IndexCount, 0, 0);

			RELEASE_COM(prefilterRTV);
		}
	}

	// Debug Name..
	GPU_RESOURCE_DEBUG_NAME(irradianceSRV, "gIBLIrradiance");
	GPU_RESOURCE_DEBUG_NAME(prefilterSRV, "gIBLPrefilter");

	// Resource 火涝..
	pResource->Irradiance->pTextureBuf = irradianceSRV;
	pResource->Prefilter->pTextureBuf = prefilterSRV;

	// Resouce 秦力..
	RESET_COM(context);
	RELEASE_COM(irradianceTex2D);
	RELEASE_COM(prefilterTex2D);
}

void BakingFactory::BakeConvertCubeMap(TextureBuffer* resource, float angle, float threshold, bool hdri, TextureBuffer* pResource)
{
	if (resource == nullptr) return;

	if (resource->pTextureBuf == nullptr) return;

	// 货肺款 Resource Pointer 积己..
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context = g_Graphic->GetContext();
	ID3D11Texture2D* cubemapTex2D = nullptr;
	ID3D11RenderTargetView* cubemapRTV = nullptr;
	ID3D11ShaderResourceView* cubemapSRV = nullptr;

	ID3D11ShaderResourceView* skycube = (ID3D11ShaderResourceView*)resource->pTextureBuf;
	ID3D11RasterizerState* cubemapRS = g_ResourceManager->GetRasterizerState<RS_CubeMap>()->Get();
	ID3D11DepthStencilState* cubemapDSS = g_ResourceManager->GetDepthStencilState<DSS_CubeMap>()->Get();

	DrawBuffer* boxDB = g_ResourceManager->GetDrawBuffer<DB_Box>();

	VertexShader* cubemap_VS = g_ShaderManager->GetShader("SkyBox_VS");
	PixelShader* cubemap_convert_PS = nullptr;

	if (hdri)
	{
		cubemap_convert_PS = g_ShaderManager->GetShader("SkyBox_Convert_HDRI_PS");
	}
	else
	{
		cubemap_convert_PS = g_ShaderManager->GetShader("SkyBox_Convert_PS");
	}
	
	ID3D11Resource* origin_Tex2D = nullptr;
	skycube->GetResource(&origin_Tex2D);

	D3D11_TEXTURE2D_DESC origin_Desc;
	((ID3D11Texture2D*)origin_Tex2D)->GetDesc(&origin_Desc);

	D3D11_TEXTURE2D_DESC cubemapDesc;
	ZeroMemory(&cubemapDesc, sizeof(cubemapDesc));
	cubemapDesc.Width = 512;
	cubemapDesc.Height = 512;
	cubemapDesc.MipLevels = 1;
	cubemapDesc.ArraySize = 6;
	cubemapDesc.Format = origin_Desc.Format;
	cubemapDesc.Usage = D3D11_USAGE_DEFAULT;
	cubemapDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	cubemapDesc.CPUAccessFlags = 0;
	cubemapDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE | D3D11_RESOURCE_MISC_GENERATE_MIPS;
	cubemapDesc.SampleDesc.Count = 1;
	cubemapDesc.SampleDesc.Quality = 0;

	D3D11_SHADER_RESOURCE_VIEW_DESC cubemapSRVDesc;
	ZeroMemory(&cubemapSRVDesc, sizeof(cubemapSRVDesc));
	cubemapSRVDesc.Format = cubemapDesc.Format;
	cubemapSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	cubemapSRVDesc.TextureCube.MostDetailedMip = 0;
	cubemapSRVDesc.TextureCube.MipLevels = 1;

	D3D11_RENDER_TARGET_VIEW_DESC cubemapRTVDesc;
	ZeroMemory(&cubemapRTVDesc, sizeof(cubemapRTVDesc));
	cubemapRTVDesc.Format = cubemapDesc.Format;
	cubemapRTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
	cubemapRTVDesc.Texture2DArray.ArraySize = 1;
	cubemapRTVDesc.Texture2DArray.MipSlice = 0;

	D3D11_VIEWPORT cubemapViewport;
	cubemapViewport.Width = 512.0f;
	cubemapViewport.Height = 512.0f;
	cubemapViewport.MinDepth = 0.0f;
	cubemapViewport.MaxDepth = 1.0f;
	cubemapViewport.TopLeftX = 0.0f;
	cubemapViewport.TopLeftY = 0.0f;

	Matrix world = Matrix::CreateRotationY(angle * PI / 180.0f);

	XMVECTOR tar[] = { XMVectorSet(1, 0, 0, 0), XMVectorSet(-1, 0, 0, 0), XMVectorSet(0, 1, 0, 0), XMVectorSet(0, -1, 0, 0), XMVectorSet(0, 0, 1, 0), XMVectorSet(0, 0, -1, 0) };
	XMVECTOR up[] = { XMVectorSet(0, 1, 0, 0), XMVectorSet(0, 1, 0, 0), XMVectorSet(0, 0, -1, 0), XMVectorSet(0, 0, 1, 0), XMVectorSet(0, 1, 0, 0), XMVectorSet(0, 1, 0, 0) };

	g_Graphic->CreateTexture2D(&cubemapDesc, nullptr, &cubemapTex2D);
	g_Graphic->CreateShaderResourceView(cubemapTex2D, &cubemapSRVDesc, &cubemapSRV);

	context->OMSetDepthStencilState(cubemapDSS, 0);
	context->RSSetState(cubemapRS);

	for (int i = 0; i < 6; i++)
	{
		cubemapRTVDesc.Texture2DArray.FirstArraySlice = i;
		g_Graphic->CreateRenderTargetView(cubemapTex2D, &cubemapRTVDesc, &cubemapRTV);

		XMVECTOR dir = XMVector3Rotate(tar[i], XMQuaternionIdentity());

		XMMATRIX view = DirectX::XMMatrixLookToLH(XMVectorSet(0, 0, 0, 0), dir, up[i]);
		XMMATRIX proj = DirectX::XMMatrixPerspectiveFovLH(0.5f * XM_PI, 1.0f, 0.1f, 4000.0f);

		context->OMSetRenderTargets(1, &cubemapRTV, 0);
		context->RSSetViewports(1, &cubemapViewport);
		context->ClearRenderTargetView(cubemapRTV, reinterpret_cast<const float*>(&DXColors::DeepDarkGray));

		CB_CubeObject cubeBuf;
		cubeBuf.gCubeWorldViewProj = view * proj;
		cubeBuf.gCubeWorld = world;

		CB_CubeConvert convertBuf;
		convertBuf.gThreshold = threshold;

		cubemap_VS->ConstantBufferUpdate(&cubeBuf);
		cubemap_VS->Update();

		cubemap_convert_PS->ConstantBufferUpdate(&convertBuf);
		cubemap_convert_PS->SetShaderResourceView<gSkyCube>(skycube);
		cubemap_convert_PS->Update();

		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		context->IASetVertexBuffers(0, 1, boxDB->VertexBuf->GetAddress(), &boxDB->Stride, &boxDB->Offset);
		context->IASetIndexBuffer(boxDB->IndexBuf->Get(), DXGI_FORMAT_R32_UINT, 0);

		context->DrawIndexed(boxDB->IndexCount, 0, 0);

		RELEASE_COM(cubemapRTV);
	}

	pResource->pTextureBuf = cubemapSRV;

	// Debug Name..
	GPU_RESOURCE_DEBUG_NAME(cubemapSRV, "ConvertCubeMap");

	// Resouce 秦力..
	RESET_COM(context);
	RELEASE_COM(cubemapTex2D);
}

void BakingFactory::SaveConvertCubeMap(TextureBuffer* resource, std::string SaveName)
{
	ID3D11ShaderResourceView* srv = (ID3D11ShaderResourceView*)resource->pTextureBuf;

	if (SaveName == "")
	{
		g_Graphic->SaveTextureDDS(srv, resource->Name.c_str());
	}
	else
	{
		g_Graphic->SaveTextureDDS(srv, SaveName.c_str());
	}
}
