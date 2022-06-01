#pragma once
#include "FormatDLL.h"
class FORMAT_PARSER_DLL FM_MATERIAL
{
public:
	FM_MATERIAL() {};
	~FM_MATERIAL() {};

	const char* Name;
	bool Alpha			= false;
	const char* DiffuseMap	= "0";
	const char* NormalMap	= "0";
	const char* EmissiveMap	= "0";
	const char* ORMMap		= "0";

	float Emissive		= 0;
	float Roughness		= 0;
	float Metallic		= 0;

	float Tileing_X		= 1;
	float Tileing_Y		= 1;

	float AddColor_R	= 0;
	float AddColor_G	= 0;
	float AddColor_B	= 0;

	float LimColor_R	= 0;
	float LimColor_G	= 0;
	float LimColor_B	= 0;

	float LimFactor		= 0;
	float LimWidth		= 0;


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

