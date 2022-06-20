#include "DirectDefine.h"
#include "RenderPassBase.h"
#include "ShaderBase.h"
#include "ComputeShader.h"
#include "GraphicState.h"
#include "GraphicView.h"
#include "GraphicResource.h"
#include "DrawBuffer.h"
#include "DepthStencil.h"
#include "RenderTarget.h"
#include "EngineData.h"
#include "RenderData.h"
#include "VertexDefine.h"
#include "Blur_Pass.h"

#include "MathDefine.h"
#include "FactoryManagerBase.h"
#include "ResourceManagerBase.h"
#include "ShaderManagerBase.h"
#include "ConstantBufferDefine.h"
#include "ShaderResourceViewDefine.h"
#include "UnorderedAccessViewDefine.h"
#include "RasterizerStateDefine.h"
#include "ViewPortDefine.h"
#include "RenderTargetDefine.h"

Blur_Pass::Blur_Pass()
{

}

Blur_Pass::~Blur_Pass()
{

}

void Blur_Pass::Create(int width, int height)
{
	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = width;
	texDesc.Height = height;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	uavDesc.Format = texDesc.Format;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	uavDesc.Texture2D.MipSlice = 0;

	g_Factory->CreateRenderTexture<RT_Blur>(&texDesc, nullptr, nullptr, &srvDesc, &uavDesc);
}

void Blur_Pass::Start(int width, int height)
{
	m_BlurHorizon_CS = g_Shader->GetShader("Blur_Horizon_CS");
	m_BlurVertical_CS = g_Shader->GetShader("Blur_Vertical_CS");

	m_Blur_RT = g_Resource->GetRenderTexture<RT_Blur>();

	m_Width = width;
	m_Height = height;
	m_NumGroupsX = (UINT)ceilf(width / 256.0f);
	m_NumGroupsY = (UINT)ceilf(height / 256.0f);
}

void Blur_Pass::OnResize(int width, int height)
{
	m_Width = width;
	m_Height = height;
	m_NumGroupsX = (UINT)ceilf(width / 256.0f);
	m_NumGroupsY = (UINT)ceilf(height / 256.0f);
}

void Blur_Pass::Release()
{

}

void Blur_Pass::RenderUpdate(RenderTexture* input, RenderTexture* output, UINT count)
{
	g_Context->OMSetRenderTargets(0, nullptr, nullptr);

	for (UINT i = 0; i < count; i++)
	{
		// HORIZONTAL blur pass.
		m_BlurHorizon_CS->SetShaderResourceView<gInputMap>(input->GetSRV()->Get());
		m_BlurHorizon_CS->SetUnorderedAccessView<gOutputUAV>(output->GetUAV()->Get());

		m_BlurHorizon_CS->Update();

		// How many groups do we need to dispatch to cover a row of pixels, where each
		// group covers 256 pixels (the 256 is defined in the CS).
		g_Context->Dispatch(m_NumGroupsX, m_Height, 1);

		g_Context->CSSetShaderResources(0, 1, &m_NullSRV);
		g_Context->CSSetUnorderedAccessViews(0, 1, &m_NullUAV, nullptr);

		// VERTICAL blur pass.
		m_BlurVertical_CS->SetShaderResourceView<gInputMap>(output->GetSRV()->Get());
		m_BlurVertical_CS->SetUnorderedAccessView<gOutputUAV>(input->GetUAV()->Get());

		m_BlurVertical_CS->Update();

		// How many groups do we need to dispatch to cover a column of pixels, where each
		// group covers 256 pixels  (the 256 is defined in the CS).
		g_Context->Dispatch(m_Width, m_NumGroupsY, 1);

		g_Context->CSSetShaderResources(0, 1, &m_NullSRV);
		g_Context->CSSetUnorderedAccessViews(0, 1, &m_NullUAV, nullptr);
	}
}

void Blur_Pass::RenderUpdate(RenderTexture* input, UINT count)
{
	g_Context->OMSetRenderTargets(0, nullptr, nullptr);

	for (UINT i = 0; i < count; i++)
	{
		// HORIZONTAL blur pass.
		m_BlurHorizon_CS->SetShaderResourceView<gInputMap>(input->GetSRV()->Get());
		m_BlurHorizon_CS->SetUnorderedAccessView<gOutputUAV>(m_Blur_RT->GetUAV()->Get());

		m_BlurHorizon_CS->Update();

		// How many groups do we need to dispatch to cover a row of pixels, where each
		// group covers 256 pixels (the 256 is defined in the CS).
		g_Context->Dispatch(m_NumGroupsX, m_Height, 1);

		g_Context->CSSetShaderResources(0, 1, &m_NullSRV);
		g_Context->CSSetUnorderedAccessViews(0, 1, &m_NullUAV, nullptr);

		// VERTICAL blur pass.
		m_BlurVertical_CS->SetShaderResourceView<gInputMap>(m_Blur_RT->GetSRV()->Get());
		m_BlurVertical_CS->SetUnorderedAccessView<gOutputUAV>(input->GetUAV()->Get());

		m_BlurVertical_CS->Update();

		// How many groups do we need to dispatch to cover a column of pixels, where each
		// group covers 256 pixels  (the 256 is defined in the CS).
		g_Context->Dispatch(m_Width, m_NumGroupsY, 1);

		g_Context->CSSetShaderResources(0, 1, &m_NullSRV);
		g_Context->CSSetUnorderedAccessViews(0, 1, &m_NullUAV, nullptr);
	}
}
