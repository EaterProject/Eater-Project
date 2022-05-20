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
#include "EngineData.h"
#include "RenderData.h"
#include "VertexDefine.h"
#include "OutLine_Pass.h"

#include "MathDefine.h"
#include "FactoryManagerBase.h"
#include "ResourceManagerBase.h"
#include "ShaderManagerBase.h"
#include "ConstantBufferDefine.h"
#include "ShaderResourceViewDefine.h"
#include "UnorderedAccessViewDefine.h"
#include "ViewPortDefine.h"
#include "RenderTargetDefine.h"
#include "DepthStencilViewDefine.h"
#include "DepthStencilStateDefine.h"
#include "RasterizerStateDefine.h"
#include "BlendStateDefine.h"
#include "DrawBufferDefine.h"

OutLine_Pass::OutLine_Pass()
{

}

OutLine_Pass::~OutLine_Pass()
{

}

void OutLine_Pass::Create(int width, int height)
{
	// OutLine RenderTarget 생성..
	D3D11_TEXTURE2D_DESC texDescRT;
	ZeroMemory(&texDescRT, sizeof(texDescRT));
	texDescRT.Width = width;
	texDescRT.Height = height;
	texDescRT.MipLevels = 1;
	texDescRT.ArraySize = 1;
	texDescRT.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDescRT.SampleDesc.Count = 1;
	texDescRT.SampleDesc.Quality = 0;
	texDescRT.Usage = D3D11_USAGE_DEFAULT;
	texDescRT.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	texDescRT.CPUAccessFlags = 0;
	texDescRT.MiscFlags = 0;

	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	ZeroMemory(&rtvDesc, sizeof(rtvDesc));
	rtvDesc.Format = texDescRT.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = texDescRT.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	uavDesc.Format = texDescRT.Format;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	uavDesc.Texture2D.MipSlice = 0;

	// OutLine DepthStencil 생성..
	D3D11_TEXTURE2D_DESC texDescDS;
	ZeroMemory(&texDescDS, sizeof(texDescDS));
	texDescDS.Width = width;
	texDescDS.Height = height;
	texDescDS.MipLevels = 1;
	texDescDS.ArraySize = 1;
	texDescDS.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	texDescDS.SampleDesc.Count = 1;
	texDescDS.SampleDesc.Quality = 0;
	texDescDS.Usage = D3D11_USAGE_DEFAULT;
	texDescDS.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	texDescDS.CPUAccessFlags = 0;
	texDescDS.MiscFlags = 0;

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Flags = 0;
	descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;

	// OutLine RenderTarget 생성..
	g_Factory->CreateRenderTexture<RT_Origin>(&texDescRT, nullptr, &rtvDesc, &srvDesc, &uavDesc);
	g_Factory->CreateRenderTexture<RT_OutLine>(&texDescRT, nullptr, &rtvDesc, &srvDesc, &uavDesc);

	// OutLine DepthStencilView 생성..
	g_Factory->CreateDepthStencil<DS_OutLine>(&texDescDS, nullptr, &descDSV, nullptr);
}

void OutLine_Pass::Start(int width, int height)
{
	m_MeshOrigin_VS = g_Shader->GetShader("Origin_StaticMesh_VS");
	m_SkinMeshOrigin_VS = g_Shader->GetShader("Origin_SkinMesh_VS");
	m_MeshOutLine_VS = g_Shader->GetShader("OutLine_StaticMesh_VS");
	m_SkinMeshOutLine_VS = g_Shader->GetShader("OutLine_SkinMesh_VS");
	m_OutLine_PS = g_Shader->GetShader("OutLine_PS");

	// DepthStencilView 설정..
	m_OutLine_DSV = g_Resource->GetDepthStencilView<DS_OutLine>()->Get();

	// Graphic State 설정..
	m_Mask_DSS = g_Resource->GetDepthStencilState<DSS_Mask>()->Get();
	m_OutLine_DSS = g_Resource->GetDepthStencilState<DSS_OutLine>()->Get();
	m_Solid_RS = g_Resource->GetRasterizerState<RS_Solid>()->Get();

	// ViewPort 설정..
	m_Screen_VP = g_Resource->GetViewPort<VP_FullScreen>()->Get();

	// RenderTarget 설정..
	m_Origin_RT = g_Resource->GetRenderTexture<RT_Origin>();
	m_OutLine_RT = g_Resource->GetRenderTexture<RT_OutLine>();

	m_Origin_RTV = m_Origin_RT->GetRTV()->Get();
	m_OutLine_RTV = m_OutLine_RT->GetRTV()->Get();

	/// Test
	m_BlurHorizon_CS = g_Shader->GetShader("Blur_Horizon_CS");
	m_BlurVertical_CS = g_Shader->GetShader("Blur_Vertical_CS");

	m_Width = width;
	m_Height = height;
	m_NumGroupsX = (UINT)ceilf(m_Width / 256.0f);
	m_NumGroupsY = (UINT)ceilf(m_Height / 256.0f);
}

