#include "DirectDefine.h"
#include "RenderPassBase.h"
#include "ShaderBase.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ComputeShader.h"
#include "GraphicState.h"
#include "GraphicView.h"
#include "GraphicResource.h"
#include "DrawBuffer.h"
#include "RenderTarget.h"
#include "BloomPass.h"

#include "MathDefine.h"
#include "FactoryManagerBase.h"
#include "ResourceManagerBase.h"
#include "ShaderManagerBase.h"
#include "RenderTargetDefine.h"
#include "DrawBufferDefine.h"
#include "ShaderResourceViewDefine.h"
#include "ConstantBufferDefine.h"
#include "UnorderedAccessViewDefine.h"
#include "DepthStencilStateDefine.h"
#include "ViewPortDefine.h"
#include "MathDefine.h"

#define BLOOM_VERTICAL_BLUR 1
#define BLOOM_HORIZONTAL_BLUR 1

BloomPass::BloomPass()
	:m_Down4x4(1.0f / 4.0f), m_Down24x24(1.0f / 24.0f), m_Down144x144(1.0f / 144.0f)
{

}

BloomPass::~BloomPass()
{

}

void BloomPass::Create(int width, int height)
{
	// Down Sampling용 돌려쓰기 위한 View Port 생성..
	g_Factory->CreateViewPort<VP_DownSampling>(0.0f, 0.0f, 1.0f, 1.0f);

	D3D11_TEXTURE2D_DESC bloomTexDesc;
	ZeroMemory(&bloomTexDesc, sizeof(bloomTexDesc));
	bloomTexDesc.MipLevels = 1;
	bloomTexDesc.ArraySize = 1;
	bloomTexDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	bloomTexDesc.SampleDesc.Count = 1;
	bloomTexDesc.Usage = D3D11_USAGE_DEFAULT;
	bloomTexDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	bloomTexDesc.CPUAccessFlags = 0;
	bloomTexDesc.MiscFlags = 0;

	D3D11_RENDER_TARGET_VIEW_DESC bloomRTVDesc;
	ZeroMemory(&bloomRTVDesc, sizeof(bloomRTVDesc));
	bloomRTVDesc.Format = bloomTexDesc.Format;
	bloomRTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	bloomRTVDesc.Texture2D.MipSlice = 0;

	D3D11_SHADER_RESOURCE_VIEW_DESC bloomSRVDesc;
	ZeroMemory(&bloomSRVDesc, sizeof(bloomSRVDesc));
	bloomSRVDesc.Format = bloomTexDesc.Format;
	bloomSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	bloomSRVDesc.Texture2D.MostDetailedMip = 0;
	bloomSRVDesc.Texture2D.MipLevels = 1;

	D3D11_UNORDERED_ACCESS_VIEW_DESC bloomUAVDesc;
	ZeroMemory(&bloomUAVDesc, sizeof(bloomUAVDesc));
	bloomUAVDesc.Format = bloomTexDesc.Format;
	bloomUAVDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	bloomUAVDesc.Texture2D.MipSlice = 0;

	// Origin RenderTarget 4x4 Down Sample + Bright RenderTarget 생성..
	bloomTexDesc.Width = (UINT)(width * m_Down4x4);
	bloomTexDesc.Height = (UINT)(height * m_Down4x4);
	g_Factory->CreateRenderTexture<RT_Bloom_Brightx4_1>(&bloomTexDesc, nullptr, &bloomRTVDesc, &bloomSRVDesc, &bloomUAVDesc);
	g_Factory->CreateRenderTexture<RT_Bloom_Brightx4_2>(&bloomTexDesc, nullptr, &bloomRTVDesc, &bloomSRVDesc, &bloomUAVDesc);

	// 4x4 Down Sample RenderTarget 24x24 Down Sample RenderTarget 생성..
	bloomTexDesc.Width = (UINT)(width * m_Down24x24);
	bloomTexDesc.Height = (UINT)(height * m_Down24x24);
	g_Factory->CreateRenderTexture<RT_Bloom_Downx24_1>(&bloomTexDesc, nullptr, &bloomRTVDesc, &bloomSRVDesc, &bloomUAVDesc);
	g_Factory->CreateRenderTexture<RT_Bloom_Downx24_2>(&bloomTexDesc, nullptr, &bloomRTVDesc, &bloomSRVDesc, &bloomUAVDesc);

	// 24x24 Down Sample RenderTarget 144x144 Down Sample RenderTarget 생성..
	bloomTexDesc.Width = (UINT)(width * m_Down144x144);
	bloomTexDesc.Height = (UINT)(height * m_Down144x144);
	g_Factory->CreateRenderTexture<RT_Bloom_Downx144_1>(&bloomTexDesc, nullptr, &bloomRTVDesc, &bloomSRVDesc, &bloomUAVDesc);
	g_Factory->CreateRenderTexture<RT_Bloom_Downx144_2>(&bloomTexDesc, nullptr, &bloomRTVDesc, &bloomSRVDesc, &bloomUAVDesc);

}

