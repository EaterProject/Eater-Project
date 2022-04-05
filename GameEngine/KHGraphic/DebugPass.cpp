#include "DirectDefine.h"
#include "RenderPassBase.h"
#include "ShaderBase.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "GraphicState.h"
#include "GraphicView.h"
#include "GraphicResource.h"
#include "DrawBuffer.h"
#include "RenderTarget.h"
#include "EngineData.h"
#include "RenderData.h"
#include "DebugPass.h"

#include "MathDefine.h"
#include "FactoryManagerBase.h"
#include "ResourceManagerBase.h"
#include "RenderTargetDefine.h"
#include "ShaderManagerBase.h"
#include "ConstantBufferDefine.h"
#include "DrawBufferDefine.h"
#include "DepthStencilViewDefine.h"
#include "ShaderResourceViewDefine.h"
#include "BlendStateDefine.h"
#include "RasterizerStateDefine.h"
#include "ViewPortDefine.h"
#include "VertexDefine.h"

DebugPass::DebugPass()
{

}

DebugPass::~DebugPass()
{

}

void DebugPass::Create(int width, int height)
{
	float debug_ratio = 1.0f / 6.0f;
	//g_Factory->CreateViewPort<VP_MRT1>(0.0f, 0.0f, 0.5f, 0.5f, (float)width, (float)height);
	//g_Factory->CreateViewPort<VP_MRT2>(0.5f, 0.0f, 0.5f, 0.5f, (float)width, (float)height);
	//g_Factory->CreateViewPort<VP_MRT3>(0.0f, 0.5f, 0.5f, 0.5f, (float)width, (float)height);
	//g_Factory->CreateViewPort<VP_MRT4>(0.5f, 0.5f, 0.5f, 0.5f, (float)width, (float)height);
	g_Factory->CreateViewPort<VP_MRT1>(0.0f, debug_ratio * 5.0f, debug_ratio, debug_ratio, (float)width, (float)height);
	g_Factory->CreateViewPort<VP_MRT2>(debug_ratio * 1.0f, debug_ratio * 5.0f, debug_ratio, debug_ratio, (float)width, (float)height);
	g_Factory->CreateViewPort<VP_MRT3>(debug_ratio * 2.0f, debug_ratio * 5.0f, debug_ratio, debug_ratio, (float)width, (float)height);
	g_Factory->CreateViewPort<VP_MRT4>(debug_ratio * 3.0f, debug_ratio * 5.0f, debug_ratio, debug_ratio, (float)width, (float)height);
	g_Factory->CreateViewPort<VP_MRT5>(debug_ratio * 4.0f, debug_ratio * 5.0f, debug_ratio, debug_ratio, (float)width, (float)height);
	g_Factory->CreateViewPort<VP_MRT6>(debug_ratio * 5.0f, debug_ratio * 5.0f, debug_ratio, debug_ratio, (float)width, (float)height);
	g_Factory->CreateViewPort<VP_MRT7>(debug_ratio * 5.0f, debug_ratio * 4.0f, debug_ratio, debug_ratio, (float)width, (float)height);
	g_Factory->CreateViewPort<VP_MRT8>(debug_ratio * 5.0f, debug_ratio * 3.0f, debug_ratio, debug_ratio, (float)width, (float)height);

	g_Factory->CreateImage<DirectionalLight_Icon>("Icon/Icon_Directionlight.png");
	g_Factory->CreateImage<PointLight_Icon>("Icon/Icon_Pointlight.png");
	g_Factory->CreateImage<SpotLight_Icon>("Icon/Icon_Spotlight.png");
	g_Factory->CreateImage<Camera_Icon>("Icon/Icon_Camera.png");
}

