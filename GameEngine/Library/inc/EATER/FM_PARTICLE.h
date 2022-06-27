#pragma once
#include "FormatDLL.h"
class FORMAT_PARSER_DLL FM_PARTICLE
{
public:
	FM_PARTICLE()	= default;
	~FM_PARTICLE()	= default;


	std::string TextureName;

	int Particle_Render_Type;
	int Particle_LifeTime_Color_Type;
	int Particle_LifeTime_Scale_Type;

	int MaxParticle = 0;
	float DelayTime = 0.0f;
	float PlayTime	= 0.0f;
	bool Loop		= false;

	///생성범위
	float ShapeRadius_X;
	float ShapeRadius_Y;
	float ShapeRadius_Z;

	///텍스쳐 타일링
	int TextureTiling_X;
	int TextureTiling_Y;

	int	SkyLightIndex	= 0;
	float	Strength	= 1;

	///시작 후 방향에 대한 힘
	float StartForce_Min_X;
	float StartForce_Min_Y;
	float StartForce_Min_Z;
	float StartForce_Max_X;
	float StartForce_Max_Y;
	float StartForce_Max_Z;

	///시작 후 색깔
	float StartColor_Min_R;
	float StartColor_Min_G;
	float StartColor_Min_B;
	float StartColor_Min_A;

	float StartColor_Max_R;
	float StartColor_Max_G;
	float StartColor_Max_B;
	float StartColor_Max_A;

	///시작후 생성 시간
	float StartLifeTime_Min;
	float StartLifeTime_Max;
	///시작 후 크기
	float StartScale_Min;
	float StartScale_Max;
	///시작 후 회전
	float StartRotation_Min;
	float StartRotation_Max;

	///실행 중 방향
	float LifeForce_Min_X;
	float LifeForce_Min_Y;
	float LifeForce_Min_Z;
	float LifeForce_Max_X;
	float LifeForce_Max_Y;
	float LifeForce_Max_Z;

	///실행 중 색깔
	float LifeColor_Min_R;
	float LifeColor_Min_G;
	float LifeColor_Min_B;
	float LifeColor_Min_A;
	float LifeColor_Max_R;
	float LifeColor_Max_G;
	float LifeColor_Max_B;
	float LifeColor_Max_A;

	///실행 중  크기
	float LifeScale_Min;
	float LifeScale_Max;

	///실행 중 회전
	float LifeRotation_Min;
	float LifeRotation_Max;
	float RateOverTime;
};

