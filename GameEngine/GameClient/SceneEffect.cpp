#include "SceneEffect.h"
#include "EaterEngineAPI.h"
#include "MessageManager.h"
#include "ClientTypeOption.h"
#include "EngineData.h"
#include "GameObject.h"
#include "Transform.h"
#include "Light.h"
#include "Material.h"

#include "ParticleSystem.h"
#include "ParticleController.h"
#include "ParticleFactory.h"

SceneEffect::SceneEffect()
{

}

SceneEffect::~SceneEffect()
{

}

void SceneEffect::Awake()
{
	// ���� �ɼ� ��������..
	m_RenderOption = GetRenderOptionData();

	// �����ؾ��ϴ� ���׸��� ��������..
	Material* material = FindMaterialName("Outside_Other_M_gate00_largebackground");
	m_ChangeMaterial = material->m_MaterialData->Material_Property;

	// �����ؾ��ϴ� ����Ʈ ��������..
	GameObject* Light1 = FindGameObjectName("BossLight_1");
	GameObject* Light2 = FindGameObjectName("BossLight_2");
	GameObject* Light3 = FindGameObjectName("BossLight_3");

	Light1->SetActive(false);
	Light2->SetActive(false);
	Light3->SetActive(false);

	m_BossLightList.push_back(Light1);
	m_BossLightList.push_back(Light2);
	m_BossLightList.push_back(Light3);

	// �𷺼� ����Ʈ ��������..
	m_DirectionLight = GetDirectionLight()->GetComponent<Light>();

	// ���� ���� ����..
	m_BossFog = ParticleFactory::Get()->CreateParticleController(PARTICLE_TYPE::BossFog);

	Transform* FogEffect = m_BossFog->GetParticle("BossFog_fog2")->gameobject->transform;

	FogEffect->SetPosition(-58.0f, 7.0f, 60.0f);
}

void SceneEffect::Start()
{
	m_BossFog->gameobject->SetActive(false);
}

void SceneEffect::Update()
{
	switch (Scene_State)
	{
	case SceneEffect::FADE_IN:
	{
		if (Fade_In_End)
		{
			Fade_In_End = false;
			Camera_End = false;
			Scene_State = FADE_OUT;

			Change_Boss_Start_Effect();

			MessageManager::GetGM()->SEND_Message(TARGET_UI, MESSAGE_UI_FADE_OUT, &Fade_Out_End);
			MessageManager::GetGM()->SEND_Message(TARGET_CAMERA_MANAGER, MESSAGE_CAMERA_CINEMATIC_BOSS_START, &Camera_End);
		}
	}
	break;
	case SceneEffect::FADE_OUT:
	{
		if (Fade_Out_End)
		{
			if (Camera_End)
			{
				Fade_Out_End = false;
				Scene_State == NONE;
				MessageManager::GetGM()->SEND_Message(TARGET_GLOBAL, MESSAGE_GLOBAL_RESUME);
			}
		}
	}
	break;
	default:
		break;
	}

}

void SceneEffect::Begin_Boss_Start_Effect()
{
	// ���̵� �� ����..
	Scene_State = FADE_IN;
	MessageManager::GetGM()->SEND_Message(TARGET_UI, MESSAGE_UI_FADE_IN, &Fade_In_End);
}

void SceneEffect::Change_Boss_Start_Effect()
{
	// ��ġ�� ����..
	m_RenderOption->FOG_MoveSpeed = Boss_Fog_MoveSpeed;
	m_RenderOption->FOG_DistanceOffset = Boss_Fog_DistanceOffset;
	m_RenderOption->FOG_HeightValue = Boss_Fog_HeightValue;
	m_RenderOption->SkyCube_Size = Boss_SkyCube_Size;

	// ���׸��� ����..
	m_ChangeMaterial->LimLightColor = { 0.639216f, 0.505882f, 0.415686 };
	m_ChangeMaterial->LimLightFactor = 0.8f;
	m_ChangeMaterial->LimLightWidth = 6.1f;

	// �𷺼� ���� 3
	m_DirectionLight->SetPower(3.0f);

	// ���� ���� ����Ʈ Ȱ��ȭ
	for (int i = 0; i < m_BossLightList.size(); i++)
	{
		m_BossLightList[i]->SetActive(true);
	}

	// ȯ�� ����..
	SetSkyCube(Boss_SkyCube_Name);

	// �÷� �׷��̵� ����..
	SetColorGradingBaseTexture(Boss_ColorGrading_Name);

	// ������ ���� Ȱ��ȭ..
	m_BossFog->gameobject->SetActive(true);
	m_BossFog->Play();
}
