#include "SceneEffect.h"
#include "EaterEngineAPI.h"
#include "MessageManager.h"
#include "ClientTypeOption.h"
#include "EngineData.h"

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
			Scene_State = FADE_OUT;
			MessageManager::GetGM()->SEND_Message(TARGET_UI, MESSAGE_UI_FADE_OUT, &Fade_Out_End);
		}
	}
		break;
	case SceneEffect::FADE_OUT:
	{
		if (Fade_Out_End)
		{
			Fade_Out_End = false;
			Scene_State = NONE;
			MessageManager::GetGM()->SEND_Message(TARGET_UI, MESSAGE_UI_FADE_OUT, &Fade_Out_End);
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

	// �𷺼� ���� 3

	// ȯ�� ����..
	SetSkyCube(Boss_SkyCube_Name);

	// �÷� �׷��̵� ����..
	SetColorGradingBaseTexture(Boss_ColorGrading_Name);

	// ���� ���� ���� ī�޶� �ִϸ��̼� ����..
	MessageManager::GetGM()->SEND_Message(TARGET_CAMERA_MANAGER, MESSAGE_CAMERA_CINEMATIC_BOSS_START);
}
