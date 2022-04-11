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
#include "UnorderedAccessViewDefine.h"
#include "DepthStencilViewDefine.h"
#include "DepthStencilStateDefine.h"
#include "ViewPortDefine.h"
#include "InstanceBufferDefine.h"
#include "RasterizerStateDefine.h"
#include "DrawBufferDefine.h"

CullingPass::CullingPass()
{

}

CullingPass::~CullingPass()
{

}

void CullingPass::Create(int width, int height)
{
	m_Width = 512.0f;
	m_Height = 256.0f;

	// Hierachical Z-Map Depth Buffer
	D3D11_TEXTURE2D_DESC hizDepthDesc;
	hizDepthDesc.Width = m_Width;
	hizDepthDesc.Height = m_Height;
	hizDepthDesc.MipLevels = 1;
	hizDepthDesc.ArraySize = 1;
	hizDepthDesc.Format = DXGI_FORMAT_D32_FLOAT;
	hizDepthDesc.SampleDesc.Count = 1;
	hizDepthDesc.SampleDesc.Quality = 0;
	hizDepthDesc.Usage = D3D11_USAGE_DEFAULT;
	hizDepthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	hizDepthDesc.CPUAccessFlags = 0;
	hizDepthDesc.MiscFlags = 0;

	// Hierachical Z-Map Depth Buffer SRV
	D3D11_DEPTH_STENCIL_VIEW_DESC hizDSVDesc;
	hizDSVDesc.Format = hizDepthDesc.Format;
	hizDSVDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	hizDSVDesc.Texture2D.MipSlice = 0;
	hizDSVDesc.Flags = 0;

	// Hierachical Z-Map Depth DepthStencilView 생성..
	g_Factory->CreateDepthStencil<DS_HizDepth>(&hizDepthDesc, nullptr, &hizDSVDesc, nullptr);

	// Hierachical Z-Map ViewPort 생성..
	g_Factory->CreateViewPort<VP_Hiz>(0.0f, 0.0f, (float)m_Width, (float)m_Height);
}

void CullingPass::Start(int width, int height)
{
	// Shader 설정..
	m_Screen_VS = g_Shader->GetShader("Screen_VS");
	m_HizMipMap_PS = g_Shader->GetShader("HizMipMap_PS");

	m_Depth_VS = g_Shader->GetShader("Depth_StaticMesh_VS");
	m_Depth_PS = g_Shader->GetShader("Depth_PS");

	m_HizCull_CS = g_Shader->GetShader("HizCull_CS");

	m_Screen_DB = g_Resource->GetDrawBuffer<DB_Quad>();

	// DepthStencilView 설정..
	m_HizDepth_DSV = g_Resource->GetDepthStencilView<DS_HizDepth>()->Get();

	// Graphic State 설정..
	m_NoCull_RS = g_Resource->GetRasterizerState<RS_NoCull>()->Get();

	// ViewPort 설정..
	m_Hiz_VP = g_Resource->GetViewPort<VP_Hiz>()->Get();

	// Hierachical Z-Map MipMap Resource 생성..
	MipMapResourceCreate();
}

void CullingPass::OnResize(int width, int height)
{
	// DepthStencilView 재설정..
	m_HizDepth_DSV = g_Resource->GetDepthStencilView<DS_HizDepth>()->Get();

	// MipMap Create & Deferred Context Reserve..
	MipMapResourceRelease();
	MipMapResourceCreate();
}

void CullingPass::InstanceResize(size_t& renderMaxCount, size_t& unRenderMaxCount)
{
	CullingBufferCreate();
}

void CullingPass::Release()
{

}

