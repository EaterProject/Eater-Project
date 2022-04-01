#pragma warning(disable: 4996)

#include "ProfilerDLL.h"
#include "Debugger.h"

#include <iostream>
#include <stdarg.h>
#include <tchar.h>

#define MAX_LEN 1024

Debugger::Debugger()
{
	AllocConsole();
	m_Console = GetStdHandle(STD_OUTPUT_HANDLE);
	m_LogCount = 0;
}

void Debugger::TimerStart(const char* func, int& line, const char* timeKey, int& totalFrame, DEBUG_OUTPUT outputType)
{
	m_NowKey = m_Timer.find(timeKey);

	if (m_NowKey == m_Timer.end())
	{
		TIME_DESC timer;
		std::string locate = std::string(std::move(func)) + " - " + std::to_string(line);
		timer.m_Location = std::move(locate.c_str());
		timer.m_TotalTime = std::chrono::seconds::zero();
		timer.m_OutputType = outputType;
		timer.m_NowFrame = totalFrame;
		timer.m_TotalFrame = totalFrame;

		m_NowKey = m_Timer.insert(std::pair<const char*, TIME_DESC>(std::move(timeKey), std::move(timer))).first;
	}

	// 현재 시작 시간 설정..
	m_NowKey->second.m_Start = std::chrono::system_clock::now();

	// 측정 프레임 카운트 감소..
	m_NowKey->second.m_NowFrame--;
}

void Debugger::TimerEnd()
{
	TIME_DESC& nowTimer = m_NowKey->second;

	nowTimer.m_TotalTime += std::chrono::system_clock::now() - nowTimer.m_Start;

	if (nowTimer.m_NowFrame <= 0)
	{
		// 해당 측정 결과 출력..
		Log(nowTimer.m_OutputType, 
			"========== Timer ==========\n [%s] [%s]\n [TotalFrame] \t: %d\n [TotalTime] \t: %.5f\n [AverageTime] \t: %.5f\n===========================",
			m_NowKey->first, 
			nowTimer.m_Location,
			nowTimer.m_TotalFrame, 
			nowTimer.m_TotalTime.count(), 
			nowTimer.m_TotalTime.count() / (double)nowTimer.m_TotalFrame);

		// 측정이 끝난 Timer 제거..
		m_Timer.erase(m_NowKey);
	}
}

void Debugger::Log(DEBUG_OUTPUT& outputType, const char* message, ...)
{
	char szBuf[MAX_LEN];

	va_list args;

	va_start(args, message);
	vsnprintf_s(szBuf, MAX_LEN, MAX_LEN, message, args);
	va_end(args);

	switch (outputType)
	{
	case DEBUG_OUTPUT::CONSOLE:
	{
		WriteFile(m_Console, szBuf, (DWORD)sizeof(szBuf), nullptr, NULL);

		m_LogCount++;
		if (m_LogCount > 10)
		{
			system("cls");
			m_LogCount = 0;
		}
	}
		break;
	case DEBUG_OUTPUT::VS_CODE:
	{
		WCHAR wszBuf[MAX_LEN] = { 0 };
		MultiByteToWideChar(CP_UTF8, 0, szBuf, -1, wszBuf, sizeof(wszBuf));

		OutputDebugStringW(wszBuf);
		OutputDebugStringA("\n\n");
	}
		break;
	default:
		break;
	}

}