void BloomPass::Start(int width, int height)
{
	// Shader..
	m_Screen_VS = g_Shader->GetShader("Screen_VS");
	m_BloomBright_PS = g_Shader->GetShader("DownSampling_Bright_PS");
	m_BloomDownSampling_PS = g_Shader->GetShader("DownSampling_PS");
	m_BloomUpSampling_PS = g_Shader->GetShader("UpSampling_PS");
	m_BloomBlur_PS = g_Shader->GetShader("BloomBlur_PS");

	m_Screen_DB = g_Resource->GetDrawBuffer<DB_Quad>();

	// Thread Group Count 설정..
	m_ThreadGroupCountX = width / 16;
	m_ThreadGroupCountY = height / 16;

	// Graphic View..
	m_DownSample_VP = g_Resource->GetViewPort<VP_DownSampling>()->Get();

	m_Brightx4_RT1 = g_Resource->GetRenderTexture<RT_Bloom_Brightx4_1>();
	m_Brightx4_RT1->SetRatio(m_Down4x4);
	m_Brightx4_RT2 = g_Resource->GetRenderTexture<RT_Bloom_Brightx4_2>();
	m_Brightx4_RT2->SetRatio(m_Down4x4);
	m_Downx24_RT1 = g_Resource->GetRenderTexture<RT_Bloom_Downx24_1>();
	m_Downx24_RT1->SetRatio(m_Down24x24);
	m_Downx24_RT2 = g_Resource->GetRenderTexture<RT_Bloom_Downx24_2>();
	m_Downx24_RT2->SetRatio(m_Down24x24);
	m_Downx144_RT1 = g_Resource->GetRenderTexture<RT_Bloom_Downx144_1>();
	m_Downx144_RT1->SetRatio(m_Down144x144);
	m_Downx144_RT2 = g_Resource->GetRenderTexture<RT_Bloom_Downx144_2>();
	m_Downx144_RT2->SetRatio(m_Down144x144);

	m_Brightx4_SRV1 = m_Brightx4_RT1->GetSRV()->Get();
	m_Brightx4_SRV2 = m_Brightx4_RT2->GetSRV()->Get();
	m_Downx24_SRV1 = m_Downx24_RT1->GetSRV()->Get();
	m_Downx24_SRV2 = m_Downx24_RT2->GetSRV()->Get();
	m_Downx144_SRV1 = m_Downx144_RT1->GetSRV()->Get();
	m_Downx144_SRV2 = m_Downx144_RT2->GetSRV()->Get();

	m_Brightx4_RTV1 = m_Brightx4_RT1->GetRTV()->Get();
	m_Brightx4_RTV2 = m_Brightx4_RT2->GetRTV()->Get();
	m_Downx24_RTV1 = m_Downx24_RT1->GetRTV()->Get();
	m_Downx24_RTV2 = m_Downx24_RT2->GetRTV()->Get();
	m_Downx144_RTV1 = m_Downx144_RT1->GetRTV()->Get();
	m_Downx144_RTV2 = m_Downx144_RT2->GetRTV()->Get();

	m_Brightx4_UAV1 = m_Brightx4_RT1->GetUAV()->Get();
	m_Brightx4_UAV2 = m_Brightx4_RT2->GetUAV()->Get();
	m_Downx24_UAV1 = m_Downx24_RT1->GetUAV()->Get();
	m_Downx24_UAV2 = m_Downx24_RT2->GetUAV()->Get();
	m_Downx144_UAV1 = m_Downx144_RT1->GetUAV()->Get();
	m_Downx144_UAV2 = m_Downx144_RT2->GetUAV()->Get();


	// Sampling View Port Data 설정..
	SamplingData* downSampleData = nullptr;
	float down4x4width		= width * m_Down4x4;
	float down4x4height		= height * m_Down4x4;
	float down24x24width	= width * m_Down24x24;
	float down24x24height	= height * m_Down24x24;
	float down144x144width	= width * m_Down144x144;
	float down144x144height = height * m_Down144x144;

	downSampleData = &m_DownSampleData[0];
	downSampleData->HalfOffsetWidth		= (down4x4width - (UINT)down4x4width) / 2.0f;
	downSampleData->HalfOffsetHeight	= (down4x4height - (UINT)down4x4height) / 2.0f;
	downSampleData->SamplingWidth		= down4x4width - downSampleData->HalfOffsetWidth;
	downSampleData->SamplingHeight		= down4x4height - downSampleData->HalfOffsetHeight;
	downSampleData->OriginWidth			= (float)(UINT)down4x4width;
	downSampleData->OriginHeight		= (float)(UINT)down4x4height;
	downSampleData->TexelWidth			= 1.0f / (UINT)down4x4width;
	downSampleData->TexelHeight			= 1.0f / (UINT)down4x4height;

	downSampleData = &m_DownSampleData[1];
	downSampleData->HalfOffsetWidth		= (down24x24width - (UINT)down24x24width) / 2.0f;
	downSampleData->HalfOffsetHeight	= (down24x24height - (UINT)down24x24height) / 2.0f;
	downSampleData->SamplingWidth		= down24x24width - downSampleData->HalfOffsetWidth;
	downSampleData->SamplingHeight		= down24x24height - downSampleData->HalfOffsetHeight;
	downSampleData->OriginWidth			= (float)(UINT)down24x24width;
	downSampleData->OriginHeight		= (float)(UINT)down24x24height;
	downSampleData->TexelWidth			= 1.0f / (UINT)down24x24width;
	downSampleData->TexelHeight			= 1.0f / (UINT)down24x24height;

	downSampleData = &m_DownSampleData[2];
	downSampleData->HalfOffsetWidth		= (down144x144width - (UINT)down144x144width) / 2.0f;
	downSampleData->HalfOffsetHeight	= (down144x144height - (UINT)down144x144height) / 2.0f;
	downSampleData->SamplingWidth		= down144x144width - downSampleData->HalfOffsetWidth;
	downSampleData->SamplingHeight		= down144x144height - downSampleData->HalfOffsetHeight;
	downSampleData->OriginWidth			= (float)(UINT)down144x144width;
	downSampleData->OriginHeight		= (float)(UINT)down144x144height;
	downSampleData->TexelWidth			= 1.0f / (UINT)down144x144width;
	downSampleData->TexelHeight			= 1.0f / (UINT)down144x144height;

	// ShaderResource 설정..
	SetConstantBuffer();
}

