#include "FM_VERTEX.h"

void FM_VERTEX::SetPosition(float& x, float& y, float& z)
{
	POS_X = x;
	POS_Y = y;
	POS_Z = z;
}

void FM_VERTEX::SetPosition(float&& x, float&& y, float&& z)
{
	POS_X = x;
	POS_Y = y;
	POS_Z = z;
}

void FM_VERTEX::SetPosition(int& x, int& y, int& z)
{
	POS_X = (float)x;
	POS_Y = (float)y;
	POS_Z = (float)z;
}

void FM_VERTEX::SetPosition(int&& x, int&& y, int&& z)
{
	POS_X = (float)x;
	POS_Y = (float)y;
	POS_Z = (float)z;
}

void FM_VERTEX::SetNomal(float& x, float& y, float& z)
{
	NOMAL_X = x;
	NOMAL_Y = y;
	NOMAL_Z = z;
}

void FM_VERTEX::SetNomal(float&& x, float&& y, float&& z)
{
	NOMAL_X = x;
	NOMAL_Y = y;
	NOMAL_Z = z;
}

void FM_VERTEX::SetNomal(int& x, int& y, int& z)
{
	NOMAL_X = (float)x;
	NOMAL_Y = (float)y;
	NOMAL_Z = (float)z;
}

void FM_VERTEX::SetNomal(int&& x, int&& y, int&& z)
{
	NOMAL_X = (float)x;
	NOMAL_Y = (float)y;
	NOMAL_Z = (float)z;
}

void FM_VERTEX::SetUV(float& u, float& v)
{
	UV_X = u;
	UV_Y = v;
}

void FM_VERTEX::SetUV(float&& u, float&& v)
{
	UV_X = u;
	UV_Y = v;
}

void FM_VERTEX::SetUV(int& u, int& v)
{
	UV_X = (float)u;
	UV_Y = (float)v;
}

void FM_VERTEX::SetUV(int&& u, int&& v)
{
	UV_X = (float)u;
	UV_Y = (float)v;
}

void FM_VERTEX::SetTangent(float& x, float& y, float& z)
{
	TANGENT_X = x;
	TANGENT_Y = y;
	TANGENT_Z = z;
}

void FM_VERTEX::SetTangent(float&& x, float&& y, float&& z)
{
	TANGENT_X = x;
	TANGENT_Y = y;
	TANGENT_Z = z;
}

void FM_VERTEX::SetTangent(int& x, int& y, int& z)
{
	TANGENT_X = (float)x;
	TANGENT_Y = (float)y;
	TANGENT_Z = (float)z;
}

void FM_VERTEX::SetTangent(int&& x, int&& y, int&& z)
{
	TANGENT_X = (float)x;
	TANGENT_Y = (float)y;
	TANGENT_Z = (float)z;
}

void FM_VERTEX::SetBoneIndex(int& _1, int& _2, int& _3, int& _4)
{
	BONE_INDEX[0] = _1;
	BONE_INDEX[1] = _2;
	BONE_INDEX[2] = _3;
	BONE_INDEX[3] = _4;
}

void FM_VERTEX::SetBoneIndex(int&& _1, int&& _2, int&& _3, int&& _4)
{
	BONE_INDEX[0] = (float)_1;
	BONE_INDEX[1] = (float)_2;
	BONE_INDEX[2] = (float)_3;
	BONE_INDEX[3] = (float)_4;
}

void FM_VERTEX::SetBoneWeights(int& _1, int& _2, int& _3, int& _4)
{
	BONE_WEIGHTS[0] = (float)_1;
	BONE_WEIGHTS[1] = (float)_2;
	BONE_WEIGHTS[2] = (float)_3;
	BONE_WEIGHTS[3] = (float)_4;
}

void FM_VERTEX::SetBoneWeights(int&& _1, int&& _2, int&& _3, int&& _4)
{
	BONE_WEIGHTS[0] = _1;
	BONE_WEIGHTS[1] = _2;
	BONE_WEIGHTS[2] = _3;
	BONE_WEIGHTS[3] = _4;
}

void FM_VERTEX::SetBoneWeights(float& _1, float& _2, float& _3, float& _4)
{
	BONE_WEIGHTS[0] = _1;
	BONE_WEIGHTS[1] = _2;
	BONE_WEIGHTS[2] = _3;
	BONE_WEIGHTS[3] = _4;
}

void FM_VERTEX::SetBoneWeights(float&& _1, float&& _2, float&& _3, float&& _4)
{
	BONE_WEIGHTS[0] = _1;
	BONE_WEIGHTS[1] = _2;
	BONE_WEIGHTS[2] = _3;
	BONE_WEIGHTS[3] = _4;
}


