#include "DirectDefine.h"
#include "RenderPassBase.h"
#include "ShaderBase.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "GraphicState.h"
#include "GraphicView.h"
#include "GraphicResource.h"
#include "DrawBuffer.h"
#include "DepthStencil.h"
#include "RenderTarget.h"
#include "EngineData.h"
#include "RenderData.h"
#include "PickingPass.h"

#include "MathDefine.h"
#include "FactoryManagerBase.h"
#include "ResourceManagerBase.h"
#include "ShaderManagerBase.h"
#include "ConstantBufferDefine.h"
#include "ShaderResourceViewDefine.h"
#include "ViewPortDefine.h"
#include "RenderTargetDefine.h"

PickingPass::PickingPass()
{

}

PickingPass::~PickingPass()
{

}

void PickingPass::Create(int width, int height)
{
	// Picking 전용 Texture 2D
	// 해당 Texture는 데이터 입력은 Shader에서 하되, 해당 값을 CPU에서 읽을수 있어야 하므로
	// Usage Option을 D3D11_USAGE_STAGING으로 설정..
	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(texDesc));
	texDesc.Width = width;
	texDesc.Height = height;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_STAGING;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	/// RenderTargetView 2D
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	ZeroMemory(&rtvDesc, sizeof(rtvDesc));
	rtvDesc.Format = texDesc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;

	/// ShaderResourceView 2D
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;

	// RenderTarget 생성..
	g_Factory->CreateRenderTexture<RT_ID>(&texDesc, nullptr, &rtvDesc, &srvDesc, nullptr);
}

void PickingPass::Start(int width, int height)
{

}

void PickingPass::OnResize(int width, int height)
{

}

void PickingPass::Release()
{

}

Vector4 PickingPass::HashIDColor(int hash)
{
	int a = (hash >> 24) & 0xff;
	int b = (hash >> 16) & 0xff;
	int g = (hash >> 8) & 0xff;
	int r = hash & 0xff;

	return Vector4(r, g, b, a);
}