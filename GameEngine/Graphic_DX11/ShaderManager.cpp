#include "DirectDefine.h"
#include "D3D11GraphicBase.h"
#include "ShaderBase.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ComputeShader.h"
#include "ShaderManagerBase.h"
#include "ShaderManager.h"
#include "ShaderResourceHashTable.h"
#include "ConstantBufferDefine.h"

using namespace Microsoft::WRL;

ShaderManager::ShaderManager(ID3D11Graphic* graphic)
{
	// Shader Global Initialize..
	IShader::Initialize(graphic->GetDevice(), graphic->GetContext());
}

ShaderManager::~ShaderManager()
{

}

void ShaderManager::Create()
{
	// Shader File Route Setting..
	IShader::SetShaderRoute("../Assets/Shader/");

	// Global Shader Create..
	CreateShader();
}

void ShaderManager::Start()
{

}

void ShaderManager::Release()
{
	IShader::Reset();

	for (std::pair<std::string, ShaderBase*> shader : m_ShaderList)
	{
		RELEASE_COM(shader.second);
	}

	m_ShaderList.clear();
}

void ShaderManager::CreateShader()
{
	// Screen Shader Macro
	D3D_SHADER_MACRO screen_macro[] = { {"SSAO"}, {NULL, NULL} };
	
	// Screen Shader
	LoadShader(SHADER_TYPE::VERTEX_SHADER, "Screen_VS.hlsl", "Screen_VS", "Screen_VS");
	LoadShader(SHADER_TYPE::VERTEX_SHADER, "Screen_VS.hlsl", "Screen_VS", "SSAO_VS", screen_macro);

	// Light Shader Macro
	D3D_SHADER_MACRO light_macro1[] = { {"SHADOW"}, {NULL, NULL} };
	D3D_SHADER_MACRO light_macro2[] = { {"SSAO"}, {NULL, NULL} };
	D3D_SHADER_MACRO light_macro3[] = { {"FOG"}, {NULL, NULL} };
	D3D_SHADER_MACRO light_macro4[] = { {"SHADOW"}, {"SSAO"}, {NULL, NULL} };
	D3D_SHADER_MACRO light_macro5[] = { {"SHADOW"}, {"FOG"}, {NULL, NULL} };
	D3D_SHADER_MACRO light_macro6[] = { {"SSAO"}, {"FOG"}, {NULL, NULL} };
	D3D_SHADER_MACRO light_macro7[] = { {"SHADOW"}, {"SSAO"}, {"FOG"}, {NULL, NULL} };

	// Light Shader
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "Light_PBR_PS.hlsl", "Light_PBR_PS", "Light_PBR_PS_Option0");					// None
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "Light_PBR_PS.hlsl", "Light_PBR_PS", "Light_PBR_PS_Option1", light_macro1);	// Shadow
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "Light_PBR_PS.hlsl", "Light_PBR_PS", "Light_PBR_PS_Option2", light_macro2);	// SSAO
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "Light_PBR_PS.hlsl", "Light_PBR_PS", "Light_PBR_PS_Option3", light_macro3);	// Fog
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "Light_PBR_PS.hlsl", "Light_PBR_PS", "Light_PBR_PS_Option4", light_macro4);	// Shadow + SSAO
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "Light_PBR_PS.hlsl", "Light_PBR_PS", "Light_PBR_PS_Option5", light_macro5);	// Shadow + Fog
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "Light_PBR_PS.hlsl", "Light_PBR_PS", "Light_PBR_PS_Option6", light_macro6);	// SSAO + Fog
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "Light_PBR_PS.hlsl", "Light_PBR_PS", "Light_PBR_PS_Option7", light_macro7);	// Shadow + SSAO + Fog

	LoadShader(SHADER_TYPE::PIXEL_SHADER, "Light_IBL_PS.hlsl", "Light_IBL_PS", "Light_IBL_PS_Option0");					// None
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "Light_IBL_PS.hlsl", "Light_IBL_PS", "Light_IBL_PS_Option1", light_macro1);	// Shadow
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "Light_IBL_PS.hlsl", "Light_IBL_PS", "Light_IBL_PS_Option2", light_macro2);	// SSAO
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "Light_IBL_PS.hlsl", "Light_IBL_PS", "Light_IBL_PS_Option3", light_macro3);	// Fog
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "Light_IBL_PS.hlsl", "Light_IBL_PS", "Light_IBL_PS_Option4", light_macro4);	// Shadow + SSAO
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "Light_IBL_PS.hlsl", "Light_IBL_PS", "Light_IBL_PS_Option5", light_macro5);	// Shadow + Fog
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "Light_IBL_PS.hlsl", "Light_IBL_PS", "Light_IBL_PS_Option6", light_macro6);	// SSAO + Fog
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "Light_IBL_PS.hlsl", "Light_IBL_PS", "Light_IBL_PS_Option7", light_macro7);	// Shadow + SSAO + Fog

	// Deferred Shader Macro
	D3D_SHADER_MACRO deferred_macro1[] = { {"TERRAIN_MESH"}, {NULL, NULL} };

	// Deferred Shader
	LoadShader(SHADER_TYPE::VERTEX_SHADER, "StaticMesh_VS.hlsl", "StaticMesh_VS", "StaticMesh_VS");
	LoadShader(SHADER_TYPE::VERTEX_SHADER, "StaticMesh_Instance_VS.hlsl", "StaticMesh_Instance_VS", "StaticMesh_Instance_VS");
	LoadShader(SHADER_TYPE::VERTEX_SHADER, "SkinMesh_VS.hlsl", "SkinMesh_VS", "SkinMesh_VS");
	LoadShader(SHADER_TYPE::VERTEX_SHADER, "SkinMesh_Instance_VS.hlsl", "SkinMesh_Instance_VS", "SkinMesh_Instance_VS");
	LoadShader(SHADER_TYPE::VERTEX_SHADER, "TerrainMesh_VS.hlsl", "TerrainMesh_VS", "TerrainMesh_VS");
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "Deferred_PBR_PS.hlsl", "Deferred_PBR_PS", "Deferred_PBR_PS");
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "Deferred_PBR_PS.hlsl", "Deferred_PBR_PS", "Terrain_PBR_PS", deferred_macro1);

	// OIT Shader
	D3D_SHADER_MACRO alpha_macro1[] = { {"SHADOW"}, {NULL, NULL} };
	D3D_SHADER_MACRO alpha_macro2[] = { {"IBL"}, {NULL, NULL} };
	D3D_SHADER_MACRO alpha_macro3[] = { {"FOG"}, {NULL, NULL} };
	D3D_SHADER_MACRO alpha_macro4[] = { {"SHADOW"}, {"IBL"}, {NULL, NULL} };
	D3D_SHADER_MACRO alpha_macro5[] = { {"SHADOW"}, {"FOG"}, {NULL, NULL} };
	D3D_SHADER_MACRO alpha_macro6[] = { {"IBL"}, {"FOG"}, {NULL, NULL} };
	D3D_SHADER_MACRO alpha_macro7[] = { {"SHADOW"}, {"IBL"}, {"FOG"}, {NULL, NULL} };
	
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "OIT_Particle_PS.hlsl", "OIT_Particle_PS", "OIT_Particle_PS_Option0");				// None
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "OIT_Particle_PS.hlsl", "OIT_Particle_PS", "OIT_Particle_PS_Option1", alpha_macro3);	// Fog

	LoadShader(SHADER_TYPE::PIXEL_SHADER, "OIT_Mesh_PS.hlsl", "OIT_Mesh_PS", "OIT_Mesh_PS_Option0");							// None
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "OIT_Mesh_PS.hlsl", "OIT_Mesh_PS", "OIT_Mesh_PS_Option1", alpha_macro1);				// Shadow
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "OIT_Mesh_PS.hlsl", "OIT_Mesh_PS", "OIT_Mesh_PS_Option2", alpha_macro2);				// IBL
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "OIT_Mesh_PS.hlsl", "OIT_Mesh_PS", "OIT_Mesh_PS_Option3", alpha_macro3);				// Fog
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "OIT_Mesh_PS.hlsl", "OIT_Mesh_PS", "OIT_Mesh_PS_Option4", alpha_macro4);				// Shadow + IBL
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "OIT_Mesh_PS.hlsl", "OIT_Mesh_PS", "OIT_Mesh_PS_Option5", alpha_macro5);				// Shadow + Fog
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "OIT_Mesh_PS.hlsl", "OIT_Mesh_PS", "OIT_Mesh_PS_Option6", alpha_macro6);				// IBL + Fog
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "OIT_Mesh_PS.hlsl", "OIT_Mesh_PS", "OIT_Mesh_PS_Option7", alpha_macro7);				// Shadow + IBL + Fog

	LoadShader(SHADER_TYPE::PIXEL_SHADER, "OIT_Blend_PS.hlsl", "OIT_PS", "OIT_PS");

	// Shadow Shader
	LoadShader(SHADER_TYPE::VERTEX_SHADER, "Shadow_StaticMesh_VS.hlsl", "Shadow_StaticMesh_VS", "Shadow_StaticMesh_VS");
	LoadShader(SHADER_TYPE::VERTEX_SHADER, "Shadow_StaticMesh_Instance_VS.hlsl", "Shadow_StaticMesh_Instance_VS", "Shadow_StaticMesh_Instance_VS");
	LoadShader(SHADER_TYPE::VERTEX_SHADER, "Shadow_SkinMesh_VS.hlsl", "Shadow_SkinMesh_VS", "Shadow_SkinMesh_VS");
	LoadShader(SHADER_TYPE::VERTEX_SHADER, "Shadow_SkinMesh_Instance_VS.hlsl", "Shadow_SkinMesh_Instance_VS", "Shadow_SkinMesh_Instance_VS");

	// SSAO Shader
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "SSAO_PS.hlsl", "SSAO_PS", "SSAO_PS");
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "SSAO_Blur_PS.hlsl", "SSAOBlur_PS", "SSAOBlur_PS");

	// Particle Shader
	LoadShader(SHADER_TYPE::VERTEX_SHADER, "Particle_Instance_VS.hlsl", "Particle_Instance_VS", "Particle_Instance_VS");

	// FXAA Shader
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "FXAA_PS.hlsl", "FXAA_PS", "FXAA_PS");

	// SkyBox Shader
	D3D_SHADER_MACRO skyBox_convert_macro1[] = { {"HDRI"}, {NULL, NULL} };
	D3D_SHADER_MACRO skyBox_macro1[] = { {"FOG"}, {NULL, NULL} };
	D3D_SHADER_MACRO skyBox_macro2[] = { {"HDR"}, {NULL, NULL} };
	D3D_SHADER_MACRO skyBox_macro3[] = { {"FOG"}, {"HDR"}, {NULL, NULL} };

	LoadShader(SHADER_TYPE::VERTEX_SHADER, "SkyBox_VS.hlsl", "SkyBox_VS", "SkyBox_VS");
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "SkyBox_Convert_PS.hlsl", "SkyBox_Convert_PS", "SkyBox_Convert_PS");								// None
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "SkyBox_Convert_PS.hlsl", "SkyBox_Convert_PS", "SkyBox_Convert_HDRI_PS", skyBox_convert_macro1);	// HDRI
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "SkyBox_PS.hlsl", "SkyBox_PS", "SkyBox_PS_Option0");					// None
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "SkyBox_PS.hlsl", "SkyBox_PS", "SkyBox_PS_Option1", skyBox_macro1);	// Fog
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "SkyBox_PS.hlsl", "SkyBox_PS", "SkyBox_PS_Option2", skyBox_macro2);	// HDR
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "SkyBox_PS.hlsl", "SkyBox_PS", "SkyBox_PS_Option3", skyBox_macro3);	// Fog + HDR

	// RectTransform Shader
	LoadShader(SHADER_TYPE::VERTEX_SHADER, "UI_VS.hlsl", "UI_VS", "UI_VS");
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "UI_PS.hlsl", "UI_PS", "UI_PS");

	// IBL Shader
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "IBL_Convolution_PS.hlsl", "IBL_Convolution_PS", "IBL_Convolution_PS");
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "IBL_Convolution_PS.hlsl", "IBL_Convolution_PS", "IBL_Convolution_HDRI_PS", skyBox_convert_macro1);
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "IBL_PrefilterMap_PS.hlsl", "IBL_PrefilterMap_PS", "IBL_PrefilterMap_PS");
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "IBL_PrefilterMap_PS.hlsl", "IBL_PrefilterMap_PS", "IBL_PrefilterMap_HDRI_PS", skyBox_convert_macro1);
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "IBL_IntegrateBRDF_PS.hlsl", "IBL_IntegrateBRDF_PS", "IBL_IntegrateBRDF_PS");

	// Bloom Shader
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "BloomSampling_PS.hlsl", "DownSampling_Bright_PS", "DownSampling_Bright_PS");
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "BloomSampling_PS.hlsl", "DownSampling_PS", "DownSampling_PS");
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "BloomSampling_PS.hlsl", "UpSampling_PS", "UpSampling_PS");
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "BloomSampling_PS.hlsl", "BloomBlur_PS", "BloomBlur_PS");
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "BloomSampling_PS.hlsl", "DrawScreen_PS", "DrawScreen_PS");

	// Tone Map Shader Macro
	D3D_SHADER_MACRO combine_macro1[] = { {"BLOOM"}, {NULL, NULL} };
	D3D_SHADER_MACRO combine_macro2[] = { {"HDR"}, {NULL, NULL} };
	D3D_SHADER_MACRO combine_macro3[] = { {"OUT_LINE"}, {NULL, NULL} };
	D3D_SHADER_MACRO combine_macro4[] = { {"BLOOM"}, {"HDR"}, {NULL, NULL} };
	D3D_SHADER_MACRO combine_macro5[] = { {"BLOOM"}, {"OUT_LINE"}, {NULL, NULL} };
	D3D_SHADER_MACRO combine_macro6[] = { {"HDR"}, {"OUT_LINE"}, {NULL, NULL} };
	D3D_SHADER_MACRO combine_macro7[] = { {"BLOOM"}, {"HDR"}, {"OUT_LINE"}, {NULL, NULL} };

	// Combine Shader
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "Combine_PS.hlsl", "Combine_PS", "Combine_PS_Option0");					// None
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "Combine_PS.hlsl", "Combine_PS", "Combine_PS_Option1", combine_macro1);	// Bloom
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "Combine_PS.hlsl", "Combine_PS", "Combine_PS_Option2", combine_macro2);	// HDR
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "Combine_PS.hlsl", "Combine_PS", "Combine_PS_Option3", combine_macro3);	// OutLine
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "Combine_PS.hlsl", "Combine_PS", "Combine_PS_Option4", combine_macro4);	// Bloom + HDR
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "Combine_PS.hlsl", "Combine_PS", "Combine_PS_Option5", combine_macro5);	// Bloom + OutLine
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "Combine_PS.hlsl", "Combine_PS", "Combine_PS_Option6", combine_macro6);	// HDR + OutLine
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "Combine_PS.hlsl", "Combine_PS", "Combine_PS_Option7", combine_macro7);	// Bloom + HDR + OutLine
	 
	// Picking ID Shader
	LoadShader(SHADER_TYPE::VERTEX_SHADER, "ID_StaticMesh_VS.hlsl", "ID_StaticMesh_VS", "ID_StaticMesh_VS");
	LoadShader(SHADER_TYPE::VERTEX_SHADER, "ID_StaticMesh_Instance_VS.hlsl", "ID_StaticMesh_Instance_VS", "ID_StaticMesh_Instance_VS");
	LoadShader(SHADER_TYPE::VERTEX_SHADER, "ID_SkinMesh_VS.hlsl", "ID_SkinMesh_VS", "ID_SkinMesh_VS");
	LoadShader(SHADER_TYPE::VERTEX_SHADER, "ID_SkinMesh_Instance_VS.hlsl", "ID_SkinMesh_Instance_VS", "ID_SkinMesh_Instance_VS");
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "ID_Mesh_PS.hlsl", "ID_Mesh_PS", "ID_Mesh_PS");

	// Depth Shader
	LoadShader(SHADER_TYPE::VERTEX_SHADER, "Depth_StaticMesh_VS.hlsl", "Depth_StaticMesh_VS", "Depth_StaticMesh_VS");
	//LoadShader(SHADER_TYPE::VERTEX_SHADER, "Depth_SkinMesh_VS.hlsl", "Depth_SkinMesh_VS", "Depth_SkinMesh_VS");
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "Depth_PS.hlsl", "Depth_PS", "Depth_PS");

	// Hierarchical Z Map Shader
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "HizMipMap_PS.hlsl", "HizMipMap_PS", "HizMipMap_PS");
	LoadShader(SHADER_TYPE::COMPUTE_SHADER, "HizCull_CS.hlsl", "HizCull_CS", "HizCull_CS");

	// OutLine Shader Macro
	D3D_SHADER_MACRO outline_macro1[] = { {"OUT_LINE"}, {NULL, NULL} };

	// OutLine Shader
	LoadShader(SHADER_TYPE::VERTEX_SHADER, "OutLine_StaticMesh_VS.hlsl", "OutLine_StaticMesh_VS", "Origin_StaticMesh_VS");
	LoadShader(SHADER_TYPE::VERTEX_SHADER, "OutLine_SkinMesh_VS.hlsl", "OutLine_SkinMesh_VS", "Origin_SkinMesh_VS");
	LoadShader(SHADER_TYPE::VERTEX_SHADER, "OutLine_StaticMesh_VS.hlsl", "OutLine_StaticMesh_VS", "OutLine_StaticMesh_VS", outline_macro1);
	LoadShader(SHADER_TYPE::VERTEX_SHADER, "OutLine_SkinMesh_VS.hlsl", "OutLine_SkinMesh_VS", "OutLine_SkinMesh_VS", outline_macro1);
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "OutLine_PS.hlsl", "OutLine_PS", "OutLine_PS");

	// Blur Shader
	LoadShader(SHADER_TYPE::COMPUTE_SHADER, "Blur_Horizon_CS.hlsl", "Blur_Horizon_CS", "Blur_Horizon_CS");
	LoadShader(SHADER_TYPE::COMPUTE_SHADER, "Blur_Vertical_CS.hlsl", "Blur_Vertical_CS", "Blur_Vertical_CS");


	// Debug Shader Macro
	D3D_SHADER_MACRO debug_macro1[] = { {"CUSTOM_COLOR"}, {NULL, NULL} };
	D3D_SHADER_MACRO debug_macro2[] = { {"RENDER_TARGET"}, {NULL, NULL} };
	
	// Debug Shader
	LoadShader(SHADER_TYPE::VERTEX_SHADER, "Debug_VS.hlsl", "Debug_VS", "Debug_VS");
	LoadShader(SHADER_TYPE::VERTEX_SHADER, "Debug_VS.hlsl", "Debug_Texture_VS", "Debug_Icon_VS");
	LoadShader(SHADER_TYPE::VERTEX_SHADER, "Debug_VS.hlsl", "Debug_Texture_VS", "Debug_MRT_VS", debug_macro2);
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "Debug_PS.hlsl", "Debug_PS", "Debug_PS_Option0");
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "Debug_PS.hlsl", "Debug_PS", "Debug_PS_Option1", debug_macro1);
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "Debug_PS.hlsl", "Debug_Texture_PS", "Debug_Icon_PS");
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "Debug_PS.hlsl", "Debug_Texture_PS", "Debug_MRT_PS", debug_macro2);

	/// Shader 생성 후 모든 Constant Buffer Usage 설정..
	AddConstantBufferUsage();
}

