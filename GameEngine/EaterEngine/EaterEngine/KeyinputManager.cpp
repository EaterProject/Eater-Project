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
			isCurrent[i] = true; //����� ���ȴ�
			if (isPrev[i] == false) //����� ���Ȱ� ���Ŵ� �ȴ�������
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
			else if (isPrev[i] == true)//����� ���Ȱ� ���ſ��� ��������
			{
				isDown[i] = false;
			}
		}
		else//���翡 �ȴ�������
		{
			isCurrent[i] = false;
			if (isPrev[i] == false) //����� �ȴ��Ȱ� ���ſ��� �ȴ�������
			{
				isUp[i] = false;
			}
			else if (isPrev[i] == true) //����� �ȴ��Ȱ� ���ſ��� ��������
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


