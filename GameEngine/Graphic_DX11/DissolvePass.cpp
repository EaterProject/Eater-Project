#include "DirectDefine.h"
#include "RenderPassBase.h"
#include "ShaderBase.h"
#include "GraphicView.h"
#include "GraphicResource.h"
#include "EngineData.h"
#include "DissolvePass.h"

#include "FactoryManagerBase.h"
#include "ResourceManagerBase.h"
#include "ShaderManagerBase.h"
#include "ConstantBufferDefine.h"
#include "ShaderResourceViewDefine.h"

#include "Profiler/Profiler.h"

Dissolve_Pass::Dissolve_Pass()
{

}

Dissolve_Pass::~Dissolve_Pass()
{

}

void Dissolve_Pass::Start(int width, int height)
{
	// Dissolve Shader List Up..
	SetShaderList();

	// Set Dissolve Shader Resoruce View..
	SetShaderResourceView();

	// Set Dissolve Shader Constant Buffer..
	SetShaderConstantBuffer();

	TextureName.push_back("Dissolve_01");
	TextureName.push_back("Dissolve_02");
	TextureName.push_back("Dissolve_03");
	TextureName.push_back("Dissolve_04");
	TextureName.push_back("Dissolve_05");
	TextureName.push_back("Dissolve_06");
	TextureName.push_back("Dissolve_07");
	TextureName.push_back("Dissolve_08");
	TextureName.push_back("Dissolve_09");
	TextureName.push_back("Dissolve_10");
	TextureName.push_back("Dissolve_11");
	TextureName.push_back("Dissolve_12");
	TextureName.push_back("Dissolve_13");
	TextureName.push_back("Dissolve_14");
	TextureName.push_back("Dissolve_15");
	TextureName.push_back("Dissolve_16");
}

void Dissolve_Pass::Release()
{

}

void Dissolve_Pass::PreUpdate()
{
	//m_DissolveTimer += g_GlobalData->Time * m_DissolveSpeed;

	Test();

	if (GetAsyncKeyState(VK_F2))
	{
		m_DissolveThickness += g_GlobalData->Time * 0.25f;
		if (m_DissolveThickness > 1.0f)
		{
			m_DissolveThickness = 0.0f;
		}
	}
	if (GetAsyncKeyState(VK_F3))
	{
		m_DissolveOuterFactor += g_GlobalData->Time * 5.0f;
		if (m_DissolveOuterFactor > 25.0f)
		{
			m_DissolveOuterFactor = 0.0f;
		}
	}
	if (GetAsyncKeyState(VK_F4))
	{
		m_DissolveInnerFactor += g_GlobalData->Time * 25.0f;
		if (m_DissolveInnerFactor > 100.0f)
		{
			m_DissolveInnerFactor = 0.0f;
		}
	}
	if (GetAsyncKeyState(VK_F5))
	{
		m_DissolveSpeed += g_GlobalData->Time * 0.5f;
		if (m_DissolveSpeed > 2.0f)
		{
			m_DissolveSpeed = 0.01f;
		}
	}

	static bool play = false;
	static bool check1 = false;

	if (GetAsyncKeyState(VK_SPACE))
	{
		if (check1 == false)
		{
			check1 = true;
			play ^= true;
		}
	}
	else
	{
		check1 = false;
	}

	if (play)
	{
		m_DissolveTimer += g_GlobalData->Time * m_DissolveSpeed;
		if (m_DissolveTimer > 1.0f)
		{
			m_DissolveTimer = 0.0f;
		}
	}

	static bool check2 = false;

	if (GetAsyncKeyState(VK_F9))
	{
		if (check2 == false)
		{
			check2 = true;

			PROFILE_LOG(PROFILE_OUTPUT::LOG_FILE, "\n# Dissolve Value\n- Texture \t: %s\n- PlaySpeed \t: %.3f\n- Thickness \t: %.3f\n- InnerFactor \t: %.3f\n- OuterFactor \t: %.3f\n",
				TextureName[index].c_str(), m_DissolveSpeed, m_DissolveThickness, m_DissolveInnerFactor, m_DissolveOuterFactor);
		}
	}
	else
	{
		check2 = false;
	}

	CB_DissolveOption dissolveOptionBuf;
	//dissolveOptionBuf.gOuterEdge = 1.0f - m_DissolvePlayTime - m_DissolveTimer;
	dissolveOptionBuf.gOuterEdge = m_DissolveTimer;
	dissolveOptionBuf.gOuterFactor = m_DissolveOuterFactor;
	dissolveOptionBuf.gInnerEdge = dissolveOptionBuf.gOuterEdge + m_DissolveThickness;
	dissolveOptionBuf.gInnerFactor = m_DissolveInnerFactor;
	dissolveOptionBuf.gThickness = 1.0f / m_DissolveThickness;
	
	// inner factor = 500;
	// outer factor = 300;

	for (ShaderBase* shader : m_OptionShaderList)
	{
		shader->ConstantBufferUpdate(&dissolveOptionBuf);
	}
}