void ShaderManager::AddSampler(Hash_Code hash_code, ID3D11SamplerState* sampler)
{
	for (std::pair<std::string, ShaderBase*> shader : m_ShaderList)
	{
		shader.second->SetSamplerState(hash_code, sampler);
	}
}

void ShaderManager::AddConstantBuffer()
{
	std::unordered_map<Hash_Code, UINT>::iterator itor;
	ShaderBase* nowShader = nullptr;

	for (std::pair<std::string, ShaderBase*> shader : m_ShaderList)
	{
		// 현재 Shader 설정..
		nowShader = shader.second;

		// 현재 Shader에 Binding된 Constant Buffer 검색..
		for (std::pair<Hash_Code, ConstantBuffer*> cBuffer : shader.second->m_ConstantBufferList)
		{
			// 해당 Constant Buffer Usage 검색..
			itor = m_ConstantBufferUsageList.find(cBuffer.first);

			// 해당 Constant Buffer가 있을경우 설정..
			if (itor != m_ConstantBufferUsageList.end())
			{
				nowShader->SetConstantBuffer(itor->first, (CBUFFER_USAGE)itor->second);
			}
		}
	}
}

void ShaderManager::AddConstantBufferUsage()
{
	/// Default Usage Constant Buffer Push..

	// Fog..
	PushConstantBufferUsage<CB_FogOption>(CBUFFER_USAGE::DEFAULT);

	// Blur..
	PushConstantBufferUsage<CB_BlurTexel>(CBUFFER_USAGE::DEFAULT);

	// SSAO..
	PushConstantBufferUsage<CB_SsaoFrustum>(CBUFFER_USAGE::DEFAULT);
	PushConstantBufferUsage<CB_SsaoOffset>(CBUFFER_USAGE::DEFAULT);
	PushConstantBufferUsage<CB_SsaoOption>(CBUFFER_USAGE::DEFAULT);

	// OIT..
	PushConstantBufferUsage<CB_OitFrame>(CBUFFER_USAGE::DEFAULT);

	// FXAA..
	PushConstantBufferUsage<CB_FxaaFrame>(CBUFFER_USAGE::DEFAULT);

	// Bloom..
	PushConstantBufferUsage<CB_BloomBright>(CBUFFER_USAGE::DEFAULT);

	// OutLine..
	PushConstantBufferUsage<CB_OutLine>(CBUFFER_USAGE::DEFAULT);
	PushConstantBufferUsage<CB_OutLineOption>(CBUFFER_USAGE::DEFAULT);

	// Combine..
	PushConstantBufferUsage<CB_DrawFinal>(CBUFFER_USAGE::DEFAULT);


	/// Dynamic Usage Constant Buffer Push..

	// Mesh..
	PushConstantBufferUsage<CB_StaticMesh>(CBUFFER_USAGE::DYNAMIC);
	PushConstantBufferUsage<CB_InstanceStaticMesh>(CBUFFER_USAGE::DYNAMIC);
	PushConstantBufferUsage<CB_SkinMesh>(CBUFFER_USAGE::DYNAMIC);
	PushConstantBufferUsage<CB_InstanceSkinMesh>(CBUFFER_USAGE::DYNAMIC);
	PushConstantBufferUsage<CB_DepthStaticMesh>(CBUFFER_USAGE::DYNAMIC);
	PushConstantBufferUsage<CB_InstanceDepthStaticMesh>(CBUFFER_USAGE::DYNAMIC);
	PushConstantBufferUsage<CB_DepthSkinMesh>(CBUFFER_USAGE::DYNAMIC);
	PushConstantBufferUsage<CB_InstanceDepthSkinMesh>(CBUFFER_USAGE::DYNAMIC);
	PushConstantBufferUsage<CB_InstanceSkinMesh>(CBUFFER_USAGE::DYNAMIC);

	// Material..
	PushConstantBufferUsage<CB_Material>(CBUFFER_USAGE::DYNAMIC);

	// Light..
	PushConstantBufferUsage<CB_Camera>(CBUFFER_USAGE::DYNAMIC);
	PushConstantBufferUsage<CB_LightSub>(CBUFFER_USAGE::DYNAMIC);
	PushConstantBufferUsage<CB_Light>(CBUFFER_USAGE::DYNAMIC);

	// Fog..
	PushConstantBufferUsage<CB_FogData>(CBUFFER_USAGE::DYNAMIC);

	// Blur..
	PushConstantBufferUsage<CB_BlurOrder>(CBUFFER_USAGE::DYNAMIC);

	// SSAO..
	PushConstantBufferUsage<CB_SsaoObject>(CBUFFER_USAGE::DYNAMIC);

	// Particle..
	PushConstantBufferUsage<CB_InstanceParticleMesh>(CBUFFER_USAGE::DYNAMIC);
	PushConstantBufferUsage<CB_ParticleOption>(CBUFFER_USAGE::DYNAMIC);

	// SkyCube..
	PushConstantBufferUsage<CB_CubeObject>(CBUFFER_USAGE::DYNAMIC);
	PushConstantBufferUsage<CB_CubeConvert>(CBUFFER_USAGE::DYNAMIC);

	// RectTransform..
	PushConstantBufferUsage<CB_UIObject>(CBUFFER_USAGE::DYNAMIC);

	// IBL Baking..
	PushConstantBufferUsage<CB_ExternalData>(CBUFFER_USAGE::DYNAMIC);

	// Bloom..
	PushConstantBufferUsage<CB_BloomBlurOrder>(CBUFFER_USAGE::DYNAMIC);

	// Picking ID..
	PushConstantBufferUsage<CB_StaticMesh_ID>(CBUFFER_USAGE::DYNAMIC);
	PushConstantBufferUsage<CB_Instance_StaticMesh_ID>(CBUFFER_USAGE::DYNAMIC);
	PushConstantBufferUsage<CB_SkinMesh_ID>(CBUFFER_USAGE::DYNAMIC);
	PushConstantBufferUsage<CB_InstanceSkinMesh_ID>(CBUFFER_USAGE::DYNAMIC);

	// Hierachical Z-Map Culling..
	PushConstantBufferUsage<CB_HizCull>(CBUFFER_USAGE::DYNAMIC);

	// OutLine..
	PushConstantBufferUsage<CB_OutLineStaticMesh>(CBUFFER_USAGE::DYNAMIC);
	PushConstantBufferUsage<CB_OutLineSkinMesh>(CBUFFER_USAGE::DYNAMIC);

	// Debug..
	PushConstantBufferUsage<CB_DebugObject>(CBUFFER_USAGE::DYNAMIC);
	PushConstantBufferUsage<CB_DebugOption>(CBUFFER_USAGE::DYNAMIC);

	/// Constant Buffer Usage List 설정 후 모든 Shader 해당 Constant Buffer 생성 및 설정..
	AddConstantBuffer();
}