void OutLine_Pass::OnResize(int width, int height)
{
	// DepthStencilView 재설정..
	m_OutLine_DSV = g_Resource->GetDepthStencilView<DS_OutLine>()->Get();

	// RenderTarget 재설정..
	m_Origin_RTV = m_Origin_RT->GetRTV()->Get();
	m_OutLine_RTV = m_OutLine_RT->GetRTV()->Get();

	m_Width = width;
	m_Height = height;
	m_NumGroupsX = (UINT)ceilf(m_Width / 256.0f);
	m_NumGroupsY = (UINT)ceilf(m_Height / 256.0f);
}

void OutLine_Pass::Release()
{

}

void OutLine_Pass::RenderUpdate()
{
	// RenderTarget 초기화.. 
	g_Context->ClearRenderTargetView(m_OutLine_RTV, reinterpret_cast<const float*>(&DXColors::NonBlack));
	g_Context->ClearRenderTargetView(m_Origin_RTV, reinterpret_cast<const float*>(&DXColors::NonBlack));

	if (g_Picking == nullptr) return;

	// Mask Render Setting..
	BeginMask();

	/// Test Code
	const CameraData* cam = g_GlobalData->MainCamera_Data;
	const ObjectData* obj = g_Picking->m_ObjectData;
	const MeshRenderBuffer* mesh = g_Picking->m_Mesh;

	const Matrix& worldviewproj = obj->World * cam->CamViewProj;
	const Matrix& invWorld = obj->InvWorld;

	CB_OutLineOption outlineOptionBuf;
	outlineOptionBuf.gOutLineColor = Vector3(1.0f, 1.0f, 1.0f);

	CB_OutLine outlineBuf;
	outlineBuf.gSize = 0.02f;

	switch (obj->ObjType)
	{
	case OBJECT_TYPE::BASE:
	{
		CB_OutLineStaticMesh objectBuf;
		objectBuf.gWorldViewProj = worldviewproj;

		m_MeshOrigin_VS->ConstantBufferUpdate(&objectBuf);

		m_MeshOrigin_VS->Update();

		// Draw..
		g_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		g_Context->IASetVertexBuffers(0, 1, &mesh->m_VertexBuf, &mesh->m_Stride, &mesh->m_Offset);
		g_Context->IASetIndexBuffer(mesh->m_IndexBuf, DXGI_FORMAT_R32_UINT, 0);

		g_Context->DrawIndexed(mesh->m_IndexCount, 0, 0);
	}
		break;
	case OBJECT_TYPE::SKINNING:
	{
		AnimationData* animation = g_Picking->m_AnimationData;
		
		CB_OutLineSkinMesh objectBuf;
		objectBuf.gWorldViewProj = worldviewproj;
		objectBuf.gPrevAnimationIndex = animation->PrevAnimationIndex + animation->PrevFrameIndex;
		objectBuf.gNextAnimationIndex = animation->NextAnimationIndex + animation->NextFrameIndex;
		objectBuf.gFrameTime = animation->FrameTime;
		
		m_SkinMeshOrigin_VS->ConstantBufferUpdate(&objectBuf);
		m_SkinMeshOrigin_VS->SetShaderResourceView<gAnimationBuffer>(g_Picking->m_Animation->m_AnimationBuf);
		
		m_SkinMeshOrigin_VS->Update();

		// Draw..
		g_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		g_Context->IASetVertexBuffers(0, 1, &mesh->m_VertexBuf, &mesh->m_Stride, &mesh->m_Offset);
		g_Context->IASetIndexBuffer(mesh->m_IndexBuf, DXGI_FORMAT_R32_UINT, 0);

		g_Context->DrawIndexed(mesh->m_IndexCount, 0, 0);
	}
		break;
	default:
		break;
	}

	// OutLine Render Setting..
	BeginOutLine();

	switch (obj->ObjType)
	{
	case OBJECT_TYPE::BASE:
	{
		CB_OutLineStaticMesh objectBuf;
		objectBuf.gWorldViewProj = worldviewproj;
		objectBuf.gWorld = invWorld;

		m_MeshOutLine_VS->ConstantBufferUpdate(&objectBuf);
		m_MeshOutLine_VS->ConstantBufferUpdate(&outlineBuf);

		m_MeshOutLine_VS->Update();

		m_OutLine_PS->ConstantBufferUpdate(&outlineOptionBuf);

		m_OutLine_PS->Update();

		// Draw..
		g_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		g_Context->IASetVertexBuffers(0, 1, &mesh->m_VertexBuf, &mesh->m_Stride, &mesh->m_Offset);
		g_Context->IASetIndexBuffer(mesh->m_IndexBuf, DXGI_FORMAT_R32_UINT, 0);

		g_Context->DrawIndexed(mesh->m_IndexCount, 0, 0);
	}
	break;
	case OBJECT_TYPE::SKINNING:
	{
		AnimationData* animation = g_Picking->m_AnimationData;

		CB_OutLineSkinMesh objectBuf;
		objectBuf.gWorldViewProj = worldviewproj;
		objectBuf.gWorld = invWorld;
		objectBuf.gPrevAnimationIndex = animation->PrevAnimationIndex + animation->PrevFrameIndex;
		objectBuf.gNextAnimationIndex = animation->NextAnimationIndex + animation->NextFrameIndex;
		objectBuf.gFrameTime = animation->FrameTime;

		m_SkinMeshOutLine_VS->ConstantBufferUpdate(&objectBuf);
		m_SkinMeshOutLine_VS->ConstantBufferUpdate(&outlineBuf);
		m_SkinMeshOutLine_VS->SetShaderResourceView<gAnimationBuffer>(g_Picking->m_Animation->m_AnimationBuf);

		m_SkinMeshOutLine_VS->Update();

		m_OutLine_PS->ConstantBufferUpdate(&outlineOptionBuf);

		m_OutLine_PS->Update();

		// Draw..
		g_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		g_Context->IASetVertexBuffers(0, 1, &mesh->m_VertexBuf, &mesh->m_Stride, &mesh->m_Offset);
		g_Context->IASetIndexBuffer(mesh->m_IndexBuf, DXGI_FORMAT_R32_UINT, 0);

		g_Context->DrawIndexed(mesh->m_IndexCount, 0, 0);
	}
	break;
	default:
		break;
	}

	//BlurOutLine();
}

