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
		// �˻��� �ֱ�(�ʴ���)
		double Check_Cycle_Sec = (double)1 / 60; // �⺻���� 1�ʿ� 60������
		std::chrono::duration<double> Passed_Time = std::chrono::duration<double>::zero();
	};

private:
	// ��밡���� Ÿ�̸�
	TIMER_DATA m_Timer[TIMER_COUNT];

	std::chrono::time_point<std::chrono::system_clock> Start_Time;									// ���� ������ �ð�
	std::chrono::time_point<std::chrono::system_clock> Prev_Time;									// ���� ������ �ð�
	std::chrono::time_point<std::chrono::system_clock> Current_Time;								// ���� ������ �ð�
	std::chrono::duration<double> Total_Passed_Time = std::chrono::duration<double>::zero();				// �����ð�
public:
	DHTimer();
	~DHTimer();

	// �ð��� ������Ʈ �ؾ� Ÿ�̸Ӹ� ����� �� �ִ�. (�������ӿ� �ѹ� ȣ���ؾ���)
	void Update();
	// �����Ӵ��� �ð����� (ex. 60 �����ϸ� 1�ʿ� 60������)
	bool SetFrame(unsigned int _FPS, TIMER_NUM _Using_Timer = TIMER_01);
	// �ʴ��� �ð� ���� (ex. 10���� �����ϸ� 10�ʿ� �ѹ� Ready_Frame �� true �� �ȴ�)
	bool SetTime(double _Sec, TIMER_NUM _Using_Timer = TIMER_01);
	// true �϶� �����ϸ��.
	bool Ready_Frame(TIMER_NUM _Using_Timer = TIMER_01);
	// Ÿ�̸Ӱ� ���۵ǰ� ������� ����� �� �ð� (�ʴ���)
	double GetTotalPassedTimeSec();

};