ShaderBase* ShaderManager::LoadShader(SHADER_TYPE shaderType, const char* fileName, const char* entry_point, const char* shaderName, const D3D_SHADER_MACRO* pDefines)
{
	// Shader Type에 맞는 Shader 생성..
	ShaderBase* newShader = nullptr;

	switch (shaderType)
	{
	case SHADER_TYPE::VERTEX_SHADER:
		newShader = new VertexShader(shaderName, fileName, entry_point, "vs_5_0", pDefines);
		break;
	case SHADER_TYPE::PIXEL_SHADER:
		newShader = new PixelShader(shaderName, fileName, entry_point, "ps_5_0", pDefines);
		break;
	case SHADER_TYPE::COMPUTE_SHADER:
		newShader = new ComputeShader(shaderName, fileName, entry_point, "cs_5_0", pDefines);
		break;
	default:
		return nullptr;
	}

	// 파일을 제대로 읽지 못하여 생성하지 못한경우 nullptr..
	if (newShader == nullptr)
		return nullptr;

	// 새로 생성한 Shader 삽입..
	m_ShaderList.insert(std::make_pair(shaderName, newShader));

	return newShader;
}

OriginalShader ShaderManager::GetShader(std::string shaderName)
{
	return OriginalShader{ this, shaderName };
}

