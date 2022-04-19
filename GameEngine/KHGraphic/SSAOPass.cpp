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
#include "VertexDefine.h"
#include "EngineData.h"
#include "ConstantBufferDefine.h"
#include "SSAOPass.h"

#include "MathDefine.h"
#include "FactoryManagerBase.h"
#include "ResourceManagerBase.h"
#include "ShaderManagerBase.h"
#include "ShaderResourceViewDefine.h"
#include "DepthStencilStateDefine.h"
#include "DepthStencilViewDefine.h"
#include "RenderTargetDefine.h"
#include "ViewPortDefine.h"
#include "DrawBufferDefine.h"
#include <random>
#include <DirectXPackedVector.h>

using namespace DirectX::PackedVector;

#define SSAO_VERTICAL_BLUR 1
#define SSAO_HORIZONTAL_BLUR 1

SSAOPass::SSAOPass()
{

}

SSAOPass::~SSAOPass()
{

}

void SSAOPass::Create(int width, int height)
{
	// Texture 2D
	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = width / 2;
	texDesc.Height = height / 2;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R16_FLOAT;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	// RenderTarget 생성..
	g_Factory->CreateRenderTexture<RT_SSAO_Main>(&texDesc, nullptr, nullptr, nullptr, nullptr);
	g_Factory->CreateRenderTexture<RT_SSAO_Blur>(&texDesc, nullptr, nullptr, nullptr, nullptr);
}

void SSAOPass::Start(int width, int height)
{
	// Shader 설정..
	m_Ssao_VS = g_Shader->GetShader("SSAO_VS");
	m_Ssao_PS = g_Shader->GetShader("SSAO_PS");
	m_Blur_VS = g_Shader->GetShader("Screen_VS");
	m_Blur_PS = g_Shader->GetShader("SSAOBlur_PS");

	// Buffer 설정..
	m_Ssao_DB = g_Resource->GetDrawBuffer<DB_SSAO>();

	// ViewPort 설정..
	m_HalfScreen_VP = g_Resource->GetViewPort<VP_HalfScreen>()->Get();

	// RenderTarget 설정..
	m_Ssao_RT = g_Resource->GetRenderTexture<RT_SSAO_Main>();
	m_Ssao_RT->SetRatio(0.5f, 0.5f);
	m_SsaoBlur_RT = g_Resource->GetRenderTexture<RT_SSAO_Blur>();
	m_SsaoBlur_RT->SetRatio(0.5f, 0.5f);

	m_Ssao_RTV = m_Ssao_RT->GetRTV()->Get();
	m_Ssao_SRV = m_Ssao_RT->GetSRV()->Get();
	m_SsaoBlur_RTV = m_SsaoBlur_RT->GetRTV()->Get();
	m_SsaoBlur_SRV = m_SsaoBlur_RT->GetSRV()->Get();

	// OffsetVector 설정..
	SetOffsetVectors();

	// RandomVectorTexture 설정..
	SetRandomVectorTexture();

	// Frustum 설정..
	SetFrustumFarCorners(width, height);

	// ShaderResource 설정..
	ShaderResourceView* randomVecMap = g_Resource->GetShaderResourceView<gRandomVecMap>();
	ShaderResourceView* normalDepthRT = g_Resource->GetShaderResourceView<RT_Deffered_NormalDepth>();

	m_Ssao_PS->SetShaderResourceView<gRandomVecMap>(randomVecMap->Get());
	m_Ssao_PS->SetShaderResourceView<gNormalDepthMap>(normalDepthRT->Get());
	m_Blur_PS->SetShaderResourceView<gNormalDepthMap>(normalDepthRT->Get());

	// Constant Buffer Update..
	CB_BlurTexel blurTexelBuf;
	blurTexelBuf.gTexelSize = DirectX::SimpleMath::Vector2(1.0f / m_HalfScreen_VP->Width, 1.0f / m_HalfScreen_VP->Height);

	m_Blur_PS->ConstantBufferUpdate(&blurTexelBuf);
}

void SSAOPass::OnResize(int width, int height)
{
	// Frustum 재설정..
	SetFrustumFarCorners(width, height);

	// RenderTarget Resource 재설정..
	m_Ssao_RTV = m_Ssao_RT->GetRTV()->Get();
	m_Ssao_SRV = m_Ssao_RT->GetSRV()->Get();
	m_SsaoBlur_RTV = m_SsaoBlur_RT->GetRTV()->Get();
	m_SsaoBlur_SRV = m_SsaoBlur_RT->GetSRV()->Get();

	// ShaderResource 설정..
	ShaderResourceView* randomVecMap = g_Resource->GetShaderResourceView<gRandomVecMap>();
	ShaderResourceView* normalDepthRT = g_Resource->GetShaderResourceView<RT_Deffered_NormalDepth>();

	m_Ssao_PS->SetShaderResourceView<gRandomVecMap>(randomVecMap->Get());
	m_Ssao_PS->SetShaderResourceView<gNormalDepthMap>(normalDepthRT->Get());
	m_Blur_PS->SetShaderResourceView<gNormalDepthMap>(normalDepthRT->Get());

	// Constant Buffer Update..
	CB_BlurTexel blurTexelBuf;
	blurTexelBuf.gTexelSize = DirectX::SimpleMath::Vector2(1.0f / m_HalfScreen_VP->Width, 1.0f / m_HalfScreen_VP->Height);

	m_Blur_PS->ConstantBufferUpdate(&blurTexelBuf);
}

