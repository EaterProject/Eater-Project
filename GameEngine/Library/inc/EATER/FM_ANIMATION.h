#pragma once

#include "FormatDLL.h"
class FORMAT_PARSER_DLL FM_ANIMATION
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

	//���� ��ġ
	void SetLocalPos(int& x, int& y, int& z);
	void SetLocalPos(int&& x, int&& y, int&& z);
	void SetLocalPos(float& x, float& y, float& z);
	void SetLocalPos(float&& x, float&& y, float&& z);
	//���� ȸ��
	void SetLocalRot(int& x, int& y, int& z,int& w);
	void SetLocalRot(int&& x, int&& y, int&& z, int&& w);
	void SetLocalRot(float& x, float& y, float& z, float& w);
	void SetLocalRot(float&& x, float&& y, float&& z, float&& w);
	//���� ũ��
	void SetLocalScl(int& x, int& y, int& z);
	void SetLocalScl(int&& x, int&& y, int&& z);
	void SetLocalScl(float& x, float& y, float& z);
	void SetLocalScl(float&& x, float&& y, float&& z);

	//���� ��ġ
	void SetWorldPos(int& x, int& y, int& z);
	void SetWorldPos(int&& x, int&& y, int&& z);
	void SetWorldPos(float& x, float& y, float& z);
	void SetWorldPos(float&& x, float&& y, float&& z);
	//���� ȸ��
	void SetWorldRot(int& x, int& y, int& z, int& w);
	void SetWorldRot(int&& x, int&& y, int&& z, int&& w);
	void SetWorldRot(float& x, float& y, float& z, float& w);
	void SetWorldRot(float&& x, float&& y, float&& z, float&& w);
};

