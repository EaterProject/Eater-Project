#pragma once
#include <string>
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
struct EATER_MATERIAL_DATA
{
	std::string MaterialName;
	bool Alpha			= false;
	bool DiffuseMap		= false;
	bool NormalMap		= false;
	bool EmissiveMap	= false;
	bool ORMMap			= false;

	float Emissive		= 0;
	float Roughness		= 0;
	float Metallic		= 0;

	float Tileing_X		= 1;
	float Tileing_Y		= 1;

	float AddColor_R	= 0;
	float AddColor_G	= 0;
	float AddColor_B	= 0;
	float AddColor_A	= 0;

	float LimColor_R	= 0;
	float LimColor_G	= 0;
	float LimColor_B	= 0;

	float LimFactor		= 0;
	float LimWidth		= 0;

	void SetColor(float &R, float &G, float &B, float &A)
	{
		AddColor_R = R;
		AddColor_G = G;
		AddColor_B = B;
		AddColor_A = A;
	}
	void SetColor(float&& R, float&& G, float&& B, float&& A = 0)
	{
		AddColor_R = R;
		AddColor_G = G;
		AddColor_B = B;
		AddColor_A = A;
	}
	void SetLimColor(float& R, float& G, float& B)
	{
		LimColor_R = R;
		LimColor_G = G;
		LimColor_B = B;
	}
	void SetLimColor(float&& R, float&& G, float&& B)
	{
		LimColor_R = R;
		LimColor_G = G;
		LimColor_B = B;
	}
	void SetTextureMapCheck(bool& _Diffuse,bool& _Normal,bool& _Emissive,bool& _ORM)
	{
		DiffuseMap	= _Diffuse;
		NormalMap	= _Normal;
		EmissiveMap = _Emissive;
		ORMMap		= _ORM;
	}
	void SetTextureMapCheck(bool&& _Diffuse, bool&& _Normal, bool&& _Emissive, bool&& _ORM)
	{
		DiffuseMap	= _Diffuse;
		NormalMap	= _Normal;
		EmissiveMap = _Emissive;
		ORMMap		= _ORM;
	}
};

struct EATER_SCENE_DATA
{

};

struct EATER_ANIMATION_DATA
{
	float FrameTime;
	float Local_Pos_X;
	float Local_Pos_Y;
	float Local_Pos_Z;

	float Local_Rot_X;
	float Local_Rot_Y;
	float Local_Rot_Z;
	float Local_Rot_W;

	float Local_Scl_X;
	float Local_Scl_Y;
	float Local_Scl_Z;

	float World_Pos_X;
	float World_Pos_Y;
	float World_Pos_Z;

	float World_Rot_X;
	float World_Rot_Y;
	float World_Rot_Z;
	float World_Rot_W;
};