void Dissolve_Pass::SetShaderList()
{
	PushShader("OIT_Mesh_PS_Option0");
	PushShader("OIT_Mesh_PS_Option1");
	PushShader("OIT_Mesh_PS_Option2");
	PushShader("OIT_Mesh_PS_Option3");
	PushShader("OIT_Mesh_PS_Option4");
	PushShader("OIT_Mesh_PS_Option5");
	PushShader("OIT_Mesh_PS_Option6");
	PushShader("OIT_Mesh_PS_Option7");
}

void Dissolve_Pass::SetShaderResourceView()
{
	//ID3D11ShaderResourceView* noiseMap = g_Resource->GetShaderResourceView<gNoiseTexture>()->Get();
	ID3D11ShaderResourceView* noiseMap = g_Resource->GetShaderResourceView<gNoise1>()->Get();

	for (ShaderBase* shader : m_OptionShaderList)
	{
		shader->SetShaderResourceView<gNoiseTexture>(noiseMap);
	}
}

void Dissolve_Pass::SetShaderConstantBuffer()
{

}

void Dissolve_Pass::Test()
{
	ID3D11ShaderResourceView* noiseMap = nullptr;

	static bool check = false;

	if (GetAsyncKeyState(VK_F1))
	{
		if (check == false)
		{
			check = true;

			index++;

			if (index > 15)
			{
				index = 0;
			}
			noiseMap = g_Resource->GetShaderResourceView<gNoise1>()->Get();
		}
	}
	else
	{
		check = false;
		return;
	}

	switch (index)
	{
	case 0:
		noiseMap = g_Resource->GetShaderResourceView<gNoise1>()->Get();
		break;
	case 1:
		noiseMap = g_Resource->GetShaderResourceView<gNoise2>()->Get();
		break; 
	case 2:
		noiseMap = g_Resource->GetShaderResourceView<gNoise3>()->Get();
		break;
	case 3:
		noiseMap = g_Resource->GetShaderResourceView<gNoise4>()->Get();
		break;
	case 4:
		noiseMap = g_Resource->GetShaderResourceView<gNoise5>()->Get();
		break;
	case 5:
		noiseMap = g_Resource->GetShaderResourceView<gNoise6>()->Get();
		break;
	case 6:
		noiseMap = g_Resource->GetShaderResourceView<gNoise7>()->Get();
		break;
	case 7:
		noiseMap = g_Resource->GetShaderResourceView<gNoise8>()->Get();
		break;
	case 8:
		noiseMap = g_Resource->GetShaderResourceView<gNoise9>()->Get();
		break;
	case 9:
		noiseMap = g_Resource->GetShaderResourceView<gNoise10>()->Get();
		break;
	case 10:
		noiseMap = g_Resource->GetShaderResourceView<gNoise11>()->Get();
		break;
	case 11:
		noiseMap = g_Resource->GetShaderResourceView<gNoise12>()->Get();
		break;
	case 12:
		noiseMap = g_Resource->GetShaderResourceView<gNoise13>()->Get();
		break;
	case 13:
		noiseMap = g_Resource->GetShaderResourceView<gNoise14>()->Get();
		break;
	case 14:
		noiseMap = g_Resource->GetShaderResourceView<gNoise15>()->Get();
		break;
	case 15:
		noiseMap = g_Resource->GetShaderResourceView<gNoise16>()->Get();
		break;
	default:
		break;
	}

	if (noiseMap == nullptr) return;

	for (ShaderBase* shader : m_OptionShaderList)
	{
		shader->SetShaderResourceView<gNoiseTexture>(noiseMap);
	}
}