void DebugPass::Start(int width, int height)
{
	// Debug Shader..
	m_DebugVS = g_Shader->GetShader("Debug_VS");
	m_DebugIconVS = g_Shader->GetShader("Debug_Icon_VS");
	m_DebugMRTVS = g_Shader->GetShader("Debug_MRT_VS");
	m_DebugPS = g_Shader->GetShader("Debug_PS_Option0");
	m_DebugColorPS = g_Shader->GetShader("Debug_PS_Option1");
	m_DebugIconPS = g_Shader->GetShader("Debug_Icon_PS");
	m_DebugMRTPS = g_Shader->GetShader("Debug_MRT_PS");

	// Debug Buffer..
	m_RayBuffer = g_Resource->GetDrawBuffer<DB_Line_Ray>();
	m_QuadBuffer = g_Resource->GetDrawBuffer<DB_Line_Quad>();
	m_AxisBuffer = g_Resource->GetDrawBuffer<DB_Line_Axis>();
	m_BoxBuffer = g_Resource->GetDrawBuffer<DB_Line_Box>();
	m_CircleBuffer = g_Resource->GetDrawBuffer<DB_Line_Circle>();
	m_CircleSphereBuffer = g_Resource->GetDrawBuffer<DB_Line_CircleSphere>();
	m_GridBuffer = g_Resource->GetDrawBuffer<DB_Line_Grid>();
	m_SphereBuffer = g_Resource->GetDrawBuffer<DB_Sphere>();
	m_IconBuffer = g_Resource->GetDrawBuffer<DB_Quad>();

	// Graphic State..
	m_AlphaBlendBS = g_Resource->GetBlendState<BS_AlphaBlend>()->Get();

	m_SolidRS = g_Resource->GetRasterizerState<RS_Solid>()->Get();
	m_WireRS = g_Resource->GetRasterizerState<RS_WireFrame>()->Get();
	m_NoCullRS = g_Resource->GetRasterizerState<RS_NoCull>()->Get();
	
	m_MRT1 = g_Resource->GetViewPort<VP_MRT1>()->Get();
	m_MRT2 = g_Resource->GetViewPort<VP_MRT2>()->Get();
	m_MRT3 = g_Resource->GetViewPort<VP_MRT3>()->Get();
	m_MRT4 = g_Resource->GetViewPort<VP_MRT4>()->Get();
	m_MRT5 = g_Resource->GetViewPort<VP_MRT5>()->Get();
	m_MRT6 = g_Resource->GetViewPort<VP_MRT6>()->Get();
	m_MRT7 = g_Resource->GetViewPort<VP_MRT7>()->Get();
	m_MRT8 = g_Resource->GetViewPort<VP_MRT8>()->Get();

	// Graphic View..
	m_MainRTV = g_Resource->GetMainRenderTarget()->GetRTV()->Get();
	m_DefaltDSV = g_Resource->GetDepthStencilView<DS_Defalt>()->Get();

	// Debug Icon..
	ShaderResourceView* iconSRV = nullptr;

	iconSRV = g_Resource->GetShaderResourceView<DirectionalLight_Icon>();
	if (iconSRV) m_DirectionalLightIcon = iconSRV->Get();
	iconSRV = g_Resource->GetShaderResourceView<PointLight_Icon>();
	if (iconSRV) m_PointLightIcon = iconSRV->Get();
	iconSRV = g_Resource->GetShaderResourceView<SpotLight_Icon>();
	if (iconSRV) m_SpotLightIcon = iconSRV->Get();
	iconSRV = g_Resource->GetShaderResourceView<Camera_Icon>();
	if (iconSRV) m_CamerIcon = iconSRV->Get();

	// Debug RenderTarget 설정..
	m_AlbedoRT = g_Resource->GetShaderResourceView<RT_Deffered_Albedo>()->Get();
	m_EmissiveRT = g_Resource->GetShaderResourceView<RT_Deffered_Emissive>()->Get();
	m_NormalRT = g_Resource->GetShaderResourceView<RT_Deffered_Normal>()->Get();
	m_PositionRT = g_Resource->GetShaderResourceView<RT_Deffered_Position>()->Get();
	m_NormalDepthRT = g_Resource->GetShaderResourceView<RT_Deffered_NormalDepth>()->Get();
	m_BloomRT = g_Resource->GetShaderResourceView<RT_Bloom_Brightx4_2>()->Get();

	m_ShadowMap = g_Resource->GetShaderResourceView<DS_Shadow>()->Get();
	m_SSAOMap = g_Resource->GetShaderResourceView<RT_SSAO_Blur>()->Get();
}

void DebugPass::OnResize(int width, int height)
{
	m_MainRTV = g_Resource->GetMainRenderTarget()->GetRTV()->Get();

	m_DefaltDSV = g_Resource->GetDepthStencilView<DS_Defalt>()->Get();

	// Debug RenderTarget 설정..
	m_AlbedoRT = g_Resource->GetShaderResourceView<RT_Deffered_Albedo>()->Get();
	m_EmissiveRT = g_Resource->GetShaderResourceView<RT_Deffered_Emissive>()->Get();
	m_NormalRT = g_Resource->GetShaderResourceView<RT_Deffered_Normal>()->Get();
	m_PositionRT = g_Resource->GetShaderResourceView<RT_Deffered_Position>()->Get();
	m_NormalDepthRT = g_Resource->GetShaderResourceView<RT_Deffered_NormalDepth>()->Get();
	m_BloomRT = g_Resource->GetShaderResourceView<RT_Bloom_Brightx4_2>()->Get();

	m_ShadowMap = g_Resource->GetShaderResourceView<DS_Shadow>()->Get();
	m_SSAOMap = g_Resource->GetShaderResourceView<RT_SSAO_Blur>()->Get();
}

void DebugPass::Release()
{

}

void DebugPass::BeginRender()
{
	g_Context->RSSetState(0);
	g_Context->OMSetBlendState(m_AlphaBlendBS, 0, 0xffffffff);
	g_Context->OMSetRenderTargets(1, &m_MainRTV, m_DefaltDSV);
}

