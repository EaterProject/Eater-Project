#pragma once

#include <windows.h>

class GameManager
{
public:
	GameManager();
	~GameManager();


	void Start(HWND _g_hWnd);
	void Update();
private:
};