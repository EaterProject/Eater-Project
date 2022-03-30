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

	bool Keeping = false;	// Ű�� ������ �������� üũ�ϱ� ����.
public:
	static KeyIO* GetInstance();

	Key_Plag m_key_plag[256];

	/// Ű�� ������ ���� ��.
	bool Is_KeyUP(BYTE Key_Input);
	/// Ű�� ó�� �������� ��.
	bool Is_KeyDown(BYTE Key_Input);
	/// Ű�� ������ ������ üũ.
	bool Is_KeyKeep(BYTE Key_Input);
	/// �Ʒ� Ű�� ������ ���� ��
	bool Is_KeyDown_Keep(BYTE Key_Input);
};