void CullingPass::RenderOccluders()
{
	if (CullingRenderMeshList.empty() || g_GlobalData->OccluderList.empty()) return;
	
	CameraData* cam = g_GlobalData->MainCamera_Data;

	Matrix& viewproj = cam->CamViewProj;

	// Occluder Depth 그리기..
	g_Context->RSSetViewports(1, m_Hiz_VP);
	g_Context->RSSetState(m_NoCull_RS);
	g_Context->OMSetRenderTargets(1, &m_MipMap_RTV[0], m_HizDepth_DSV);
	g_Context->ClearRenderTargetView(m_MipMap_RTV[0], reinterpret_cast<const float*>(&DXColors::White));
	g_Context->ClearDepthStencilView(m_HizDepth_DSV, D3D11_CLEAR_DEPTH, 1.0, 0);

	g_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

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

		CB_DepthStaticMesh objectBuf;
		objectBuf.gWorldViewProj = viewproj;

		m_Depth_VS->ConstantBufferUpdate(&objectBuf);

		m_Depth_VS->Update();
		m_Depth_PS->Update();

		g_Context->IASetVertexBuffers(0, 1, &convertVertex, &originVertex->Stride, &originVertex->Offset);
		g_Context->IASetIndexBuffer(convertIndex, DXGI_FORMAT_R32_UINT, 0);

		g_Context->DrawIndexed(originIndex->Count, 0, 0);
	}

	// 예약해둔 Z MipMap Sampling 실행..
	g_Context->ExecuteCommandList(m_MipMap_CommandList, FALSE);
}

void CullingPass::OcclusionCullingQuery()
{
	if (CullingRenderMeshList.empty() || g_GlobalData->OccluderList.empty()) return;

	CameraData* cam = g_GlobalData->MainCamera_Data;

	Matrix& view = cam->CamView;
	Matrix& proj = cam->CamProj;
	Matrix& viewproj = cam->CamViewProj;

	// View Frustum 설정..
	m_Frustum.FrustumTransform(viewproj);

	// 실질적인 Render Count 초기화..
	m_RenderCount = 0;

	for (int i = 0; i < CullingRenderMeshList.size(); i++)
	{
		m_RenderData = CullingRenderMeshList[i];

		// 모든 오브젝트 Draw 초기화..
		m_RenderData->m_Draw = false;

		// 활성화 되있는 Object만 Culling..
		if (m_RenderData->m_ObjectData->IsActive == false) continue;

		// 해당 Collider Transform Update..
		m_Sphere = m_RenderData->m_Mesh->m_MeshSubData->BoundSphere;
		m_Sphere.Transform(m_Sphere, *m_RenderData->m_ObjectData->World);

		// Collider XYZ (Center) W (Radius) 형태로 Buffer 삽입..
		m_ColliderData.x = m_Sphere.Center.x;
		m_ColliderData.y = m_Sphere.Center.y;
		m_ColliderData.z = m_Sphere.Center.z;
		m_ColliderData.w = m_Sphere.Radius;

		m_ColliderList[m_RenderCount++] = m_ColliderData;
	}

	// Hiz Cull
	D3D11_MAPPED_SUBRESOURCE pCullingBoundsBufferMapped;
	g_Context->Map(m_Collider_Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pCullingBoundsBufferMapped);
	
	// 모든 Rendering Object Collider Data Map & UnMap
	memcpy(pCullingBoundsBufferMapped.pData, &m_ColliderList[0], sizeof(Vector4) * m_RenderCount);
	
	g_Context->Unmap(m_Collider_Buffer, 0);

	// Culling Constant Buffer Update..
	CB_HizCull cullBuf;
	cullBuf.gView = view;
	cullBuf.gProj = proj;
	cullBuf.gViewProj = viewproj;
	cullBuf.gFrustumPlanes[0] = m_Frustum.Planes[0].point;
	cullBuf.gFrustumPlanes[1] = m_Frustum.Planes[1].point;
	cullBuf.gFrustumPlanes[2] = m_Frustum.Planes[2].point;
	cullBuf.gFrustumPlanes[3] = m_Frustum.Planes[3].point;
	cullBuf.gFrustumPlanes[4] = m_Frustum.Planes[4].point;
	cullBuf.gFrustumPlanes[5] = m_Frustum.Planes[5].point;
	cullBuf.gEyePos.x = cam->CamPos.x;
	cullBuf.gEyePos.y = cam->CamPos.y;
	cullBuf.gEyePos.z = cam->CamPos.z;
	cullBuf.gViewportSize = Vector2(m_Width, m_Height);

	m_HizCull_CS->ConstantBufferUpdate(&cullBuf);

	m_HizCull_CS->SetShaderResourceView<gHizMap>(m_Hiz_SRV);
	m_HizCull_CS->SetShaderResourceView<gColliderBuffer>(m_Collider_SRV);
	m_HizCull_CS->SetUnorderedAccessView<gCullingBuffer>(m_Culling_UAV);

	m_HizCull_CS->Update();

	g_Context->Dispatch(m_RenderCount, 1, 1);
}

