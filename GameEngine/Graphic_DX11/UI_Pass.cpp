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
#include "RenderData.h"
#include "UI_Pass.h"

#include "MathDefine.h"
#include "ResourceFactoryBase.h"
#include "ResourceManagerBase.h"
#include "ShaderManagerBase.h"
#include "ConstantBufferDefine.h"
#include "ShaderResourceViewDefine.h"
#include "DrawBufferDefine.h"

UI_Pass::UI_Pass()
{

}

UI_Pass::~UI_Pass()
{

}

void UI_Pass::Create(int width, int height)
{

}

void UI_Pass::Start(int width, int height)
{
	m_UI_VS = g_Shader->GetShader("UI_VS");
	m_UI_PS = g_Shader->GetShader("UI_PS");

	// Buffer 설정..
	m_Screen_DB = g_Resource->GetDrawBuffer<DB_Quad>();

	// Graphic View..
	m_OutPut_RTV = g_Resource->GetMainRenderTarget()->GetRTV()->Get();
}

void UI_Pass::OnResize(int width, int height)
{
	// Graphic View..
	m_OutPut_RTV = g_Resource->GetMainRenderTarget()->GetRTV()->Get();
}

void UI_Pass::Release()
{

}

void UI_Pass::ApplyOption()
{

}

void UI_Pass::RenderUpdate(std::vector<RenderData*>& meshlist)
{
	

	m_RenderCount = (UINT)meshlist.size();

	if (m_RenderCount == 0) return;

	// Render State Update..
	g_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	g_Context->IASetVertexBuffers(0, 1, m_Screen_DB->VertexBuf->GetAddress(), &m_Screen_DB->Stride, &m_Screen_DB->Offset);
	g_Context->IASetIndexBuffer(m_Screen_DB->IndexBuf->Get(), DXGI_FORMAT_R32_UINT, 0);

	const Matrix& proj = g_GlobalData->MainCamera_Data->CamOrthoProj;

	UIData* ui = nullptr;
	CB_UIObject objectBuf;

	for (UINT i = 0; i < m_RenderCount; i++)
	{
		m_RenderData = meshlist[i];

		ui = m_RenderData->m_UIData;

		// 활성화 상태의 UI가 아니라면 그리지 않는다..
		if (m_RenderData->m_ObjectData->IsActive == false) continue;

		// Vertex Shader Update..
		objectBuf.gWorldViewProj = ui->World * proj;
		m_UI_VS->ConstantBufferUpdate(&objectBuf);

		m_UI_VS->Update();

		// Pixel Shader Update..
		m_UI_PS->SetShaderResourceView<gDiffuseMap>(m_RenderData->m_Material->m_Albedo);

		m_UI_PS->Update();

		g_Context->DrawIndexed(m_Screen_DB->IndexCount, 0, 0);
	}
}
