#include "DirectDefine.h"
#include "RenderPassBase.h"
#include "ShaderBase.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "GraphicState.h"
#include "GraphicView.h"
#include "GraphicResource.h"
#include "DrawBuffer.h"
#include "RenderTarget.h"
#include "ToneMapPass.h"

#include "MathDefine.h"
#include "FactoryManagerBase.h"
#include "ResourceManagerBase.h"
#include "ShaderManagerBase.h"
#include "RenderTargetDefine.h"
#include "DrawBufferDefine.h"
#include "ShaderResourceViewDefine.h"
#include "ConstantBufferDefine.h"
#include "ViewPortDefine.h"

ToneMapPass::ToneMapPass()
{

}

ToneMapPass::~ToneMapPass()
{

}

void ToneMapPass::Create(int width, int height)
{

}

void ToneMapPass::Start(int width, int height)
{
	// Shader..
	m_Screen_VS = g_Shader->GetShader("Screen_VS");
	m_ToneMapping_PS = g_Shader->GetShader("BloomToneMapping_PS");

	m_Screen_DB = g_Resource->GetDrawBuffer<DB_Quad>();

	m_Screen_VP = g_Resource->GetViewPort<VP_FullScreen>()->Get();

	m_Bloom_RT = g_Resource->GetRenderTexture<RT_Bloom_Brightx4_2>();
}

void ToneMapPass::OnResize(int width, int height)
{
	// 현재 RenderTarget 재설정..
	m_OutPut_RTV = m_OutPut_RT->GetRTV()->Get();

	// ShaderResource 재설정..
	ID3D11ShaderResourceView* originSRV = m_Origin_RT->GetSRV()->Get();
	ID3D11ShaderResourceView* bloomSRV = m_Bloom_RT->GetSRV()->Get();

	m_ToneMapping_PS->SetShaderResourceView<gOriginMap>(originSRV);
	m_ToneMapping_PS->SetShaderResourceView<gBloomMap>(bloomSRV);
}

void ToneMapPass::Release()
{

}

void ToneMapPass::SetOption(RenderOption* renderOption)
{
	UINT tonemapOption = renderOption->PostProcessOption & (RENDER_BLOOM | RENDER_HDR);

	switch (tonemapOption)
	{
	case RENDER_BLOOM:
		m_ToneMapping_PS = g_Shader->GetShader("Bloom_PS");
		break;
	case RENDER_HDR:
		m_ToneMapping_PS = g_Shader->GetShader("ToneMapping_PS");
		break;
	case RENDER_BLOOM | RENDER_HDR:
		m_ToneMapping_PS = g_Shader->GetShader("BloomToneMapping_PS");
		break;
	default:
		m_ToneMapping_PS = g_Shader->GetShader("GammaCorrection_PS");
		break;
	}

	if (renderOption->PostProcessOption & RENDER_FOG)
	{
		m_Origin_RT = g_Resource->GetRenderTexture<RT_OutPut1>();
	}
	else
	{
		m_Origin_RT = g_Resource->GetRenderTexture<RT_OutPut2>();
	}

	if (renderOption->PostProcessOption & RENDER_FXAA)
	{
		m_OutPut_RT = g_Resource->GetRenderTexture<RT_OutPut3>();
	}
	else
	{
		m_OutPut_RT = g_Resource->GetMainRenderTarget();
	}

	// 현재 RenderTarget 설정..
	m_OutPut_RTV = m_OutPut_RT->GetRTV()->Get();

	// ShaderResource 설정..
	ID3D11ShaderResourceView* originSRV = m_Origin_RT->GetSRV()->Get();
	ID3D11ShaderResourceView* bloomSRV = m_Bloom_RT->GetSRV()->Get();

	m_ToneMapping_PS->SetShaderResourceView<gOriginMap>(originSRV);
	m_ToneMapping_PS->SetShaderResourceView<gBloomMap>(bloomSRV);

	SetConstantBuffer();
}

void ToneMapPass::RenderUpdate()
{
	GPU_MARKER_DEBUG_NAME("Tone Mapping + Bloom Blend");

	g_Context->OMSetRenderTargets(1, &m_OutPut_RTV, nullptr);
	g_Context->RSSetViewports(1, m_Screen_VP);
	
	g_Context->ClearRenderTargetView(m_OutPut_RTV, reinterpret_cast<const float*>(&DXColors::NonBlack));

	m_Screen_VS->Update();

	m_ToneMapping_PS->Update();

	g_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	g_Context->IASetVertexBuffers(0, 1, m_Screen_DB->VertexBuf->GetAddress(), &m_Screen_DB->Stride, &m_Screen_DB->Offset);
	g_Context->IASetIndexBuffer(m_Screen_DB->IndexBuf->Get(), DXGI_FORMAT_R32_UINT, 0);

	g_Context->DrawIndexed(m_Screen_DB->IndexCount, 0, 0);
}

void ToneMapPass::SetConstantBuffer()
{
	CB_BloomFinal bloomFinalBuf;
	bloomFinalBuf.gCoefficient = 0.25f;
	m_ToneMapping_PS->ConstantBufferUpdate(&bloomFinalBuf);
}