void BloomPass::OnResize(int width, int height)
{
	// Thread Group Count 재설정..
	m_ThreadGroupCountX = width / 16;
	m_ThreadGroupCountY = height / 16;

	// Graphic View 재설정..
	m_Brightx4_SRV1 = m_Brightx4_RT1->GetSRV()->Get();
	m_Brightx4_SRV2 = m_Brightx4_RT2->GetSRV()->Get();
	m_Downx24_SRV1 = m_Downx24_RT1->GetSRV()->Get();
	m_Downx24_SRV2 = m_Downx24_RT2->GetSRV()->Get();
	m_Downx144_SRV1 = m_Downx144_RT1->GetSRV()->Get();
	m_Downx144_SRV2 = m_Downx144_RT2->GetSRV()->Get();

	m_Brightx4_RTV1 = m_Brightx4_RT1->GetRTV()->Get();
	m_Brightx4_RTV2 = m_Brightx4_RT2->GetRTV()->Get();
	m_Downx24_RTV1 = m_Downx24_RT1->GetRTV()->Get();
	m_Downx24_RTV2 = m_Downx24_RT2->GetRTV()->Get();
	m_Downx144_RTV1 = m_Downx144_RT1->GetRTV()->Get();
	m_Downx144_RTV2 = m_Downx144_RT2->GetRTV()->Get();
	m_Brightx4_UAV1 = m_Brightx4_RT1->GetUAV()->Get();
	m_Brightx4_UAV2 = m_Brightx4_RT2->GetUAV()->Get();
	m_Downx24_UAV1 = m_Downx24_RT1->GetUAV()->Get();
	m_Downx24_UAV2 = m_Downx24_RT2->GetUAV()->Get();
	m_Downx144_UAV1 = m_Downx144_RT1->GetUAV()->Get();
	m_Downx144_UAV2 = m_Downx144_RT2->GetUAV()->Get();

	// Sampling View Port Data 설정..
	SamplingData* downSampleData = nullptr;
	float down4x4width		= width * m_Down4x4;
	float down4x4height		= height * m_Down4x4;
	float down24x24width	= width * m_Down24x24;
	float down24x24height	= height * m_Down24x24;
	float down144x144width	= width * m_Down144x144;
	float down144x144height = height * m_Down144x144;

	downSampleData = &m_DownSampleData[0];
	downSampleData->HalfOffsetWidth		= (down4x4width - (UINT)down4x4width) / 2.0f;
	downSampleData->HalfOffsetHeight	= (down4x4height - (UINT)down4x4height) / 2.0f;
	downSampleData->SamplingWidth		= down4x4width - downSampleData->HalfOffsetWidth;
	downSampleData->SamplingHeight		= down4x4height - downSampleData->HalfOffsetHeight;
	downSampleData->OriginWidth			= (float)(UINT)down4x4width;
	downSampleData->OriginHeight		= (float)(UINT)down4x4height;
	downSampleData->TexelWidth			= 1.0f / (UINT)down4x4width;
	downSampleData->TexelHeight			= 1.0f / (UINT)down4x4height;

	downSampleData = &m_DownSampleData[1];
	downSampleData->HalfOffsetWidth		= (down24x24width - (UINT)down24x24width) / 2.0f;
	downSampleData->HalfOffsetHeight	= (down24x24height - (UINT)down24x24height) / 2.0f;
	downSampleData->SamplingWidth		= down24x24width - downSampleData->HalfOffsetWidth;
	downSampleData->SamplingHeight		= down24x24height - downSampleData->HalfOffsetHeight;
	downSampleData->OriginWidth			= (float)(UINT)down24x24width;
	downSampleData->OriginHeight		= (float)(UINT)down24x24height;
	downSampleData->TexelWidth			= 1.0f / (UINT)down24x24width;
	downSampleData->TexelHeight			= 1.0f / (UINT)down24x24height;

	downSampleData = &m_DownSampleData[2];
	downSampleData->HalfOffsetWidth		= (down144x144width - (UINT)down144x144width) / 2.0f;
	downSampleData->HalfOffsetHeight	= (down144x144height - (UINT)down144x144height) / 2.0f;
	downSampleData->SamplingWidth		= down144x144width - downSampleData->HalfOffsetWidth;
	downSampleData->SamplingHeight		= down144x144height - downSampleData->HalfOffsetHeight;
	downSampleData->OriginWidth			= (float)(UINT)down144x144width;
	downSampleData->OriginHeight		= (float)(UINT)down144x144height;
	downSampleData->TexelWidth			= 1.0f / (UINT)down144x144width;
	downSampleData->TexelHeight			= 1.0f / (UINT)down144x144height;

	// ShaderResource 설정..
	ShaderResourceView* originSRV = m_Origin_RT->GetSRV();

	m_BloomBright_PS->SetShaderResourceView<gOriginMap>(originSRV->Get());
}

