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
#include "Combine_Pass.h"

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

Combine_Pass::Combine_Pass()
{

}

Combine_Pass::~Combine_Pass()
{

}

void Combine_Pass::Create(int width, int height)
{

}

void Combine_Pass::Start(int width, int height)
{
	// Shader..
	m_Screen_VS = g_Shader->GetShader("Screen_VS");
	m_Combine_PS = g_Shader->GetShader("Combine_PS_Option4");

	m_Screen_DB = g_Resource->GetDrawBuffer<DB_Quad>();

	m_Screen_VP = g_Resource->GetViewPort<VP_FullScreen>()->Get();

	m_Origin_RT = g_Resource->GetRenderTexture<RT_OutPut2>();

	m_Bloom_RT = g_Resource->GetRenderTexture<RT_Bloom_Brightx4_2>();
	m_OutLine_RT = g_Resource->GetRenderTexture<RT_OutLine>();

	// Combine Shader List Up..
	SetShaderList();

	// Set Combine Shader Resoruce View..
	SetShaderResourceView();

	// Set Combine Shader Constant Buffer..
	SetConstantBuffer();
}

void Combine_Pass::OnResize(int width, int height)
{
	// 현재 RenderTarget 재설정..
	m_OutPut_RTV = m_OutPut_RT->GetRTV()->Get();

	// Set Combine Shader Resoruce View..
	SetShaderResourceView();
}

void Combine_Pass::Release()
{

}

void Combine_Pass::ApplyOption()
{
	UINT combineOption = g_RenderOption->PostProcessOption & (RENDER_BLOOM | RENDER_HDR);

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

	if (g_RenderOption->PostProcessOption & RENDER_FXAA)
	{
		m_OutPut_RT = g_Resource->GetRenderTexture<RT_OutPut1>();
	}
	else
	{
		m_OutPut_RT = g_Resource->GetMainRenderTarget();
	}

	// 현재 RenderTarget 설정..
	m_OutPut_RTV = m_OutPut_RT->GetRTV()->Get();

	// Set Combine Shader Constant Buffer..
	SetConstantBuffer();
}

void Combine_Pass::RenderUpdate()
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

void Combine_Pass::SetShaderList()
{
	PushShader("Combine_PS_Option0");
	PushShader("Combine_PS_Option1");
	PushShader("Combine_PS_Option2");
	PushShader("Combine_PS_Option3");
	PushShader("Combine_PS_Option4");
	PushShader("Combine_PS_Option5");
	PushShader("Combine_PS_Option6");
	PushShader("Combine_PS_Option7");
}

void Combine_Pass::SetShaderResourceView()
{
	ID3D11ShaderResourceView* originSRV = m_Origin_RT->GetSRV()->Get();
	ID3D11ShaderResourceView* bloomSRV = m_Bloom_RT->GetSRV()->Get();
	ID3D11ShaderResourceView* outlineSRV = m_OutLine_RT->GetSRV()->Get();

	for (ShaderBase* shader : m_OptionShaderList)
	{
		shader->SetShaderResourceView<gOriginMap>(originSRV);
		shader->SetShaderResourceView<gBloomMap>(bloomSRV);
		shader->SetShaderResourceView<gOutLineMap>(outlineSRV);
	}
}

void Combine_Pass::SetConstantBuffer()
{
	CB_DrawFinal drawFinalBuf;
	drawFinalBuf.gBloomFactor = g_RenderOption->BLOOM_Factor;

	for (ShaderBase* shader : m_OptionShaderList)
	{
		shader->ConstantBufferUpdate(&drawFinalBuf);
	}
}
