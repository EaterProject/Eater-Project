#pragma once
struct EATER_VERTEX_BASE
{
	float POS_X;
	float POS_Y;
	float POS_Z;

	float NOMAL_X;
	float NOMAL_Y;
	float NOMAL_Z;

	float UV_X;
	float UV_Y;

	float TANGENT_X;
	float TANGENT_Y;
	float TANGENT_Z;
};

struct EATER_VERTEX_POS
{
	float POS_X;
	float POS_Y;
	float POS_Z;
};

struct EATER_VERTEX_SKIN
{
	float POS_X;
	float POS_Y;
	float POS_Z;

	float NOMAL_X;
	float NOMAL_Y;
	float NOMAL_Z;

	float UV_X;
	float UV_Y;

	float TANGENT_X;
	float TANGENT_Y;
	float TANGENT_Z;

	int		BONE_INDEX[8];
	float	BONE_WEIGHTS[8];
};