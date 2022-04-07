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
	m_SkyBoxVS = g_Shader->GetShader("SkyBox_VS");
	m_SkyBoxPS = g_Shader->GetShader("SkyBox_PS");

	m_BoxDB = g_Resource->GetDrawBuffer<DB_Sphere>();

	m_OutPut_RT = g_Resource->GetRenderTexture<RT_OutPut1>();
	m_Position_RT = g_Resource->GetRenderTexture<RT_Deffered_Position>();
	m_OutPut_RTV = m_OutPut_RT->GetRTV()->Get();
	m_Position_RTV = m_Position_RT->GetRTV()->Get();

	m_DefaltDSV = g_Resource->GetDepthStencilView<DS_Defalt>()->Get();

	m_CubeMapRS = g_Resource->GetRasterizerState<RS_CubeMap>()->Get();
	m_CubeMapDSS = g_Resource->GetDepthStencilState<DSS_CubeMap>()->Get();
}

void EnvironmentPass::OnResize(int width, int height)
{
	// 현재 RenderTargetView 재설정..
	m_OutPut_RTV = m_OutPut_RT->GetRTV()->Get();
	m_Position_RTV = m_Position_RT->GetRTV()->Get();

	// DepthStencilView 재설정..
	m_DefaltDSV = g_Resource->GetDepthStencilView<DS_Defalt>()->Get();
}

void EnvironmentPass::Release()
{

}

void EnvironmentPass::SetOption(RenderOption* renderOption)
{

}

void EnvironmentPass::SetEnvironmentMapResource(bool enable)
{
	if (enable)
	{
		// SkyCube Resource 설정..
		ID3D11ShaderResourceView* skycube = g_Resource->GetShaderResourceView<gSkyCube>()->Get();

		m_SkyBoxPS->SetShaderResourceView<gSkyCube>(skycube);
	}
	else
	{
		m_SkyBoxPS->SetShaderResourceView<gSkyCube>(nullptr);
	}
}

void EnvironmentPass::RenderUpdate()
{
	g_Context->RSSetState(m_CubeMapRS);
	g_Context->OMSetDepthStencilState(m_CubeMapDSS, 0);
	g_Context->OMSetRenderTargets(1, &m_OutPut_RTV, m_DefaltDSV);

	CameraData* cam = g_GlobalData->MainCamera_Data;

	Matrix& proj = cam->CamProj;
	Matrix& view = cam->CamView;
	view._41 = 0; view._42 = 0; view._43 = 0;

	CB_CubeObject cubeBuf;
	cubeBuf.gViewProj = Matrix::CreateScale(10.0f) * view * proj;
	m_SkyBoxVS->ConstantBufferUpdate(&cubeBuf);
	m_SkyBoxVS->Update();

	m_SkyBoxPS->Update();

	g_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	g_Context->IASetVertexBuffers(0, 1, m_BoxDB->VertexBuf->GetAddress(), &m_BoxDB->Stride, &m_BoxDB->Offset);
	g_Context->IASetIndexBuffer(m_BoxDB->IndexBuf->Get(), DXGI_FORMAT_R32_UINT, 0);

	g_Context->DrawIndexed(m_BoxDB->IndexCount, 0, 0);
}