void CullingPass::DrawStateUpdate()
{
	if (CullingRenderMeshList.empty() || g_GlobalData->OccluderList.empty()) return;

	// GPU -> CPU Culling Result Data Copy
	g_Context->CopyResource(m_ResultCopy_Buffer, m_Culling_Buffer);

	D3D11_MAPPED_SUBRESOURCE MappedResults;
	g_Context->Map(m_ResultCopy_Buffer, 0, D3D11_MAP_READ, 0, &MappedResults);

	// 해당 Result Data를 통해 Culling 여부 판단..
	memcpy(&m_ResultList[0], (float*)MappedResults.pData, sizeof(float) * m_RenderCount);

	g_Context->Unmap(m_ResultCopy_Buffer, 0);

	// 실질적인 Render Count 초기화..
	m_RenderCount = 0;

	for (int i = 0; i < CullingRenderMeshList.size(); i++)
	{
		m_RenderData = CullingRenderMeshList[i];

		// 활성화 되있는 Object만 Culling..
		if (m_RenderData->m_ObjectData->IsActive == false) continue;

		// Culling 결과에 대한 Draw 상태 업데이트..
		m_RenderData->m_Draw = (bool)m_ResultList[m_RenderCount++];
	}
}

void CullingPass::FrustumCulling()
{
	BoundingFrustum frustum = g_GlobalData->MainCamera_Data->BoundFrustum;
	BoundingSphere boundSphere;

	for (int i = 0; i < CullingRenderMeshList.size(); i++)
	{
		m_RenderData = CullingRenderMeshList[i];

		Matrix& world = *m_RenderData->m_ObjectData->World;

		boundSphere = m_RenderData->m_Mesh->m_MeshSubData->BoundSphere;

		boundSphere.Transform(boundSphere, world);

		m_RenderData->m_Draw = boundSphere.Intersects(frustum);
	}
}

void CullingPass::PushCullingMesh(RenderData* meshData)
{
	CullingRenderMeshList.push_back(meshData);
}

void CullingPass::DeleteCullingMesh(RenderData* meshData)
{
	// 해당 Render Data와 동일한 Mesh 제거..
	for (int index = 0; index < CullingRenderMeshList.size(); index++)
	{
		if (CullingRenderMeshList[index] == meshData)
		{
			CullingRenderMeshList.erase(std::next(CullingRenderMeshList.begin(), index));
			break;
		}
	}
}

void CullingPass::MipMapResourceRelease()
{
	RELEASE_COM(m_Hiz_Buffer);
	RELEASE_COM(m_Hiz_SRV);

	for (ID3D11RenderTargetView* rtv : m_MipMap_RTV)
	{
		RELEASE_COM(rtv);
	}

	for (ID3D11ShaderResourceView* srv : m_MipMap_SRV)
	{
		RELEASE_COM(srv);
	}
}

