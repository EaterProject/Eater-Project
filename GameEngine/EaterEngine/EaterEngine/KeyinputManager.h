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
	const LPPOINT GetMousePos();

	void Initialize(HWND hwnd);
	void Update();
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

	POINT CursorPos;
	HWND hwnd;

	void UpdataMouseCursor(HWND hwnd);
	void KeyUpDate();
};

