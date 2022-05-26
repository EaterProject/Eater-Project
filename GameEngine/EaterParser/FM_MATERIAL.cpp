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
	Emissive = (int)_Emissive;
	Roughness = (int)_Roughness;
	Metallic = (int)_Metallic;

}

void FM_MATERIAL::SetMaterial(int&& _Emissive, int&& _Roughness, int&& _Metallic)
{
	Emissive = (int)_Emissive;
	Roughness = (int)_Roughness;
	Metallic = (int)_Metallic;
}
