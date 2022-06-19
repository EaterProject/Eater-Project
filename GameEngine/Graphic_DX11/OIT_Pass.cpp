#include "DirectDefine.h"
#include "RenderPassBase.h"
#include "ShaderBase.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ComputeShader.h"
#include "GraphicState.h"
#include "GraphicView.h"
#include "GraphicResource.h"
#include "DrawBuffer.h"
#include "DepthStencil.h"
#include "RenderTarget.h"
#include "MathDefine.h"
#include "EngineData.h"
#include "OIT_Pass.h"

#include "VertexDefine.h"
#include "FactoryManagerBase.h"
#include "ResourceManagerBase.h"
#include "ShaderManagerBase.h"
#include "ConstantBufferDefine.h"
#include "DepthStencilViewDefine.h"
#include "DepthStencilStateDefine.h"
#include "BlendStateDefine.h"
#include "ShaderResourceViewDefine.h"
#include "RenderTargetDefine.h"
#include "RenderBufferDefine.h"
#include "RasterizerStateDefine.h"
#include "DrawBufferDefine.h"

OIT_Pass::OIT_Pass()
{
	m_Multiple = 4;
	m_MagicValue = 0xffffffff;
	m_InitCounts[0] = 0;
	m_InitCounts[1] = 0;
}

OIT_Pass::~OIT_Pass()
{

}

void OIT_Pass::Create(int width, int height)
{
	D3D11_BUFFER_DESC structbufferDesc;
	structbufferDesc.Usage = D3D11_USAGE_DEFAULT;
	structbufferDesc.ByteWidth = width * height * m_Multiple * 16;
	structbufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	structbufferDesc.CPUAccessFlags = 0;
	structbufferDesc.StructureByteStride = 16;
	structbufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

	D3D11_BUFFER_DESC rawbufferDesc;
	rawbufferDesc.Usage = D3D11_USAGE_DEFAULT;
	rawbufferDesc.ByteWidth = width * height * m_Multiple;
	rawbufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	rawbufferDesc.CPUAccessFlags = 0;
	rawbufferDesc.StructureByteStride = 0;
	rawbufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;

	D3D11_SHADER_RESOURCE_VIEW_DESC structsrvDesc;
	structsrvDesc.Format = DXGI_FORMAT_UNKNOWN;
	structsrvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	structsrvDesc.Buffer.FirstElement = 0;
	structsrvDesc.Buffer.NumElements = width * height * m_Multiple;

	D3D11_SHADER_RESOURCE_VIEW_DESC rawsrvDesc;
	rawsrvDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	rawsrvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	rawsrvDesc.BufferEx.FirstElement = 0;
	rawsrvDesc.BufferEx.NumElements = width * height;
	rawsrvDesc.BufferEx.Flags = D3D11_BUFFEREX_SRV_FLAG_RAW;

	D3D11_UNORDERED_ACCESS_VIEW_DESC structuavDesc;
	structuavDesc.Format = DXGI_FORMAT_UNKNOWN;
	structuavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	structuavDesc.Buffer.FirstElement = 0;
	structuavDesc.Buffer.NumElements = width * height * m_Multiple;
	structuavDesc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_COUNTER;

	D3D11_UNORDERED_ACCESS_VIEW_DESC rawuavDesc;
	rawuavDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	rawuavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	rawuavDesc.Buffer.FirstElement = 0;
	rawuavDesc.Buffer.NumElements = width * height;
	rawuavDesc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_RAW;

	g_Factory->CreateRenderBuffer<RB_PieceLink>(&structbufferDesc, nullptr, nullptr, &structsrvDesc, &structuavDesc);
	g_Factory->CreateRenderBuffer<RB_FirstOffset>(&rawbufferDesc, nullptr, nullptr, &rawsrvDesc, &rawuavDesc);
}

void OIT_Pass::Start(int width, int height)
{
	// OIT Shader List Up..
	SetShaderList();

	// Shader 설정..
	m_OITRender_VS = g_Shader->GetShader("Screen_VS");
	m_OITRender_PS = g_Shader->GetShader("OIT_Blend_PS");
	m_OITRender_CS = g_Shader->GetShader("OIT_Blend_CS");

	// Buffer 설정..
	m_Screen_DB = g_Resource->GetDrawBuffer<DB_Quad>();

	m_OutPut_RT = g_Resource->GetRenderTexture<RT_OutPut2>();
	m_OutPut_RTV = m_OutPut_RT->GetRTV()->Get();

	// DepthStencilView 설정..
	m_Defalt_DSV = g_Resource->GetDepthStencilView<DS_Defalt>()->Get();

	// Render Buffer 설정..
	m_PieceLink_RB = g_Resource->GetRenderBuffer<RB_PieceLink>();
	m_FirstOffset_RB = g_Resource->GetRenderBuffer<RB_FirstOffset>();

	// UnorderedAccessView 설정..
	m_PieceLink_UAV = m_PieceLink_RB->GetUAV()->Get();
	m_FirstOffset_UAV = m_FirstOffset_RB->GetUAV()->Get();

	// UnorderedAccessView List 설정..
	m_UAV_List[0] = m_PieceLink_UAV;
	m_UAV_List[1] = m_FirstOffset_UAV;

	// Graphic State 설정..
	m_NoStencil_DSS = g_Resource->GetDepthStencilState<DSS_NoStencil>()->Get();
	m_NoCull_RS = g_Resource->GetRasterizerState<RS_NoCull>()->Get();

	// Shader Resource 설정..
	SetShaderConstantBuffer(width);

	ShaderResourceView* backGround = g_Resource->GetShaderResourceView<RT_OutPut1>();
	
	m_OITRender_PS->SetShaderResourceView<gPieceLinkBuffer>(m_PieceLink_RB->GetSRV()->Get());
	m_OITRender_PS->SetShaderResourceView<gFirstOffsetBuffer>(m_FirstOffset_RB->GetSRV()->Get());
	m_OITRender_PS->SetShaderResourceView<gBackGround>(backGround->Get());
}

