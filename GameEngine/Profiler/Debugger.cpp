#pragma warning(disable: 4996)

#include "ProfilerDLL.h"
#include "Debugger.h"
#include "Timer.h"
#include "Logger.h"

#include <iostream>
#include <stdarg.h>
#include <tchar.h>

Debugger::Debugger()
{
	AllocConsole();
	system("cls");
	m_Console = GetStdHandle(STD_OUTPUT_HANDLE);
	m_LogCount = 0;
}

void Debugger::TimerStart(DEBUG_OUTPUT outputType, const char* func, int& line, const char* key, int& totalFrame)
{
	// 해당 문자열 Hash 추출..
	Hash hashKey = std::hash<const char*>()(key);

	// Hash Key 검색..
	m_NowKey = m_Timer.find(hashKey);

	// 만약 등록된 Key가 아니라면 등록..
	if (m_NowKey == m_Timer.end())
	{
		TIME_DESC timer;
		timer.m_Location = std::string(func) + " - " + std::to_string(line);
		timer.m_TotalTime = std::chrono::seconds::zero();
		timer.m_OutputType = outputType;
		timer.m_NowFrame = totalFrame;
		timer.m_TotalFrame = totalFrame;

		m_NowKey = m_Timer.insert(std::pair<Hash, TIME_DESC>(hashKey, timer)).first;
	}

	// 현재 시작 시간 설정..
	m_NowKey->second.m_Start = std::chrono::system_clock::now();

	// 측정 프레임 카운트 감소..
	m_NowKey->second.m_NowFrame--;
}

void Debugger::TimerEnd(const char* key)
{
	// Hash Key 검색..
	m_NowKey = m_Timer.find(std::hash<const char*>()(key));

	// 등록된 Hash Key가 아니라면 처리하지 않음.. 사용자의 실수
	if (m_NowKey == m_Timer.end()) return;

	// 검색된 Timer 추출..
	TIME_DESC& nowTimer = m_NowKey->second;

	// 출력 전까지 시간 누적..
	nowTimer.m_TotalTime += std::chrono::system_clock::now() - nowTimer.m_Start;

	if (nowTimer.m_NowFrame <= 0)
	{
		// 해당 측정 결과 출력..
		Log(nowTimer.m_OutputType, 
			"-----------------------[Timer]-----------------------\n [%s] [%s]\n [TotalFrame] \t: %d\n [TotalTime] \t: %.5f\n [AverageTime] \t: %.5f\n-----------------------------------------------------\n\n",
			key,
			nowTimer.m_Location.c_str(),
			nowTimer.m_TotalFrame, 
			nowTimer.m_TotalTime.count(), 
			nowTimer.m_TotalTime.count() / (double)nowTimer.m_TotalFrame);

		// 측정이 끝난 Timer 제거..
		m_Timer.erase(m_NowKey);
	}
}

void Debugger::Log(DEBUG_OUTPUT& outputType, const char* message, ...)
{
	int length;
	va_list args;

	va_start(args, message);

	// 가변인자 문자열 길이 측정..
	length = _vscprintf(message, args) + 1;

	// 가변인자 문자열 변환..
	char* cBuf = new char[length];
	memset(cBuf, NULL, length);

	vsprintf(cBuf, message, args);

	va_end(args);

	// 출력 타입에 따른 처리..
	switch (outputType)
	{
	case DEBUG_OUTPUT::CONSOLE:
	{
		WriteFile(m_Console, cBuf, (DWORD)(length), NULL, NULL);

		m_LogCount++;
		if (m_LogCount > 25)
		{
			system("cls");
			m_LogCount = 0;
		}
	}
		break;
	case DEBUG_OUTPUT::VS_CODE:
	{
		wchar_t* wBuf = new wchar_t[length];
		memset(wBuf, NULL, length);
		MultiByteToWideChar(CP_UTF8, 0, cBuf, -1, wBuf, length);

		OutputDebugStringW(wBuf);
		OutputDebugStringA("\n");

		delete[] wBuf;
	}
		break;
	default:
		break;
	}

	delete[] cBuf;
}

void Debugger::Log(DEBUG_OUTPUT& outputType, char* message, int length)
{
	// 출력 타입에 따른 처리..
	switch (outputType)
	{
	case DEBUG_OUTPUT::CONSOLE:
	{
		WriteFile(m_Console, message, (DWORD)(length), NULL, NULL);

		m_LogCount++;
		if (m_LogCount > 25)
		{
			system("cls");
			m_LogCount = 0;
		}
	}
	break;
	case DEBUG_OUTPUT::VS_CODE:
	{
		wchar_t* wBuf = new wchar_t[length];
		memset(wBuf, NULL, length);
		MultiByteToWideChar(CP_UTF8, 0, message, -1, wBuf, length);

		OutputDebugStringW(wBuf);
		OutputDebugStringA("\n");

		delete[] wBuf;
	}
	break;
	default:
		break;
	}
}