void CullingPass::MipMapResourceCreate()
{
	D3D11_TEXTURE2D_DESC hizDesc;
	hizDesc.Width = m_Width;
	hizDesc.Height = m_Height;
	hizDesc.MipLevels = 0;
	hizDesc.ArraySize = 1;
	hizDesc.Format = DXGI_FORMAT_R32_FLOAT;
	hizDesc.SampleDesc.Count = 1;
	hizDesc.SampleDesc.Quality = 0;
	hizDesc.Usage = D3D11_USAGE_DEFAULT;
	hizDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	hizDesc.CPUAccessFlags = 0;
	hizDesc.MiscFlags = 0;

	g_Device->CreateTexture2D(&hizDesc, nullptr, &m_Hiz_Buffer);
	
	// Mip Level이 생성되었고 필드가 실제 Mip Level 수로 채워졌으므로 이제 Description을 가져옵니다.
	m_Hiz_Buffer->GetDesc(&hizDesc);

	// 실제 Mip Level 설정..
	m_MaxMipLevel = hizDesc.MipLevels;

	// Resource List Size 재설정..
	m_MipMap_RTV.resize(m_MaxMipLevel);
	m_MipMap_SRV.resize(m_MaxMipLevel);

	for (UINT miplevel = 0; miplevel < m_MaxMipLevel; miplevel++)
	{
		// 해당 Mip Level에 대응하는 SRV, RTV 생성..

		D3D11_RENDER_TARGET_VIEW_DESC downSampleHizRTVD;
		downSampleHizRTVD.Format = hizDesc.Format;
		downSampleHizRTVD.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		downSampleHizRTVD.Texture2D.MipSlice = miplevel;

		ID3D11RenderTargetView* pHizRenderView;
		g_Device->CreateRenderTargetView(m_Hiz_Buffer, &downSampleHizRTVD, &pHizRenderView);

		D3D11_SHADER_RESOURCE_VIEW_DESC lastMipSRVD;
		lastMipSRVD.Format = hizDesc.Format;
		lastMipSRVD.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		lastMipSRVD.Texture2D.MipLevels = 1;
		lastMipSRVD.Texture2D.MostDetailedMip = miplevel;

		ID3D11ShaderResourceView* pLastMipSRV;
		g_Device->CreateShaderResourceView(m_Hiz_Buffer, &lastMipSRVD, &pLastMipSRV);

		m_MipMap_RTV[miplevel] = pHizRenderView;
		m_MipMap_SRV[miplevel] = pLastMipSRV;
	}

	// Create a shader resource view that can see the entire mip chain.
	D3D11_SHADER_RESOURCE_VIEW_DESC hizSRVD;
	hizSRVD.Format = hizDesc.Format;
	hizSRVD.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	hizSRVD.Texture2D.MipLevels = hizDesc.MipLevels;
	hizSRVD.Texture2D.MostDetailedMip = 0;

	g_Device->CreateShaderResourceView(m_Hiz_Buffer, &hizSRVD, &m_Hiz_SRV);

	// 해당 MipMap Sampling CommandList Reserve..
	MipMapCommandListReserve(&hizDesc);
}

void CullingPass::MipMapCommandListReserve(const D3D11_TEXTURE2D_DESC* hizDesc)
{
	ID3D11DeviceContext* context;
	g_Device->CreateDeferredContext(0, &context);

	context->RSSetViewports(1, m_Hiz_VP);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	context->IASetVertexBuffers(0, 1, m_Screen_DB->VertexBuf->GetAddress(), &m_Screen_DB->Stride, &m_Screen_DB->Offset);
	context->IASetIndexBuffer(m_Screen_DB->IndexBuf->Get(), DXGI_FORMAT_R32_UINT, 0);

	m_Screen_VS->Update(context);

	// Draw Screen MipMap..
	// 최대 크기의 Mip Map을 제외한 모든 Mip Map Sampling 실행..
	for (UINT miplevel = 1; miplevel < hizDesc->MipLevels; miplevel++)
	{
		context->OMSetRenderTargets(1, &m_MipMap_RTV[miplevel], nullptr);

		// 한단계 위의 Mip Level을 Resource로 설정..
		m_HizMipMap_PS->SetShaderResourceView<gLastMipMap>(m_MipMap_SRV[miplevel - 1]);

		m_HizMipMap_PS->Update(context);

		context->DrawIndexed(m_Screen_DB->IndexCount, 0, 0);
	}

	context->FinishCommandList(FALSE, &m_MipMap_CommandList);

	// 예약 후 해당 DeviceContext는 해제..
	context->Release();
}