void BloomPass::Release()
{

}

void BloomPass::SetOption(RenderOption* renderOption)
{
	if (renderOption->PostProcessOption & RENDER_FOG)
	{
		m_Origin_RT = g_Resource->GetRenderTexture<RT_OutPut1>();
	}
	else
	{
		m_Origin_RT = g_Resource->GetRenderTexture<RT_OutPut2>();
	}

	// ShaderResource 설정..
	ShaderResourceView* originSRV = m_Origin_RT->GetSRV();

	m_BloomBright_PS->SetShaderResourceView<gOriginMap>(originSRV->Get());

	g_Context->ClearRenderTargetView(m_Brightx4_RTV2, reinterpret_cast<const float*>(&DXColors::NonBlack));
}

void BloomPass::RenderUpdate()
{
	ID3D11RenderTargetView* rtv[2] = { nullptr,nullptr };
	ID3D11ShaderResourceView* srv[2] = { nullptr,nullptr };

	/// DownSampling 4x4 + Bright
	GPU_MARKER_DEBUG_NAME("Bloom Down 4x4 + Bright");
	BrightRender();

	/// DownSampling 24x24
	GPU_MARKER_DEBUG_NAME("Bloom Down 24x24");
	DownSampling(m_Downx24_RTV1, m_Brightx4_SRV1, m_DownSampleData[1]);

	/// DownSampling 144x144 + Gaussian Blur
	GPU_MARKER_DEBUG_NAME("Bloom Down 144x144 + Blur");
	DownSampling(m_Downx144_RTV1, m_Downx24_SRV1, m_DownSampleData[2]);

	rtv[0] = m_Downx144_RTV2; rtv[1] = m_Downx144_RTV1;
	srv[0] = m_Downx144_SRV1; srv[1] = m_Downx144_SRV2;
	GaussianBlur(rtv, srv, m_DownSampleData[2]);

	/// UpSampling + Gaussian Blur
	GPU_MARKER_DEBUG_NAME("Bloom Up 24x24 + Blur");
	UpSampling(m_Downx24_RTV2, m_Downx24_SRV1, m_Downx144_SRV1, m_DownSampleData[1]);

	rtv[0] = m_Downx24_RTV1; rtv[1] = m_Downx24_RTV2;
	srv[0] = m_Downx24_SRV2; srv[1] = m_Downx24_SRV1;
	GaussianBlur(rtv, srv, m_DownSampleData[1]);

	/// UpSampling + Gaussian Blur
	GPU_MARKER_DEBUG_NAME("Bloom Up 4x4 + Blur");
	UpSampling(m_Brightx4_RTV2, m_Brightx4_SRV1, m_Downx24_SRV2, m_DownSampleData[0]);

	rtv[0] = m_Brightx4_RTV1; rtv[1] = m_Brightx4_RTV2;
	srv[0] = m_Brightx4_SRV2; srv[1] = m_Brightx4_SRV1;
	GaussianBlur(rtv, srv, m_DownSampleData[0]);
}

