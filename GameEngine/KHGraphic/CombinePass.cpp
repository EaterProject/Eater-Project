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
#include "CombinePass.h"

#include "MathDefine.h"
#include "FactoryManagerBase.h"
#include "ResourceManagerBase.h"
#include "ShaderManagerBase.h"
#include "DepthStencilViewDefine.h"
#include "RenderTargetDefine.h"
#include "DrawBufferDefine.h"
#include "ShaderResourceViewDefine.h"
#include "ConstantBufferDefine.h"
#include "ViewPortDefine.h"

CombinePass::CombinePass()
{

}

CombinePass::~CombinePass()
{

}

void CombinePass::Create(int width, int height)
{

}

void CombinePass::Start(int width, int height)
{
	// Shader..
	m_Screen_VS = g_Shader->GetShader("Screen_VS");
	m_Combine_PS = g_Shader->GetShader("Combine_PS_Option4");

	m_Screen_DB = g_Resource->GetDrawBuffer<DB_Quad>();

	m_Screen_VP = g_Resource->GetViewPort<VP_FullScreen>()->Get();

	m_Bloom_RT = g_Resource->GetRenderTexture<RT_Bloom_Brightx4_2>();
	m_OutLine_RT = g_Resource->GetRenderTexture<RT_OutLine>();
}

void CombinePass::OnResize(int width, int height)
{
	// 현재 RenderTarget 재설정..
	m_OutPut_RTV = m_OutPut_RT->GetRTV()->Get();

	// ShaderResource 재설정..
	ID3D11ShaderResourceView* originSRV = m_Origin_RT->GetSRV()->Get();
	ID3D11ShaderResourceView* bloomSRV = m_Bloom_RT->GetSRV()->Get();
	ID3D11ShaderResourceView* outlineSRV = m_OutLine_RT->GetSRV()->Get();

	m_Combine_PS->SetShaderResourceView<gOriginMap>(originSRV);
	m_Combine_PS->SetShaderResourceView<gBloomMap>(bloomSRV);
	m_Combine_PS->SetShaderResourceView<gOutLineMap>(outlineSRV);
}

void CombinePass::Release()
{

}

void CombinePass::SetOption(RenderOption* renderOption)
{
	UINT combineOption = renderOption->PostProcessOption & (RENDER_BLOOM | RENDER_HDR);

	switch (combineOption)
	{
	case RENDER_BLOOM:
		m_Combine_PS = g_Shader->GetShader("Combine_PS_Option1");
		break;
	case RENDER_HDR:
		m_Combine_PS = g_Shader->GetShader("Combine_PS_Option2");
		break;
	case RENDER_BLOOM | RENDER_HDR:
		m_Combine_PS = g_Shader->GetShader("Combine_PS_Option7");
		break;
	default:
		m_Combine_PS = g_Shader->GetShader("Combine_PS_Option0");
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
	ID3D11ShaderResourceView* outlineSRV = m_OutLine_RT->GetSRV()->Get();

	m_Combine_PS->SetShaderResourceView<gOriginMap>(originSRV);
	m_Combine_PS->SetShaderResourceView<gBloomMap>(bloomSRV);
	m_Combine_PS->SetShaderResourceView<gOutLineMap>(outlineSRV);

	SetConstantBuffer();
}

void CombinePass::RenderUpdate()
{
	GPU_MARKER_DEBUG_NAME("Tone Mapping + Bloom Blend");

	g_Context->OMSetRenderTargets(1, &m_OutPut_RTV, nullptr);
	g_Context->RSSetViewports(1, m_Screen_VP);
	
	g_Context->ClearRenderTargetView(m_OutPut_RTV, reinterpret_cast<const float*>(&DXColors::NonBlack));

	m_Screen_VS->Update();

	m_Combine_PS->Update();

	g_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	g_Context->IASetVertexBuffers(0, 1, m_Screen_DB->VertexBuf->GetAddress(), &m_Screen_DB->Stride, &m_Screen_DB->Offset);
	g_Context->IASetIndexBuffer(m_Screen_DB->IndexBuf->Get(), DXGI_FORMAT_R32_UINT, 0);

	g_Context->DrawIndexed(m_Screen_DB->IndexCount, 0, 0);
}

void CombinePass::SetConstantBuffer()
{
	CB_DrawFinal drawFinalBuf;
	drawFinalBuf.gBloomFactor = 0.25f;
	m_Combine_PS->ConstantBufferUpdate(&drawFinalBuf);
}