void DebugPass::RenderUpdate(const RenderData* meshData)
{
	CB_DebugObject object;
	CB_DebugOption option;

	const CameraData* cam = g_GlobalData->Camera_Data;
	const MeshRenderBuffer* mesh = meshData->m_Mesh;

	Matrix world = *meshData->m_ObjectData->World;
	const Matrix& invView = cam->CamInvView;
	const Matrix& viewproj = cam->CamViewProj;

	// Transform Debug..
	object.gWorldViewProj = world * viewproj;

	m_DebugVS->ConstantBufferUpdate(&object);
	m_DebugVS->Update();

	m_DebugPS->Update();

	BufferUpdate(DEBUG_TYPE::DEBUG_AXIS);
	g_Context->DrawIndexed(m_DebugBuffer->IndexCount, 0, 0);

	switch (meshData->m_ObjectData->ObjType)
	{
	case OBJECT_TYPE::BASE:
	case OBJECT_TYPE::SKINNING:
	{
		m_DrawCount++;

		//g_Context->RSSetState(m_WireRS);
		Matrix sphereWorld = world;

		//BoundingSphere sphere;
		//mesh->m_MeshSubData->BoundSphere.Transform(sphere, world);
		BoundingBox box;
		mesh->m_MeshSubData->BoundBox.Transform(box, world);

		//object.gWorldViewProj = Matrix::CreateScale(sphere.Radius * 2.0f) * Matrix::CreateTranslation(sphere.Center) * viewproj;
		object.gWorldViewProj = Matrix::CreateScale(box.Extents) * Matrix::CreateTranslation(box.Center) * viewproj;
		
		m_DebugVS->ConstantBufferUpdate(&object);
		m_DebugVS->Update();

		option.gColor = Vector3(1.0f, 1.0f, 0.0f);

		m_DebugColorPS->ConstantBufferUpdate(&option);
		m_DebugColorPS->Update();

		//BufferUpdate(DEBUG_TYPE::DEBUG_SPHERE);
		BufferUpdate(DEBUG_TYPE::DEBUG_BOX);
		g_Context->DrawIndexed(m_DebugBuffer->IndexCount, 0, 0);

		//g_Context->RSSetState(m_SolidRS);

		//RayCastData ray;
		//ray.RayStart = sphere.Center;
		//ray.RayEnd = sphere.Center + Vector3(0.0f, sphere.Radius, 0.0f);
		//
		//option.gColor = Vector3(0.0f, 1.0f, 0.0f);
		//
		//object.gWorldViewProj = viewproj;
		//
		//m_DebugVS->ConstantBufferUpdate(&object);
		//m_DebugVS->Update();
		//
		//m_DebugColorPS->ConstantBufferUpdate(&option);
		//m_DebugColorPS->Update();
		//
		//// Ray Buffer Update
		//SetRay(ray.RayStart, ray.RayEnd);
		//
		//BufferUpdate(DEBUG_TYPE::DEBUG_RAY);
		//g_Context->DrawIndexed(m_DebugBuffer->IndexCount, 0, 0);
	}
		break;
	case OBJECT_TYPE::BONE:
	{
		object.gWorldViewProj = Matrix::CreateScale(0.05f) * world * viewproj;

		m_DebugVS->ConstantBufferUpdate(&object);
		m_DebugVS->Update();

		m_DebugPS->Update();

		BufferUpdate(DEBUG_TYPE::DEBUG_CIRCLE);
		g_Context->DrawIndexed(m_DebugBuffer->IndexCount, 0, 0);
	}
	break;
	case OBJECT_TYPE::CAMERA:
		break;
	case OBJECT_TYPE::PARTICLE_SYSTEM:
	{
		ParticleData* particles = meshData->m_ParticleData;

		Vector3 radius = particles->Area_Radius;

		world._11 = radius.x; world._22 = radius.y; world._33 = radius.z;

		object.gWorldViewProj = world * viewproj;

		m_DebugVS->ConstantBufferUpdate(&object);
		m_DebugVS->Update();

		m_DebugPS->Update();

		BufferUpdate(DEBUG_TYPE::DEBUG_BOX);
		g_Context->DrawIndexed(m_DebugBuffer->IndexCount, 0, 0);

		Matrix converseTM = Matrix::Identity;
		switch (particles->RenderType)
		{
		case PARTICLE_RENDER_OPTION::BILLBOARD:
			converseTM = invView;
			break;
		case PARTICLE_RENDER_OPTION::VERTICAL_BILLBOARD:
			converseTM = invView;
			converseTM._21 = 0; converseTM._22 = 1; converseTM._23 = 0;
			break;
		case PARTICLE_RENDER_OPTION::HORIZONTAL_BILLBOARD:
			converseTM = Matrix::CreateRotationX(3.1415926535f / 2.0f);
			break;
		case PARTICLE_RENDER_OPTION::MESH:
			break;
		default:
			break;
		}

		for (int i = 0; i < particles->Particle_Count; i++)
		{
			OneParticle* particle = particles->m_Particles[i];

			if (particle->Playing == false) continue;

			Matrix particleWorld = *particle->World * converseTM;
			particleWorld._41 = particle->World->_41;
			particleWorld._42 = particle->World->_42;
			particleWorld._43 = particle->World->_43;

			object.gWorldViewProj = particleWorld * viewproj;

			m_DebugVS->ConstantBufferUpdate(&object);
			m_DebugVS->Update();

			BufferUpdate(DEBUG_TYPE::DEBUG_AXIS);
			g_Context->DrawIndexed(m_DebugBuffer->IndexCount, 0, 0);

			BufferUpdate(DEBUG_TYPE::DEBUG_QUAD);
			g_Context->DrawIndexed(m_DebugBuffer->IndexCount, 0, 0);
		}
	}
	break;
	default:
		return;
	}
}

