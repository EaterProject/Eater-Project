#include "FM_MATERIAL.h"
#include "FM_MATERIAL.h"

void FM_MATERIAL::SetColor(float& R, float& G, float& B)
{
	AddColor_R = R;
	AddColor_G = G;
	AddColor_B = B;
}

void FM_MATERIAL::SetColor(float&& R, float&& G, float&& B)
{
	AddColor_R = R;
	AddColor_G = G;
	AddColor_B = B;
}

void FM_MATERIAL::SetColor(int& R, int& G, int& B)
{
	AddColor_R = (float)R;
	AddColor_G = (float)G;
	AddColor_B = (float)B;
}

void FM_MATERIAL::SetColor(int&& R, int&& G, int&& B)
{
	AddColor_R = (float)R;
	AddColor_G = (float)G;
	AddColor_B = (float)B;
}

void FM_MATERIAL::SetLimColor(float& R, float& G, float& B)
{
	LimColor_R = R;
	LimColor_G = G;
	LimColor_B = B;
}

void FM_MATERIAL::SetLimColor(float&& R, float&& G, float&& B)
{
	LimColor_R = R;
	LimColor_G = G;
	LimColor_B = B;
}

void FM_MATERIAL::SetLimColor(int& R, int& G, int& B)
{
	LimColor_R = (float)R;
	LimColor_G = (float)G;
	LimColor_B = (float)B;
}

void FM_MATERIAL::SetLimColor(int&& R, int&& G, int&& B)
{
	LimColor_R = (float)R;
	LimColor_G = (float)G;
	LimColor_B = (float)B;
}


void FM_MATERIAL::SetMaterial(float& _Emissive, float& _Roughness, float& _Metallic)
{
	Emissive = _Emissive;
	Roughness = _Roughness;
	Metallic = _Metallic;
}

void FM_MATERIAL::SetMaterial(float&& _Emissive, float&& _Roughness, float&& _Metallic)
{

	Emissive = _Emissive;
	Roughness = _Roughness;
	Metallic = _Metallic;
}

void FM_MATERIAL::SetMaterial(int& _Emissive, int& _Roughness, int& _Metallic)
{
	Emissive = (float)_Emissive;
	Roughness = (float)_Roughness;
	Metallic = (float)_Metallic;

}

void FM_MATERIAL::SetMaterial(int&& _Emissive, int&& _Roughness, int&& _Metallic)
{
	Emissive = (float)_Emissive;
	Roughness = (float)_Roughness;
	Metallic = (float)_Metallic;
}

void FM_MATERIAL::ReSet()
{
	Name			= "";
	Alpha			= false;
	DiffuseMap		= "0";
	NormalMap		= "0";
	EmissiveMap		= "0";
	ORMMap			= "0";

	Emissive		= 0;
	Roughness		= 0;
	Metallic		= 0;

	Tileing_X		= 1;
	Tileing_Y		= 1;

	AddColor_R		= 0;
	AddColor_G		= 0;
	AddColor_B		= 0;

	LimColor_R		= 0;
	LimColor_G		= 0;
	LimColor_B		= 0;

	LimFactor		= 0;
	LimWidth		= 0;
}