void SSAOPass::Release()
{

}

void SSAOPass::SetOption(RenderOption* renderOption)
{
	// SSAO RenderTargetView 초기화..
	g_Context->ClearRenderTargetView(m_Ssao_RTV, reinterpret_cast<const float*>(&DXColors::Black));
	g_Context->ClearRenderTargetView(m_SsaoBlur_RTV, reinterpret_cast<const float*>(&DXColors::Black));
}

void SSAOPass::RenderUpdate()
{
	GPU_MARKER_DEBUG_NAME("SSAO Render");
	g_Context->OMSetBlendState(0, 0, 0xffffffff);
	g_Context->OMSetRenderTargets(1, &m_Ssao_RTV, 0);
	g_Context->ClearRenderTargetView(m_Ssao_RTV, reinterpret_cast<const float*>(&DXColors::Black));
	g_Context->RSSetViewports(1, m_HalfScreen_VP);

	CameraData* cam = g_GlobalData->MainCamera_Data;
	Matrix& proj = cam->CamProj;

	CB_SsaoObject objectBuf;
	objectBuf.gViewToTexSpace = proj * g_GlobalData->TexSpace;

	m_Ssao_PS->ConstantBufferUpdate(&objectBuf);

	// Vertex Shader Update..
	m_Ssao_VS->Update();

	// Pixel Shader Update..
	m_Ssao_PS->Update();

	g_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	g_Context->IASetVertexBuffers(0, 1, m_Ssao_DB->VertexBuf->GetAddress(), &m_Ssao_DB->Stride, &m_Ssao_DB->Offset);
	g_Context->IASetIndexBuffer(m_Ssao_DB->IndexBuf->Get(), DXGI_FORMAT_R16_UINT, 0);

	g_Context->DrawIndexed(m_Ssao_DB->IndexCount, 0, 0);
}

void SSAOPass::BlurRender(int blurCount)
{
	GPU_MARKER_DEBUG_NAME("SSAO Blur");
	
	for (int i = 0; i < blurCount; i++)
	{
		BlurRender();
	}
}

void SSAOPass::BlurRender()
{
	CB_BlurOrder blurOrderBuf;

	// Vertex Shader Update..
	m_Blur_VS->Update();

	/// Horizontal Blur
	blurOrderBuf.gBlurOrder = { SSAO_HORIZONTAL_BLUR, 0.0f };
	m_Blur_PS->ConstantBufferUpdate(&blurOrderBuf);

	g_Context->OMSetRenderTargets(1, &m_SsaoBlur_RTV, 0);
	g_Context->ClearRenderTargetView(m_SsaoBlur_RTV, reinterpret_cast<const float*>(&DXColors::Black));

	m_Blur_PS->SetShaderResourceView<gInputMap>(m_Ssao_SRV);
	
	// Pixel Shader Update..
	m_Blur_PS->Update();

	g_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	g_Context->IASetVertexBuffers(0, 1, m_Ssao_DB->VertexBuf->GetAddress(), &m_Ssao_DB->Stride, &m_Ssao_DB->Offset);
	g_Context->IASetIndexBuffer(m_Ssao_DB->IndexBuf->Get(), DXGI_FORMAT_R16_UINT, 0);

	g_Context->DrawIndexed(m_Ssao_DB->IndexCount, 0, 0);

	/// Vertical Blur
	blurOrderBuf.gBlurOrder = { 0.0f, SSAO_VERTICAL_BLUR };
	m_Blur_PS->ConstantBufferUpdate(&blurOrderBuf);

	g_Context->OMSetRenderTargets(1, &m_Ssao_RTV, 0);
	g_Context->ClearRenderTargetView(m_Ssao_RTV, reinterpret_cast<const float*>(&DXColors::Black));

	m_Blur_PS->SetShaderResourceView<gInputMap>(m_SsaoBlur_SRV);

	// Pixel Shader Update..
	m_Blur_PS->Update();

	g_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	g_Context->IASetVertexBuffers(0, 1, m_Ssao_DB->VertexBuf->GetAddress(), &m_Ssao_DB->Stride, &m_Ssao_DB->Offset);
	g_Context->IASetIndexBuffer(m_Ssao_DB->IndexBuf->Get(), DXGI_FORMAT_R16_UINT, 0);

	g_Context->DrawIndexed(m_Ssao_DB->IndexCount, 0, 0);
}