void BloomPass::BrightRender()
{
	// 해당 DownSample Screen 설정..
	SetSamplingViewPort(m_DownSampleData[0]);

	g_Context->OMSetRenderTargets(1, &m_Brightx4_RTV1, nullptr);
	g_Context->RSSetViewports(1, m_DownSample_VP);

	// Vertex Shader Update..
	m_Screen_VS->Update();

	// Pixel Shader Update..
	m_BloomBright_PS->Update();

	g_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	g_Context->IASetVertexBuffers(0, 1, m_Screen_DB->VertexBuf->GetAddress(), &m_Screen_DB->Stride, &m_Screen_DB->Offset);
	g_Context->IASetIndexBuffer(m_Screen_DB->IndexBuf->Get(), DXGI_FORMAT_R32_UINT, 0);

	g_Context->DrawIndexed(m_Screen_DB->IndexCount, 0, 0);
}

void BloomPass::DownSampling(ID3D11RenderTargetView* rtv, ID3D11ShaderResourceView* originSRV, const SamplingData& downScreen)
{
	// 해당 DownSample Screen 설정..
	SetSamplingViewPort(downScreen);

	g_Context->OMSetRenderTargets(1, &rtv, nullptr);
	g_Context->RSSetViewports(1, m_DownSample_VP);

	// Vertex Shader Update..
	m_Screen_VS->Update();

	// Pixel Shader Update..
	m_BloomDownSampling_PS->SetShaderResourceView<gOriginMap>(originSRV);
	m_BloomDownSampling_PS->Update();

	g_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	g_Context->IASetVertexBuffers(0, 1, m_Screen_DB->VertexBuf->GetAddress(), &m_Screen_DB->Stride, &m_Screen_DB->Offset);
	g_Context->IASetIndexBuffer(m_Screen_DB->IndexBuf->Get(), DXGI_FORMAT_R32_UINT, 0);

	g_Context->DrawIndexed(m_Screen_DB->IndexCount, 0, 0);
}