void OIT_Pass::OnResize(int width, int height)
{
	// 현재 RenderTarget 재설정..
	m_OutPut_RTV = m_OutPut_RT->GetRTV()->Get();

	// UnorderedAccessView 설정..
	m_PieceLink_UAV = m_PieceLink_RB->GetUAV()->Get();
	m_FirstOffset_UAV = m_FirstOffset_RB->GetUAV()->Get();

	// UnorderedAccessView List 설정..
	m_UAV_List[0] = m_PieceLink_UAV;
	m_UAV_List[1] = m_FirstOffset_UAV;

	// DepthStencilView 설정..
	m_Defalt_DSV = g_Resource->GetDepthStencilView<DS_Defalt>()->Get();

	// Shader Resource 설정..
	SetShaderConstantBuffer(width);

	ShaderResourceView* backGround = g_Resource->GetShaderResourceView<RT_OutPut1>();

	m_OITRender_PS->SetShaderResourceView<gPieceLinkBuffer>(m_PieceLink_RB->GetSRV()->Get());
	m_OITRender_PS->SetShaderResourceView<gFirstOffsetBuffer>(m_FirstOffset_RB->GetSRV()->Get());
	m_OITRender_PS->SetShaderResourceView<gBackGround>(backGround->Get());
}

void OIT_Pass::Release()
{

}

void OIT_Pass::SetResize(int width, int height)
{
	m_PieceLink_RB->OnResize(width * height * m_Multiple * 16);
	m_PieceLink_RB->SetNumElements(width * height * m_Multiple);

	m_FirstOffset_RB->OnResize(width * height * m_Multiple);
	m_FirstOffset_RB->SetNumElements(width * height);
}

void OIT_Pass::BeginRender()
{
	g_Context->RSSetState(m_NoCull_RS);
	g_Context->OMSetDepthStencilState(m_NoStencil_DSS, 0);
	g_Context->OMSetRenderTargets(0, 0, m_Defalt_DSV);
	g_Context->OMSetBlendState(0, 0, 0xffffffff);

	g_Context->ClearUnorderedAccessViewUint(m_PieceLink_UAV, &m_MagicValue);
	g_Context->ClearUnorderedAccessViewUint(m_FirstOffset_UAV, &m_MagicValue);

	g_Context->OMSetRenderTargetsAndUnorderedAccessViews(D3D11_KEEP_RENDER_TARGETS_AND_DEPTH_STENCIL,
		nullptr, nullptr, 0, 2, m_UAV_List, m_InitCounts);
}

void OIT_Pass::RenderUpdate()
{
	g_Context->ClearRenderTargetView(m_OutPut_RTV, reinterpret_cast<const float*>(&DXColors::NonBlack));
	g_Context->OMSetRenderTargets(1, &m_OutPut_RTV, nullptr);
	g_Context->OMSetDepthStencilState(nullptr, 0);

	m_OITRender_VS->Update();
	m_OITRender_PS->Update();

	g_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	g_Context->IASetVertexBuffers(0, 1, m_Screen_DB->VertexBuf->GetAddress(), &m_Screen_DB->Stride, &m_Screen_DB->Offset);
	g_Context->IASetIndexBuffer(m_Screen_DB->IndexBuf->Get(), DXGI_FORMAT_R32_UINT, 0);

	g_Context->DrawIndexed(m_Screen_DB->IndexCount, 0, 0);
}

void OIT_Pass::SetShaderList()
{
	PushShader("OIT_PS");

	PushShader("OIT_Particle_PS_Option0");
	PushShader("OIT_Particle_PS_Option1");

	PushShader("OIT_Mesh_PS_Option0");
	PushShader("OIT_Mesh_PS_Option1");
	PushShader("OIT_Mesh_PS_Option2");
	PushShader("OIT_Mesh_PS_Option3");
	PushShader("OIT_Mesh_PS_Option4");
	PushShader("OIT_Mesh_PS_Option5");
	PushShader("OIT_Mesh_PS_Option6");
	PushShader("OIT_Mesh_PS_Option7");
}

void OIT_Pass::SetShaderConstantBuffer(UINT width)
{
	CB_OitFrame oitBuf;
	oitBuf.gFrameWidth = width;

	for (ShaderBase* shader : m_OptionShaderList)
	{
		shader->ConstantBufferUpdate(&oitBuf);
	}
}
