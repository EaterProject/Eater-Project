#pragma comment(lib,"winmm")
#include <windows.h>
//#include <digitalv.h>
#include "framework.h"
#include "stdio.h"
#include "GameManager.h"
#include "EaterEngineAPI.h"

//#include "Dwmapi.h"
//#pragma comment( lib, "Dwmapi.lib" )

//#define _CRTDBG_MAP_ALLOC
//#include <cstdlib>
//#include <crtdbg.h>

//#ifdef _DEBUG
//#define new new( _NORMAL_BLOCK , __FILE__ , __LINE__ )
//#endif

// 윈도 프로시저의 전방선언
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// 메인 함수
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR    lpCmdLine, _In_ int nCmdShow)
{
	GameManager* GM = new GameManager();

	/// 윈도를 등록한다.
	WNDCLASSEXW wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;			// LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PROJECT1));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); //(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;		// MAKEINTRESOURCEW(IDC_PROJECT1);
	wcex.lpszClassName = L"PROJECT1";
	wcex.hIconSm = NULL;			//LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	RegisterClassExW(&wcex);

	//현재 모니터의 최대 크기 해상도
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);     // 현재 모니터 가로 해상도
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);    // 현재 모니터 세로 해상도

	RECT R = { 0, 0, 1920, 1080 };
	AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
	int width = R.right - R.left;
	int height = R.bottom - R.top;

	// 애플리케이션 초기화를 수행합니다:
	HWND hWnd = CreateWindowW(L"PROJECT1", L"Eater", WS_OVERLAPPEDWINDOW,
		(screenWidth - width) / 2, (screenHeight - height) / 2, width, height, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	MSG msg;

	GM->Start(hWnd);

	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
			{
				break;
			}
		}
		else
		{
			GM->Update();
		}
	}

	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_SETFOCUS:
	{
		WindowFocus(true);
	}
	break;
	case WM_KILLFOCUS:
	{
		WindowFocus(false);
	}
		break;
	case WM_LBUTTONUP:
	{
		//Picking(LOWORD(lParam), HIWORD(lParam));
	}
	break;
	case WM_SIZE:
	{
		int SizeX = LOWORD(lParam);
		int SizeY = HIWORD(lParam);
		OnReSize(SizeX, SizeY);

		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_PAINT:
		PAINTSTRUCT pr;
		BeginPaint(hWnd, &pr);
		EndPaint(hWnd, &pr);
		break;
	case WM_MOUSEMOVE:
	break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

