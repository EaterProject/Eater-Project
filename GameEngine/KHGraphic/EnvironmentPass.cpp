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
	m_SkyBox_PS = g_Shader->GetShader("SkyBox_PS");

	m_Box_DB = g_Resource->GetDrawBuffer<DB_Sphere>();

	m_Albedo_RT = g_Resource->GetRenderTexture<RT_Deffered_Albedo>();
	m_Position_RT = g_Resource->GetRenderTexture<RT_Deffered_Position>();

	m_RTV_List.resize(2);
	m_RTV_List[0] = m_Albedo_RT->GetRTV()->Get();
	m_RTV_List[1] = m_Position_RT->GetRTV()->Get();

	m_Defalt_DSV = g_Resource->GetDepthStencilView<DS_Defalt>()->Get();

	m_CubeMap_RS = g_Resource->GetRasterizerState<RS_CubeMap>()->Get();
	m_CubeMap_DSS = g_Resource->GetDepthStencilState<DSS_CubeMap>()->Get();
}

void EnvironmentPass::OnResize(int width, int height)
{
	// 현재 RenderTargetView 재설정..
	m_RTV_List[0] = m_Albedo_RT->GetRTV()->Get();
	m_RTV_List[1] = m_Position_RT->GetRTV()->Get();

	// DepthStencilView 재설정..
	m_Defalt_DSV = g_Resource->GetDepthStencilView<DS_Defalt>()->Get();
}

void EnvironmentPass::Release()
{

}

void EnvironmentPass::ApplyOption()
{
	m_EnvironmentWorld = Matrix::CreateScale(g_RenderOption->EnvironmentSize);
}

void EnvironmentPass::SetEnvironmentMapResource(EnvironmentBuffer* resource)
{
	// SkyCube Resource 설정..
	ID3D11ShaderResourceView* skycube = (ID3D11ShaderResourceView*)resource->Environment->pTextureBuf;

	m_SkyBox_PS->SetShaderResourceView<gSkyCube>(skycube);
}

void EnvironmentPass::RenderUpdate()
{
	g_Context->RSSetState(m_CubeMap_RS);
	g_Context->OMSetDepthStencilState(m_CubeMap_DSS, 0);
	g_Context->OMSetRenderTargets(2, &m_RTV_List[0], m_Defalt_DSV);

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