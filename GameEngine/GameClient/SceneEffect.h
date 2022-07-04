#pragma once
#include <Component.h>

class RenderOption;

class SceneEffect : public Component
{
public:
	SceneEffect();
	~SceneEffect();

private:
	enum STATE
	{
		NONE,
		FADE_IN,
		FADE_OUT,
	};

public:
	void Awake() override;
	void Update() override;

public:
	void Begin_Boss_Start_Effect();

private:
	void Change_Boss_Start_Effect();

private:
	/// ���� ����� ���ϴ� ��ġ��
	const float Boss_Fog_MoveSpeed = 0.016f;
	const float Boss_Fog_DistanceOffset = 2.0f;
	const float Boss_Fog_HeightValue = 2.0f;
	
	const float Boss_SkyCube_Size = 208.0f;

	const std::string Boss_SkyCube_Name = "Sky_boss";
	const std::string Boss_ColorGrading_Name = "LUT_boss";

private:
	STATE Scene_State;
	
	bool Fade_In_End = false;
	bool Fade_Out_End = false;

private:
	RenderOption* m_RenderOption;
};

