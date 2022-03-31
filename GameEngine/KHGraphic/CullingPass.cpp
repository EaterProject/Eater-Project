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
#include "CullingPass.h"

#include "Patch_Chapters.h"
#include "MathDefine.h"
#include "FactoryManagerBase.h"
#include "ResourceManagerBase.h"
#include "ShaderManagerBase.h"
#include "ConstantBufferDefine.h"
#include "ShaderResourceViewDefine.h"
#include "DepthStencilViewDefine.h"
#include "DepthStencilStateDefine.h"
#include "ViewPortDefine.h"
#include "InstanceBufferDefine.h"
#include "RasterizerStateDefine.h"

CullingPass::CullingPass()
{

}

CullingPass::~CullingPass()
{

}

void CullingPass::Create(int width, int height)
{

}

void CullingPass::Start(int width, int height)
{
	// Shader 설정..
	
	// DepthStencilView 설정..
	m_DefaltDSV = g_Resource->GetDepthStencilView<DS_Defalt>()->Get();

	// Graphic State 설정..
	m_DefaltDSS = g_Resource->GetDepthStencilState<DSS_Defalt>()->Get();
	m_SolidRS = g_Resource->GetRasterizerState<RS_Solid>()->Get();

	// ViewPort 설정..
	m_ScreenVP = g_Resource->GetViewPort<VP_FullScreen>()->Get();
}

void CullingPass::OnResize(int width, int height)
{
	// DepthStencilView 재설정..
	m_DefaltDSV = g_Resource->GetDepthStencilView<DS_Defalt>()->Get();
}

void CullingPass::Release()
{

}

bool CullingPass::FrustumCulling(const RenderData* meshData)
{
	m_CullingCount++;

	const Matrix& world = *meshData->m_ObjectData->World;

	BoundingFrustum frustum = g_GlobalData->Camera_Data->BoundFrustum;
	BoundingSphere boundSphere = meshData->m_Mesh->m_MeshSubData->BoundSphere;

	boundSphere.Transform(boundSphere, world);

	if (boundSphere.Intersects(frustum) == false)
	{
		return false;
	}
	else
	{
		// Occlusion Culling..
		m_RenderCount++;
		return true;
	}
}

void CullingPass::ResetCount()
{
	m_CullingCount = 0;
	m_RenderCount = 0;
}
