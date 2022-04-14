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
	Microsoft::WRL::ComPtr<ID3D11Texture2D> brdflutTex2D = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> brdflutRTV = nullptr;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> brdflutSRV = nullptr;
	
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

	g_Graphic->CreateTexture2D(&brdflutDesc, nullptr, brdflutTex2D.GetAddressOf());
	g_Graphic->CreateRenderTargetView(brdflutTex2D.Get(), &brdflutRTVDesc, brdflutRTV.GetAddressOf());
	g_Graphic->CreateShaderResourceView(brdflutTex2D.Get(), &brdflutSRVDesc, brdflutSRV.GetAddressOf());

	// ShaderResourceView 积己..
	ShaderResourceView* newResource = new ShaderResourceView(gBRDFlut::GetHashCode(), brdflutSRV.Get());

	// Resource 殿废..
	g_ResourceManager->AddResource<gBRDFlut>(newResource);

	// BRDF LUT Texture Create..
	context->OMSetRenderTargets(1, brdflutRTV.GetAddressOf(), 0);
	context->RSSetViewports(1, &brdflutViewport);
	context->ClearRenderTargetView(brdflutRTV.Get(), reinterpret_cast<const float*>(&DXColors::DeepDarkGray));

	screenVS->Update();

	brdflutPS->Update();

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->IASetVertexBuffers(0, 1, quadDB->VertexBuf->GetAddress(), &quadDB->Stride, &quadDB->Offset);
	context->IASetIndexBuffer(quadDB->IndexBuf->Get(), DXGI_FORMAT_R32_UINT, 0);

	context->DrawIndexed(quadDB->IndexCount, 0, 0);

	// Save Texture..
	g_Graphic->SaveTextureDDS(brdflutSRV.Get(), "BRDF_LUT");

	// Debug Name..
	CPU_RESOURCE_DEBUG_NAME(newResource, "gBRDFlut");

	GPU_RESOURCE_DEBUG_NAME(brdflutSRV.Get(), "gBRDFlut");

	RESET_COM(context);
	RESET_COM(brdflutSRV);
	RELEASE_COM(brdflutRTV);
	RELEASE_COM(brdflutTex2D);
}

