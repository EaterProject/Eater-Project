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
#include "LightPass.h"

#include "MathDefine.h"
#include "ResourceFactoryBase.h"
#include "ResourceManagerBase.h"
#include "ShaderManagerBase.h"
#include "ConstantBufferDefine.h"
#include "ShaderResourceViewDefine.h"
#include "DepthStencilStateDefine.h"
#include "DepthStencilViewDefine.h"
#include "RenderTargetDefine.h"
#include "ViewPortDefine.h"
#include "DrawBufferDefine.h"

#include "RenderTargetViewDefine.h"
#include "RasterizerStateDefine.h"
#include "ResourceManager.h"

LightPass::LightPass()
{

}

LightPass::~LightPass()
{

}

void LightPass::Create(int width, int height)
{
}

void LightPass::Start(int width, int height)
{
	// Shader 설정..
	m_Light_VS = g_Shader->GetShader("Screen_VS");
	m_Light_PS = g_Shader->GetShader("Light_PBR_PS_Option7");

	// Buffer 설정..
	m_Screen_DB = g_Resource->GetDrawBuffer<DB_Quad>();

	// ViewPort 설정..
	m_Screen_VP = g_Resource->GetViewPort<VP_FullScreen>()->Get();

	// Multi RenderTarget 설정..
	m_OutPut_RT = g_Resource->GetRenderTexture<RT_OutPut1>();
	m_Albedo_RT = g_Resource->GetRenderTexture<RT_Deffered_Albedo>();
	m_Emissive_RT = g_Resource->GetRenderTexture<RT_Deffered_Emissive>();
	m_Normal_RT = g_Resource->GetRenderTexture<RT_Deffered_Normal>();
	m_Position_RT = g_Resource->GetRenderTexture<RT_Deffered_Position>();

	// RenderTargetView 설정..
	m_OutPut_RTV = m_OutPut_RT->GetRTV()->Get();
}

void LightPass::OnResize(int width, int height)
{
	// 현재 RenderTargetView 재설정..
	m_OutPut_RTV = m_OutPut_RT->GetRTV()->Get();

	// ShaderResource 재설정..
	m_Light_PS->SetShaderResourceView<gAlbedoRT>(m_Albedo_RT->GetSRV()->Get());
	m_Light_PS->SetShaderResourceView<gEmissiveRT>(m_Emissive_RT->GetSRV()->Get());
	m_Light_PS->SetShaderResourceView<gNormalRT>(m_Normal_RT->GetSRV()->Get());
	m_Light_PS->SetShaderResourceView<gPositionRT>(m_Position_RT->GetSRV()->Get());

	// Sub Resource 재설정..
	ShaderResourceView* ssaoSRV = g_Resource->GetShaderResourceView<RT_SSAO_Main>();
	ShaderResourceView* shadowSRV = g_Resource->GetShaderResourceView<DS_Shadow>();

	m_Light_PS->SetShaderResourceView<gShadowMap>(shadowSRV->Get());
	m_Light_PS->SetShaderResourceView<gSsaoMap>(ssaoSRV->Get());
}

void LightPass::Release()
{

}

void LightPass::ApplyOption()
{
	std::string renderType;

	if (g_RenderOption->RenderingOption & RENDER_IBL)
	{
		renderType = "_IBL_PS_";
	}
	else
	{
		renderType = "_PBR_PS_";
	}

	UINT lightOption = g_RenderOption->RenderingOption & (RENDER_SHADOW | RENDER_SSAO);
	
	switch (lightOption)
	{
	case RENDER_SHADOW:
		m_Light_PS = g_Shader->GetShader("Light" + renderType + "Option1");
		break;
	case RENDER_SSAO:
		m_Light_PS = g_Shader->GetShader("Light" + renderType + "Option2");
		break;
	case RENDER_SHADOW | RENDER_SSAO:
		m_Light_PS = g_Shader->GetShader("Light" + renderType + "Option3");
		break;
	default:
		m_Light_PS = g_Shader->GetShader("Light" + renderType + "Option0");
		break;
	}

	Reset();
}