void DebugPass::GlobalRender()
{
	CB_DebugObject object;
	CB_DebugOption option;

	float offsetAngle = 90.0f * 3.141592f / 180.0f;
	Vector3 axis, look, right, up, pos;
	DebugData ray;

	const CameraData* cam = g_GlobalData->Camera_Data;

	const Matrix& invView = cam->CamInvView;
	const Matrix& viewproj = cam->CamViewProj;

	std::vector<DirectionalLightData*>* directionList = &g_GlobalData->DirectionLights;
	std::vector<PointLightData*>* pointList = &g_GlobalData->PointLights;
	std::vector<SpotLightData*>* spotList = &g_GlobalData->SpotLights;

	// Global Axis..
	object.gWorldViewProj = Matrix::CreateScale(1000.0f) * Matrix::CreateTranslation(0.0f, 0.1f, 0.0f) * viewproj;

	m_DebugVS->ConstantBufferUpdate(&object);
	m_DebugVS->Update();

	m_DebugPS->Update();

	BufferUpdate(DEBUG_TYPE::DEBUG_AXIS);
	g_Context->DrawIndexed(m_DebugBuffer->IndexCount, 0, 0);

	// Global Grid..
	object.gWorldViewProj = viewproj;

	m_DebugVS->ConstantBufferUpdate(&object);
	m_DebugVS->Update();

	BufferUpdate(DEBUG_TYPE::DEBUG_GRID);
	g_Context->DrawIndexed(m_DebugBuffer->IndexCount, 0, 0);

	// Global Data..
	g_Context->RSSetState(m_NoCullRS);

	for (DirectionalLightData* light : *directionList)
	{
		float RayLength = 10.0f;
		Vector3 RayOffset = Vector3(1.0f);
		Vector3 LightPos = Vector3(0.0f, 100.0f, 0.0f);

		// Icon Draw
		option.gColor = light->Diffuse;

		Matrix lightWorld = invView;
		lightWorld._41 = LightPos.x;
		lightWorld._42 = LightPos.y;
		lightWorld._43 = LightPos.z;

		object.gWorldViewProj = Matrix::CreateScale(2.0f) * lightWorld * viewproj;

		m_DebugIconVS->ConstantBufferUpdate(&object);
		m_DebugIconVS->Update();

		m_DebugIconPS->ConstantBufferUpdate(&option);
		m_DebugIconPS->SetShaderResourceView<gDiffuseMap>(m_DirectionalLightIcon);
		m_DebugIconPS->Update();

		BufferUpdate(DEBUG_TYPE::DEBUG_TEXTURE);
		g_Context->DrawIndexed(m_DebugBuffer->IndexCount, 0, 0);

		// Light Look 최대 Range가 곧 해당 Position..
		pos = LightPos + light->Direction * Vector3(RayLength);
		look = light->Direction * Vector3(RayLength);

		Matrix world = LookAt_Matrix(pos, look);
		object.gWorldViewProj = Matrix::CreateRotationX(offsetAngle) * world * viewproj;
		option.gColor = Vector3(1.0f, 1.0f, 0.0f);

		m_DebugVS->ConstantBufferUpdate(&object);
		m_DebugVS->Update();

		m_DebugColorPS->ConstantBufferUpdate(&option);
		m_DebugColorPS->Update();

		// Draw Ray..
		BufferUpdate(DEBUG_TYPE::DEBUG_CIRCLE);
		g_Context->DrawIndexed(m_DebugBuffer->IndexCount, 0, 0);

		// Look Vector 기준 Right, Up Vector 추출..
		look = light->Direction;
		right = light->Direction.Cross(Vector3(0.0f, 1.0f, 0.0f));
		up = right.Cross(look);

		right.Normalize();
		up.Normalize();

		// Light Direction Ray Center..
		ray.RayStart = LightPos;
		ray.RayEnd = ray.RayStart + light->Direction * RayLength;

		object.gWorldViewProj = viewproj;
		option.gColor = Vector3(1.0f, 0.0f, 0.0f);

		m_DebugVS->ConstantBufferUpdate(&object);
		m_DebugVS->Update();

		m_DebugColorPS->ConstantBufferUpdate(&option);
		m_DebugColorPS->Update();

		// Ray Buffer Update
		SetRay(ray.RayStart, ray.RayEnd);

		BufferUpdate(DEBUG_TYPE::DEBUG_RAY);
		g_Context->DrawIndexed(m_DebugBuffer->IndexCount, 0, 0);

		// Light Direction Ray Up..
		ray.RayStart = LightPos - up * RayOffset;
		ray.RayEnd = ray.RayStart + light->Direction * RayLength;
		
		option.gColor = Vector3(1.0f, 1.0f, 0.0f);

		m_DebugColorPS->ConstantBufferUpdate(&option);
		m_DebugColorPS->Update();

		// Ray Buffer Update
		SetRay(ray.RayStart, ray.RayEnd);

		BufferUpdate(DEBUG_TYPE::DEBUG_RAY);
		g_Context->DrawIndexed(m_DebugBuffer->IndexCount, 0, 0);

		// Light Direction Ray Down..
		ray.RayStart = LightPos + up * RayOffset;
		ray.RayEnd = ray.RayStart + light->Direction * RayLength;

		// Ray Buffer Update
		SetRay(ray.RayStart, ray.RayEnd);

		BufferUpdate(DEBUG_TYPE::DEBUG_RAY);
		g_Context->DrawIndexed(m_DebugBuffer->IndexCount, 0, 0);

		// Light Direction Ray Left..
		ray.RayStart = LightPos - right * RayOffset;
		ray.RayEnd = ray.RayStart + light->Direction * RayLength;

		// Ray Buffer Update
		SetRay(ray.RayStart, ray.RayEnd);

		BufferUpdate(DEBUG_TYPE::DEBUG_RAY);
		g_Context->DrawIndexed(m_DebugBuffer->IndexCount, 0, 0);

		// Light Direction Ray Right..
		ray.RayStart = LightPos + right * RayOffset;
		ray.RayEnd = ray.RayStart + light->Direction * RayLength;

		// Ray Buffer Update
		SetRay(ray.RayStart, ray.RayEnd);

		BufferUpdate(DEBUG_TYPE::DEBUG_RAY);
		g_Context->DrawIndexed(m_DebugBuffer->IndexCount, 0, 0);
	}
	for (PointLightData* light : *pointList)
	{
		// Icon Draw
		option.gColor = light->Diffuse;

		Matrix lightWorld = invView;
		lightWorld._41 = light->Position.x;
		lightWorld._42 = light->Position.y;
		lightWorld._43 = light->Position.z;

		object.gWorldViewProj = lightWorld * viewproj;

		m_DebugIconVS->ConstantBufferUpdate(&object);
		m_DebugIconVS->Update();

		m_DebugIconPS->ConstantBufferUpdate(&option);
		m_DebugIconPS->SetShaderResourceView<gDiffuseMap>(m_PointLightIcon);
		m_DebugIconPS->Update();

		BufferUpdate(DEBUG_TYPE::DEBUG_TEXTURE);
		g_Context->DrawIndexed(m_DebugBuffer->IndexCount, 0, 0);

		// Light Range Sphere..
		lightWorld = Matrix::CreateScale(light->Range) * Matrix::CreateTranslation(light->Position);
		
		object.gWorldViewProj = lightWorld * viewproj;
		option.gColor = Vector3(1.0f, 1.0f, 0.0f);

		m_DebugVS->ConstantBufferUpdate(&object);
		m_DebugVS->Update();

		m_DebugColorPS->ConstantBufferUpdate(&option);
		m_DebugColorPS->Update();

		BufferUpdate(DEBUG_TYPE::DEBUG_CIRCLESPHERE);
		g_Context->DrawIndexed(m_DebugBuffer->IndexCount, 0, 0);

		// Light Range X Ray..
		ray.RayStart = light->Position + Vector3(light->Range, 0.0f, 0.0f);
		ray.RayEnd = light->Position + Vector3(-light->Range, 0.0f, 0.0f);
		ray.Color = Vector3(1.0f, 0.0f, 0.0f);

		option.gColor = ray.Color;
		object.gWorldViewProj = viewproj;

		m_DebugVS->ConstantBufferUpdate(&object);
		m_DebugVS->Update();

		m_DebugColorPS->ConstantBufferUpdate(&option);
		m_DebugColorPS->Update();

		// Ray Buffer Update
		SetRay(ray.RayStart, ray.RayEnd);

		BufferUpdate(DEBUG_TYPE::DEBUG_RAY);
		g_Context->DrawIndexed(m_DebugBuffer->IndexCount, 0, 0);

		// Light Range Y Ray..
		ray.RayStart = light->Position + Vector3(0.0f, light->Range, 0.0f);
		ray.RayEnd = light->Position + Vector3(0.0f, -light->Range, 0.0f);

		// Ray Buffer Update
		SetRay(ray.RayStart, ray.RayEnd);

		BufferUpdate(DEBUG_TYPE::DEBUG_RAY);
		g_Context->DrawIndexed(m_DebugBuffer->IndexCount, 0, 0);

		// Light Range Z Ray..
		ray.RayStart = light->Position + Vector3(0.0f, 0.0f, light->Range);
		ray.RayEnd = light->Position + Vector3(0.0f, 0.0f, -light->Range);

		// Ray Buffer Update
		SetRay(ray.RayStart, ray.RayEnd);

		BufferUpdate(DEBUG_TYPE::DEBUG_RAY);
		g_Context->DrawIndexed(m_DebugBuffer->IndexCount, 0, 0);

	}
	for (SpotLightData* light : *spotList)
	{
		// Icon Draw
		option.gColor = light->Diffuse;

		Matrix lightWorld = invView;
		lightWorld._41 = light->Position.x;
		lightWorld._42 = light->Position.y;
		lightWorld._43 = light->Position.z;

		object.gWorldViewProj = lightWorld * viewproj;

		m_DebugIconVS->ConstantBufferUpdate(&object);
		m_DebugIconVS->Update();

		m_DebugIconPS->ConstantBufferUpdate(&option);
		m_DebugIconPS->SetShaderResourceView<gDiffuseMap>(m_SpotLightIcon);
		m_DebugIconPS->Update();

		BufferUpdate(DEBUG_TYPE::DEBUG_TEXTURE);
		g_Context->DrawIndexed(m_DebugBuffer->IndexCount, 0, 0);

		// Light Direction Ray..
		ray.RayStart = light->Position;
		ray.RayEnd = light->Position + (light->Direction * Vector3(light->Range));
		ray.Color = Vector3(1.0f, 0.0f, 0.0f);
		
		// Ray Buffer Update
		SetRay(ray.RayStart, ray.RayEnd);
		
		option.gColor = ray.Color;
		object.gWorldViewProj = viewproj;
		
		m_DebugVS->ConstantBufferUpdate(&object);
		m_DebugVS->Update();
		
		m_DebugColorPS->ConstantBufferUpdate(&option);
		m_DebugColorPS->Update();
		
		BufferUpdate(DEBUG_TYPE::DEBUG_RAY);
		g_Context->DrawIndexed(m_DebugBuffer->IndexCount, 0, 0);

		// Look Vector 기준 Right, Up Vector 추출..
		look = light->Direction;
		right = light->Direction.Cross(Vector3(0.0f, 1.0f, 0.0f));
		up = right.Cross(look);

		ray.RayStart = light->Position;
		ray.Color = Vector3(1.0f, 1.0f, 0.0f);

		option.gColor = ray.Color;
		object.gWorldViewProj = viewproj;

		m_DebugVS->ConstantBufferUpdate(&object);
		m_DebugVS->Update();

		m_DebugColorPS->ConstantBufferUpdate(&option);
		m_DebugColorPS->Update();

		// Look Vector를 Right Vector 기준 Angle로 이동..
		axis = XMQuaternionRotationAxis(right, light->Angle * 2.0f);

		ray.RayEnd = light->Position + (axis + light->Direction) * Vector3(light->Range);

		// Ray Buffer Update
		SetRay(ray.RayStart, ray.RayEnd);

		// Draw Ray..
		BufferUpdate(DEBUG_TYPE::DEBUG_RAY);
		g_Context->DrawIndexed(m_DebugBuffer->IndexCount, 0, 0);

		// Look Vector를 Right Vector 기준 -Angle로 이동..
		axis = XMQuaternionRotationAxis(right, -light->Angle * 2.0f);

		ray.RayEnd = light->Position + (axis + light->Direction) * Vector3(light->Range);

		// Ray Buffer Update
		SetRay(ray.RayStart, ray.RayEnd);

		// Draw Ray..
		BufferUpdate(DEBUG_TYPE::DEBUG_RAY);
		g_Context->DrawIndexed(m_DebugBuffer->IndexCount, 0, 0);

		// Look Vector를 Up Vector 기준 Angle로 이동..
		axis = XMQuaternionRotationAxis(up, light->Angle * 2.0f);
		
		ray.RayEnd = light->Position + (axis + light->Direction) * Vector3(light->Range);
		
		// Ray Buffer Update
		SetRay(ray.RayStart, ray.RayEnd);
		
		// Draw Ray..
		BufferUpdate(DEBUG_TYPE::DEBUG_RAY);
		g_Context->DrawIndexed(m_DebugBuffer->IndexCount, 0, 0);
		
		// Look Vector를 Up Vector 기준 -Angle로 이동..
		axis = XMQuaternionRotationAxis(up, -light->Angle * 2.0f);

		ray.RayEnd = light->Position + (axis + light->Direction) * Vector3(light->Range);
		
		// Ray Buffer Update
		SetRay(ray.RayStart, ray.RayEnd);
		
		// Draw Ray..
		BufferUpdate(DEBUG_TYPE::DEBUG_RAY);
		g_Context->DrawIndexed(m_DebugBuffer->IndexCount, 0, 0);

		// Light Look 최대 Range가 곧 해당 Position..
		pos = light->Position + light->Direction * Vector3(light->Range);
		look = -light->Direction * Vector3(light->Range);

		// Look, Right, Up 기준 Matrix 설정..
		Matrix world = LookAt_Matrix(pos, look);

		// Light Look 기준 Angle 회전한 지점의 거리가 곧 Scale..
		float length = (axis * Vector3(light->Range)).Length();

		Matrix scale = Matrix::CreateScale(length);

		object.gWorldViewProj = scale * Matrix::CreateRotationX(offsetAngle) * world * viewproj;

		m_DebugVS->ConstantBufferUpdate(&object);
		m_DebugVS->Update();

		// Draw Ray..
		BufferUpdate(DEBUG_TYPE::DEBUG_CIRCLE);
		g_Context->DrawIndexed(m_DebugBuffer->IndexCount, 0, 0);
	}

	// Debug Data Draw..
	std::queue<DebugData>* debugList = &g_GlobalData->Debug_Data;

	DebugData debugData;
	DEBUG_TYPE debugType;

	// Debug Mesh Data..
	while (!debugList->empty())
	{
		debugData = debugList->front();

		option.gColor = debugData.Color;


		switch (debugData.DebugType)
		{
		case DEBUG_MESH_TYPE::DEBUG_MESH_RAY:
		{
			object.gWorldViewProj = viewproj;

			// Ray Buffer Update
			SetRay(debugData.RayStart, debugData.RayEnd);

			debugType = DEBUG_TYPE::DEBUG_RAY;
		}
		break;
		case DEBUG_MESH_TYPE::DEBUG_MESH_CIRCLE:
		{
			object.gWorldViewProj = debugData.World * viewproj;

			debugType = DEBUG_TYPE::DEBUG_CIRCLE;
		}
		break;
		case DEBUG_MESH_TYPE::DEBUG_MESH_BOX:
		{
			object.gWorldViewProj = debugData.World * viewproj;

			debugType = DEBUG_TYPE::DEBUG_BOX;
		}
			break;
		case DEBUG_MESH_TYPE::DEBUG_MESH_SPHERE:
		{
			object.gWorldViewProj = debugData.World * viewproj;

			debugType = DEBUG_TYPE::DEBUG_CIRCLESPHERE;
		}
			break;
		default:
			break;
		}

		m_DebugColorPS->ConstantBufferUpdate(&option);
		m_DebugColorPS->Update(); 
		
		m_DebugVS->ConstantBufferUpdate(&object);
		m_DebugVS->Update();

		BufferUpdate(debugType);
		g_Context->DrawIndexed(m_DebugBuffer->IndexCount, 0, 0);

		debugList->pop();
	}
}

