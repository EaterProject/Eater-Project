#include "windows.h"
#include "Conio.h"
#include "KeyinputManager.h"

KeyinputManager::KeyinputManager()
{
	KeyReset();
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

void KeyinputManager::KeyReset()
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
}

void KeyinputManager::UpdataMouseCursor()
{
	GetCursorPos(&ScreenCursorPos);
	GetCursorPos(&ClientCursorPos);
	ScreenToClient(hwnd, &ClientCursorPos);
}

const LPPOINT KeyinputManager::GetMousePos()
{
	return &ScreenCursorPos;
}

const LPPOINT KeyinputManager::GetClientMousePos()
{
	return &ClientCursorPos;
}

void KeyinputManager::Initialize(HWND mhwnd)
{
	hwnd = mhwnd;
}

void KeyinputManager::Update()
{
	// 현재 Window가 Focus 상태일때만 Update..
	if (isFocus)
	{
		KeyUpDate();
		UpdataMouseCursor();
	}
}

void KeyinputManager::SetFocus(bool focus)
{
	isFocus = focus;
}

void KeyinputManager::SetMouseCenter()
{
	RECT rt;
	GetClientRect(hwnd,&rt);

	SetCursorPos(rt.right / 2,rt.bottom/2);
}

void KeyinputManager::SetMousePos(int x, int y)
{
	SetCursorPos(x,y);
}

void KeyinputManager::ShowMouse(bool show)
{
	if (show)
	{
		int count = -1;

		while (count < 0)
		{
			count = ShowCursor(show);
		}
	}
	else
	{
		int count = 0;

		while (count > -1)
		{
			count = ShowCursor(show);
		}
	}


}
