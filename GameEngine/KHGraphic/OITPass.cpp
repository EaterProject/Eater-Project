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
#include "MathDefine.h"
#include "EngineData.h"
#include "OITPass.h"

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

OITPass::OITPass()
{
	m_Multiple = 4;
}

OITPass::~OITPass()
{

}

void OITPass::Create(int width, int height)
{

	D3D11_BUFFER_DESC structbufferDesc;
	structbufferDesc.Usage = D3D11_USAGE_DEFAULT;
	structbufferDesc.ByteWidth = width * height * m_Multiple * 12;
	structbufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	structbufferDesc.CPUAccessFlags = 0;
	structbufferDesc.StructureByteStride = 12;
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

void OITPass::Start(int width, int height)
{
	// Shader 설정..
	m_OITRenderVS = g_Shader->GetShader("Screen_VS");
	m_OITRenderPS = g_Shader->GetShader("OIT_PS");
	m_OITParticlePS = g_Shader->GetShader("OIT_Particle_PS");

	// Buffer 설정..
	m_Screen_DB = g_Resource->GetDrawBuffer<DB_Quad>();

	m_OutPut_RT = g_Resource->GetRenderTexture<RT_OutPut2>();
	m_OutPut_RTV = m_OutPut_RT->GetRTV()->Get();

	// DepthStencilView 설정..
	m_DefaltDSV = g_Resource->GetDepthStencilView<DS_Defalt>()->Get();

	// Render Buffer 설정..
	m_PieceLinkRB = g_Resource->GetRenderBuffer<RB_PieceLink>();
	m_FirstOffsetRB = g_Resource->GetRenderBuffer<RB_FirstOffset>();

	// UnorderedAccessView 설정..
	m_PieceLinkUAV = m_PieceLinkRB->GetUAV()->Get();
	m_FirstOffsetUAV = m_FirstOffsetRB->GetUAV()->Get();

	// UnorderedAccessView List 설정..
	m_UAVList.resize(2);
	m_UAVList[0] = m_PieceLinkUAV;
	m_UAVList[1] = m_FirstOffsetUAV;

	// Graphic State 설정..
	m_NoStencilDSS = g_Resource->GetDepthStencilState<DSS_NoStencil>()->Get();
	m_NoCullRS = g_Resource->GetRasterizerState<RS_NoCull>()->Get();

	// Shader Resource 설정..
	CB_OitFrame oitBuf;
	oitBuf.gFrameWidth = width;
	m_OITRenderPS->ConstantBufferCopy<CB_OitFrame>(&oitBuf);
	m_OITParticlePS->ConstantBufferCopy<CB_OitFrame>(&oitBuf);

	ShaderResourceView* backGround = g_Resource->GetShaderResourceView<RT_OutPut1>();
	
	m_OITRenderPS->SetShaderResourceView<gPieceLinkBuffer>(m_PieceLinkRB->GetSRV()->Get());
	m_OITRenderPS->SetShaderResourceView<gFirstOffsetBuffer>(m_FirstOffsetRB->GetSRV()->Get());
	m_OITRenderPS->SetShaderResourceView<gBackGround>(backGround->Get());
}

void OITPass::OnResize(int width, int height)
{
	// 현재 RenderTarget 재설정..
	m_OutPut_RTV = m_OutPut_RT->GetRTV()->Get();

	// UnorderedAccessView 설정..
	m_PieceLinkUAV = m_PieceLinkRB->GetUAV()->Get();
	m_FirstOffsetUAV = m_FirstOffsetRB->GetUAV()->Get();

	// UnorderedAccessView List 설정..
	m_UAVList[0] = m_PieceLinkUAV;
	m_UAVList[1] = m_FirstOffsetUAV;

	// DepthStencilView 설정..
	m_DefaltDSV = g_Resource->GetDepthStencilView<DS_Defalt>()->Get();

	// Shader Resource 설정..
	CB_OitFrame oitBuf;
	oitBuf.gFrameWidth = width;
	m_OITRenderPS->ConstantBufferCopy<CB_OitFrame>(&oitBuf);
	m_OITParticlePS->ConstantBufferCopy<CB_OitFrame>(&oitBuf);

	ShaderResourceView* backGround = g_Resource->GetShaderResourceView<RT_OutPut1>();

	m_OITRenderPS->SetShaderResourceView<gPieceLinkBuffer>(m_PieceLinkRB->GetSRV()->Get());
	m_OITRenderPS->SetShaderResourceView<gFirstOffsetBuffer>(m_FirstOffsetRB->GetSRV()->Get());
	m_OITRenderPS->SetShaderResourceView<gBackGround>(backGround->Get());
}

void OITPass::Release()
{

}

void OITPass::SetResize(int width, int height)
{
	m_PieceLinkRB->OnResize(width * height * m_Multiple * 12);
	m_PieceLinkRB->SetNumElements(width * height * m_Multiple);

	m_FirstOffsetRB->OnResize(width * height * m_Multiple);
	m_FirstOffsetRB->SetNumElements(width * height);
}

void OITPass::SetOption(UINT renderOption)
{

}

void OITPass::BeginRender()
{
	g_Context->RSSetState(m_NoCullRS);
	g_Context->OMSetDepthStencilState(m_NoStencilDSS, 0);
	g_Context->OMSetRenderTargets(0, 0, m_DefaltDSV);
	g_Context->OMSetBlendState(0, 0, 0xffffffff);

	UINT magicValue =  0xffffffff;
	g_Context->ClearUnorderedAccessViewUint(m_PieceLinkUAV, &magicValue);
	g_Context->ClearUnorderedAccessViewUint(m_FirstOffsetUAV, &magicValue);

	ID3D11UnorderedAccessView* pUAVs[2] = { m_PieceLinkUAV, m_FirstOffsetUAV };
	UINT initCounts[2] = { 0, 0 };
	g_Context->OMSetRenderTargetsAndUnorderedAccessViews(D3D11_KEEP_RENDER_TARGETS_AND_DEPTH_STENCIL,
		nullptr, nullptr, 0, 2, pUAVs, initCounts);
}

void OITPass::RenderUpdate()
{
	g_Context->ClearRenderTargetView(m_OutPut_RTV, reinterpret_cast<const float*>(&DXColors::DeepDarkGray));
	g_Context->OMSetRenderTargets(1, &m_OutPut_RTV, nullptr);
	g_Context->OMSetDepthStencilState(nullptr, 0);

	m_OITRenderVS->Update();
	m_OITRenderPS->Update();

	g_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	g_Context->IASetVertexBuffers(0, 1, m_Screen_DB->VertexBuf->GetAddress(), &m_Screen_DB->Stride, &m_Screen_DB->Offset);
	g_Context->IASetIndexBuffer(m_Screen_DB->IndexBuf->Get(), DXGI_FORMAT_R32_UINT, 0);

	g_Context->DrawIndexed(m_Screen_DB->IndexCount, 0, 0);

}
