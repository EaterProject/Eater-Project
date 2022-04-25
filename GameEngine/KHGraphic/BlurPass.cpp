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
#include "BlurPass.h"

#include "MathDefine.h"
#include "FactoryManagerBase.h"
#include "ResourceManagerBase.h"
#include "ShaderManagerBase.h"
#include "ConstantBufferDefine.h"
#include "ShaderResourceViewDefine.h"
#include "ViewPortDefine.h"

BlurPass::BlurPass()
{

}

BlurPass::~BlurPass()
{

}

void BlurPass::Create(int width, int height)
{
	m_BlurHorizon_CS = g_Shader->GetShader("Blur_Horizon_CS");
	m_BlurVertical_CS = g_Shader->GetShader("Blur_Vertical_CS");

}

void BlurPass::Start(int width, int height)
{

}

void BlurPass::OnResize(int width, int height)
{

}

void BlurPass::Release()
{

}

void BlurPass::RenderUpdate()
{

}
