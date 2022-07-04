#pragma once
#include <Component.h>

class Light;
class MaterialProperty;
class ParticleController;
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
	void Start() override;
	void Update() override;

public:
	void Begin_Boss_Start_Effect();

private:
	void Change_Boss_Start_Effect();

private:
	/// 보스 등장시 변하는 수치들
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
	bool Camera_End = false;

private:
	RenderOption* m_RenderOption;

	MaterialProperty* m_ChangeMaterial;

	ParticleController* m_BossFog;

	Light* m_DirectionLight;
	std::vector<GameObject*> m_BossLightList;
};

