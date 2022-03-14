#pragma once
#include "windows.h"
class KeyinputManager
{
public:
	KeyinputManager();
	~KeyinputManager();

	//키를 계속 누르고있을떄
	bool GetKey(byte _keyNum);
	//키를 눌렀을때 한번
	bool GetKeyDown(byte _keyNum);
	//키를 한번누르고 떘을떄
	bool GetKeyUp(byte _keyNum);
	//누를때 true 면 false 로 false 면 true로 
	bool GetKeyToggle(byte _keyNum);
	const LPPOINT GetMousePos();

	void Initialize(HWND hwnd);
	void Update();
private:
	//현재 눌린키상태
	bool isCurrent[256];

	//과거에 눌린 키상태
	bool isPrev[256];

	//키를눌렀을때 한번만 true
	bool isDown[256];

	//키를 땠을때 한번만 true;
	bool isUp[256];

	//한번누르면 true 다시한번누르면 false
	bool isToggle[256];

	POINT CursorPos;
	HWND hwnd;

	void UpdataMouseCursor(HWND hwnd);
	void KeyUpDate();
};

