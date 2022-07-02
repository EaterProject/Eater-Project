#pragma once
#include "windows.h"
class KeyinputManager
{
public:
	KeyinputManager();
	~KeyinputManager();

	//Ű�� ��� ������������
	bool GetKey(byte _keyNum);
	//Ű�� �������� �ѹ�
	bool GetKeyDown(byte _keyNum);
	//Ű�� �ѹ������� ������
	bool GetKeyUp(byte _keyNum);
	//������ true �� false �� false �� true�� 
	bool GetKeyToggle(byte _keyNum);

	void KeyReset();

	const LPPOINT GetMousePos();
	const LPPOINT GetClientMousePos();

	void Initialize(HWND hwnd);
	void Update();

	void SetFocus(bool focus);
	void SetMouseCenter();
	void SetMousePos(int x, int y);
	void ShowMouse(bool show);
private:
	//���� ����Ű����
	bool isCurrent[256];

	//���ſ� ���� Ű����
	bool isPrev[256];

	//Ű���������� �ѹ��� true
	bool isDown[256];

	//Ű�� ������ �ѹ��� true;
	bool isUp[256];

	//�ѹ������� true �ٽ��ѹ������� false
	bool isToggle[256];

	POINT ScreenCursorPos;
	POINT ClientCursorPos;
	HWND hwnd;

	bool isFocus;

	void UpdataMouseCursor();
	void KeyUpDate();
};

