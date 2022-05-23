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

Blur_Pass::Blur_Pass()
{

}

Blur_Pass::~Blur_Pass()
{

}

void Blur_Pass::Create(int width, int height)
{

}

void Blur_Pass::Start(int width, int height)
{
	m_BlurHorizon_CS = g_Shader->GetShader("Blur_Horizon_CS");
	m_BlurVertical_CS = g_Shader->GetShader("Blur_Vertical_CS");

	m_Solid_RS = g_Resource->GetRasterizerState<RS_Solid>()->Get();

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

void Blur_Pass::RenderUpdate(RenderTarget* input, RenderTarget* output)
{
	g_Context->RSSetState(m_Solid_RS);

	// HORIZONTAL blur pass.
	m_BlurHorizon_CS->SetShaderResourceView<gInputMap>(input->GetSRV()->Get());
	m_BlurHorizon_CS->SetUnorderedAccessView<gOutputUAV>(output->GetUAV()->Get());

	m_BlurHorizon_CS->Update();

	// How many groups do we need to dispatch to cover a row of pixels, where each
	// group covers 256 pixels (the 256 is defined in the CS).
	g_Context->Dispatch(m_NumGroupsX, m_Height, 1);

	// VERTICAL blur pass.
	m_BlurVertical_CS->SetShaderResourceView<gInputMap>(output->GetSRV()->Get());
	m_BlurVertical_CS->SetUnorderedAccessView<gOutputUAV>(input->GetUAV()->Get());

	m_BlurVertical_CS->Update();

	// How many groups do we need to dispatch to cover a column of pixels, where each
	// group covers 256 pixels  (the 256 is defined in the CS).
	g_Context->Dispatch(m_Width, m_NumGroupsY, 1);
}