void DebugPass::MRTRender()
{
	CB_DebugOption option;

	BufferUpdate(DEBUG_TYPE::DEBUG_TEXTURE);

	// MRT 1 (Albedo RT)
	g_Context->RSSetViewports(1, m_MRT1);

	m_DebugMRTVS->Update();

	m_DebugMRTPS->SetShaderResourceView<gDiffuseMap>(m_AlbedoRT);
	m_DebugMRTPS->Update();

	g_Context->DrawIndexed(m_DebugBuffer->IndexCount, 0, 0);

	// MRT 2 (Emissive RT)
	g_Context->RSSetViewports(1, m_MRT2);

	m_DebugMRTVS->Update();

	m_DebugMRTPS->SetShaderResourceView<gDiffuseMap>(m_EmissiveRT);
	m_DebugMRTPS->Update();

	g_Context->DrawIndexed(m_DebugBuffer->IndexCount, 0, 0);

	// MRT 3 (Normal RT)
	g_Context->RSSetViewports(1, m_MRT3);

	m_DebugMRTVS->Update();

	m_DebugMRTPS->SetShaderResourceView<gDiffuseMap>(m_NormalRT);
	m_DebugMRTPS->Update();

	g_Context->DrawIndexed(m_DebugBuffer->IndexCount, 0, 0);

	// MRT 4 (Position RT)
	g_Context->RSSetViewports(1, m_MRT4);

	m_DebugMRTVS->Update();

	m_DebugMRTPS->SetShaderResourceView<gDiffuseMap>(m_PositionRT);
	m_DebugMRTPS->Update();

	g_Context->DrawIndexed(m_DebugBuffer->IndexCount, 0, 0);

	// MRT 5 (NormalDepth Map)
	g_Context->RSSetViewports(1, m_MRT5);

	m_DebugMRTVS->Update();

	m_DebugMRTPS->SetShaderResourceView<gDiffuseMap>(m_NormalDepthRT);
	m_DebugMRTPS->Update();

	g_Context->DrawIndexed(m_DebugBuffer->IndexCount, 0, 0);

	// MRT 6 (Bloom Map)
	g_Context->RSSetViewports(1, m_MRT6);
	
	m_DebugMRTVS->Update();
	
	m_DebugMRTPS->SetShaderResourceView<gDiffuseMap>(m_BloomRT);
	m_DebugMRTPS->Update();
	
	g_Context->DrawIndexed(m_DebugBuffer->IndexCount, 0, 0);

	// MRT 7 (SSAO Map)
	g_Context->RSSetViewports(1, m_MRT7);

	m_DebugMRTVS->Update();

	m_DebugMRTPS->SetShaderResourceView<gDiffuseMap>(m_SSAOMap);
	m_DebugMRTPS->Update();

	g_Context->DrawIndexed(m_DebugBuffer->IndexCount, 0, 0);

	// MRT 8 (Shadow Map)
	g_Context->RSSetViewports(1, m_MRT8);
	
	m_DebugMRTVS->Update();
	
	m_DebugMRTPS->SetShaderResourceView<gDiffuseMap>(m_ShadowMap);
	m_DebugMRTPS->Update();
	
	g_Context->DrawIndexed(m_DebugBuffer->IndexCount, 0, 0);
}

