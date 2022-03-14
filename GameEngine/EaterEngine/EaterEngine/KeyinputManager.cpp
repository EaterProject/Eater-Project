#include "windows.h"
#include "Conio.h"
#include "KeyinputManager.h"

KeyinputManager::KeyinputManager()
{
	for (int i = 0; i < 256; i++)
	{
		isCurrent[i] = false;
		isPrev[i] = false;
		isDown[i] = false;
		isUp[i] = false;
		isToggle[i] = false;
	}
}

KeyinputManager::~KeyinputManager()
{

}

bool KeyinputManager::GetKey(byte _keyNum)
{
	return isCurrent[_keyNum];
}

bool KeyinputManager::GetKeyDown(byte _keyNum)
{
	return isDown[_keyNum];
}

bool KeyinputManager::GetKeyUp(byte _keyNum)
{
	return isUp[_keyNum];
}

bool KeyinputManager::GetKeyToggle(byte _keyNum)
{
	return isToggle[_keyNum];
}

void KeyinputManager::KeyUpDate()
{
	for (int i = 0; i < 256; i++)
	{
		if (GetAsyncKeyState(i))
		{
			isCurrent[i] = true; //현재는 눌렸다
			if (isPrev[i] == false) //현재는 눌렸고 과거는 안눌렸을때
			{
				if (isToggle[i] == true)
				{
					isToggle[i] = false;
				}
				else
				{
					isToggle[i] = true;
				}

				isDown[i] = true;
			}
			else if (isPrev[i] == true)//현재는 눌렸고 과거에도 눌렸을때
			{
				isDown[i] = false;
			}
		}
		else//현재에 안눌렸을때
		{
			isCurrent[i] = false;
			if (isPrev[i] == false) //현재는 안눌렸고 과거에도 안눌렸을때
			{
				isUp[i] = false;
			}
			else if (isPrev[i] == true) //현재는 안눌렸고 과거에는 눌렸을때
			{
				isUp[i] = true;
			}
		}
		isPrev[i] = isCurrent[i];
	}

	int num = 0;
}

void KeyinputManager::UpdataMouseCursor(HWND hwnd)
{
	GetCursorPos(&CursorPos);
	ScreenToClient(hwnd,&CursorPos);
}

 const LPPOINT KeyinputManager::GetMousePos()
{
	return &CursorPos;
}

 void KeyinputManager::Initialize(HWND mhwnd)
 {
	 hwnd = mhwnd;
	
 }

 void KeyinputManager::Update()
 {
	 UpdataMouseCursor(hwnd);
	 KeyUpDate();
 }


