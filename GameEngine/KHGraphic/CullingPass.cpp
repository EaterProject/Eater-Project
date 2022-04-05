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
	return;
	// HiZ Depth Buffer
//---------------------------------------------------------------------------------------
	D3D11_TEXTURE2D_DESC hizDepthDesc;
	hizDepthDesc.Width = width;
	hizDepthDesc.Height = height;
	hizDepthDesc.MipLevels = 1;
	hizDepthDesc.ArraySize = 1;
	hizDepthDesc.Format = DXGI_FORMAT_D32_FLOAT;
	hizDepthDesc.SampleDesc.Count = 1;
	hizDepthDesc.SampleDesc.Quality = 0;
	hizDepthDesc.Usage = D3D11_USAGE_DEFAULT;
	hizDepthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	hizDepthDesc.CPUAccessFlags = 0;
	hizDepthDesc.MiscFlags = 0;

	// HiZ Depth Buffer SRV
	//---------------------------------------------------------------------------------------
	D3D11_DEPTH_STENCIL_VIEW_DESC hizDSVDesc;
	hizDSVDesc.Format = hizDepthDesc.Format;
	hizDSVDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	hizDSVDesc.Texture2D.MipSlice = 0;
	hizDSVDesc.Flags = 0;

	D3D11_TEXTURE2D_DESC hizDesc;
	hizDesc.Width = width;
	hizDesc.Height = height;
	hizDesc.MipLevels = 0;
	hizDesc.ArraySize = 1;
	hizDesc.Format = DXGI_FORMAT_R32_FLOAT;
	hizDesc.SampleDesc.Count = 1;
	hizDesc.SampleDesc.Quality = 0;
	hizDesc.Usage = D3D11_USAGE_DEFAULT;
	hizDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	hizDesc.CPUAccessFlags = 0;
	hizDesc.MiscFlags = 0;

	// Shadow DepthStencilView 생성..
	g_Factory->CreateDepthStencil<DS_HizMipMap>(&hizDepthDesc, nullptr, &hizDSVDesc, nullptr);
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

	return; 
	// MipMap Deferred Context Reserve..
	MipMap_CommandList_Reserve(width, height);
}

void CullingPass::OnResize(int width, int height)
{
	return;
	// DepthStencilView 재설정..
	m_DefaltDSV = g_Resource->GetDepthStencilView<DS_Defalt>()->Get();

	// MipMap Deferred Context Reserve..
	MipMap_CommandList_Reserve(width, height);
}

void CullingPass::Release()
{

}

bool CullingPass::FrustumCulling(const RenderData* meshData)
{
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
		return true;
	}
}

void CullingPass::ResetCount()
{

}

void CullingPass::MipMap_CommandList_Reserve(int width, int height)
{
	ID3D11DeviceContext* context;
	g_Device->CreateDeferredContext(0, &context);

	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	context->RSSetViewports(1, &vp);

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	VertexBuffer* originVertex = nullptr;
	IndexBuffer* originIndex = nullptr;

	ID3D11Buffer* convertVertex = nullptr;
	ID3D11Buffer* convertIndex = nullptr;

	for (MeshBuffer* occluder : g_GlobalData->OccluderList)
	{
		// 해당 Occluder Vertex & Index Buffer..
		originVertex = occluder->VertexBuf;
		originIndex = occluder->IndexBuf;

		convertVertex = (ID3D11Buffer*)originVertex->pVertexBuf;
		convertIndex = (ID3D11Buffer*)originIndex->pIndexBuf;

		context->IASetVertexBuffers(0, 1, &convertVertex, &originVertex->Stride, &originVertex->Offset);
		context->IASetIndexBuffer(convertIndex, DXGI_FORMAT_R32_UINT, 0);

		context->DrawIndexed(originIndex->Count, 0, 0);
	}

	context->FinishCommandList(FALSE, &m_MipMapCommandList);
}
