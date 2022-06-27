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

	///��������
	float ShapeRadius_X;
	float ShapeRadius_Y;
	float ShapeRadius_Z;

	///�ؽ��� Ÿ�ϸ�
	int TextureTiling_X;
	int TextureTiling_Y;

	int	SkyLightIndex	= 0;
	float	Strength	= 1;

	///���� �� ���⿡ ���� ��
	float StartForce_Min_X;
	float StartForce_Min_Y;
	float StartForce_Min_Z;
	float StartForce_Max_X;
	float StartForce_Max_Y;
	float StartForce_Max_Z;

	///���� �� ����
	float StartColor_Min_R;
	float StartColor_Min_G;
	float StartColor_Min_B;
	float StartColor_Min_A;

	float StartColor_Max_R;
	float StartColor_Max_G;
	float StartColor_Max_B;
	float StartColor_Max_A;

	///������ ���� �ð�
	float StartLifeTime_Min;
	float StartLifeTime_Max;
	///���� �� ũ��
	float StartScale_Min;
	float StartScale_Max;
	///���� �� ȸ��
	float StartRotation_Min;
	float StartRotation_Max;

	///���� �� ����
	float LifeForce_Min_X;
	float LifeForce_Min_Y;
	float LifeForce_Min_Z;
	float LifeForce_Max_X;
	float LifeForce_Max_Y;
	float LifeForce_Max_Z;

	///���� �� ����
	float LifeColor_Min_R;
	float LifeColor_Min_G;
	float LifeColor_Min_B;
	float LifeColor_Min_A;
	float LifeColor_Max_R;
	float LifeColor_Max_G;
	float LifeColor_Max_B;
	float LifeColor_Max_A;

	///���� ��  ũ��
	float LifeScale_Min;
	float LifeScale_Max;

	///���� �� ȸ��
	float LifeRotation_Min;
	float LifeRotation_Max;
	float RateOverTime;
};

