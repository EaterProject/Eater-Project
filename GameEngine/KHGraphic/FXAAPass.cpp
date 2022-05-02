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
#include "EngineData.h"
#include "FXAAPass.h"

#include "MathDefine.h"
#include "ResourceManagerBase.h"
#include "ShaderManagerBase.h"
#include "RenderTargetDefine.h"
#include "DrawBufferDefine.h"
#include "ShaderResourceViewDefine.h"
#include "ConstantBufferDefine.h"
#include "DepthStencilViewDefine.h"
#include "ViewPortDefine.h"

FXAAPass::FXAAPass()
{

}

FXAAPass::~FXAAPass()
{

}

void FXAAPass::Create(int width, int height)
{

}

void FXAAPass::Start(int width, int height)
{
	m_Screen_VS = g_Shader->GetShader("Screen_VS");
	m_FXAA_PS = g_Shader->GetShader("FXAA_PS");

	m_Screen_DB = g_Resource->GetDrawBuffer<DB_Quad>();

	// Graphic View..
	m_Origin_RT = g_Resource->GetRenderTexture<RT_OutPut3>();
	m_OutPut_RTV = g_Resource->GetMainRenderTarget()->GetRTV()->Get();

	// Shader Resource 설정..
	m_FXAA_PS->SetShaderResourceView<gOriginMap>(m_Origin_RT->GetSRV()->Get());

	CB_FxaaFrame fxaaBuf;
	fxaaBuf.gFrame = Vector4(1.0f / width, 1.0f / height, 0.0f, 0.0f);
	m_FXAA_PS->ConstantBufferUpdate<CB_FxaaFrame>(&fxaaBuf);
}

void FXAAPass::OnResize(int width, int height)
{
	// Graphic View..
	m_OutPut_RTV = g_Resource->GetMainRenderTarget()->GetRTV()->Get();

	// Shader Resource 재설정..
	m_FXAA_PS->SetShaderResourceView<gOriginMap>(m_Origin_RT->GetSRV()->Get());

	CB_FxaaFrame fxaaBuf;
	fxaaBuf.gFrame = Vector4(1.0f / width, 1.0f / height, 0.0f, 0.0f);
	m_FXAA_PS->ConstantBufferUpdate<CB_FxaaFrame>(&fxaaBuf);
}

void FXAAPass::Release()
{

}

void FXAAPass::ApplyOption()
{

}

void FXAAPass::RenderUpdate()
{
	g_Context->RSSetState(0);
	g_Context->OMSetBlendState(0, 0, 0xffffffff);
	g_Context->OMSetRenderTargets(1, &m_OutPut_RTV, nullptr);

	g_Context->ClearRenderTargetView(m_OutPut_RTV, reinterpret_cast<const float*>(&DXColors::NonBlack));

	// Vertex Shader Update..
	m_Screen_VS->Update();

	// Pixel Shader Update..
	m_FXAA_PS->Update();

	g_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	g_Context->IASetVertexBuffers(0, 1, m_Screen_DB->VertexBuf->GetAddress(), &m_Screen_DB->Stride, &m_Screen_DB->Offset);
	g_Context->IASetIndexBuffer(m_Screen_DB->IndexBuf->Get(), DXGI_FORMAT_R32_UINT, 0);

	g_Context->DrawIndexed(m_Screen_DB->IndexCount, 0, 0);
}