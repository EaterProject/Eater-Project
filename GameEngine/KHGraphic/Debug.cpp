#pragma warning(disable: 4996)

#include "Debug.h"

#include <iostream>
#include <stdarg.h>
#include <tchar.h>

#define MAX_LEN 1024

Timer Debug::m_Timer;

void Debug::TimerStart(int&& totalFrame)
{
	if (!m_Timer.m_SetFrame)
	{
		m_Timer.m_TotalFrame = totalFrame;
		m_Timer.m_NowFrame = totalFrame;
		m_Timer.m_SetFrame = true;
	}

	m_Timer.m_Start = clock();
	m_Timer.m_NowFrame--;
}

void Debug::TimerEnd()
{
	m_Timer.m_End = clock();

	if (m_Timer.m_NowFrame == 0)
	{
		Log("===== Timer =====\n[Frame] : %d\n[Time] : %.5f\n=================", m_Timer.m_TotalFrame, m_Timer.m_TotalTime / m_Timer.m_TotalFrame);
		
		m_Timer.m_SetFrame = false; 
		m_Timer.m_TotalFrame = 0;
		m_Timer.m_TotalTime = 0;
	}
	else
	{
		m_Timer.m_TotalTime += ((float)(m_Timer.m_End - m_Timer.m_Start) / CLOCKS_PER_SEC);
	}
}

void Debug::Log(const char* message, ...)
{
	char szBuf[MAX_LEN];

	va_list args;

	va_start(args, message);
	vsnprintf_s(szBuf, MAX_LEN, MAX_LEN, message, args);
	va_end(args);

	WCHAR wszBuf[MAX_LEN] = { 0 };
	MultiByteToWideChar(CP_UTF8, 0, szBuf, -1, wszBuf, sizeof(wszBuf));

	OutputDebugStringW(wszBuf);
	OutputDebugStringA("\n\n");
}

