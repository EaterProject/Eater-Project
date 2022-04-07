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
#include "VertexDefine.h"
#include "EngineData.h"
#include "FogPass.h"

#include "MathDefine.h"
#include "FactoryManagerBase.h"
#include "ResourceManagerBase.h"
#include "ShaderManagerBase.h"
#include "ConstantBufferDefine.h"
#include "ShaderResourceViewDefine.h"
#include "RenderTargetDefine.h"
#include "ViewPortDefine.h"
#include "DrawBufferDefine.h"

FogPass::FogPass()
{

}

FogPass::~FogPass()
{

}

void FogPass::Create(int width, int height)
{
	// Noise Texture 생성..
	g_Factory->CreateImage<gNoiseVolume>("Noise/NoiseVolume.dds");

}

void FogPass::Start(int width, int height)
{
	// Shader..
	m_Screen_VS = g_Shader->GetShader("Screen_VS");
	m_Fog_PS = g_Shader->GetShader("Fog_PS");

	// Buffer 설정..
	m_Screen_DB = g_Resource->GetDrawBuffer<DB_Quad>();

	m_OutPut_RT = g_Resource->GetRenderTexture<RT_OutPut1>();
	m_OutPut_RTV = m_OutPut_RT->GetRTV()->Get();

	// ViewPort 설정..
	m_Screen_VP = g_Resource->GetViewPort<VP_FullScreen>()->Get();

	// ShaderResource 설정..
	ShaderResourceView* originSRV = g_Resource->GetShaderResourceView<RT_OutPut2>();
	ShaderResourceView* positionSRV = g_Resource->GetShaderResourceView<RT_Deffered_Position>();
	ShaderResourceView* noiseSRV = g_Resource->GetShaderResourceView<gNoiseVolume>();

	m_Fog_PS->SetShaderResourceView<gOriginMap>(originSRV->Get());
	m_Fog_PS->SetShaderResourceView<gPositionRT>(positionSRV->Get());
	m_Fog_PS->SetShaderResourceView<gNoiseVolume>(noiseSRV->Get());
}

void FogPass::OnResize(int width, int height)
{
	// 현재 RenderTarget 재설정..
	m_OutPut_RTV = m_OutPut_RT->GetRTV()->Get();

	// ShaderResource 재설정..
	ShaderResourceView* originSRV = g_Resource->GetShaderResourceView<RT_OutPut2>();
	ShaderResourceView* positionSRV = g_Resource->GetShaderResourceView<RT_Deffered_Position>();

	m_Fog_PS->SetShaderResourceView<gOriginMap>(originSRV->Get());
	m_Fog_PS->SetShaderResourceView<gPositionRT>(positionSRV->Get());
}

void FogPass::Release()
{

}

void FogPass::SetOption(RenderOption* renderOption)
{

}

void FogPass::RenderUpdate()
{
	const Vector3 FogColor = { 0.9f, 0.745f, 0.35f };
	const Vector3 FogHighlightColor = { 0.835f, 0.7f, 0.33f };
	const float FogStartDepth = 50.0f;
	static float Time = 0;
	Time += g_GlobalData->Time * 0.05f;

	g_Context->OMSetRenderTargets(1, &m_OutPut_RTV, nullptr);
	g_Context->RSSetViewports(1, m_Screen_VP);

	// Vertex Shader Update..
	m_Screen_VS->Update();

	CB_Fog fogBuf;
	fogBuf.gFogColor = FogColor;
	fogBuf.gFogStartPos = FogStartDepth;
	fogBuf.gEyePosW = g_GlobalData->MainCamera_Data->CamPos;
	fogBuf.gTime = Time;

	m_Fog_PS->ConstantBufferUpdate(&fogBuf);

	// Pixel Shader Update..
	m_Fog_PS->Update();

	g_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	g_Context->IASetVertexBuffers(0, 1, m_Screen_DB->VertexBuf->GetAddress(), &m_Screen_DB->Stride, &m_Screen_DB->Offset);
	g_Context->IASetIndexBuffer(m_Screen_DB->IndexBuf->Get(), DXGI_FORMAT_R32_UINT, 0);

	g_Context->DrawIndexed(m_Screen_DB->IndexCount, 0, 0);
}