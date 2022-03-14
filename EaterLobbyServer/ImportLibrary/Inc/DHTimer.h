#pragma once

#ifdef DHTIMER_EXPORTS
#define DHTIMER_DLL __declspec(dllexport)
#else
#define DHTIMER_DLL __declspec(dllimport)

#ifdef _DEBUG
#pragma comment(lib,"DHTimer_x64d")
#else
#pragma comment(lib,"DHTimer_x64r")
#endif

#endif

#include <chrono>

class DHTIMER_DLL DHTimer
{
public:
	enum TIMER_NUM
	{
		TIMER_01 = 0,
		TIMER_02,
		TIMER_03,
		TIMER_04,
		TIMER_COUNT
	};

private:
	struct TIMER_DATA
	{
		// 검사할 주기(초단위)
		double Check_Cycle_Sec = (double)1 / 60; // 기본값은 1초에 60프레임
		std::chrono::duration<double> Passed_Time = std::chrono::duration<double>::zero();
	};

private:
	// 사용가능한 타이머
	TIMER_DATA m_Timer[TIMER_COUNT];

	std::chrono::time_point<std::chrono::system_clock> Start_Time;									// 지난 프레임 시간
	std::chrono::time_point<std::chrono::system_clock> Prev_Time;									// 지난 프레임 시간
	std::chrono::time_point<std::chrono::system_clock> Current_Time;								// 현재 프레임 시간
	std::chrono::duration<double> Total_Passed_Time = std::chrono::duration<double>::zero();				// 지난시간
public:
	DHTimer();
	~DHTimer();

	// 시간을 업데이트 해야 타이머를 사용할 수 있다. (한프레임에 한번 호출해야함)
	void Update();
	// 프레임단위 시간설정 (ex. 60 으로하면 1초에 60프레임)
	bool SetFrame(unsigned int _FPS, TIMER_NUM _Using_Timer = TIMER_01);
	// 초단위 시간 설정 (ex. 10으로 설정하면 10초에 한번 Ready_Frame 이 true 가 된다)
	bool SetTime(double _Sec, TIMER_NUM _Using_Timer = TIMER_01);
	// true 일때 진행하면됨.
	bool Ready_Frame(TIMER_NUM _Using_Timer = TIMER_01);
	// 타이머가 시작되고 현재까지 진행된 총 시간 (초단위)
	double GetTotalPassedTimeSec();

};

