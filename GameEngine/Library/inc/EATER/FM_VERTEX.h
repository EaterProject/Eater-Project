#pragma once

#include "FormatDLL.h"

enum class FM_VERTEX_TYPE : int
{
	POS,
	STATIC,
	SKIN,
};

class FORMAT_PARSER_DLL FM_VERTEX
{
public:
	FM_VERTEX_TYPE Type = FM_VERTEX_TYPE::STATIC;
	float POS_X = 0;
	float POS_Y = 0;
	float POS_Z = 0;

	float NOMAL_X = 0;
	float NOMAL_Y = 0;
	float NOMAL_Z = 0;

	float UV_X = 0;
	float UV_Y = 0;

	float TANGENT_X =0;
	float TANGENT_Y =0;
	float TANGENT_Z =0;

	int		BONE_INDEX[4];
	float	BONE_WEIGHTS[4];

	//À§Ä¡
	void SetPosition(float& x, float& y, float& z);
	void SetPosition(float&& x, float&& y, float&& z);
	void SetPosition(int& x, int& y, int& z);
	void SetPosition(int&& x, int&& y, int&& z);
	//³ë¸»
	void SetNomal(float& x, float& y, float& z);
	void SetNomal(float&& x, float&& y, float&& z);
	void SetNomal(int& x, int& y, int& z);
	void SetNomal(int&& x, int&& y, int&& z);
	//UV
	void SetUV(float& u,float& v);
	void SetUV(float&& u,float&& v);
	void SetUV(int& u, int& v);
	void SetUV(int&& u, int&& v);
	//ÅºÁ¨Æ®
	void SetTangent(float& x, float& y, float& z);
	void SetTangent(float&& x, float&& y, float&& z);
	void SetTangent(int& x, int& y, int& z);
	void SetTangent(int&& x, int&& y, int&& z);

	//º» ÀÎµ¦½º
	void SetBoneIndex(int& _1, int& _2, int& _3, int& _4);
	void SetBoneIndex(int&& _1, int&& _2, int&& _3, int&& _4);

	//º» ¿þÀÌÆÃ
	void SetBoneWeights(int& _1, int& _2, int& _3, int& _4);
	void SetBoneWeights(int&& _1, int&& _2, int&& _3, int&& _4);
	void SetBoneWeights(float& _1, float& _2, float& _3, float& _4);
	void SetBoneWeights(float&& _1, float&& _2, float&& _3, float&& _4);
};

