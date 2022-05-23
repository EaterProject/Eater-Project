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
#include "VertexDefine.h"
#include "EngineData.h"
#include "Fog_Pass.h"

#include "MathDefine.h"
#include "FactoryManagerBase.h"
#include "ResourceManagerBase.h"
#include "ShaderManagerBase.h"
#include "ConstantBufferDefine.h"
#include "ShaderResourceViewDefine.h"
#include "RenderTargetDefine.h"
#include "ViewPortDefine.h"
#include "DrawBufferDefine.h"
#include "RenderData.h"

Fog_Pass::Fog_Pass()
{

}

Fog_Pass::~Fog_Pass()
{

}

void Fog_Pass::Create(int width, int height)
{
	// Noise Texture »ý¼º..
	g_Factory->CreateImage<gNoiseVolume>("Noise/NoiseVolume.dds");
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

void Fog_Pass::OnResize(int width, int height)
{

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
	g_RenderSceneData->Fog_Timer += g_GlobalData->Time * g_RenderOption->FOG_MoveSpeed;

	if (g_RenderSceneData->Fog_Timer > 100.0f)
	{
		g_RenderSceneData->Fog_Timer = 0.0f;
	}

	CB_FogData fogDataBuf;
	fogDataBuf.gEyePosW = g_GlobalData->MainCamera_Data->CamPos;
	fogDataBuf.gTime = g_RenderSceneData->Fog_Timer;

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
