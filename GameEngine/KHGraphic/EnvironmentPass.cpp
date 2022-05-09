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
#include "EnvironmentPass.h"

#include "MathDefine.h"
#include "ResourceFactoryBase.h"
#include "ResourceManagerBase.h"
#include "ShaderManagerBase.h"
#include "ConstantBufferDefine.h"
#include "ShaderResourceViewDefine.h"
#include "DepthStencilStateDefine.h"
#include "DepthStencilViewDefine.h"
#include "RasterizerStateDefine.h"
#include "DrawBufferDefine.h"
#include "RenderTargetDefine.h"


EnvironmentPass::EnvironmentPass()
{

}

EnvironmentPass::~EnvironmentPass()
{

}

void EnvironmentPass::Create(int width, int height)
{

}

void EnvironmentPass::Start(int width, int height)
{
	m_SkyBox_VS = g_Shader->GetShader("SkyBox_VS");
	m_SkyBox_PS = g_Shader->GetShader("SkyBox_PS_Option1");

	m_Box_DB = g_Resource->GetDrawBuffer<DB_Sphere>();

	m_OutPut_RT = g_Resource->GetRenderTexture<RT_OutPut1>();

	m_OutPut_RTV = m_OutPut_RT->GetRTV()->Get();

	m_Defalt_DSV = g_Resource->GetDepthStencilView<DS_Defalt>()->Get();

	m_CubeMap_RS = g_Resource->GetRasterizerState<RS_CubeMap>()->Get();
	m_CubeMap_DSS = g_Resource->GetDepthStencilState<DSS_CubeMap>()->Get();

	// Environment & IBL Shader List Up..
	SetShaderList();
}

void EnvironmentPass::OnResize(int width, int height)
{
	// 현재 RenderTargetView 재설정..
	m_OutPut_RTV = m_OutPut_RT->GetRTV()->Get();

	// DepthStencilView 재설정..
	m_Defalt_DSV = g_Resource->GetDepthStencilView<DS_Defalt>()->Get();
}

void EnvironmentPass::Release()
{

}

void EnvironmentPass::ApplyOption()
{
	m_EnvironmentWorld = Matrix::CreateScale(g_RenderOption->EnvironmentSize);

	if (g_RenderOption->RenderingOption & RENDER_FOG)
	{
		m_SkyBox_PS = g_Shader->GetShader("SkyBox_PS_Option1");
	}
	else
	{
		m_SkyBox_PS = g_Shader->GetShader("SkyBox_PS_Option0");
	}
}

void EnvironmentPass::SetEnvironmentMapResource(EnvironmentBuffer* resource)
{
	// SkyCube & IBL Shader Resource 설정..
	ID3D11ShaderResourceView* skycube = (ID3D11ShaderResourceView*)resource->Environment->pTextureBuf;
	ID3D11ShaderResourceView* brdflut = g_Resource->GetShaderResourceView<gBRDFlut>()->Get();
	ID3D11ShaderResourceView* prefilter = (ID3D11ShaderResourceView*)resource->Prefilter->pTextureBuf;
	ID3D11ShaderResourceView* irradiance = (ID3D11ShaderResourceView*)resource->Irradiance->pTextureBuf;

	for (ShaderBase* shader : m_OptionShaderList)
	{
		shader->SetShaderResourceView<gSkyCube>(skycube);

		shader->SetShaderResourceView<gBRDFlut>(brdflut);
		shader->SetShaderResourceView<gIBLPrefilter>(prefilter);
		shader->SetShaderResourceView<gIBLIrradiance>(irradiance);
	}
}

void EnvironmentPass::RenderUpdate()
{
	g_Context->RSSetState(m_CubeMap_RS);
	g_Context->OMSetDepthStencilState(m_CubeMap_DSS, 0);
	g_Context->OMSetRenderTargets(1, &m_OutPut_RTV, m_Defalt_DSV);

	CameraData* cam = g_GlobalData->MainCamera_Data;

	Matrix& proj = cam->CamProj;
	Matrix view = cam->CamView;
	view._41 = 0; view._42 = 0; view._43 = 0;

	CB_CubeObject cubeBuf;
	cubeBuf.gCubeWorld = m_EnvironmentWorld;
	cubeBuf.gCubeWorldViewProj = view * proj;
	m_SkyBox_VS->ConstantBufferUpdate(&cubeBuf);
	m_SkyBox_VS->Update();

	m_SkyBox_PS->Update();

	g_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	g_Context->IASetVertexBuffers(0, 1, m_Box_DB->VertexBuf->GetAddress(), &m_Box_DB->Stride, &m_Box_DB->Offset);
	g_Context->IASetIndexBuffer(m_Box_DB->IndexBuf->Get(), DXGI_FORMAT_R32_UINT, 0);

	g_Context->DrawIndexed(m_Box_DB->IndexCount, 0, 0);
}

void EnvironmentPass::SetShaderList()
{
	PushShader("SkyBox_PS_Option0");
	PushShader("SkyBox_PS_Option1");

	PushShader("Light_IBL_PS_Option0");
	PushShader("Light_IBL_PS_Option1");
	PushShader("Light_IBL_PS_Option2");
	PushShader("Light_IBL_PS_Option3");
	PushShader("Light_IBL_PS_Option4");
	PushShader("Light_IBL_PS_Option5");
	PushShader("Light_IBL_PS_Option6");
	PushShader("Light_IBL_PS_Option7");

	PushShader("OIT_Mesh_PS_Option2");
	PushShader("OIT_Mesh_PS_Option4");
	PushShader("OIT_Mesh_PS_Option6");
	PushShader("OIT_Mesh_PS_Option7");
}
