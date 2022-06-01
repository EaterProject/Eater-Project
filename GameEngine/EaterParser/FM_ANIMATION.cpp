#include "FM_ANIMATION.h"

void FM_ANIMATION::SetLocalPos(int& x, int& y, int& z)
{
	Local_Pos_X = (float)x;
	Local_Pos_Y = (float)y;
	Local_Pos_Z = (float)z;
}

void FM_ANIMATION::SetLocalPos(int&& x, int&& y, int&& z)
{
	Local_Pos_X = (float)x;
	Local_Pos_Y = (float)y;
	Local_Pos_Z = (float)z;
}

void FM_ANIMATION::SetLocalPos(float& x, float& y, float& z)
{
	Local_Pos_X = x;
	Local_Pos_Y = y;
	Local_Pos_Z = z;
}

void FM_ANIMATION::SetLocalPos(float&& x, float&& y, float&& z)
{
	Local_Pos_X = x;
	Local_Pos_Y = y;
	Local_Pos_Z = z;
}

void FM_ANIMATION::SetLocalRot(int& x, int& y, int& z, int& w)
{
	Local_Rot_X = (float)x;
	Local_Rot_Y = (float)y;
	Local_Rot_Z = (float)z;
	Local_Rot_W = (float)w;
}

void FM_ANIMATION::SetLocalRot(int&& x, int&& y, int&& z, int&& w)
{
	Local_Rot_X = (float)x;
	Local_Rot_Y = (float)y;
	Local_Rot_Z = (float)z;
	Local_Rot_W = (float)w;
}

void FM_ANIMATION::SetLocalRot(float& x, float& y, float& z, float& w)
{
	Local_Rot_X = x;
	Local_Rot_Y = y;
	Local_Rot_Z = z;
	Local_Rot_W = w;
}

void FM_ANIMATION::SetLocalRot(float&& x, float&& y, float&& z, float&& w)
{
	Local_Rot_X = x;
	Local_Rot_Y = y;
	Local_Rot_Z = z;
	Local_Rot_W = w;
}

void FM_ANIMATION::SetLocalScl(int& x, int& y, int& z)
{
	Local_Scl_X = (float)x;
	Local_Scl_Y = (float)y;
	Local_Scl_Z = (float)z;
}

void FM_ANIMATION::SetLocalScl(int&& x, int&& y, int&& z)
{
	Local_Scl_X = (float)x;
	Local_Scl_Y = (float)y;
	Local_Scl_Z = (float)z;
}

void FM_ANIMATION::SetLocalScl(float& x, float& y, float& z)
{
	Local_Scl_X = x;
	Local_Scl_Y = y;
	Local_Scl_Z = z;
}

void FM_ANIMATION::SetLocalScl(float&& x, float&& y, float&& z)
{
	Local_Scl_X = x;
	Local_Scl_Y = y;
	Local_Scl_Z = z;
}

void FM_ANIMATION::SetWorldPos(int& x, int& y, int& z)
{
	World_Pos_X = (float)x;
	World_Pos_Y = (float)y;
	World_Pos_Z = (float)z;
}

void FM_ANIMATION::SetWorldPos(int&& x, int&& y, int&& z)
{
	World_Pos_X = (float)x;
	World_Pos_Y = (float)y;
	World_Pos_Z = (float)z;
}

void FM_ANIMATION::SetWorldPos(float& x, float& y, float& z)
{
	World_Pos_X = x;
	World_Pos_Y = y;
	World_Pos_Z = z;
}

void FM_ANIMATION::SetWorldPos(float&& x, float&& y, float&& z)
{
	World_Pos_X = x;
	World_Pos_Y = y;
	World_Pos_Z = z;
}

void FM_ANIMATION::SetWorldRot(int& x, int& y, int& z, int& w)
{
	World_Rot_X = (float)x;
	World_Rot_Y = (float)y;
	World_Rot_Z = (float)z;
	World_Rot_W = (float)w;
}

void FM_ANIMATION::SetWorldRot(int&& x, int&& y, int&& z, int&& w)
{
	World_Rot_X = (float)x;
	World_Rot_Y = (float)y;
	World_Rot_Z = (float)z;
	World_Rot_W = (float)w;
}

void FM_ANIMATION::SetWorldRot(float& x, float& y, float& z, float& w)
{
	World_Rot_X = x;
	World_Rot_Y = y;
	World_Rot_Z = z;
	World_Rot_W = w;
}

void FM_ANIMATION::SetWorldRot(float&& x, float&& y, float&& z, float&& w)
{
	World_Rot_X = x;
	World_Rot_Y = y;
	World_Rot_Z = z;
	World_Rot_W = w;
}