void CullingPass::CullingBufferCreate()
{
	/// 일단 해보고 버퍼 재설정은 나중에 고민하자
	m_RenderMaxCount = (UINT)CullingRenderMeshList.size();

	if (m_RenderMaxCount == 0) return;

	m_ColliderList.resize(m_RenderMaxCount);
	m_ResultList.resize(m_RenderMaxCount);

	D3D11_BUFFER_DESC colliderBufferDesc;
	ZeroMemory(&colliderBufferDesc, sizeof(colliderBufferDesc));
	colliderBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	colliderBufferDesc.ByteWidth = sizeof(Vector4) * m_RenderMaxCount;
	colliderBufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	colliderBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	colliderBufferDesc.StructureByteStride = sizeof(Vector4);
	colliderBufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

	// Collider Buffer 생성..
	g_Device->CreateBuffer(&colliderBufferDesc, nullptr, &m_Collider_Buffer);

	D3D11_SHADER_RESOURCE_VIEW_DESC colliderSRVDesc;
	ZeroMemory(&colliderSRVDesc, sizeof(colliderSRVDesc));
	colliderSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	colliderSRVDesc.BufferEx.FirstElement = 0;
	colliderSRVDesc.Format = DXGI_FORMAT_UNKNOWN;
	colliderSRVDesc.BufferEx.NumElements = colliderBufferDesc.ByteWidth / colliderBufferDesc.StructureByteStride;

	// Collider Buffer SRV 생성..
	g_Device->CreateShaderResourceView(m_Collider_Buffer, &colliderSRVDesc, &m_Collider_SRV);

	D3D11_BUFFER_DESC cullingBufferDesc;
	ZeroMemory(&cullingBufferDesc, sizeof(cullingBufferDesc));
	cullingBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	cullingBufferDesc.ByteWidth = sizeof(float) * m_RenderMaxCount;
	cullingBufferDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	cullingBufferDesc.CPUAccessFlags = 0;
	cullingBufferDesc.StructureByteStride = sizeof(float);
	cullingBufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

	// Culling Result Buffer 생성..
	g_Device->CreateBuffer(&cullingBufferDesc, nullptr, &m_Culling_Buffer);

	D3D11_UNORDERED_ACCESS_VIEW_DESC cullingUAVDesc;
	ZeroMemory(&cullingUAVDesc, sizeof(cullingUAVDesc));
	cullingUAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	cullingUAVDesc.Buffer.FirstElement = 0;
	cullingUAVDesc.Format = DXGI_FORMAT_UNKNOWN;
	cullingUAVDesc.Buffer.NumElements = cullingBufferDesc.ByteWidth / cullingBufferDesc.StructureByteStride;

	// Collider Buffer SRV 생성..
	g_Device->CreateUnorderedAccessView(m_Culling_Buffer, &cullingUAVDesc, &m_Culling_UAV);

	D3D11_BUFFER_DESC resultCopyBufferDesc;
	ZeroMemory(&resultCopyBufferDesc, sizeof(resultCopyBufferDesc));
	resultCopyBufferDesc.ByteWidth = m_RenderMaxCount * sizeof(float);
	resultCopyBufferDesc.Usage = D3D11_USAGE_STAGING;
	resultCopyBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	resultCopyBufferDesc.StructureByteStride = sizeof(float);

	// Culling Result Copy Buffer 생성..
	g_Device->CreateBuffer(&resultCopyBufferDesc, nullptr, &m_ResultCopy_Buffer);
}