void DebugPass::BufferUpdate(DEBUG_TYPE type)
{
	CB_DebugOption option;

	switch (type)
	{
	case DEBUG_AXIS:
		m_DebugBuffer = m_AxisBuffer;
		g_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		break;
	case DEBUG_BOX:
		m_DebugBuffer = m_BoxBuffer;
		g_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		break;
	case DEBUG_CIRCLE:
		m_DebugBuffer = m_CircleBuffer;
		g_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		break;
	case DEBUG_CIRCLESPHERE:
		m_DebugBuffer = m_CircleSphereBuffer;
		g_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		break;
	case DEBUG_GRID:
		m_DebugBuffer = m_GridBuffer;
		g_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		break;
	case DEBUG_QUAD:
		m_DebugBuffer = m_QuadBuffer;
		g_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		break;
	case DEBUG_RAY:
		m_DebugBuffer = m_RayBuffer;
		g_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		break;
	case DEBUG_SPHERE:
		m_DebugBuffer = m_SphereBuffer;
		g_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		break;
	case DEBUG_TEXTURE:
		m_DebugBuffer = m_IconBuffer;
		g_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		break;
	default:
		break;
	}

	g_Context->IASetVertexBuffers(0, 1, m_DebugBuffer->VertexBuf->GetAddress(), &m_DebugBuffer->Stride, &m_DebugBuffer->Offset);
	g_Context->IASetIndexBuffer(m_DebugBuffer->IndexBuf->Get(), DXGI_FORMAT_R32_UINT, 0);
}

