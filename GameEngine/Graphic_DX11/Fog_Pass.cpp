#include "DirectDefine.h"
#include "RenderPassBase.h"
#include "ShaderBase.h"
#include "GraphicView.h"
#include "GraphicResource.h"
#include "EngineData.h"
#include "Fog_Pass.h"

#include "FactoryManagerBase.h"
#include "ResourceManagerBase.h"
#include "ShaderManagerBase.h"
#include "ConstantBufferDefine.h"
#include "ShaderResourceViewDefine.h"

Fog_Pass::Fog_Pass()
	:m_FogTimer(0.0f)
{

}

Fog_Pass::~Fog_Pass()
{

}

void Fog_Pass::Start(int width, int height)
{
	// Fog Shader List Up..
	SetShaderList();

	// Set Fog Shader Resoruce View..
	SetShaderResourceView();

	// Set Fog Shader Constant Buffer..
	SetShaderConstantBuffer();
}

void Fog_Pass::Release()
{

}

void Fog_Pass::ApplyOption()
{
	// Set Fog Shader Constant Buffer..
	SetShaderConstantBuffer();
}

void Fog_Pass::PreUpdate()
{
	m_FogTimer += g_GlobalData->Time * g_RenderOption->FOG_MoveSpeed;

	if (m_FogTimer > 100.0f)
	{
		m_FogTimer = 0.0f;
	}

	CB_FogData fogDataBuf;
	fogDataBuf.gEyePosW = g_GlobalData->MainCamera_Data->CamPos;
	fogDataBuf.gTime = m_FogTimer;

	for (ShaderBase* shader : m_OptionShaderList)
	{
		shader->ConstantBufferUpdate(&fogDataBuf);
	}
}

void Fog_Pass::SetShaderList()
{
	PushShader("Light_PBR_PS_Option3");
	PushShader("Light_PBR_PS_Option5");
	PushShader("Light_PBR_PS_Option6");
	PushShader("Light_PBR_PS_Option7");

	PushShader("Light_IBL_PS_Option3");
	PushShader("Light_IBL_PS_Option5");
	PushShader("Light_IBL_PS_Option6");
	PushShader("Light_IBL_PS_Option7");

	PushShader("SkyBox_PS_Option1");
	PushShader("SkyBox_PS_Option3");

	PushShader("OIT_Particle_PS_Option1");

	PushShader("OIT_Mesh_PS_Option3");
	PushShader("OIT_Mesh_PS_Option5");
	PushShader("OIT_Mesh_PS_Option6");
	PushShader("OIT_Mesh_PS_Option7");
}

void Fog_Pass::SetShaderResourceView()
{
	ID3D11ShaderResourceView* noiseMap = g_Resource->GetShaderResourceView<gNoiseVolume>()->Get();

	for (ShaderBase* shader : m_OptionShaderList)
	{
		shader->SetShaderResourceView<gNoiseVolume>(noiseMap);
	}
}

void Fog_Pass::SetShaderConstantBuffer()
{
	CB_FogOption fogOptionBuf;
	fogOptionBuf.gFogColor = g_RenderOption->FOG_Color;
	fogOptionBuf.gFogStartDistance = g_RenderOption->FOG_StartDistance;
	fogOptionBuf.gFogDistanceOffset = g_RenderOption->FOG_DistanceOffset;
	fogOptionBuf.gFogDistanceValue = g_RenderOption->FOG_DistanceValue;
	fogOptionBuf.gFogHeightOffset = g_RenderOption->FOG_HeightOffset;
	fogOptionBuf.gFogHeightValue = g_RenderOption->FOG_HeightValue;

	for (ShaderBase* shader : m_OptionShaderList)
	{
		shader->ConstantBufferUpdate(&fogOptionBuf);
	}
}