void SSAOPass::SetOffsetVectors()
{
	CB_SsaoOption option;
	option.gOcclusionRadius = 0.25f;
	option.gOcclusionFadeStart = 0.01f;
	option.gOcclusionFadeEnd = 2.0f;
	option.gSurfaceEpsilon = 0.05f;

	// 8 cube corners
	XMFLOAT4 m_Offsets[14];
	m_Offsets[0] = XMFLOAT4(+1.0f, +1.0f, +1.0f, 0.0f);
	m_Offsets[1] = XMFLOAT4(-1.0f, -1.0f, -1.0f, 0.0f);

	m_Offsets[2] = XMFLOAT4(-1.0f, +1.0f, +1.0f, 0.0f);
	m_Offsets[3] = XMFLOAT4(+1.0f, -1.0f, -1.0f, 0.0f);

	m_Offsets[4] = XMFLOAT4(+1.0f, +1.0f, -1.0f, 0.0f);
	m_Offsets[5] = XMFLOAT4(-1.0f, -1.0f, +1.0f, 0.0f);

	m_Offsets[6] = XMFLOAT4(-1.0f, +1.0f, -1.0f, 0.0f);
	m_Offsets[7] = XMFLOAT4(+1.0f, -1.0f, +1.0f, 0.0f);

	// 6 centers of cube faces
	m_Offsets[8] = XMFLOAT4(-1.0f, 0.0f, 0.0f, 0.0f);
	m_Offsets[9] = XMFLOAT4(+1.0f, 0.0f, 0.0f, 0.0f);

	m_Offsets[10] = XMFLOAT4(0.0f, -1.0f, 0.0f, 0.0f);
	m_Offsets[11] = XMFLOAT4(0.0f, +1.0f, 0.0f, 0.0f);

	m_Offsets[12] = XMFLOAT4(0.0f, 0.0f, -1.0f, 0.0f);
	m_Offsets[13] = XMFLOAT4(0.0f, 0.0f, +1.0f, 0.0f);

	std::mt19937 randEngine;
	randEngine.seed(std::random_device()());
	std::uniform_real_distribution<float> randF(0.25f, 1.0f);
	for (int i = 0; i < 14; ++i)
	{
		// Create random lengths in [0.25, 1.0].
		float s = randF(randEngine);

		XMVECTOR v = s * XMVector4Normalize(XMLoadFloat4(&m_Offsets[i]));

		// OffsetVector Constant Buffer Data 삽입..
		XMStoreFloat4(&option.gOffsetVectors[i], v);
	}

	// SSAO Option Constant Buffer Update..
	m_Ssao_PS->ConstantBufferUpdate(&option);
}

void SSAOPass::SetRandomVectorTexture()
{
	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = 256;
	texDesc.Height = 256;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_IMMUTABLE;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	std::mt19937 randEngine;
	randEngine.seed(std::random_device()());
	std::uniform_real_distribution<float> randF(0.0f, 1.0f);

	std::vector<XMCOLOR> randomVectors(256 * 256);
	for (int i = 0; i < 256 * 256; ++i)
	{
		randomVectors[i] = XMCOLOR(randF(randEngine), randF(randEngine), randF(randEngine), 0.0f);
	}

	D3D11_SUBRESOURCE_DATA initData = { 0 };
	initData.pSysMem = randomVectors.data();
	initData.SysMemPitch = 256 * sizeof(XMCOLOR);

	// RandomVectorMap 생성..
	g_Factory->CreateShaderResourceView<gRandomVecMap>(&texDesc, &initData, nullptr);
}

void SSAOPass::SetFrustumFarCorners(int width, int height)
{
	CB_SsaoFrustum frustum;

	float aspect = (float)width / (float)height;

	float fovY = 0.25f * 3.1415926535f;
	float farZ = 4000.0f;
	float halfHeight = farZ * tanf(0.5f * fovY);
	float halfWidth = aspect * halfHeight;

	frustum.gFrustumCorners[0] = XMFLOAT4(-halfWidth, -halfHeight, farZ, 0.0f);
	frustum.gFrustumCorners[1] = XMFLOAT4(-halfWidth, +halfHeight, farZ, 0.0f);
	frustum.gFrustumCorners[2] = XMFLOAT4(+halfWidth, +halfHeight, farZ, 0.0f);
	frustum.gFrustumCorners[3] = XMFLOAT4(+halfWidth, -halfHeight, farZ, 0.0f);

	// FrustumCorner Constant Buffer Data 삽입..
	m_Ssao_VS->ConstantBufferUpdate(&frustum);
}
