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
	// �ش� ���ڿ� Hash ����..
	Hash hashKey = std::hash<const char*>()(key);

	// Hash Key �˻�..
	m_NowKey = m_Timer.find(hashKey);

	// ���� ��ϵ� Key�� �ƴ϶�� ���..
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

	// ���� ���� �ð� ����..
	m_NowKey->second.m_Start = std::chrono::system_clock::now();

	// ���� ������ ī��Ʈ ����..
	m_NowKey->second.m_NowFrame--;
}

void Debugger::TimerEnd(const char* key)
{
	// Hash Key �˻�..
	m_NowKey = m_Timer.find(std::hash<const char*>()(key));

	// ��ϵ� Hash Key�� �ƴ϶�� ó������ ����.. ������� �Ǽ�
	if (m_NowKey == m_Timer.end()) return;

	// �˻��� Timer ����..
	TIME_DESC& nowTimer = m_NowKey->second;

	// ��� ������ �ð� ����..
	nowTimer.m_TotalTime += std::chrono::system_clock::now() - nowTimer.m_Start;

	if (nowTimer.m_NowFrame <= 0)
	{
		// �ش� ���� ��� ���..
		Log(nowTimer.m_OutputType, 
			"-----------------------[Timer]-----------------------\n [%s] [%s]\n [TotalFrame] \t: %d\n [TotalTime] \t: %.5f\n [AverageTime] \t: %.5f\n-----------------------------------------------------\n\n",
			key,
			nowTimer.m_Location.c_str(),
			nowTimer.m_TotalFrame, 
			nowTimer.m_TotalTime.count(), 
			nowTimer.m_TotalTime.count() / (double)nowTimer.m_TotalFrame);

		// ������ ���� Timer ����..
		m_Timer.erase(m_NowKey);
	}
}

void Debugger::Log(DEBUG_OUTPUT& outputType, const char* message, ...)
{
	int length;
	va_list args;

	va_start(args, message);

	// �������� ���ڿ� ���� ����..
	length = _vscprintf(message, args) + 1;

	// �������� ���ڿ� ��ȯ..
	char* cBuf = new char[length];
	memset(cBuf, NULL, length);

	vsprintf(cBuf, message, args);

	va_end(args);

	// ��� Ÿ�Կ� ���� ó��..
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
	// ��� Ÿ�Կ� ���� ó��..
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