void BakingFactory::PreBakeEnvironmentMap(EnvironmentBuffer* tex)
{
	if (tex == nullptr) return;

	// 货肺款 Resource Pointer 积己..
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context = g_Graphic->GetContext();
	Microsoft::WRL::ComPtr<ID3D11Texture2D> irradianceTex2D = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> irradianceRTV = nullptr;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> irradianceSRV = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> prefilterTex2D = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> prefilterRTV = nullptr;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> prefilterSRV = nullptr;

	D3D11_TEXTURE2D_DESC irradianceDesc;
	ZeroMemory(&irradianceDesc, sizeof(irradianceDesc));
	irradianceDesc.Width = 64;
	irradianceDesc.Height = 64;
	irradianceDesc.MipLevels = 1;
	irradianceDesc.ArraySize = 6;
	irradianceDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
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
	prefilterDesc.Width = 256;
	prefilterDesc.Height = 256;
	prefilterDesc.MipLevels = maxMipLevels;
	prefilterDesc.ArraySize = 6;
	prefilterDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
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

	g_Graphic->CreateTexture2D(&irradianceDesc, nullptr, irradianceTex2D.GetAddressOf());
	g_Graphic->CreateTexture2D(&prefilterDesc, nullptr, prefilterTex2D.GetAddressOf());
	g_Graphic->CreateShaderResourceView(irradianceTex2D.Get(), &irradianceSRVDesc, irradianceSRV.GetAddressOf());
	g_Graphic->CreateShaderResourceView(prefilterTex2D.Get(), &prefilterSRVDesc, prefilterSRV.GetAddressOf());

	XMVECTOR tar[] = { XMVectorSet(1, 0, 0, 0), XMVectorSet(-1, 0, 0, 0), XMVectorSet(0, 1, 0, 0), XMVectorSet(0, -1, 0, 0), XMVectorSet(0, 0, 1, 0), XMVectorSet(0, 0, -1, 0) };
	XMVECTOR up[] = { XMVectorSet(0, 1, 0, 0), XMVectorSet(0, 1, 0, 0), XMVectorSet(0, 0, -1, 0), XMVectorSet(0, 0, 1, 0), XMVectorSet(0, 1, 0, 0), XMVectorSet(0, 1, 0, 0) };

	ID3D11ShaderResourceView* skycube		= (ID3D11ShaderResourceView*)tex->Environment->pTextureBuf;
	ID3D11RasterizerState* cubemapRS		= g_ResourceManager->GetRasterizerState<RS_CubeMap>()->Get();
	ID3D11DepthStencilState* cubemapDSS		= g_ResourceManager->GetDepthStencilState<DSS_CubeMap>()->Get();

	DrawBuffer* boxDB = g_ResourceManager->GetDrawBuffer<DB_Box>();

	VertexShader* cubmapVS		= g_ShaderManager->GetShader("SkyBox_VS");
	PixelShader* irradiancePS	= g_ShaderManager->GetShader("IBL_Convolution_PS");
	PixelShader* prefilterPS	= g_ShaderManager->GetShader("IBL_PrefilterMap_PS");

	context->OMSetDepthStencilState(cubemapDSS, 0);
	context->RSSetState(cubemapRS);
	context->GenerateMips(prefilterSRV.Get());

	// Irradiance IBL EnvMap Create..
	for (unsigned int i = 0; i < 6; i++)
	{
		irradianceRTVDesc.Texture2DArray.FirstArraySlice = i;
		g_Graphic->CreateRenderTargetView(irradianceTex2D.Get(), &irradianceRTVDesc, irradianceRTV.GetAddressOf());

		XMVECTOR dir = XMVector3Rotate(tar[i], XMQuaternionIdentity());

		XMMATRIX view = DirectX::XMMatrixLookToLH(XMVectorSet(0, 0, 0, 0), dir, up[i]);
		XMMATRIX proj = DirectX::XMMatrixPerspectiveFovLH(0.5f * XM_PI, 1.0f, 0.1f, 4000.0f);

		context->OMSetRenderTargets(1, irradianceRTV.GetAddressOf(), 0);
		context->RSSetViewports(1, &irradianceViewport);
		context->ClearRenderTargetView(irradianceRTV.Get(), reinterpret_cast<const float*>(&DXColors::DeepDarkGray));

		CB_CubeObject cubeBuf;
		cubeBuf.gViewProj = view * proj;

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

		prefilterViewport.Width = 256.0f * powf(0.5f, (float)mip);
		prefilterViewport.Height = 256.0f * powf(0.5f, (float)mip);

		float roughness = (float)mip / (float)(maxMipLevels - 1);
		for (int i = 0; i < 6; i++)
		{
			prefilterRTVDesc.Texture2DArray.FirstArraySlice = i;
			g_Graphic->CreateRenderTargetView(prefilterTex2D.Get(), &prefilterRTVDesc, prefilterRTV.GetAddressOf());

			XMVECTOR dir = XMVector3Rotate(tar[i], XMQuaternionIdentity());

			XMMATRIX view = DirectX::XMMatrixLookToLH(XMVectorSet(0, 0, 0, 0), dir, up[i]);
			XMMATRIX proj = DirectX::XMMatrixPerspectiveFovLH(0.5f * XM_PI, 1.0f, 0.1f, 4000.0f);

			context->OMSetRenderTargets(1, prefilterRTV.GetAddressOf(), 0);
			context->RSSetViewports(1, &prefilterViewport);
			context->ClearRenderTargetView(prefilterRTV.Get(), reinterpret_cast<const float*>(&DXColors::DeepDarkGray));

			CB_CubeObject cubeBuf;
			cubeBuf.gViewProj = view * proj;

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

	g_Graphic->SaveTextureDDS(irradianceSRV.Get(), std::string(tex->Environment->Name + "_Irradiance").c_str());
	g_Graphic->SaveTextureDDS(prefilterSRV.Get(), std::string(tex->Environment->Name + "_Prefilter").c_str());

	// Debug Name..
	GPU_RESOURCE_DEBUG_NAME(irradianceSRV.Get(), "gIBLIrradiance");
	GPU_RESOURCE_DEBUG_NAME(prefilterSRV.Get(), "gIBLPrefilter");

	// Resource 火涝..
	tex->Irradiance = new TextureBuffer();
	tex->Irradiance->pTextureBuf = irradianceSRV.Detach();

	tex->Prefilter = new TextureBuffer();
	tex->Prefilter->pTextureBuf = prefilterSRV.Detach();

	RESET_COM(context);
	RESET_COM(irradianceSRV);
	RESET_COM(prefilterSRV);
	RELEASE_COM(irradianceTex2D);
	RELEASE_COM(prefilterTex2D);
}