void LightPass::SetIBLEnvironmentMapResource(EnvironmentBuffer* resource)
{
	ID3D11ShaderResourceView* brdflut = g_Resource->GetShaderResourceView<gBRDFlut>()->Get();
	ID3D11ShaderResourceView* prefilter = (ID3D11ShaderResourceView*)resource->Prefilter->pTextureBuf;
	ID3D11ShaderResourceView* irradiance = (ID3D11ShaderResourceView*)resource->Irradiance->pTextureBuf;

	PixelShader* IBL_Shader0 = g_Shader->GetShader("Light_IBL_PS_Option0");
	PixelShader* IBL_Shader1 = g_Shader->GetShader("Light_IBL_PS_Option1");
	PixelShader* IBL_Shader2 = g_Shader->GetShader("Light_IBL_PS_Option2");
	PixelShader* IBL_Shader3 = g_Shader->GetShader("Light_IBL_PS_Option3");

	IBL_Shader0->SetShaderResourceView<gBRDFlut>(brdflut);
	IBL_Shader0->SetShaderResourceView<gIBLPrefilter>(prefilter);
	IBL_Shader0->SetShaderResourceView<gIBLIrradiance>(irradiance);

	IBL_Shader1->SetShaderResourceView<gBRDFlut>(brdflut);
	IBL_Shader1->SetShaderResourceView<gIBLPrefilter>(prefilter);
	IBL_Shader1->SetShaderResourceView<gIBLIrradiance>(irradiance);

	IBL_Shader2->SetShaderResourceView<gBRDFlut>(brdflut);
	IBL_Shader2->SetShaderResourceView<gIBLPrefilter>(prefilter);
	IBL_Shader2->SetShaderResourceView<gIBLIrradiance>(irradiance);

	IBL_Shader3->SetShaderResourceView<gBRDFlut>(brdflut);
	IBL_Shader3->SetShaderResourceView<gIBLPrefilter>(prefilter);
	IBL_Shader3->SetShaderResourceView<gIBLIrradiance>(irradiance);
}

void LightPass::Reset()
{
	// ShaderResource 재설정..
	m_Light_PS->SetShaderResourceView<gAlbedoRT>(m_Albedo_RT->GetSRV()->Get());
	m_Light_PS->SetShaderResourceView<gNormalRT>(m_Normal_RT->GetSRV()->Get());
	m_Light_PS->SetShaderResourceView<gPositionRT>(m_Position_RT->GetSRV()->Get());
	m_Light_PS->SetShaderResourceView<gEmissiveRT>(m_Emissive_RT->GetSRV()->Get());

	// Sub Resource 재설정..
	ShaderResourceView* ssaoSRV = g_Resource->GetShaderResourceView<RT_SSAO_Main>();
	ShaderResourceView* shadowSRV = g_Resource->GetShaderResourceView<DS_Shadow>();

	m_Light_PS->SetShaderResourceView<gShadowMap>(shadowSRV->Get());
	m_Light_PS->SetShaderResourceView<gSsaoMap>(ssaoSRV->Get());
}

void LightPass::RenderUpdate()
{
	CameraData* cam = g_GlobalData->MainCamera_Data;

	Matrix& texSpace = g_GlobalData->TexSpace;

	g_Context->OMSetRenderTargets(1, &m_OutPut_RTV, nullptr);
	g_Context->ClearRenderTargetView(m_OutPut_RTV, reinterpret_cast<const float*>(&DXColors::DeepDarkGray));
	g_Context->RSSetViewports(1, m_Screen_VP);

	CB_Light lightBuf;
	lightBuf.gDirLightCount = (UINT)g_GlobalData->DirectionLightList.size();
	lightBuf.gPointLightCount = (UINT)g_GlobalData->PointLightList.size();
	lightBuf.gSpotLightCount = (UINT)g_GlobalData->SpotLightList.size();

	for (UINT d = 0; d < lightBuf.gDirLightCount; d++)
	{
		if (d >= DIRECTION_LIGHT_COUNT) break;

		lightBuf.gDirLights[d] = *g_GlobalData->DirectionLightList[d];
		lightBuf.gDirLights[d].LightViewProj *= texSpace;
	}
	for (UINT p = 0; p < lightBuf.gPointLightCount; p++)
	{
		if (p >= POINT_LIGHT_COUNT) break;
		
		lightBuf.gPointLights[p] = *g_GlobalData->PointLightList[p];
		lightBuf.gPointLights[p].LightViewProj *= texSpace;
	}
	for (UINT s = 0; s < lightBuf.gSpotLightCount; s++)
	{
		if (s >= SPOT_LIGHT_COUNT) break;
		
		lightBuf.gSpotLights[s] = *g_GlobalData->SpotLightList[s];
		lightBuf.gSpotLights[s].LightViewProj *= texSpace;
	}

	CB_LightSub lightsubBuf;
	lightsubBuf.gEyePosW = cam->CamPos;
	lightsubBuf.gViewProjTex = cam->CamView * cam->CamProj * texSpace;
	lightsubBuf.gIBLFactor = g_RenderOption->IBL_Factor;

	// Vertex Shader Update..
	m_Light_VS->Update();

	// Pixel Shader Update..
	m_Light_PS->ConstantBufferUpdate(&lightBuf);
	m_Light_PS->ConstantBufferUpdate(&lightsubBuf);
	m_Light_PS->Update();

	g_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	g_Context->IASetVertexBuffers(0, 1, m_Screen_DB->VertexBuf->GetAddress(), &m_Screen_DB->Stride, &m_Screen_DB->Offset);
	g_Context->IASetIndexBuffer(m_Screen_DB->IndexBuf->Get(), DXGI_FORMAT_R32_UINT, 0);

	g_Context->DrawIndexed(m_Screen_DB->IndexCount, 0, 0);
}