void OutLine_Pass::BeginMask()
{
	g_Context->OMSetRenderTargets(0, nullptr, m_OutLine_DSV);

	g_Context->ClearDepthStencilView(m_OutLine_DSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	g_Context->OMSetDepthStencilState(m_Mask_DSS, 0);

	g_Context->RSSetViewports(1, m_Screen_VP);
	g_Context->RSSetState(m_Solid_RS);
}

void OutLine_Pass::BeginOutLine()
{
	// OutLine RenderTarget 설정..
	g_Context->OMSetRenderTargets(1, &m_OutLine_RTV, m_OutLine_DSV);
	g_Context->OMSetDepthStencilState(m_OutLine_DSS, 0);
}

void OutLine_Pass::BlurOutLine()
{
	g_Context->OMSetRenderTargets(0, nullptr, nullptr);

	// HORIZONTAL blur pass.
	m_BlurHorizon_CS->SetShaderResourceView<gInputMap>(m_OutLine_RT->GetSRV()->Get());
	m_BlurHorizon_CS->SetUnorderedAccessView<gOutputUAV>(m_Origin_RT->GetUAV()->Get());

	m_BlurHorizon_CS->Update();

	// How many groups do we need to dispatch to cover a row of pixels, where each
	// group covers 256 pixels (the 256 is defined in the CS).
	g_Context->Dispatch(m_NumGroupsX, m_Height, 1);

	ComputeShader::UnBindShaderResourceView(0, 1);
	ComputeShader::UnBindUnorderedAccessView(0, 1);

	// VERTICAL blur pass.
	m_BlurVertical_CS->SetShaderResourceView<gInputMap>(m_Origin_RT->GetSRV()->Get());
	m_BlurVertical_CS->SetUnorderedAccessView<gOutputUAV>(m_OutLine_RT->GetUAV()->Get());

	m_BlurVertical_CS->Update();

	// How many groups do we need to dispatch to cover a column of pixels, where each
	// group covers 256 pixels  (the 256 is defined in the CS).
	g_Context->Dispatch(m_Width, m_NumGroupsY, 1);

	ComputeShader::UnBindShaderResourceView(0, 1);
	ComputeShader::UnBindUnorderedAccessView(0, 1);

	ComputeShader::UnBindComputeShader();
}
