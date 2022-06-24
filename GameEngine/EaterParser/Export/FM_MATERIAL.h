#pragma once
#include "FormatDLL.h"
#include <string>
class FORMAT_PARSER_DLL FM_MATERIAL
{
public:
	FM_MATERIAL() {};
	~FM_MATERIAL() {};

	std::string Name;
	std::string DiffuseMap	= "0";
	std::string NormalMap	= "0";
	std::string EmissiveMap	= "0";
	std::string ORMMap		= "0";

	bool Alpha			= false;
	float Emissive		= 0;
	float Roughness		= 0;
	float Metallic		= 0;

	float Tileing_X		= 1;
	float Tileing_Y		= 1;
	int	  SkyLightIndex = 0;

	float AddColor_R	= 0;
	float AddColor_G	= 0;
	float AddColor_B	= 0;

	float LimColor_R	= 0;
	float LimColor_G	= 0;
	float LimColor_B	= 0;

	float LimFactor		= 0;
	float LimWidth		= 0;

	void SetAlpha(bool mAlpha);
	void SetColor(float& R, float& G, float& B);
	void SetColor(float&& R, float&& G, float&& B);
	void SetColor(int& R, int& G, int& B);
	void SetColor(int&& R, int&& G, int&& B);
	void SetLimColor(float& R, float& G, float& B);
	void SetLimColor(float&& R, float&& G, float&& B);
	void SetLimColor(int& R, int& G, int& B);
	void SetLimColor(int&& R, int&& G, int&& B);
	void SetMaterial(float& _Emissive, float& _Roughness, float& _Metallic);
	void SetMaterial(float&& _Emissive, float&& _Roughness, float&& _Metallic);
	void SetMaterial(int& _Emissive, int& _Roughness, int& _Metallic);
	void SetMaterial(int&& _Emissive, int&& _Roughness, int&& _Metallic);
	void ReSet();
};

