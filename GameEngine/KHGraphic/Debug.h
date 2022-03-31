#pragma once
#include <time.h>
#include <Windows.h>
#include <string>

struct Timer
{
	bool m_SetFrame;

	float m_TotalTime;
	int m_TotalFrame;
	int m_NowFrame;

	clock_t m_Start;
	clock_t m_End;
};

class Debug
{
public:
	static void TimerStart(int&& totalFrame);
	static void TimerEnd();

	static void Log(const char* message, ...);

private:
	static Timer m_Timer;
};