ShaderBase* ShaderManager::GetBaseShader(std::string shaderName)
{
	std::unordered_map<std::string, ShaderBase*>::iterator shader = m_ShaderList.find(shaderName);

	// 해당 Shader가 없을 경우..
	if (shader == m_ShaderList.end()) return nullptr;

	return shader->second;
}

VertexShader* ShaderManager::GetVertexShader(std::string shaderName)
{
	std::unordered_map<std::string, ShaderBase*>::iterator shader = m_ShaderList.find(shaderName);

	// 해당 Shader가 없을 경우..
	if (shader == m_ShaderList.end()) return nullptr;

	switch (shader->second->GetType())
	{
	case SHADER_TYPE::VERTEX_SHADER:
		return reinterpret_cast<VertexShader*>(shader->second);
	default:
		return nullptr;
	}
}

PixelShader* ShaderManager::GetPixelShader(std::string shaderName)
{
	std::unordered_map<std::string, ShaderBase*>::iterator shader = m_ShaderList.find(shaderName);

	// 해당 Shader가 없을 경우..
	if (shader == m_ShaderList.end()) return nullptr;

	switch (shader->second->GetType())
	{
	case SHADER_TYPE::PIXEL_SHADER:
		return reinterpret_cast<PixelShader*>(shader->second);
	default:
		return nullptr;
	}
}

ComputeShader* ShaderManager::GetComputeShader(std::string shaderName)
{
	std::unordered_map<std::string, ShaderBase*>::iterator shader = m_ShaderList.find(shaderName);

	// 해당 Shader가 없을 경우..
	if (shader == m_ShaderList.end()) return nullptr;

	switch (shader->second->GetType())
	{
	case SHADER_TYPE::COMPUTE_SHADER:
		return reinterpret_cast<ComputeShader*>(shader->second);
	default:
		return nullptr;
	}
}