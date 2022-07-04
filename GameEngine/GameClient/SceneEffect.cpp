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
	// 랜더 옵션 가져오기..
	m_RenderOption = GetRenderOptionData();

	// 변경해야하는 메테리얼 가져오기..
	Material* material = FindMaterialName("Outside_Other_M_gate00_largebackground");
	m_ChangeMaterial = material->m_MaterialData->Material_Property;

	// 변경해야하는 라이트 가져오기..
	GameObject* Light1 = FindGameObjectName("BossLight_1");
	GameObject* Light2 = FindGameObjectName("BossLight_2");
	GameObject* Light3 = FindGameObjectName("BossLight_3");

	Light1->SetActive(false);
	Light2->SetActive(false);
	Light3->SetActive(false);

	m_BossLightList.push_back(Light1);
	m_BossLightList.push_back(Light2);
	m_BossLightList.push_back(Light3);

	// 디렉션 라이트 가져오기..
	m_DirectionLight = GetDirectionLight()->GetComponent<Light>();

	// 보스 포그 생성..
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
	// 페이드 인 시작..
	Scene_State = FADE_IN;
	MessageManager::GetGM()->SEND_Message(TARGET_UI, MESSAGE_UI_FADE_IN, &Fade_In_End);
}

void SceneEffect::Change_Boss_Start_Effect()
{
	// 수치값 조정..
	m_RenderOption->FOG_MoveSpeed = Boss_Fog_MoveSpeed;
	m_RenderOption->FOG_DistanceOffset = Boss_Fog_DistanceOffset;
	m_RenderOption->FOG_HeightValue = Boss_Fog_HeightValue;
	m_RenderOption->SkyCube_Size = Boss_SkyCube_Size;

	// 메테리얼 변경..
	m_ChangeMaterial->LimLightColor = { 0.639216f, 0.505882f, 0.415686 };
	m_ChangeMaterial->LimLightFactor = 0.8f;
	m_ChangeMaterial->LimLightWidth = 6.1f;

	// 디렉션 강도 3
	m_DirectionLight->SetPower(3.0f);

	// 보스 전용 라이트 활성화
	for (int i = 0; i < m_BossLightList.size(); i++)
	{
		m_BossLightList[i]->SetActive(true);
	}

	// 환경 변경..
	SetSkyCube(Boss_SkyCube_Name);

	// 컬러 그레이딩 변경..
	SetColorGradingBaseTexture(Boss_ColorGrading_Name);

	// 보스존 포그 활성화..
	m_BossFog->gameobject->SetActive(true);
	m_BossFog->Play();
}