void BloomPass::UpSampling(ID3D11RenderTargetView* rtv, ID3D11ShaderResourceView* originSRV, ID3D11ShaderResourceView* downSRV, const SamplingData& downScreen)
{
	// 해당 UpSample Screen 설정..
	SetSamplingViewPort(downScreen);

	g_Context->OMSetRenderTargets(1, &rtv, nullptr);
	g_Context->RSSetViewports(1, m_DownSample_VP);

	// Vertex Shader Update..
	m_Screen_VS->Update();

	// Pixel Shader Update..
	m_BloomUpSampling_PS->SetShaderResourceView<gOriginMap>(originSRV);
	m_BloomUpSampling_PS->SetShaderResourceView<gDownMap>(downSRV);
	m_BloomUpSampling_PS->Update();

	g_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	g_Context->IASetVertexBuffers(0, 1, m_Screen_DB->VertexBuf->GetAddress(), &m_Screen_DB->Stride, &m_Screen_DB->Offset);
	g_Context->IASetIndexBuffer(m_Screen_DB->IndexBuf->Get(), DXGI_FORMAT_R32_UINT, 0);

	g_Context->DrawIndexed(m_Screen_DB->IndexCount, 0, 0);
}

void BloomPass::GaussianBlur(ID3D11RenderTargetView* rtv[2], ID3D11ShaderResourceView* srv[2], const SamplingData& downScreen)
{
	CB_BloomBlurOrder bloomBlurOptionBuf;
	bloomBlurOptionBuf.gBlurTexelSize = { downScreen.TexelWidth, downScreen.TexelHeight };
	
	// 해당 Origin Screen 설정..
	SetBlurViewPort(downScreen);

	/// Vertical Pass
	g_Context->OMSetRenderTargets(1, &rtv[0], nullptr);
	g_Context->RSSetViewports(1, m_DownSample_VP);

	// Vertex Shader Update..
	m_Screen_VS->Update();

	// Pixel Shader Update..
	bloomBlurOptionBuf.gBlurOrder = { BLOOM_VERTICAL_BLUR , 0 };
	m_BloomBlur_PS->ConstantBufferUpdate(&bloomBlurOptionBuf);
	m_BloomBlur_PS->SetShaderResourceView<gOriginMap>(srv[0]);
	m_BloomBlur_PS->Update();

	g_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	g_Context->IASetVertexBuffers(0, 1, m_Screen_DB->VertexBuf->GetAddress(), &m_Screen_DB->Stride, &m_Screen_DB->Offset);
	g_Context->IASetIndexBuffer(m_Screen_DB->IndexBuf->Get(), DXGI_FORMAT_R32_UINT, 0);

	g_Context->DrawIndexed(m_Screen_DB->IndexCount, 0, 0);

	/// Horizontal Pass
	g_Context->OMSetRenderTargets(1, &rtv[1], nullptr);

	// Vertex Shader Update..
	m_Screen_VS->Update();

	// Pixel Shader Update..
	bloomBlurOptionBuf.gBlurOrder = { 0 , BLOOM_HORIZONTAL_BLUR };
	m_BloomBlur_PS->ConstantBufferUpdate(&bloomBlurOptionBuf);
	m_BloomBlur_PS->SetShaderResourceView<gOriginMap>(srv[1]);
	m_BloomBlur_PS->Update();

	g_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	g_Context->IASetVertexBuffers(0, 1, m_Screen_DB->VertexBuf->GetAddress(), &m_Screen_DB->Stride, &m_Screen_DB->Offset);
	g_Context->IASetIndexBuffer(m_Screen_DB->IndexBuf->Get(), DXGI_FORMAT_R32_UINT, 0);

	g_Context->DrawIndexed(m_Screen_DB->IndexCount, 0, 0);
}

void BloomPass::SetConstantBuffer()
{
	CB_BloomBright bloomdownsamplingBuf;
	bloomdownsamplingBuf.gThreshold = 1.0f;
	m_BloomBright_PS->ConstantBufferUpdate(&bloomdownsamplingBuf);
}

void BloomPass::SetSamplingViewPort(const SamplingData& downScreen)
{
	m_DownSample_VP->TopLeftX = downScreen.HalfOffsetWidth;
	m_DownSample_VP->TopLeftY = downScreen.HalfOffsetHeight;
	m_DownSample_VP->Width = downScreen.SamplingWidth;
	m_DownSample_VP->Height = downScreen.SamplingHeight;
}

void BloomPass::SetBlurViewPort(const SamplingData& downScreen)
{
	m_DownSample_VP->TopLeftX = 0.0f;
	m_DownSample_VP->TopLeftY = 0.0f;
	m_DownSample_VP->Width = downScreen.OriginWidth;
	m_DownSample_VP->Height = downScreen.OriginHeight;
}