void DebugPass::CountReset()
{
	m_DrawCount = 0;
}

void DebugPass::SetRay(const Vector3& start, const Vector3& end)
{
	static VertexInput::PosColorVertex vertices[2];
	vertices[0].Pos = start;
	vertices[1].Pos = end;

	ID3D11Buffer* buffer = m_RayBuffer->VertexBuf->Get();

	// Mapping SubResource Data..
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	// GPU Access Lock Buffer Data..
	g_Context->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	// Copy Resource Data..
	memcpy(mappedResource.pData, vertices, sizeof(vertices));

	// GPU Access UnLock Buffer Data..
	g_Context->Unmap(buffer, 0);

}

Matrix DebugPass::LookAt_Matrix(Vector3 pos, Vector3 look)
{
	Vector3 right, up;

	// 해당 위치 기준 Look, Right, Up Vector 추출..
	right = look.Cross(Vector3(0.0f, 1.0f, 0.0f));
	up = right.Cross(look);

	// 해당 Vector Normalize..
	right.Normalize();
	up.Normalize();
	look.Normalize();

	// Look, Right, Up 기준 Matrix 설정..
	Matrix world;
	world.m[0][0] = right.x;	world.m[0][1] = right.y;	world.m[0][2] = right.z;
	world.m[1][0] = up.x;		world.m[1][1] = up.y;		world.m[1][2] = up.z;
	world.m[2][0] = look.x;		world.m[2][1] = look.y;		world.m[2][2] = look.z;
	world.m[3][0] = pos.x;		world.m[3][1] = pos.y;		world.m[3][2] = pos.z;

	return world;
}