#pragma once

#include <windows.h>

struct Key_Plag
{
	bool Key_Current_State = false;
	bool Key_Prev_State = false;
	bool Key_Timer_Flag = false;
	bool Key_Dash_Flag = false;
	int Key_Timer = 0;
};

class KeyIO
{
private:
	static KeyIO* m_Instance;

	bool Keeping = false;	// 키를 누르고 있을때를 체크하기 위함.
public:
	static KeyIO* GetInstance();

	Key_Plag m_key_plag[256];

	/// 키를 눌렀다 뗏을 때.
	bool Is_KeyUP(BYTE Key_Input);
	/// 키가 처음 눌러졌을 때.
	bool Is_KeyDown(BYTE Key_Input);
	/// 키가 눌리고 있을때 체크.
	bool Is_KeyKeep(BYTE Key_Input);
	/// 아래 키를 누르고 있을 때
	bool Is_KeyDown_Keep(BYTE Key_Input);
};